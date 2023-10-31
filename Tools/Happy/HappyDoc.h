// HappyDoc.h : CHappyDoc Ŭ������ �������̽�
//


#pragma once

#include <afxinet.h>

#include "HSession.h"
#include "DlgUploadProgress.h"
#include "DlgPatchProgress.h"
#include "ServiceTree.h"
#include "ServiceList.h"
#include "AutoPatch.h"
#include "PatchList.h"
#include "ServiceGraph.h" // ���·� Graph
#include "Machine.h" // ���·� Machine
#include "MachineGraph.h"
#include "HappyGroup.h"
#include "MonsterSpawn.h"
#include "DlgServiceLog.h"
#include "RetDlg.h"
#include "PreVersion.h"

#include "GMToolDlg.h"

class CHappyDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CHappyDoc();
	DECLARE_DYNCREATE(CHappyDoc)

public:
	void ClearMachine();
	void ClearGroup();
	void ClearSvrType();
	void ClearService();
	void ClearPlatform();
	void ClearPatchFiles();
	void ClearServiceGraph();
	void ClearData();
	void DeletePatchProgress();
	BYTE ServiceUpload();
	void NotifyUploadEnd(BYTE bRet);
	void UpdateUploadProgress();
	BYTE IsSamePatchFile(LPPATCHFILE pFile); // ���� �̸��� ������ ������ FALSE ����
	void AddPatchFile(LPPATCHFILE pFile);
	void OnPatchUpload();
	BOOL CreateTempDirectory(CString &strFullPath);
	BOOL ZipPatchFile(LPCTSTR strTargetPath);
	BOOL ZipPatchFiles(LPCTSTR strTargetPath);
	BOOL UploadPatchFiles(LPCTSTR strSourcePath);
	void RetrieveZipFile(LPCTSTR strTargetPath, LPPATCHFILE pPatchFile);
	void DeleteDirContents(LPCTSTR strPath);
	BOOL CreateDirectoryEx(CString strPath);
	BOOL VerifyRemoteDirectory(HINTERNET hConnect, LPPATCHFILE pPatch);
	BOOL CreateFtpDirectoryEx(HINTERNET hConnect, CString strPath);
	BYTE UploadPatchFile(HINTERNET hConnect, LPPATCHFILE pPatch, int& nSentTotal);
	CWnd * GetPatchListView();
	CWnd * GetServiceListView();
	void SaveLogData();
	BYTE NewUploadPatchFiles(LPCTSTR strSourcePath);
	BYTE CheckBeep();
	void CloseAllDialog();
	CString GetSvrName(BYTE bGroup,BYTE bSvrType,BYTE bSvrID);
	void InitEventTitle();
	void ClearLuckyEvent();
	void AddLuckEvent(LUCKYEVENT stLuckyEvent);
	void DelLuckyEvent(WORD wID);
	LPLUCKYEVENT GetLuckyEvent(WORD wID);
	void AddTournament(LPTOURNAMENT pTNT);
	void DelTournament(WORD wID);
	void ClearTournament();
	LPTOURNAMENT GetTournament(WORD wID);
	LPTOURNAMENT GetTournament(CString strTitle);
	void ChangeXListCtrl(DWORD dwCtrlID,int nRow, int nCol, int nCtrlType);
	int GetClassIndex(DWORD dwClass);



	

