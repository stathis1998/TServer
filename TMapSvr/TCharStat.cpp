// Per-character combat statistics: monster kills, damage dealt, damage taken and healing done.
//
// Counters only ever grow. A future daily quest stores a counter when it is accepted and works
// out progress as (current - stored), so nothing here needs a daily reset.
//
// Threads: the Add* functions, the packet functions and SendDM_SAVECHARSTAT_REQ run on the batch
// thread and never touch SQL. WriteCharStatFromDB and OnDM_SAVECHARSTAT_REQ run on the DB thread.
//
// The load packet and the channel-change packet share one layout, always their last section.
// It is written by WriteCharStatFromDB and CTPlayer::WrapCharStat, and read by UnwrapCharStat:
//   DWORD dwTickPassed, DWORD dwTotalKills, INT64 nDamageDealt, INT64 nDamageTaken, INT64 nHealDone,
//   WORD wCount, { WORD wMonID, DWORD dwKills, INT64 nDamage } x wCount,
//   WORD wCounters, { WORD wStatID, INT64 nValue } x wCounters,
//   BYTE bFreezeLevel (0 means no level freeze)
//
// To track something new: add an ID to enum CHARCOUNTER in TMapType.h, then call AddCounter where
// it happens. No packet, procedure or table change is needed.
#include "StdAfx.h"
#include <SvrInc.h>
#include "TMapSvrModule.h"

// Bytes for one { wMonID, dwKills, nDamage } entry in a packet.
#define CHARSTAT_ENTRY_SIZE		(sizeof(WORD) + sizeof(DWORD) + sizeof(__int64))
// Space left free in the channel-change packet. Its WORD size field saturates at MAX_PACKET_SIZE,
// and the world server then drops the whole packet, so it must never get close to that limit.
#define CHARSTAT_PACKET_MARGIN	(2048)
// Most general counter rows accepted when loading. Rows with an unknown ID are ignored.
#define CHARCOUNTER_MAX_ROWS	(256)

void CTPlayer::ClearCharStat()
{
	memset(&m_charStat, 0, sizeof(m_charStat));
	m_mapMonKillStat.clear();
	m_wStatDirtyMon = 0;
}

void CTPlayer::AddMonKill(WORD wMonID)
{
	TMONKILLSTAT& stat = m_mapMonKillStat[wMonID];
	stat.m_dwKills++;
	if(!stat.m_bDirty)
	{
		stat.m_bDirty = TRUE;
		m_wStatDirtyMon++;
	}

	m_charStat.m_dwTotalKills++;
	m_charStat.m_bDirty = TRUE;
}

void CTPlayer::AddMonDamage(WORD wMonID, DWORD dwDamage)
{
	TMONKILLSTAT& stat = m_mapMonKillStat[wMonID];
	stat.m_nDamage += dwDamage;
	if(!stat.m_bDirty)
	{
		stat.m_bDirty = TRUE;
		m_wStatDirtyMon++;
	}

	m_charStat.m_nDamageDealt += dwDamage;
	m_charStat.m_bDirty = TRUE;
}

void CTPlayer::AddDamageTaken(DWORD dwDamage)
{
	m_charStat.m_nDamageTaken += dwDamage;
	m_charStat.m_bDirty = TRUE;
}

void CTPlayer::AddHealDone(DWORD dwHeal)
{
	m_charStat.m_nHealDone += dwHeal;
	m_charStat.m_bDirty = TRUE;
}

// Only the main copy owns statistics. The check lives here because the money functions this is
// called from have many callers.
void CTPlayer::AddCounter(BYTE bCounter, __int64 nValue)
{
	if(!m_bMain || bCounter >= CHARCOUNTER_COUNT || nValue <= 0)
		return;

	m_charStat.m_aCounter[bCounter] += nValue;
	m_charStat.m_aCounterDirty[bCounter] = TRUE;
	m_charStat.m_bCounterDirty = TRUE;
}

__int64 CTPlayer::GetCounter(BYTE bCounter)
{
	return bCounter < CHARCOUNTER_COUNT ? m_charStat.m_aCounter[bCounter] : 0;
}

