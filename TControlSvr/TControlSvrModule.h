#pragma once

#include "TServerSystem.h"

#define ON_RECEIVE(p)							case p : return On##p(pBUF);

////////////////////// �ſ�ſ� �߿� - ���� ������ /////////////////////////////////////
//
// 1. �ڱ��ڽ��� ���� �Ϸ��� �ڵ鷯�Լ����� EC_SESSION_INVALIDCHAR�� �����Ѵ�.
//    (�ڵ鷯 �Լ��� �Ķ���ͷ� �Ѿ�� ���� pBUF->m_pSESSION�� ����)
//
// 2. �ٸ� ������ ���� �Ϸ��� �������� �Ķ���ͷ� �Ͽ� CloseSession()�� ȣ���Ѵ�.
//    (�� : CloseSession(pTarget); pTarget�� ������ �����ϵ��� �����ȴ�)
//
// 3. �� �̿��� ������� ���� ������ �����ϸ� �ȵȴ�.
//
//////////////////////////////////////////////////////////////////////////////////////////

class CTControlSvrModule : public CAtlServiceModuleT< CTControlSvrModule, IDS_SERVICENAME >, public TServerSystem
{
protected:
	QPACKETBUF m_qBATCHJOB;					// queue for batch job
	QPACKETBUF m_qSMJOB;					// queue for SM job	

	MAPTSVRTEMP m_mapTSVRTEMP;				// Server template data
	MAPTMANAGER m_mapSESSION;				// all session pool
	MAPTMANAGER m_mapMANAGER;				// valid manager pool
	//MAPTMANAGERTEMP m_mapMANTEMP;			// ���·� �Ŵ��� ����
	MAPPLATFORM m_mapPLATFORM;				// ���·� Performance

	MAPTMACHINE m_mapTMachine;
	MAPTGROUP m_mapTGroup;
	MAPTSVRTYPE m_mapTSvrType;

	MAPBANINFO	m_mapBanInfo;
	MAPEVENTINFO m_mapEVENT;

	CRITICAL_SECTION m_csSMQUEUE;			// sync object for SM queue
	CRITICAL_SECTION m_csQUEUE;				// sync object for batch queue
	CRITICAL_SECTION m_csBATCH;				// sync object for batch thread

	CString m_strUploadFileName;
	CString m_strUploadFilePath;
	CString m_strMyAddr;
	FILE * m_fileTemp;

protected:
	string m_szDBUserID;
	string m_szDBPasswd;
	string m_szDSN;
	int m_wPort;

	BYTE m_bNumWorker;
	BYTE m_bThreadRun;
	BYTE m_bBatchRun;
	BYTE m_bSMRun;
	BYTE m_bTimerRun;

	HANDLE m_hWorker[MAX_THREAD];
	HANDLE m_hControl;
	HANDLE m_hBatch;
	HANDLE m_hSM;
	HANDLE m_hTimer;

	HANDLE m_hBatchEvent;
	HANDLE m_hSMEvent;
	HANDLE m_hTimerEvent;

	HANDLE m_hIocpControl;
	HANDLE m_hIocpWork;
	SOCKET m_accept;

	CSqlDatabase m_db;
	CSession m_listen;

	COverlappedEx m_ovAccept;
	CPacket m_vAccept;

	BYTE m_bAutoStart; // ���� �������� �� �ڵ����� �ٽ� �����ϱ�.
	DWORD m_dwManagerSeq;
	DWORD m_dwChatBanSeq;
	DWORD m_dwSendCount;
	BYTE m_bChatBanSuccess;
	DWORD m_dwEventIndex;

	CDebugSocket *m_pDebugSocket;
	SOCKADDR_IN	m_svraddr;

protected:
	void UploadEnd();	
	BYTE StartService(LPTSVRTEMP pService);
	BYTE StopService(LPTSVRTEMP pService);
	void QueryStatus();
	void QueryPlatform();	
	void OpenPlatform();

