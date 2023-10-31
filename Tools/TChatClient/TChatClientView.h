// TChatClientView.h : iCTChatClientView Ŭ������ �������̽�
//

#pragma once
#include "TChatSocket.h"
#include "TChatEdit.h"
#include "TChatInven.h"
#include "PingTestDlg.h"
#include "md5.h"

class CTChatClientView : public CView
{
protected: // serialization������ ��������ϴ�.
	CTChatClientView();
	DECLARE_DYNCREATE(CTChatClientView)

// Ư��
public:
	CTChatClientDoc* GetDocument() const;
	CTChatSocket *GetIdleSession();

	CTChatSocket m_vTSESSION[MAX_CON];
	CTChatSocket *m_pSOCKET;
	CTChatSocket *m_pRelay;
	CString m_strTARGET;
	CString m_strNAME;

	MAPTSELLITEM m_mapTSELLITEM;
	MAPTINVEN m_mapTINVEN;
	MAPCHAR m_mapTCHAR;
	VTCHATMSG m_vTCHATMSG;

	BYTE m_bChatView;
	BYTE m_bChatType;
	BYTE m_bGroupID;
	BYTE m_bChannel;
	BYTE m_bLogout;
	BYTE m_bLogin;
	BYTE m_bShop;
	BYTE m_bOpen;

	CTChatEdit m_cEditCtrl;
	CListCtrl m_cListCtrl;
		
	CString m_strID;
	CString m_strPW;

	DWORD m_dwCharID;
	DWORD m_dwUserID;
	DWORD m_dwKEY;
	BYTE m_bContryID;

	DWORD m_dwPrvTick;
	DWORD m_nLeftTick;

	CPingTestDlg* m_pdlgPingTest;
	CString		m_strIP;
	DWORD		m_dwPort;
	BYTE		m_bPingTestView;
	BYTE		m_bPWEncrypt;
	
	

// �۾�
public:
	void OnSessionMsg( CTChatSocket *pTSocket, CPacket *pPacket);
	void OnReceive( CTChatSocket *pTSocket, int nErrorCode);

	void OnConnect( CTChatSocket *pTSocket, int nErrorCode);
	void OnClose( CTChatSocket *pTSocket, int nErrorCode);
	void OnERROR( int nERROR);

	void SayToSvr( CPacket& packet, BYTE bServerID);
	void SayToAll( CPacket& packet);
	void SetChatTarget();
	void Chat( CString strTMSG);
	void ClearTCHATMSG();
	void ClearTINVEN();

	void AddChatMSG( LPTCHATMSG pTMSG);
	void ResetTCHATMSG();

	void ReleaseTITEMTEMP();
	void InitTITEMTEMP();

	void StartBeep(DWORD nDuration);
	CTChatSocket *RelaySocket();
	CString EncryptPW(CString& strSOURCE);

// ������
	public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ����
public:
	virtual ~CTChatClientView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Msg Sender (send to all enabled map)
public:
	void SendCS_CONREADY_REQ( BYTE bServerID);

// Msg Handler
public:
	int OnCS_INVALIDCHAR_ACK( CTChatSocket *pTSocket, CPacket *pPacket);
	int OnCS_ADDCONNECT_ACK( CTChatSocket *pTSocket, CPacket *pPacket);
	int OnCS_SHUTDOWN_ACK( CTChatSocket *pTSocket, CPacket *pPacket);
	int OnCS_CONNECT_ACK( CTChatSocket *pTSocket, CPacket *pPacket);

	int OnCS_CHANNELLIST_ACK( CTChatSocket *pTSocket, CPacket *pPacket);
	int OnCS_GROUPLIST_ACK( CTChatSocket *pTSocket, CPacket *pPacket);
	int OnCS_CHARLIST_ACK( CTChatSocket *pTSocket, CPacket *pPacket);
	int OnCS_LOGIN_ACK( CTChatSocket *pTSocket, CPacket *pPacket);
	int OnCS_START_ACK( CTChatSocket *pTSocket, CPacket *pPacket);
	int OnCS_CHAT_ACK( CTChatSocket *pTSocket, CPacket *pPacket);

	int OnCS_STOREOPEN_ACK( CTChatSocket *pTSocket, CPacket *pPacket);
	int OnCS_STORECLOSE_ACK( CTChatSocket *pTSocket, CPacket *pPacket);
	int OnCS_STOREITEMLIST_ACK( CTChatSocket *pTSocket, CPacket *pPacket);
	int OnCS_STOREITEMSELL_ACK( CTChatSocket *pTSocket, CPacket *pPacket);
	
	int OnCS_CHARINFO_ACK( CTChatSocket *pTSocket, CPacket *pPacket);

	int OnCS_RELAYCONNECT_ACK( CTChatSocket *pTSocket, CPacket *pPacket);
	int OnCS_RELAYCHARDATA_ACK( CTChatSocket *pTSocket, CPacket *pPacket);

	int OnCS_ENTER_ACK( CTChatSocket *pTSocket, CPacket *pPacket);
	int OnCS_LEAVE_ACK( CTChatSocket *pTSocket, CPacket *pPacket);

	int OnCT_SERVICEMONITOR_REQ(CTChatSocket *pTSocket, CPacket *pPacket);

// �޽��� �� �Լ��� �����߽��ϴ�.
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg LRESULT OnSessionMsg( WPARAM wParam, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnUpdateEditCopy(CCmdUI *pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnUpdateConLogin(CCmdUI *pCmdUI);
	afx_msg void OnConLogin();
	afx_msg void OnUpdateConLogout(CCmdUI *pCmdUI);
	afx_msg void OnConLogout();
	afx_msg void OnUpdateChattabAll(CCmdUI *pCmdUI);
	afx_msg void OnChattabAll();
	afx_msg void OnUpdateChattabAnnounce(CCmdUI *pCmdUI);
	afx_msg void OnChattabAnnounce();
	afx_msg void OnUpdateChattabNormal(CCmdUI *pCmdUI);
	afx_msg void OnChattabNormal();
	afx_msg void OnUpdateChattabWisper(CCmdUI *pCmdUI);
	afx_msg void OnChattabWisper();
	afx_msg void OnUpdateSetTarget(CCmdUI *pCmdUI);
	afx_msg void OnSetTarget();
	afx_msg void OnUpdateShopOpen(CCmdUI *pCmdUI);
	afx_msg void OnShopOpen();
	afx_msg void OnUpdateShopClose(CCmdUI *pCmdUI);
	afx_msg void OnShopClose();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnUpdateShopAutoOpen(CCmdUI *pCmdUI);
	afx_msg void OnShopAutoOpen();
	afx_msg void OnToolPingtest();
	afx_msg void OnUpdateToolPingtest(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // TChatClientView.cpp�� ����� ����
inline CTChatClientDoc* CTChatClientView::GetDocument() const
   { return reinterpret_cast<CTChatClientDoc*>(m_pDocument); }
#endif