DWORD CTPlayer::GetMonKills(WORD wMonID)
{
	MAPTMONKILLSTAT::iterator it = m_mapMonKillStat.find(wMonID);
	return it != m_mapMonKillStat.end() ? (*it).second.m_dwKills : 0;
}

__int64 CTPlayer::GetMonDamage(WORD wMonID)
{
	MAPTMONKILLSTAT::iterator it = m_mapMonKillStat.find(wMonID);
	return it != m_mapMonKillStat.end() ? (*it).second.m_nDamage : 0;
}

void CTPlayer::WrapCharStat(CPacket* pPacket)
{
	(*pPacket)
		<< DWORD(_AtlModule.m_dwTick - m_dwStatSaveTick)
		<< m_charStat.m_dwTotalKills
		<< m_charStat.m_nDamageDealt
		<< m_charStat.m_nDamageTaken
		<< m_charStat.m_nHealDone;

	WORD wSize = pPacket->GetSize();
	WORD wCount = 0;
	(*pPacket) << wCount;

	int nRoom = (MAX_PACKET_SIZE - CHARSTAT_PACKET_MARGIN - int(pPacket->GetSize())) / int(CHARSTAT_ENTRY_SIZE);
	if(nRoom > CHARSTAT_MAX_MONKILL)
		nRoom = CHARSTAT_MAX_MONKILL;

	for(MAPTMONKILLSTAT::iterator it = m_mapMonKillStat.begin(); it != m_mapMonKillStat.end() && int(wCount) < nRoom; it++)
	{
		(*pPacket)
			<< (*it).first
			<< (*it).second.m_dwKills
			<< (*it).second.m_nDamage;

		wCount++;
	}
	memcpy(pPacket->GetBuffer() + wSize, &wCount, sizeof(wCount));

	// Entries left out stay safe in the DB, because the sender flushes before this runs.
	if(size_t(wCount) < m_mapMonKillStat.size())
		_AtlModule.LogEvent("CharStat: channel change kept %d of %d monster entries. CharID: %d", wCount, int(m_mapMonKillStat.size()), m_dwID);

	// CHARSTAT_PACKET_MARGIN leaves room for these.
	wSize = pPacket->GetSize();
	wCount = 0;
	(*pPacket) << wCount;

	for(WORD i = 0; i < CHARCOUNTER_COUNT; i++)
	{
		if(!m_charStat.m_aCounter[i])
			continue;

		(*pPacket)
			<< i
			<< m_charStat.m_aCounter[i];

		wCount++;
	}
	memcpy(pPacket->GetBuffer() + wSize, &wCount, sizeof(wCount));

	(*pPacket) << m_bFreezeLevel;
}

void CTPlayer::UnwrapCharStat(CPacket* pPacket)
{
	ClearCharStat();

	DWORD dwTickPassed = 0;
	WORD wCount = 0;

	(*pPacket)
		>> dwTickPassed
		>> m_charStat.m_dwTotalKills
		>> m_charStat.m_nDamageDealt
		>> m_charStat.m_nDamageTaken
		>> m_charStat.m_nHealDone
		>> wCount;

	if(wCount > CHARSTAT_MAX_MONKILL)
		wCount = CHARSTAT_MAX_MONKILL;

	for(WORD i = 0; i < wCount; i++)
	{
		WORD wMonID = 0;
		TMONKILLSTAT stat;
		stat.m_bDirty = FALSE;

		(*pPacket)
			>> wMonID
			>> stat.m_dwKills
			>> stat.m_nDamage;

		m_mapMonKillStat.insert(MAPTMONKILLSTAT::value_type(wMonID, stat));
	}

	(*pPacket) >> wCount;

	if(wCount > CHARCOUNTER_MAX_ROWS)
		wCount = CHARCOUNTER_MAX_ROWS;

	for(WORD i = 0; i < wCount; i++)
	{
		WORD wStatID = 0;
		__int64 nValue = 0;

		(*pPacket)
			>> wStatID
			>> nValue;

		if(wStatID < CHARCOUNTER_COUNT)
			m_charStat.m_aCounter[wStatID] = nValue;
	}

	(*pPacket) >> m_bFreezeLevel;

	m_dwStatSaveTick = _AtlModule.m_dwTick - dwTickPassed;
}