// Ư��
public:
	CHSession	m_session;
	BYTE	m_bAlertOnFail;
	BYTE	m_bViewAllService;
	BYTE	m_bErrorStatus;
	BYTE	m_bServiceAutoStart; // ���� Stop �ÿ� �ڵ����� Start
	BYTE	m_bBtnAutoStart; //���� �ڵ����� ��ư�� ���°�(Ǫ�������ΰ� �ƴѰ�)

	DWORD	m_dwIP;
	DWORD	m_dwPort;
	CString m_strIP;
	CString m_strUserID;
	CString m_strPasswd;

	DWORD	m_dwPatchIP;
	CString m_strPatchUserID;
	CString m_strPatchPasswd;
	DWORD	m_dwPatchPort;
	BYTE	m_bSameFileExist;
	
	CString m_strFtpAddr;
	CString m_strFtpUser;
	CString m_strFtpPasswd;
	WORD	m_wFtpPort;

	MAPMACHINE	m_mapMachine;
	MAPGROUP	m_mapGroup;
	MAPSERVICE	m_mapService;
	MAPSVRTYPE	m_mapSvrType;
	VPATCHFILE	m_vPatchFiles;

	// ���·� Machine
	MAPPLATFORM m_mapPLATFORM;

	// ���·� Service Graph
	MAPSERVICEGRAPH m_mapSERVICEGRAPH;

	DWORD m_dwSelectedService;
	CDlgUploadProgress  m_dlgUploadProgress;
	CPreVersion*		m_dlgPreVersion;
	CFile m_fileUpload;
	DWORD m_dwUploadedSize;
	DWORD m_dwPatchSize;
	//DWORD m_dwZippedPatchSize;
	//CString m_strZippedFile;

	CString m_strNextFolder;
	CString m_strAdminWeb;
	MAPBYTESTRING m_mapEventName;
	MAPDWORDSTRING m_mapEventSvr;
	MAPEVENTINFO m_mapEventInfo;
	MAPTCASHITEM m_mapCashItem;
	MAPLUCKYEVENT m_mapLuckEvent;
	MAPTOURNAMENT m_mapTournament;
	
	BYTE	m_bPatchType;
	CString m_strZippedPatchFile;
	CString m_strZippedPatchFileOnly;
	DWORD m_dwSizeZippedPatch;
private:
	HANDLE		m_hThreadPatchUpload;
	static DWORD WINAPI _ThreadPatchUpload(LPVOID lpParam);
	DWORD ThreadPatchUpload();

	BYTE	m_bPatchUploading;
	CDlgPatchProgress * m_pPatchProgress;

