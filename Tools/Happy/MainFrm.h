// MainFrm.h : CMainFrame Ŭ������ �������̽�
//

#pragma once

#include "HappySplitter.h"
#include "PatchBar.h"

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();
	CDocument * GetFirstDocument();
	void ShowToolBar(int nIndex);
	void GetFtp(CString *strAddr, CString *strUser, CString *strPasswd, WORD *wPort);

	void GetFtpAddAndAcc(DWORD *dwAddr, CString *strUser, DWORD *dwPort);
	void SetFtpAddAndAcc(DWORD *dwAddr, CString *strUser, DWORD *dwPort);
// Ư��
public:
	CHappySplitter m_wndSplitter;
	int m_nGroup;
	int m_nTree;
	int m_nList;
	int m_nPatch;
	int m_nListPatch;

	///////////////////////////////////////////////////////////
	// ���·� Graph
	BYTE m_bGraph;
	int m_nGraph; 
	BYTE GetCurView();
	void SetCurView(BYTE bCur);
	void SetGraphButton(BYTE bGraph);	
	
	BYTE GetGraphCheck(); // ���·� Graph
	void SetGraphCheck();

	// ���·� �溸��, ���� ��ü����, ���� ����/���� ��ư Ȱ��ȭ/��Ȱ��ȭ
	BYTE m_bAllView;
	BYTE m_bAlert;
	BYTE m_bService;
	BYTE m_bAutoStart;
	BYTE m_bServiceDataClear;
	BYTE m_bExportHappyData;
	BYTE m_bChatBanList;

	DWORD m_dwAutoNormal;
	DWORD m_dwAutoPush;

	void SetAllView(BYTE bAllView);
	void SetAlertButton(BYTE bAlert);
	void SetServiceButton(BYTE bService);
	void SetAutoStart(BYTE _bAutoStart);
	void SetServiceDataClear(BYTE bView);
	void SetExportHappyData(BYTE bView);
	void SetChatBanList(BYTE bView);

	// ���·� �������� Ȱ��ȭ/��Ȱ��ȭ
	BYTE m_bLog;
	BYTE m_bUserWeb;
	BYTE m_bGMTool;

	void SetServiceLog(BYTE bLog);
	void SetUserWeb(BYTE bWeb);
	void SetGMTool(BYTE bGMTool);

	void SetLogTimer(BYTE bResult, DWORD dwTime);

	void SetAllControlEnable(BYTE bEnable, BYTE bServiceChange = TRUE );
	// ���·� Machine
	int m_nMachine;
	int m_nMachineGraph;
	///////////////////////////////////////////////////////////

// ������
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ����
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // ��Ʈ�� ������ ���Ե� ����Դϴ�.
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CPatchBar	m_wndPatchBar;

// �޽��� �� �Լ��� �����߽��ϴ�.
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnUpdateServerConnect(CCmdUI *pCmdUI);
	afx_msg void OnUpdateServiceStart(CCmdUI *pCmdUI);
	afx_msg void OnUpdateServiceStop(CCmdUI *pCmdUI);
	afx_msg void OnUpdateServerDisconnect(CCmdUI *pCmdUI);
	afx_msg void OnUpdateServiceAlert(CCmdUI *pCmdUI);
	afx_msg void OnUpdateServiceAllView(CCmdUI *pCmdUI);	
	void SetPatchBarCompareVisible(BYTE bCompare); // PatchBar�� Compare ��ư Visible Setting	
	void SetPatchBarPatchUpdateVisible(BYTE bPatchUpdate);// PatchBar�� PatchUpdate ��ư Visible Setting
	afx_msg void OnUpdateServiceUpload(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonCompare(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonPatchup(CCmdUI *pCmdUI);	
	afx_msg void OnUpdateCuruserGraph(CCmdUI *pCmdUI);// ���·� ���� �׷��� Ȱ��ȭ/��Ȱ��ȭ	
	afx_msg void OnUpdateAnnounce(CCmdUI *pCmdUI);// ���·� �������� Ȱ��ȭ/��Ȱ��ȭ
	afx_msg void OnUpdateUserkickout(CCmdUI *pCmdUI);// ���·� ���� �������� Ȱ��ȭ/��Ȱ��ȭ	
	afx_msg void OnUpdateUsermove(CCmdUI *pCmdUI);// ���·� ���� ��ġ�̵� Ȱ��ȭ/��Ȱ��ȭ	
	afx_msg void OnUpdateMonsterspawn(CCmdUI *pCmdUI);// ���·� ���� ���� Ȱ��ȭ/��Ȱ��ȭ	
	afx_msg void OnUpdateServcieLog(CCmdUI *pCmdUI);// ���·� �α� ��� Ȱ��ȭ/��Ȱ��ȭ	
	afx_msg void OnUpdateUserweb(CCmdUI *pCmdUI);// ���·� �������� �� Ȱ��ȭ/��Ȱ��ȭ	
	afx_msg void OnUpdateGmtool(CCmdUI *pCmdUI);// ���·� ��� �� Ȱ��ȭ/��Ȱ��ȭ
	afx_msg void OnUpdateServiceAutoStart(CCmdUI *pCmdUI);
	afx_msg void OnUpdateServiceDataClear(CCmdUI *pCmdUI);
	afx_msg void OnUpdateExportHappydata(CCmdUI *pCmdUI);
	afx_msg void OnUpdateUseradminChatbanlist(CCmdUI *pCmdUI);
};