// DB thread. Appends the shared layout to a DM_LOADCHAR_ACK packet.
void CTMapSvrModule::WriteCharStatFromDB(DWORD dwCharID, CPacket* pMsg)
{
	// Spreads each character's first flush across the save interval, so a restart does not make
	// every online character write at the same moment.
	(*pMsg) << DWORD((dwCharID * 7919u) % CHARSTAT_SAVE_TICK);

	DWORD dwTotalKills = 0;
	__int64 nDamageDealt = 0;
	__int64 nDamageTaken = 0;
	__int64 nHealDone = 0;

	DEFINE_QUERY(&m_db, CSPGetCharStat)
	query->m_dwCharID = dwCharID;
	query->m_dwTotalKills = 0;
	query->m_nDamageDealt = 0;
	query->m_nDamageTaken = 0;
	query->m_nHealDone = 0;

	// A statistics failure must never block a login, so it only logs and loads zeros.
	if(query->Call())
	{
		dwTotalKills = query->m_dwTotalKills;
		nDamageDealt = query->m_nDamageDealt;
		nDamageTaken = query->m_nDamageTaken;
		nHealDone = query->m_nHealDone;
	}
	else
		LogEvent("CharStat: TGetCharStat failed. CharID: %d", dwCharID);
	UNDEFINE_QUERY()

	(*pMsg)
		<< dwTotalKills
		<< nDamageDealt
		<< nDamageTaken
		<< nHealDone;

	WORD wSize = pMsg->GetSize();
	WORD wCount = 0;
	(*pMsg) << wCount;

	DEFINE_QUERY(&m_db, CTBLCharMonKillTable)
	query->m_dwCharID = dwCharID;

	if(query->Open())
	{
		while(query->Fetch() && wCount < CHARSTAT_MAX_MONKILL)
		{
			(*pMsg)
				<< query->m_wMonID
				<< query->m_dwKills
				<< query->m_nDamage;

			wCount++;
		}

		query->Close();
	}
	else
		LogEvent("CharStat: TCHARMONKILLTABLE read failed. CharID: %d", dwCharID);
	UNDEFINE_QUERY()

	memcpy(pMsg->GetBuffer() + wSize, &wCount, sizeof(wCount));

	wSize = pMsg->GetSize();
	wCount = 0;
	(*pMsg) << wCount;

	DEFINE_QUERY(&m_db, CTBLCharCounterTable)
	query->m_dwCharID = dwCharID;

	if(query->Open())
	{
		while(query->Fetch() && wCount < CHARCOUNTER_MAX_ROWS)
		{
			(*pMsg)
				<< query->m_wStatID
				<< query->m_nValue;

			wCount++;
		}

		query->Close();
	}
	else
		LogEvent("CharStat: TCHARCOUNTERTABLE read failed. CharID: %d", dwCharID);
	UNDEFINE_QUERY()

	memcpy(pMsg->GetBuffer() + wSize, &wCount, sizeof(wCount));

	// The portal changes this only while the character is logged out, so reading it at load is enough.
	BYTE bFreezeLevel = 0;

	DEFINE_QUERY(&m_db, CTBLCharLevelFreeze)
	query->m_dwCharID = dwCharID;

	if(query->Open())
	{
		if(query->Fetch())
			bFreezeLevel = query->m_bFreezeLevel;

		query->Close();
	}
	else
		LogEvent("CharStat: TCHARLEVELFREEZETABLE read failed. CharID: %d", dwCharID);
	UNDEFINE_QUERY()

	(*pMsg) << bFreezeLevel;
}

