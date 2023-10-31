#include "StdAfx.h"
#include <SvrInc.h>
#include "TMapSvrModule.h"
#include "TTextLinkData.h"

// ===============================================================================
const CHAR CTTextLinkData::REAL_REP_MARK = 0x02;
const CHAR CTTextLinkData::NULL_REP_MARK = 0x03;
// ===============================================================================
CTTextLinkData::CTTextLinkData()
	: m_nPos(0), m_pLastUnpackQuest(NULL), m_pLastUnpackItem(NULL)
{
}
// -------------------------------------------------------------------------------
CTTextLinkData::~CTTextLinkData()
{
	if(m_pLastUnpackItem)
		delete m_pLastUnpackItem;
}
// ===============================================================================
TTEXT_LINK_TYPE CTTextLinkData::GetType()
{
	UINT nBackup = m_nPos;
	m_nPos = 0;

	TTEXT_LINK_TYPE type;	
	(*this) >> type;

	m_nPos = nBackup;
	return type;
}
// ===============================================================================
void CTTextLinkData::PackQuest(LPQUESTTEMP pQuest)
{
	(*this) << (DWORD)TTEXT_LINK_TYPE_QUEST << pQuest->m_dwQuestID;
}
// -------------------------------------------------------------------------------
LPQUESTTEMP CTTextLinkData::UnPackQuest()
{
	m_pLastUnpackQuest = NULL;
	ResetPosition();

	//TTEXT_LINK_TYPE nType;
	DWORD nType;
	DWORD dwID;

	(*this) >> nType;
	if( nType != TTEXT_LINK_TYPE_QUEST )
		return m_pLastUnpackQuest;

	(*this) >> dwID;
	m_pLastUnpackQuest = _AtlModule.FindQuestTemplate(dwID);

	return m_pLastUnpackQuest;
}
// ===============================================================================
void CTTextLinkData::PackItem(CTItem* pClientItem)
{
	if( !pClientItem )
		return;

	(*this) 
		<< (DWORD)TTEXT_LINK_TYPE_ITEM 
		<< pClientItem->m_pTITEM->m_wItemID;

	(*this) 
		<< pClientItem->m_bLevel
		<< pClientItem->m_bGem
		<< pClientItem->m_dwDuraCur
		<< pClientItem->m_dwDuraMax
		<< pClientItem->m_bRefineCur
		<< pClientItem->m_pTITEM->m_bRefineMax
		<< pClientItem->m_bGLevel
		<< (BYTE)pClientItem->m_dwExtValue[IEV_WRAP]
		<< (BYTE)pClientItem->m_dwExtValue[IEV_ELD]
		<< pClientItem->m_wMoggItemID
		<< (WORD)pClientItem->m_dwExtValue[IEV_COMPANION]
		<< pClientItem->m_bGradeEffect
		<< pClientItem->m_dEndTime;

	BYTE bMagicID;
	WORD wValue;
	MAPTMAGIC::iterator itM;
	for(itM = pClientItem->m_mapTMAGIC.begin();itM != pClientItem->m_mapTMAGIC.end(); itM++)
	{
		bMagicID = (*itM).first;
		wValue = pClientItem->GetMagicValue(bMagicID);
		(*this) 
			<< bMagicID
			<< wValue;
	}
}
// -------------------------------------------------------------------------------
CTItem* CTTextLinkData::UnPackItem()
{
	if(m_pLastUnpackItem)
	{
		delete m_pLastUnpackItem;
		m_pLastUnpackItem = NULL;
	}
	ResetPosition();

	//TTEXT_LINK_TYPE nType;
	DWORD nType;
	(*this) >> nType;

	if( nType != TTEXT_LINK_TYPE_ITEM )
		return m_pLastUnpackItem;

	WORD wItemID;
	(*this) >> wItemID;

	LPTITEM pTITEM = _AtlModule.FindTItem(wItemID);
	if(!pTITEM)
		return m_pLastUnpackItem;

	m_pLastUnpackItem = new CTItem();
	m_pLastUnpackItem->m_pTITEM = pTITEM;
	m_pLastUnpackItem->m_mapTMAGIC.clear();

	if(!IsEndOfPosition())
	{
		BYTE bGrade;
		BYTE bGem;
		DWORD dwDuraCurrent;
		DWORD dwDuraMax;
		BYTE bRefineCurrent;
		BYTE bRefineMax;
		BYTE bCanGamble;
		BYTE bWrap;
		BYTE bELD;
		WORD wMoggItemID;
		WORD wCompanionID;
		BYTE bGradeEffect;
		__time64_t dEndTime;

		(*this)
			>> bGrade
			>> bGem
			>> dwDuraCurrent
			>> dwDuraMax
			>> bRefineCurrent
			>> bRefineMax
			>> bCanGamble
			>> bWrap
			>> bELD
			>> wMoggItemID
			>> wCompanionID
			>> bGradeEffect
			>> dEndTime;

		m_pLastUnpackItem->m_bLevel = bGrade;
		m_pLastUnpackItem->m_bGem = bGem;
		m_pLastUnpackItem->m_dwDuraCur = dwDuraCurrent;
		m_pLastUnpackItem->m_dwDuraMax = dwDuraMax;
		m_pLastUnpackItem->m_bRefineCur = bRefineCurrent;
		m_pLastUnpackItem->m_bGLevel = bCanGamble;
		m_pLastUnpackItem->m_dwExtValue[IEV_WRAP] = bWrap;
		m_pLastUnpackItem->m_dwExtValue[IEV_ELD] = bELD;
		m_pLastUnpackItem->m_wMoggItemID = wMoggItemID;
		m_pLastUnpackItem->m_bGradeEffect = bGradeEffect;
		m_pLastUnpackItem->m_dEndTime = dEndTime;
		m_pLastUnpackItem->m_dwExtValue[IEV_COMPANION] = wCompanionID;
	}

	while(!IsEndOfPosition())
	{
		BYTE bMagicID;
		WORD wValue;

		(*this)
			>> bMagicID 
			>> wValue;
	
		LPTITEMMAGIC pMagic = _AtlModule.GetItemMagic(bMagicID);
		if(pMagic)
		{
			MAPTMAGIC::iterator itTMAGIC = m_pLastUnpackItem->m_mapTMAGIC.find(bMagicID);
			LPTMAGIC pTMAGIC = new TMAGIC();

			FLOAT fRevision = FLOAT(pMagic->m_bRvType ? m_pLastUnpackItem->m_pTITEM->m_fRevision[pMagic->m_bRvType-1] : 1.0);
			pTMAGIC->m_wValue = WORD(wValue * 100 / (fRevision * pMagic->m_wMaxValue));
			pTMAGIC->m_pMagic = pMagic;

			if( itTMAGIC != m_pLastUnpackItem->m_mapTMAGIC.end() )
			{
				delete (*itTMAGIC).second;
				m_pLastUnpackItem->m_mapTMAGIC.erase(itTMAGIC);
			}

			m_pLastUnpackItem->m_mapTMAGIC.insert( MAPTMAGIC::value_type( bMagicID, pTMAGIC));
		}
	}

	return m_pLastUnpackItem;
}