	LPTSVRTEMP FindService(BYTE bGroup, BYTE bType, BYTE bID);
	LPTSVRTEMP FindService(CTServer * pServer);
	LPTSVRTEMP FindService(DWORD dwID);
protected:
	DWORD OnSM_DELSESSION_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_OPLOGIN_REQ( LPPACKETBUF pBUF);
	DWORD OnCT_SERVICESTAT_REQ( LPPACKETBUF pBUF);
	DWORD OnCT_SERVICECONTROL_REQ( LPPACKETBUF pBUF);
	DWORD OnCT_NEWCONNECT_REQ( LPPACKETBUF pBUF);
	DWORD OnCT_SERVICECHANGE_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_TIMER_REQ(LPPACKETBUF pBUF);	
	DWORD OnCT_SERVICEMONITOR_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_SERVICEUPLOADSTART_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_SERVICEUPLOAD_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_SERVICEUPLOADEND_REQ(LPPACKETBUF	pBUF);
	DWORD OnCT_UPDATEPATCH_REQ(LPPACKETBUF pBUF); // ���·� CT_UPDATEPATCH_REQ
	DWORD OnCT_ANNOUNCEMENT_REQ(LPPACKETBUF pBUF); // ���·� CT_ANNOUNCEMENT_REQ
	DWORD OnCT_USERKICKOUT_REQ(LPPACKETBUF pBUF); // ���·� CT_USERKICKOUT_REQ
	DWORD OnCT_USERMOVE_REQ(LPPACKETBUF pBUF); // ���·� CT_USERMOVE_REQ
	DWORD OnCT_USERPOSITION_REQ(LPPACKETBUF pBUF); // ���·� CT_USERPOSITION_REQ
	DWORD OnCT_MONSPAWNFIND_REQ(LPPACKETBUF pBUF); // ���·� CT_MONSPAWNFIND_REQ
	DWORD OnCT_MONSPAWNFIND_ACK(LPPACKETBUF pBUF); // ���·� CT_MONSPAWNFIND_ACK
	DWORD OnCT_MONACTION_REQ(LPPACKETBUF pBUF); // ���·� CT_MONACTION_REQ
	DWORD OnCT_STLOGIN_REQ(LPPACKETBUF pBUF); // ���·� CT_STLOGIN_REQ
	DWORD OnCT_PLATFORM_REQ(LPPACKETBUF pBUF); // ���·� CT_PLATFORM_REQ
	DWORD OnCT_USERPROTECTED_REQ(LPPACKETBUF pBUF); // ���·� CT_USERPROTECTED_REQ
	DWORD OnCT_CHARMSG_REQ(LPPACKETBUF pBUF); // ���·� CT_CHARMSG_REQ
	DWORD OnCT_RECONNECT_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_SERVICEAUTOSTART_REQ(LPPACKETBUF pBUF); //���� �ڵ����� ��� ��û.
	DWORD OnCT_CHATBAN_REQ(LPPACKETBUF pBUF); //ä�ñ�����û.
	DWORD OnCT_CHATBAN_ACK(LPPACKETBUF pBUF);
	DWORD OnCT_SERVICEDATACLEAR_REQ(LPPACKETBUF pBUF); 
	DWORD OnCT_ITEMFIND_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_ITEMSTATE_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_ITEMFIND_ACK(LPPACKETBUF pBUF); // World -> Control
	DWORD OnCT_ITEMSTATE_ACK(LPPACKETBUF pBUF); // World -> Control
	DWORD OnCT_CHATBANLIST_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_CHATBANLISTDEL_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_CASTLEINFO_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_CASTLEINFO_ACK(LPPACKETBUF pBUF);
	DWORD OnCT_CASTLEGUILDCHG_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_CASTLEGUILDCHG_ACK(LPPACKETBUF pBUF);
	DWORD OnCT_CASTLEENABLE_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_EVENTUPDATE_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_EVENTCHANGE_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_EVENTMSG_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_EVENTLIST_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_EVENTDEL_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_CASHSHOPSTOP_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_CASHITEMSALE_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_CASHITEMLIST_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_CASHITEMSALE_ACK(LPPACKETBUF pBUF);
	DWORD OnCT_EVENTQUARTERLIST_REQ(LPPACKETBUF pBUF);  
	DWORD OnCT_EVENTQUARTERLIST_ACK(LPPACKETBUF pBUF);
	DWORD OnCT_EVENTQUARTERUPDATE_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_EVENTQUARTERUPDATE_ACK(LPPACKETBUF pBUF);
	DWORD OnCT_TOURNAMENTEVENT_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_TOURNAMENTEVENT_ACK(LPPACKETBUF pBUF);
	DWORD OnCT_HELPMESSAGE_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_PREVERSIONUPDATE_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_PREVERSIONTABLE_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_RPSGAMEDATA_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_RPSGAMECHANGE_REQ(LPPACKETBUF pBUF);
	DWORD OnCT_RPSGAMEDATA_ACK(LPPACKETBUF pBUF);	
	DWORD OnCT_CMGIFT_REQ(LPPACKETBUF pBUF);	// ����� ����
	DWORD OnCT_CMGIFT_ACK(LPPACKETBUF pBUF);	// ���Ƿ� ����
	DWORD OnCT_CMGIFTLIST_REQ(LPPACKETBUF pBUF);	// ���Ƿ� ����Ʈ ����
	DWORD OnCT_CMGIFTLIST_ACK(LPPACKETBUF pBUF);	// ���Ƿ� ����Ʈ ����
	DWORD OnCT_CMGIFTCHARTUPDATE_REQ(LPPACKETBUF pBUF);	// db������Ʈ��, ���嵥���� ������Ʈ

protected:
	void ProcessSession( CTControlSession *pSession, DWORD dwIoBytes);
	void OnSendComplete( CTControlSession *pSession, DWORD dwIoBytes);