// Batch thread. Queues everything changed since the last flush. Callers make sure this is the
// main copy. The channel-change caller runs after m_bMain has already been cleared, so this
// function cannot check m_bMain itself.
void CTMapSvrModule::SendDM_SAVECHARSTAT_REQ(CTPlayer* pPlayer)
{
#ifndef BOW_COMPILE_MODE
	pPlayer->m_dwStatSaveTick = m_dwTick;

	if(pPlayer->m_bLoadCharError || !pPlayer->HasDirtyCharStat())
		return;

	LPPACKETBUF pBUF = new PACKETBUF();
	pBUF->m_packet.SetID(DM_SAVECHARSTAT_REQ)
		<< pPlayer->m_dwID
		<< pPlayer->m_charStat.m_bDirty
		<< pPlayer->m_charStat.m_dwTotalKills
		<< pPlayer->m_charStat.m_nDamageDealt
		<< pPlayer->m_charStat.m_nDamageTaken
		<< pPlayer->m_charStat.m_nHealDone;

	WORD wSize = pBUF->m_packet.GetSize();
	WORD wCount = 0;
	pBUF->m_packet << wCount;

	for(WORD i = 0; i < CHARCOUNTER_COUNT; i++)
	{
		if(!pPlayer->m_charStat.m_aCounterDirty[i])
			continue;

		pBUF->m_packet
			<< i
			<< pPlayer->m_charStat.m_aCounter[i];

		pPlayer->m_charStat.m_aCounterDirty[i] = FALSE;
		wCount++;
	}
	memcpy(pBUF->m_packet.GetBuffer() + wSize, &wCount, sizeof(wCount));
	pPlayer->m_charStat.m_bCounterDirty = FALSE;

	wSize = pBUF->m_packet.GetSize();
	wCount = 0;
	pBUF->m_packet << wCount;

	for(MAPTMONKILLSTAT::iterator it = pPlayer->m_mapMonKillStat.begin();
		it != pPlayer->m_mapMonKillStat.end() && wCount < CHARSTAT_MAX_MONKILL;
		it++)
	{
		if(!(*it).second.m_bDirty)
			continue;

		pBUF->m_packet
			<< (*it).first
			<< (*it).second.m_dwKills
			<< (*it).second.m_nDamage;

		(*it).second.m_bDirty = FALSE;
		wCount++;
	}
	memcpy(pBUF->m_packet.GetBuffer() + wSize, &wCount, sizeof(wCount));

	// Anything past the per-packet cap stays dirty and goes out with the next flush.
	pPlayer->m_charStat.m_bDirty = FALSE;
	pPlayer->m_wStatDirtyMon -= wCount;

	SayToDB(pBUF);
#endif
}

// DB thread. Reads only the packet, never a CTPlayer.
DWORD CTMapSvrModule::OnDM_SAVECHARSTAT_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	BYTE bTotals;
	WORD wCount;

	pBUF->m_packet
		>> dwCharID
		>> bTotals;

	DWORD dwTotalKills;
	__int64 nDamageDealt;
	__int64 nDamageTaken;
	__int64 nHealDone;

	pBUF->m_packet
		>> dwTotalKills
		>> nDamageDealt
		>> nDamageTaken
		>> nHealDone
		>> wCount;

	// Read these out first, so the packet stays in step whatever happens to the queries below.
	WORD aStatID[CHARCOUNTER_COUNT];
	__int64 aValue[CHARCOUNTER_COUNT];
	WORD wCounters = wCount < CHARCOUNTER_COUNT ? wCount : WORD(CHARCOUNTER_COUNT);

	for(WORD i = 0; i < wCounters; i++)
	{
		pBUF->m_packet
			>> aStatID[i]
			>> aValue[i];
	}

	pBUF->m_packet
		>> wCount;

	if(wCounters)
	{
		DEFINE_QUERY(&m_db, CSPSaveCharCounter)
		query->m_dwCharID = dwCharID;

		for(WORD i = 0; i < wCounters; i++)
		{
			query->m_wStatID = aStatID[i];
			query->m_nValue = aValue[i];

			if(!query->Call())
				LogEvent("CharStat: TSaveCharCounter failed. CharID: %d StatID: %d", dwCharID, aStatID[i]);
		}
		UNDEFINE_QUERY()
	}

	if(bTotals)
	{
		DEFINE_QUERY(&m_db, CSPSaveCharStat)
		query->m_dwCharID = dwCharID;
		query->m_dwTotalKills = dwTotalKills;
		query->m_nDamageDealt = nDamageDealt;
		query->m_nDamageTaken = nDamageTaken;
		query->m_nHealDone = nHealDone;

		if(!query->Call())
			LogEvent("CharStat: TSaveCharStat failed. CharID: %d", dwCharID);
		UNDEFINE_QUERY()
	}

	DEFINE_QUERY(&m_db, CSPSaveCharMonKill)
	query->m_dwCharID = dwCharID;

	for(WORD i = 0; i < wCount; i++)
	{
		pBUF->m_packet
			>> query->m_wMonID
			>> query->m_dwKills
			>> query->m_nDamage;

		if(!query->Call())
			LogEvent("CharStat: TSaveCharMonKill failed. CharID: %d MonID: %d", dwCharID, query->m_wMonID);
	}
	UNDEFINE_QUERY()

	return EC_NOERROR;
}

