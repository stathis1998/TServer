#pragma once
#include "afxwin.h"

#include "TChatSocket.h"

// CPingTestDlg ��ȭ �����Դϴ�.

#define MAXCOLOR		5
#define XINTERVAL		10
#define MAXPINGCOUNT	1000

typedef vector< DWORD > VECDWORD; 
class CPingTestDlg : public CDialog
{
	DECLARE_DYNAMIC(CPingTestDlg)

public:
	CPingTestDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPingTestDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_PINGTEST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CButton m_chkPingLoop;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnSend();
	afx_msg void OnBnClickedChkPingloop();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL OnInitDialog();

public:
	CTChatSocket* m_pTSocket;
	DWORD m_dwTimer;
	DWORD m_dwMaxPing;
	DWORD m_dwLimitPing;

	BYTE m_bFlag;
	BYTE m_bDrawing;

	CDC MemDC; // ���� ���۸�
	CBitmap *pOldBitmap, MemBitmap;
	CPen lineColor[MAXCOLOR];
	CRect m_rectBack;
	CRect m_rectPing;
	CRect m_rectGraph;

	VECDWORD m_vPing;
	

	void SetSocket(CTChatSocket* pTSocket);
	void SetIPPort(CString strIP, DWORD dwPort);
	void SetPing(DWORD dwPing);	

	void Init();
	void DrawGraph();
	void DrawBackGround();
	void DrawRect();
	BYTE DrawPingGraph(BYTE bColor = 0);	
	void ClearGraph();
	void ClearObject();
	
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
