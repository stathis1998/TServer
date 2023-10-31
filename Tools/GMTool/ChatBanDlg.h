#pragma once
#include "afxwin.h"
#include "RetDlg.h"

// CChatBanDlg ��ȭ �����Դϴ�.

class CChatBanDlg : public CDialog
{
	DECLARE_DYNAMIC(CChatBanDlg)

public:
	CChatBanDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CChatBanDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_CHATBAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strChatBanName;
	CEdit m_editChatBanName;
	CEdit m_ebBanReason;

	int m_iChatBanMin;

	CRetDlg*	m_pdlgRet;

public:
	void Init();
	void ShowDlgRet(BYTE bShow, BYTE bRet);

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();	
	
	CEdit m_ebBanTime;
};