// Upgrade log: one row in TITEMUPGRADELOGTABLE per upgrade scroll or gemstone roll, so the portal
// can show success rates by level, destroyed items and saves. Unlike the counters above, each row
// is queued at once, because upgrades are single clicks and a row must not wait for a flush.

// Batch thread. The tincture OnCS_ITEMUPGRADE_REQ applies to a failure at this level, checked in
// the same order as its branches. Call it before the roll, because a failure can use the tincture up.
BYTE CTMapSvrModule::GetUpgradeProtect(CTPlayer* pPlayer, BYTE bLevel)
{
	BYTE bGreater = pPlayer->HaveItemProbBuff(SDT_STATUS_GREATER_ITEMGUARD) ? TRUE : FALSE;
	BYTE bMedium = pPlayer->HaveItemProbBuff(SDT_STATUS_MEDIUM_ITEMGUARD) ? TRUE : FALSE;

	if(bGreater && bLevel >= 19)
		return UPGRADELOG_PROTECT_GREATER;
	if(bMedium && bLevel >= 15)
		return UPGRADELOG_PROTECT_MEDIUM;
	if(pPlayer->HaveItemProbBuff(SDT_STATUS_ITEMGUARD))
		return UPGRADELOG_PROTECT_SURVIVAL;
	if(bMedium)
		return UPGRADELOG_PROTECT_MEDIUM;
	if(bGreater)
		return UPGRADELOG_PROTECT_GREATER;

	return UPGRADELOG_PROTECT_NONE;
}

// Batch thread. Only the main copy of a properly loaded character writes.
void CTMapSvrModule::SendDM_LOGITEMUPGRADE_REQ(CTPlayer* pPlayer, const TUPGRADELOG& log)
{
#ifndef BOW_COMPILE_MODE
	if(!pPlayer->m_bMain || pPlayer->m_bLoadCharError)
		return;

	LPPACKETBUF pBUF = new PACKETBUF();
	pBUF->m_packet.SetID(DM_LOGITEMUPGRADE_REQ)
		<< pPlayer->m_dwID
		<< log.m_bKind
		<< log.m_bResult
		<< log.m_dwItemID
		<< log.m_dwScrollID
		<< log.m_bLevelBefore
		<< log.m_bLevelAfter
		<< log.m_bChance
		<< log.m_bProtect
		<< DWORD(pPlayer->m_wMapID)
		<< log.m_dwNpcID
		<< log.m_nFee;

	SayToDB(pBUF);
#endif
}

// DB thread. Reads only the packet, never a CTPlayer. A failed write is only logged, so the
// upgrade itself is never affected.
DWORD CTMapSvrModule::OnDM_LOGITEMUPGRADE_REQ(LPPACKETBUF pBUF)
{
	DEFINE_QUERY(&m_db, CSPLogItemUpgrade)
	pBUF->m_packet
		>> query->m_dwCharID
		>> query->m_bKind
		>> query->m_bResult
		>> query->m_dwItemID
		>> query->m_dwScrollID
		>> query->m_bLevelBefore
		>> query->m_bLevelAfter
		>> query->m_bChance
		>> query->m_bProtect
		>> query->m_dwMapID
		>> query->m_dwNpcID
		>> query->m_nFee;

	if(!query->Call())
		LogEvent("UpgradeLog: TLogItemUpgrade failed. CharID: %d ItemID: %d Result: %d", query->m_dwCharID, query->m_dwItemID, query->m_bResult);
	UNDEFINE_QUERY()

	return EC_NOERROR;
}