// �۾�
public:
	LPHAPPYSERVICE FindService(DWORD dwID);
	BYTE IsGroup();
	void GroupStart();
	void GroupStop();
	void DisconnectServer();

	void OnConnect(int nErrorCode);
	void OnClose(int nErrorCode);
	void OnReceive(CPacket * pPacket);
	void SessionStart(CString strIp, DWORD dwPort);
	void Say(CPacket * pPacket);

	void OnCT_OPLOGIN_ACK(CPacket * pPacket);
	void OnCT_SVRTYPELIST_ACK(CPacket * pPacket);
	void OnCT_MACHINELIST_ACK(CPacket * pPacket);
	void OnCT_GROUPLIST_ACK(CPacket * pPacket);
	void OnCT_SERVICESTAT_ACK(CPacket * pPacket);
	void OnCT_SERVICECHANGE_ACK(CPacket * pPacket);
	void OnCT_SERVICEDATA_ACK(CPacket * pPacket);
	void OnCT_SERVICEUPLOADSTART_ACK(CPacket * pPacket);
	void OnCT_SERVICEUPLOAD_ACK(CPacket * pPacket);
	void OnCT_SERVICEUPLOADEND_ACK(CPacket * pPacket);
	void OnCT_UPDATEPATCH_ACK(CPacket * pPacket);
	void OnCT_AUTHORITY_ACK(CPacket * pPacket); // ���·� CT_AUTHORITY_ACK
	void OnCT_PLATFORM_ACK(CPacket * pPacket); // ���·� CT_PLATFORM_ACK
	void OnCT_MONSPAWNFIND_ACK(CPacket * pPacket); // ���·� CT_MONSPAWNFIND_ACK
	void OnCT_USERPROTECTED_ACK(CPacket * pPacket);		
	void OnCT_SERVICEAUTOSTART_ACK(CPacket* pPacket);	// ���� Stop �� �ڵ����� Start �ϴ����� ����.
	void OnCT_CHATBAN_ACK(CPacket* pPacket); // ä�ñ�����û�� ����.
	void OnCT_ITEMFIND_ACK(CPacket* pPacket);
	void OnCT_ITEMSTATE_ACK(CPacket* pPacket);
	void OnCT_CHATBANLIST_ACK(CPacket* pPacket);
	void OnCT_CASTLEINFO_ACK(CPacket* pPacket);
	void OnCT_CASTLEGUILDCHG_ACK(CPacket* pPacket);
	void OnCT_EVENTCHANGE_ACK(CPacket* pPacket);
	void OnCT_EVENTLIST_ACK(CPacket* pPacket);
	void OnCT_CASHITEMLIST_ACK(CPacket* pPacket);
	void OnCT_EVENTQUARTERUPDATE_ACK(CPacket* pPacket);
	void OnCT_EVENTQUARTERLIST_ACK(CPacket* pPacket);
	void OnCT_TOURNAMENTEVENT_ACK(CPacket* pPacket);
	void OnCT_RPSGAMEDATA_ACK(CPacket* pPacket);
	void OnCT_PREVERSIONTABLE_ACK(CPacket* pPacket);	
	void OnCT_CMGIFT_ACK(CPacket* pPacket);
	void OnCT_CMGIFTLIST_ACK(CPacket* pPacket);

	void SendCT_OPLOGIN_REQ();
	void SendCT_SERVICESTAT_REQ();
	void SendCT_SERVICECONTROL_REQ(BYTE bStart);
	void SendCT_SERVICECONTROL_REQ(DWORD dwID, BYTE bStart);
	void SendCT_SERVICEUPLOADSTART_REQ(BYTE bMachine, CString strFile);
	void SendCT_SERVICEUPLOAD_REQ(BYTE data[], WORD wSize);
	void SendCT_SERVICEUPLOADEND_REQ(BYTE bCancel, BYTE data[], WORD wSize);
	void SendCT_UPDATEPATCH_REQ();

	void SendCT_ANNOUNCEMENT_REQ(DWORD nID, CString strAnnounce); // ���·� CT_ANNOUNCEMENT_REQ
	void SendCT_USERKICKOUT_REQ(CString strUser); // ���·� CT_USERKICKOUT_REQ
	void SendCT_USERMOVE_REQ(queue<CString>* pqStrUser, BYTE bWorld, BYTE bChannel, WORD wMapID, FLOAT fPosX, FLOAT fPosY, FLOAT fPosZ); // ���·� CT_USERMOVE_REQ
	void SendCT_USERPOSITION_REQ(BYTE bWorld, queue<CString>* pqStrUser, CString strTarget); // ���·� CT_USERPOSITION_REQ
	void SendCT_MONSPAWNFIND_REQ(BYTE bGroupID, BYTE bChannel, WORD wMapID, WORD wSpawnID); // ���·� CT_MONSPAWNFIND_REQ
	void SendCT_MONACTION_REQ(BYTE bGroup, BYTE bChannel, WORD wMapID, DWORD dwMonID, BYTE bAction, DWORD dwTriggerID, DWORD dwHostID, DWORD dwRHID, BYTE bRHType,WORD wSpawnID);
	void SendCT_USERPROTECTED_REQ(CString strUserID, DWORD dwTime);
	void SendCT_CHARMSG_REQ(CString strName, CString strMsg);

	//////////////////////////////////////////////////////////////
	
	void SendCT_RECONNECT_REQ(DWORD _dwID);
	void SendCT_DISCONNECT_REQ(DWORD _dwID);
	void SendCT_SERVICEAUTOSTART_REQ(BYTE _bAutoStart); // ���� Stop �� �ڵ����� Start ������ ��û	
	void SendCT_CHATBAN_REQ(CString strName,WORD wMin,CString strReason); // Ư�� ĳ���� ä�� ���� ��û.
	void SendCT_SERVICEDATACLEAR_REQ(); // ������ ������(MAXUSER,ACTIVEUSER,STOPCOUNT,LATEST START,PEEK TIME) �ʱ�ȭ
	
	void SendCT_ITEMFIND_REQ(WORD wItemID,CString strName,BYTE bWorldID);
	void SendCT_ITEMSTATE_REQ(LPVITEMSTATE pVITEMSTATE,BYTE bWorldID);
	void SendCT_CHATBANLIST_REQ();
	void SendCT_CHATBANLISTDEL_REQ(DWORD dwID);
	void SendCT_CASTLEINFO_REQ(BYTE bWorld);
	void SendCT_CASTLEGUILDCHG_REQ(BYTE bWorld,WORD wCastleID,DWORD dwDefGuildID,DWORD dwAtkGuildID,__time64_t tTime);	
	void SendCT_CASTLEENABLE_REQ(BYTE bWorld,BYTE bStatus,DWORD dwSecond);

	void SendCT_EVENTCHANGE_REQ(BYTE bType, LPEVENTINFO pEvent);
	void SendCT_EVENTLIST_REQ();
	void SendCT_CASHITEMLIST_REQ();
	void SendCT_EVENTQUARTERUPDATE_REQ(BYTE bGroup, BYTE bType, LPLUCKYEVENT pLucky);
	void SendCT_EVENTQUARTERLIST_REQ(BYTE bGroup, BYTE bDay);
	
	void SendCT_TOURNAMENTEVENT_REQ(BYTE bGroupID, BYTE bType,LPTOURNAMENT pTNT,LPTNTCHAR pTNTCHAR = NULL);
	
	void SendCT_HELPMESSAGE_REQ(BYTE bGroupID, LPHELPMSG pHelpMsg);

	void SendCT_PREVERSIONTABLE_REQ();
	void SendCT_PREVERSIONUPDATE_REQ(WORD wMoveCount, WORD wDelCount, WORD wNewCount, LPVPATFILE pPatch);

	void SendCT_RPSGAMEDATA_REQ(BYTE bGroupID);
	void SendCT_RPSGAMECHANGE_REQ(BYTE bGroupID, LPVRPSGameData pData);

	void SendCT_CMGIFT_REQ(CString strTarget, WORD wGiftID, BYTE bGroup);
	void SendCT_CMGIFTLIST_REQ(BYTE bGroup);
	void SendCT_CMGIFTCHARTUPDATE_REQ(LPQCMGIFT pQList, BYTE bGroup);
