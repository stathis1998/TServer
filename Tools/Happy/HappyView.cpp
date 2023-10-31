// HappyView.cpp : CHappyView Ŭ������ ����
//

#include "stdafx.h"
#include "Happy.h"

#include "HappyDoc.h"
#include "HappyView.h"
#include "MainFrm.h"
#include ".\happyview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHappyView

IMPLEMENT_DYNCREATE(CHappyView, CView)

BEGIN_MESSAGE_MAP(CHappyView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_COMMAND(ID_SERVER_CONNECT, OnServerConnect)
	ON_COMMAND(ID_SERVICE_START, OnServiceStart)
	ON_COMMAND(ID_SERVICE_STOP, OnServiceStop)
	ON_COMMAND(ID_SERVER_DISCONNECT, OnServerDisconnect)
	ON_COMMAND(ID_SERVICE_ALL_VIEW, OnServiceAllView)
	ON_COMMAND(ID_SERVICE_UPLOAD, OnServiceUpload)
	ON_COMMAND(ID_CURUSER_GRAPH, OnCuruserGraph)
	ON_COMMAND(ID_GMTOOL, OnGmtool)
	ON_COMMAND(ID_SERVCIE_LOG, OnServcieLog)
	ON_COMMAND(ID_USERWEB, OnUserweb)
	ON_COMMAND(ID_VIEW_SERVICEAUTOSTART, OnServiceAutoStart)
	ON_COMMAND(ID_SDATACLEAR, OnServiceDataClear)
	ON_COMMAND(ID_USERADMIN_EXPORTHAPPYDATA, OnExportHappydata)
	ON_COMMAND(ID_USERADMIN_CHATBANLIST, OnUseradminChatbanlist)
END_MESSAGE_MAP()

// CHappyView ����/�Ҹ�

CHappyView::CHappyView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	m_dlgGMTool = new CGMToolDlg();
	m_dlgGMTool->Create(IDD_GMTOOLDLG);

	m_dlgChatBanList = new CChatBanList();
	m_dlgChatBanList->Create(IDD_CHATBANLIST);
	m_dlgChatBanList->Init();
}

CHappyView::~CHappyView()
{
	m_dlgGMTool->DestroyWindow();
	delete m_dlgGMTool;

	m_dlgChatBanList->DestroyWindow();
	delete m_dlgChatBanList;
}

BOOL CHappyView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	// Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CHappyView �׸���

void CHappyView::OnDraw(CDC* /*pDC*/)
{
	CHappyDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CHappyView �μ�

BOOL CHappyView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CHappyView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CHappyView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CHappyView ����

#ifdef _DEBUG
void CHappyView::AssertValid() const
{
	CView::AssertValid();
}

void CHappyView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CHappyDoc* CHappyView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHappyDoc)));
	return (CHappyDoc*)m_pDocument;
}
#endif //_DEBUG