// Formats copper as "12g 340s 56c", leaving out the parts that are zero. A season's total can
// pass the 4294g a DWORD holds, so this takes an INT64.
static CString FormatCounterMoney(__int64 nMoney)
{
	DWORD dwGold = 0;
	DWORD dwSilver = 0;
	DWORD dwCopper = 0;
	CalcMoney(nMoney, dwGold, dwSilver, dwCopper);

	CString strMoney;
	CString strPart;

	if(dwGold)
	{
		strPart.Format(_T("%ug "), dwGold);
		strMoney += strPart;
	}
	if(dwSilver)
	{
		strPart.Format(_T("%us "), dwSilver);
		strMoney += strPart;
	}
	if(dwCopper)
	{
		strPart.Format(_T("%uc "), dwCopper);
		strMoney += strPart;
	}

	strMoney.TrimRight();
	if(strMoney.IsEmpty())
		strMoney = _T("0c");

	return strMoney;
}

// Batch thread. Chat command for checking a character's own numbers. Returns TRUE when the text
// was this command, so the caller does not broadcast it.
//   !cstat        shows your totals
//   !cstat save   also queues a flush straight away
// It uses "!" because the client handles "/" chat locally and never sends it. It is not limited
// to operators, because it only shows the sender's own numbers and writes only changed values.
BYTE CTMapSvrModule::OnCharStatCmd(CTPlayer* pPlayer, const CString& strTalk)
{
	// Chat text is "%04X%04X" (header and body lengths), then the header, then the body. Typed
	// text has an empty header, so the body starts at offset 8.
	if(strTalk.GetLength() < 8 + 6 || strTalk.Mid(8, 6).CompareNoCase(_T("!cstat")) != 0)
		return FALSE;

	CString strArg = strTalk.Mid(8 + 6);
	strArg.Trim();

	if(!strArg.IsEmpty() && strArg.CompareNoCase(_T("save")) != 0)
		return FALSE;

	BYTE bSaved = FALSE;
	if(!strArg.IsEmpty())
	{
		SendDM_SAVECHARSTAT_REQ(pPlayer);
		bSaved = TRUE;
	}

	CString strMsg;
	strMsg.Format(_T("Kills %u, damage dealt %I64d, taken %I64d, healed %I64d, monster types %u, unsaved %u%s"),
		pPlayer->GetTotalKills(),
		pPlayer->GetTotalDamageDealt(),
		pPlayer->GetTotalDamageTaken(),
		pPlayer->GetTotalHealDone(),
		DWORD(pPlayer->m_mapMonKillStat.size()),
		DWORD(pPlayer->HasDirtyCharStat() ? 1 : 0),
		bSaved ? _T(" (flush queued)") : _T(""));

	pPlayer->SendCS_CHAT_ACK(CHAT_OPERATOR, 0, GetSvrMsg(NAME_OPERATOR), BuildNetString(NAME_NULL, strMsg));

	strMsg.Format(_T("Upgrades %I64d succeeded, %I64d failed. Money from monsters %I64d, spent %I64d"),
		pPlayer->GetCounter(CHARCOUNTER_UPGRADE_SUCCESS),
		pPlayer->GetCounter(CHARCOUNTER_UPGRADE_FAIL),
		pPlayer->GetCounter(CHARCOUNTER_MONMONEY),
		pPlayer->GetCounter(CHARCOUNTER_MONEYSPENT));

	pPlayer->SendCS_CHAT_ACK(CHAT_OPERATOR, 0, GetSvrMsg(NAME_OPERATOR), BuildNetString(NAME_NULL, strMsg));

	strMsg.Format(_T("Soul lottery: %I64d won, %I64d lost. Gold from wins %s, consolation %s"),
		pPlayer->GetCounter(CHARCOUNTER_SOULLOT_WON),
		pPlayer->GetCounter(CHARCOUNTER_SOULLOT_LOST),
		(LPCTSTR) FormatCounterMoney(pPlayer->GetCounter(CHARCOUNTER_SOULLOT_MONEY_WON)),
		(LPCTSTR) FormatCounterMoney(pPlayer->GetCounter(CHARCOUNTER_SOULLOT_MONEY_CONSOLATION)));

	pPlayer->SendCS_CHAT_ACK(CHAT_OPERATOR, 0, GetSvrMsg(NAME_OPERATOR), BuildNetString(NAME_NULL, strMsg));

	return TRUE;
}