	void OnInvalidSession( CTControlSession *pServer);			// ��� ����
	void OnCloseSession( CTControlSession *pServer);			// ��� ����
	void ClosingSession( CTControlSession *pServer);			// ��� ����
	void CloseSession( CTControlSession *pServer);				// ������ �����Ϸ��� �� �Լ��� ȣ��

	void SayToBATCH( LPPACKETBUF pBUF);
	void SayToSM( LPPACKETBUF pBUF);
	

	void ClearThread();
	void UpdateData();

	DWORD CreateThreads();
	BYTE ResumeThreads();

	DWORD LoadConfig();
	DWORD LoadData();

	DWORD InitNetwork();
	DWORD InitDB();

	BYTE WaitForConnect();
	BYTE Accept();

	DWORD OnReceive( LPPACKETBUF pBUF);

	DWORD ControlThread();
	DWORD BatchThread();
	DWORD WorkThread();
	DWORD SMThread();
	DWORD TimerThread();
	
	void SendSvrStatusSMS(BYTE _bSvrType, DWORD _dwSvrID, BYTE _bSvrStatus);
	CTManager* FindManager(DWORD dwID);
	CTManager* FindManager(CString strID);
	void CheckEvent();
	void SendEventToNewConnect(LPTSVRTEMP pService);
	void ParseStrValue(EVENTINFO& stEVENTINFO,CString str,BYTE bEventID);
	void MakeStrValue(EVENTINFO& stEVENTINFO,CString& str,BYTE bEventID);
	
protected:
	virtual void OnERROR( DWORD dwErrorCode);