void CHappyView::OnServerConnect()
{
	m_dlgConnect.m_dwIP = GetDocument()->m_dwIP;
	m_dlgConnect.m_dwPort = GetDocument()->m_dwPort;
	m_dlgConnect.m_strUserID = GetDocument()->m_strUserID;

	if(m_dlgConnect.DoModal() == IDOK)
	{
		GetDocument()->m_strUserID = m_dlgConnect.m_strUserID;
		GetDocument()->m_strPasswd = m_dlgConnect.m_strPassword;

		SOCKADDR_IN addr;
		DWORD dwIP = m_dlgConnect.m_dwIP << 24 | (m_dlgConnect.m_dwIP << 8 & 0x00ff0000) |
			(m_dlgConnect.m_dwIP >> 8 & 0x0000ff00) | (m_dlgConnect.m_dwIP >> 24 & 0x000000ff);

		GetDocument()->m_dwIP = m_dlgConnect.m_dwIP;
		GetDocument()->m_dwPort = (WORD)m_dlgConnect.m_dwPort;

		addr.sin_addr.s_addr = dwIP;
		GetDocument()->m_strIP = inet_ntoa(addr.sin_addr);
		

		GetDocument()->SessionStart(
			inet_ntoa(addr.sin_addr),
			m_dlgConnect.m_dwPort);
		
		//--------------------------------------------------------
		CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
		pFrm->SetFtpAddAndAcc( &GetDocument()->m_dwPatchIP,
								&GetDocument()->m_strPatchUserID,
								&GetDocument()->m_dwPatchPort );		
	}
}
void CHappyView::OnServerDisconnect()
{
	int nRet = AfxMessageBox(_T("Are you sure you want to disconnect?"),MB_YESNO); // ���� ������ �����ϰڽ��ϱ�?
	if(nRet == IDYES)
	{
		GetDocument()->DisconnectServer();
		CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
		pFrm->SetPatchBarCompareVisible(FALSE);			// 06/02/02 �ڻ� ��������� Compare ��ư ��Ȱ��ȭ
		pFrm->SetPatchBarPatchUpdateVisible(FALSE);		// 06/02/02 �ڻ� ��������� PatchUpdate ��ư ��Ȱ��ȭ

		// ���·� Service Graph
		CServiceGraph * pGraph = (CServiceGraph *)pFrm->m_wndSplitter.GetView(pFrm->m_nGraph);
		pGraph->ClearGraph();		
		
		if(pFrm->GetGraphCheck())
		{	
			pFrm->SetGraphCheck();
			pFrm->m_wndSplitter.ShowView(pFrm->m_nList);
		}

		// ���·� ��ư Ȱ��ȭ/��Ȱ��ȭ
		pFrm->SetAllControlEnable(FALSE,FALSE);

		CMachineGraph * pMachine = (CMachineGraph *)pFrm->m_wndSplitter.GetView(pFrm->m_nMachineGraph);
		pMachine->ClearGraph();

		m_dlgGMTool->OnBnClickedOk();

		// ���� ������ Ÿ��Ʋ �ٲٱ�		
		CString strTitle = _T(" - Happy ");
		CWnd* pWnd = AfxGetMainWnd();	
		pWnd->SetWindowText(strTitle);
	}
}

void CHappyView::OnServiceStart()
{
	if(SVRID(GetDocument()->m_dwSelectedService) != 0)
		GetDocument()->SendCT_SERVICECONTROL_REQ(TRUE);
	else if(GetDocument()->IsGroup())
		GetDocument()->GroupStart();
	else
		AfxMessageBox(_T("Select the group/service")); //�׷�/���񽺸� �����ϼ���
}

void CHappyView::OnServiceStop()
{
	if(SVRID(GetDocument()->m_dwSelectedService) != 0)
	{
		int nRet = AfxMessageBox(_T("Are you sure you want to close the service?"),MB_YESNO);//���� ���񽺸� �����ϰڽ��ϱ�?
		if(nRet == IDYES)
			GetDocument()->SendCT_SERVICECONTROL_REQ(FALSE);
	}
	else if(GetDocument()->IsGroup())
	{
		int nRet = AfxMessageBox(_T("Are you sure you want to close the service?"),MB_YESNO);//���� ���񽺸� �����ϰڽ��ϱ�?
		if(nRet == IDYES)
			GetDocument()->GroupStop();
	}
	else
		AfxMessageBox(_T("Select the group/service"));//�׷�/���񽺸� �����ϼ���
}

void CHappyView::OnServiceAllView()
{
	CHappyDoc * pDoc = (CHappyDoc *)GetDocument();
	if(!pDoc)
		return;

	pDoc->m_bViewAllService = !pDoc->m_bViewAllService;	
}