// DB thread. Replaces the character's freeze; a level of 0 only removes it.
DWORD CTMapSvrModule::OnDM_SAVELEVELFREEZE_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	BYTE bFreezeLevel;

	pBUF->m_packet
		>> dwCharID
		>> bFreezeLevel;

	DEFINE_QUERY(&m_db, CTBLDeleteCharLevelFreeze)
	query->m_dwCharID = dwCharID;

	if(!query->Call())
		LogEvent("LevelFreeze: delete failed. CharID: %d", dwCharID);
	UNDEFINE_QUERY()

	if(bFreezeLevel)
	{
		DEFINE_QUERY(&m_db, CTBLInsertCharLevelFreeze)
		query->m_dwCharID = dwCharID;
		query->m_bFreezeLevel = bFreezeLevel;

		if(!query->Call())
			LogEvent("LevelFreeze: insert failed. CharID: %d Level: %d", dwCharID, bFreezeLevel);
		UNDEFINE_QUERY()
	}

	return EC_NOERROR;
}

// Batch thread. Chat command for the sender's own level freeze. Returns TRUE when the text was
// this command, so the caller does not broadcast it.
//   !freeze        shows the freeze
//   !freeze 50     freezes at level 50, from the next EXP gain
//   !freeze off    removes the freeze
// GainExp reads m_bFreezeLevel, so a change works at once, with no need to log out.
BYTE CTMapSvrModule::OnLevelFreezeCmd(CTPlayer* pPlayer, const CString& strTalk)
{
	// Same chat text layout as OnCharStatCmd: typed text starts at offset 8.
	if(strTalk.GetLength() < 8 + 7 || strTalk.Mid(8, 7).CompareNoCase(_T("!freeze")) != 0)
		return FALSE;

	CString strArg = strTalk.Mid(8 + 7);
	if(!strArg.IsEmpty() && strArg[0] != _T(' '))
		return FALSE;
	strArg.Trim();

	CString strMsg;

#ifdef BOW_COMPILE_MODE
	strMsg = _T("The level freeze can't be used here.");
#else
	BYTE bSave = FALSE;
	BYTE bNewFreeze = 0;

	if(strArg.IsEmpty())
	{
		if(pPlayer->m_bFreezeLevel)
			strMsg.Format(_T("Level freeze: level %u. Any EXP past it turns into money. !freeze off removes it."), pPlayer->m_bFreezeLevel);
		else
			strMsg = _T("Level freeze: off. Type !freeze and a level to stop at that level.");
	}
	else if(strArg.CompareNoCase(_T("off")) == 0)
	{
		bSave = TRUE;
		strMsg = _T("Level freeze removed.");
	}
	else if(pPlayer->m_bLevel > LEVELFREEZE_MAX_LEVEL)
		strMsg = _T("This character is already at the highest level.");
	else
	{
		int nLevel = (strArg.GetLength() <= 3 && strArg.SpanIncluding(_T("0123456789")) == strArg) ? _ttoi(strArg) : 0;

		if(nLevel < pPlayer->m_bLevel || nLevel > LEVELFREEZE_MAX_LEVEL)
			strMsg.Format(_T("Choose a level from %u to %u, or off."), pPlayer->m_bLevel, LEVELFREEZE_MAX_LEVEL);
		else
		{
			bSave = TRUE;
			bNewFreeze = BYTE(nLevel);
			strMsg.Format(_T("Level freeze set to level %u. Any EXP past it turns into money."), nLevel);
		}
	}

	if(bSave)
	{
		// Only the main copy of a properly loaded character may write its freeze.
		if(!pPlayer->m_bMain || pPlayer->m_bLoadCharError)
			return TRUE;

		pPlayer->m_bFreezeLevel = bNewFreeze;

		LPPACKETBUF pBUF = new PACKETBUF();
		pBUF->m_packet.SetID(DM_SAVELEVELFREEZE_REQ)
			<< pPlayer->m_dwID
			<< bNewFreeze;

		SayToDB(pBUF);
	}
#endif

	pPlayer->SendCS_CHAT_ACK(CHAT_OPERATOR, 0, GetSvrMsg(NAME_OPERATOR), BuildNetString(NAME_NULL, strMsg));

	return TRUE;
}