public:
	// ���·� Graph 
	BYTE	m_bGraphCheck; 
	BYTE	m_curView; 

	BYTE	GetCurView();
	void	SetCurView(BYTE bCur);

	BYTE	m_bAuthority; // �Ŵ��� ����
	DWORD	m_dwManagerID;

	// ���·� �α� ���
	BYTE	m_bLog;
	BYTE	m_bLogTime;

	CMonsterSpawn * m_pMonSpawn;

// ������
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ����
public:
	virtual ~CHappyDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// �޽��� �� �Լ��� �����߽��ϴ�.
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnServiceAlert();
	afx_msg void OnBnClickedButtonCompare();
	afx_msg void OnBnClickedButtonPatchup();
	afx_msg void OnBnClickedButtonPrepatchup();

	
public:
	TCHAR	m_szCurrentDirectory[_MAX_PATH];

	CGMToolDlg* m_pdlgGMTool;	
	SETUSERPOS m_setUSERPOS;
	void SavePos(USERPOS& _UserPos); // return ���� true�� ���ο� �� ���� false �� ������ ����
	void LoadPosListFromFile();
	void ClearPosList();
	void SavePosListToFile();
	void CopyFileNotZip();

	CRetDlg* m_pdlgRet;
	void ShowRetDlg(BYTE bShow,BYTE bRet, CString str);
};