	virtual DWORD OnEnter();
	virtual void OnExit();

private:
	static DWORD WINAPI _ControlThread( LPVOID lpParam);
	static DWORD WINAPI _BatchThread( LPVOID lpParam);
	static DWORD WINAPI _WorkThread( LPVOID lpParam);
	static DWORD WINAPI _SMThread( LPVOID lpParam);
	static DWORD WINAPI _TimerThread(LPVOID lpParam);

public:
	CTControlSvrModule();
	virtual ~CTControlSvrModule();

public:
	DECLARE_LIBID(LIBID_TControlSvrLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_TCONTROLSVR, "{A666C778-2308-47B0-A6F4-AAE1B0BB50D8}")
	HRESULT InitializeSecurity() throw()
	{
		// TODO : CoInitializeSecurity�� ȣ���ϰ� ���񽺿� 
		// �ùٸ� ���� ������
		// �����Ͻʽÿ�. PKT ���� ����, 
		// RPC_C_IMP_LEVEL_IDENTIFY ���� ���� ���� 
		// �� Null�� �ƴ� ������ ���� ������ ���� �����ϸ� �˴ϴ�.

		return S_OK;
	}
	HRESULT RegisterAppId( bool bService = false) throw()
	{
		if(!Uninstall())
			return E_FAIL;

		HRESULT hr = UpdateRegistryAppId(TRUE);
		if(FAILED(hr))
			return hr;

		CRegKey keyAppID;
		LONG lRes = keyAppID.Open( HKEY_CLASSES_ROOT, _T("AppID"), KEY_WRITE);
		if( lRes != ERROR_SUCCESS )
			return AtlHresultFromWin32(lRes);

		CRegKey key;
		lRes = key.Create( keyAppID, GetAppIdT());

		if( lRes != ERROR_SUCCESS )
			return AtlHresultFromWin32(lRes);

		key.DeleteValue(_T("LocalService"));
		if(!bService)
			return S_OK;
		key.SetStringValue(_T("LocalService"), m_szServiceName);

		// Create service
		if(!Install())
			return E_FAIL;

		return S_OK;
	}
	bool ParseCommandLine( LPCTSTR lpCmdLine, HRESULT* pnRetCode) throw()
	{
		TCHAR szTokens[] = _T("-/");
		*pnRetCode = S_OK;

		LPCTSTR lpszToken = FindOneOf( lpCmdLine, szTokens);
		while(lpszToken)
		{
			if( WordCmpI( lpszToken, _T("UnregServer")) == 0 )
			{
				*pnRetCode = UnregisterServer(TRUE);

				if(SUCCEEDED(*pnRetCode))
					*pnRetCode = UnregisterAppId();

				return false;
			}

			// Register as Local Server
			if( WordCmpI( lpszToken, _T("RegServer")) == 0 )
			{
				*pnRetCode = RegisterAppId();

				if(SUCCEEDED(*pnRetCode))
					*pnRetCode = RegisterServer(TRUE);

				return false;
			}

			if( WordCmpI( lpszToken, _T("Service")) == 0 ||
				WordCmpI( lpszToken, _T("Install")) == 0 )
			{
				*pnRetCode = RegisterAppId(true);

				if(SUCCEEDED(*pnRetCode))
					*pnRetCode = RegisterServer(TRUE);

				return false;
			}

			if( WordCmpI( lpszToken, _T("UnInstall")) == 0 )
			{
				*pnRetCode = S_OK;
				if(!Uninstall())
					*pnRetCode = E_FAIL;

				return false;
			}

			if( WordCmpI( lpszToken, _T("Name")) == 0 )
			{
				CString strNAME(FindOneOf( lpszToken, _T(" ")));
				int nPOS = 0;

				strNAME = strNAME.Tokenize( _T(" "), nPOS);
				strcpy( m_szServiceName, LPCTSTR(strNAME));
			}

			lpszToken = FindOneOf( lpszToken, szTokens);
		}

		return true;
	}
	BOOL Install() throw()
	{
		if(IsInstalled())
			return TRUE;

		// Get the executable file path
		TCHAR szFilePath[MAX_PATH + _ATL_QUOTES_SPACE];
		DWORD dwFLen = ::GetModuleFileName(NULL, szFilePath + 1, MAX_PATH);
		if( dwFLen == 0 || dwFLen == MAX_PATH )
			return FALSE;

		// Quote the FilePath before calling CreateService
		szFilePath[0] = _T('\"');
		szFilePath[dwFLen + 1] = _T('\"');
		szFilePath[dwFLen + 2] = 0;

		CString strFilePath;
		strFilePath.Format( "%s -Name %s", szFilePath, m_szServiceName);

		SC_HANDLE hSCM = ::OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS);
		if( hSCM == NULL )
		{
			TCHAR szBuf[1024];

			if( AtlLoadString( ATL_SERVICE_MANAGER_OPEN_ERROR, szBuf, 1024) == 0 )
				lstrcpy( szBuf,  _T("Could not open Service Manager"));
			MessageBox( NULL, szBuf, m_szServiceName, MB_OK);

			return FALSE;
		}

		SC_HANDLE hService = ::CreateService(
			hSCM,
			m_szServiceName,
			m_szServiceName,
			SERVICE_ALL_ACCESS,
			SERVICE_WIN32_OWN_PROCESS,
			SERVICE_DEMAND_START,
			SERVICE_ERROR_NORMAL,
			LPCSTR(strFilePath),
			NULL, NULL,
			_T("RPCSS\0"),
			NULL, NULL);

		if( hService == NULL )
		{
			::CloseServiceHandle(hSCM);
			TCHAR szBuf[1024];

			if( AtlLoadString( ATL_SERVICE_START_ERROR, szBuf, 1024) == 0 )
				lstrcpy(szBuf,  _T("Could not start service"));
			MessageBox( NULL, szBuf, m_szServiceName, MB_OK);

			return FALSE;
		}

		::CloseServiceHandle(hService);
		::CloseServiceHandle(hSCM);

		return TRUE;
	}
	HRESULT PreMessageLoop( int nShowCmd);
	HRESULT PostMessageLoop();
};


extern CTControlSvrModule _AtlModule;