void CHappyView::OnServiceUpload()
{
	m_dlgUpload.DoModal();

	/*
	if(IDOK == m_dlgUpload.DoModal())
	{
		CHappyDoc * pDoc = (CHappyDoc *)GetDocument();
		if(pDoc->m_fileUpload.Open(m_dlgUpload.m_strFile,CFile::modeRead|CFile::shareExclusive))
		{
			pDoc->SendCT_SERVICEUPLOADSTART_REQ(
				m_dlgUpload.m_bMachine,
				pDoc->m_fileUpload.GetFileName());

			pDoc->m_dlgUploadProgress.m_strFileName = m_dlgUpload.m_strFile;
			pDoc->m_dlgUploadProgress.m_dwFileSize = (DWORD)pDoc->m_fileUpload.GetLength();

			if(IDCANCEL == pDoc->m_dlgUploadProgress.DoModal())
			{
				if(pDoc->m_fileUpload.m_hFile != CFile::hFileNull)
				{
					pDoc->SendCT_SERVICEUPLOADEND_REQ(TRUE, NULL, 0);
					pDoc->m_fileUpload.Close();
				}
			}
		}
	}
	*/
}
///////////////////////////////////////////////////////////////////////////////
// ���·� Graph
void CHappyView::OnCuruserGraph()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CHappyDoc * pDoc = (CHappyDoc *)GetDocument();
	if(!pDoc)
		return;

	if( pDoc->GetCurView() == 2 ||
		pDoc->GetCurView() == 3 ) 
		return;

	pDoc->m_bGraphCheck = !pDoc->m_bGraphCheck;

    if( pDoc->m_bGraphCheck &&
		pDoc->GetCurView() == 0)
	{		
		pDoc->SetCurView(1); // Graph View
		CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
		pFrm->m_wndSplitter.ShowView(pFrm->m_nGraph);
	}
	else
	{
		pDoc->SetCurView(0); // List View
		CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
		pFrm->m_wndSplitter.ShowView(pFrm->m_nList);
	}
}
// ���·� ��� ��
void CHappyView::OnGmtool()
{
	if(!m_dlgGMTool)
		return;

	m_dlgGMTool->Init();
	m_dlgGMTool->ShowWindow(SW_SHOW);	
}
// ���·� �α� ���
void CHappyView::OnServcieLog()
{
	CDlgServiceLog dlg;

	CHappyDoc * pDoc = (CHappyDoc *)GetDocument();
	if(!pDoc)
		return;

	dlg.m_bStat = pDoc->m_bLog;
	if(dlg.DoModal() == IDOK)
	{
		if(dlg.m_bStat == 3)
			return;

		pDoc->m_bLog = dlg.m_bStat;
		pDoc->m_bLogTime = dlg.m_bMin;
		DWORD dwTime = dlg.m_bMin * 60000;
		
		if(dlg.m_bMin == 0 )
			pDoc->SaveLogData();
		if(pDoc->m_bLog == 0 )
			pDoc->m_bLogTime = 0;

		CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
		pFrm->SetLogTimer(pDoc->m_bLog, dwTime);

	}
}
// ���·� �������� ��
void CHappyView::OnUserweb()
{
	CString strWeb;		
	CHappyDoc * pDoc = (CHappyDoc *)GetDocument();
	if(pDoc)
		strWeb = pDoc->m_strAdminWeb;
	else
		strWeb = OPWEB_STR;
	
	ShellExecute(NULL, _T("open"), strWeb, NULL, NULL, SW_SHOWNORMAL);
}
///////////////////////////////////////////////////////////////////////////////
void CHappyView::OnServiceAutoStart()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.	
	CHappyDoc * pDoc = (CHappyDoc *)GetDocument();
	if(!pDoc)
		return;

	if( AfxMessageBox(_T("Change Service Auto Start?"), MB_YESNO) != IDYES)
		return;

	pDoc->m_bServiceAutoStart = !pDoc->m_bServiceAutoStart;
	pDoc->SendCT_SERVICEAUTOSTART_REQ(pDoc->m_bServiceAutoStart);
}

void CHappyView::OnServiceDataClear()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CHappyDoc* pDoc = (CHappyDoc*)GetDocument();
	if(!pDoc)
		return;

	if( AfxMessageBox(_T("Clear Happy Data ?"), MB_YESNO) != IDYES)
		return;

	pDoc->SendCT_SERVICEDATACLEAR_REQ();
}

void CHappyView::OnExportHappydata()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CHappyDoc* pDoc = (CHappyDoc*)GetDocument();
	if(!pDoc)
		return;

	if( AfxMessageBox(_T("Export Happy Data ?"), MB_YESNO) != IDYES)
		return;

	SYSTEMTIME ST;
	GetLocalTime(&ST);
	CString strFileName;
	strFileName.Format("%d-%d-%d %d.%d [%s].txt",ST.wYear, ST.wMonth, ST.wDay, ST.wHour, ST.wMinute, pDoc->m_strIP);
	FILE* pFile = fopen(strFileName,"wt");
	if(!pFile)
		return;

	CString strMax;
	INT dwMaxString = 0;
	MAPSERVICE::iterator itT;
	for(itT = pDoc->m_mapService.begin(); itT != pDoc->m_mapService.end(); itT++)
	{
		strMax = (*itT).second->m_strName;
		if( dwMaxString < strMax.GetLength() )
			dwMaxString = strMax.GetLength();
	}

	CString strTab= _T("\t");
	BYTE bTab = 7;
	BYTE bMaxTabCnt = BYTE((dwMaxString+1) / bTab);
	for(BYTE b = 1; b < bMaxTabCnt; b++)
		strTab += strTab;	

	fprintf(pFile,"Service %s Cur_Session \t Cur_User \t Max_User \t Active_User \t StopCount \t LatestTime \t\t PickTime \n",strTab);
	
	
	BYTE bTabCnt = 0;
	MAPGROUP::iterator itG;
	MAPSERVICE::iterator itS;
	for(itG = pDoc->m_mapGroup.begin(); itG != pDoc->m_mapGroup.end(); itG++)
	{
		fprintf(pFile,"\n[ Group %d ] \n",(*itG).second->m_bID);
        for(itS = pDoc->m_mapService.begin(); itS != pDoc->m_mapService.end(); itS++)
		{			
			LPHAPPYSERVICE pS = (*itS).second;
			if( pS->m_pGroup->m_bID != (*itG).second->m_bID || pS->m_dwStatus != DCSVC_STAT_RUNNING || pS->m_pSvrType->m_bType == SVRGRP_LOG )
				continue;
			
			strTab = _T("\t");
			CString strLStart,strPTime;
			CTime sTime(pS->m_nStopTime);
			CTime pTime(pS->m_nPickTime);

			bTabCnt = BYTE((pS->m_strName.GetLength()+1) / bTab);
			while(bTabCnt < bMaxTabCnt && bTabCnt > 0)
			{
				strTab += strTab;
				bTabCnt++;
			}

			strLStart.Format("%d-%d-%d %d:%d",sTime.GetYear(),sTime.GetMonth(),sTime.GetDay(),sTime.GetHour(),sTime.GetMinute());
			strPTime.Format("%d-%d-%d %d:%d",sTime.GetYear(),sTime.GetMonth(),sTime.GetDay(),sTime.GetHour(),sTime.GetMinute());
            
			fprintf(pFile," %s %s %d \t\t %d \t\t %d \t\t %d \t\t %d \t\t %s \t %s \n",
				pS->m_strName,strTab,pS->m_dwCurSession,pS->m_dwCurUser,pS->m_dwMaxUser,pS->m_dwActiveUser,pS->m_dwStopCount, strLStart,strPTime);
		}
	}
	
	fclose(pFile);
}

void CHappyView::OnUseradminChatbanlist()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	if(!m_dlgChatBanList)
		return;
	
	m_dlgChatBanList->ShowWindow(SW_SHOW);
}
