//---------------------------------------
// 08-01-16
// JINUK
// ä�� ���� ���̾�α�
//---------------------------------------

#pragma once
#include "afxwin.h"

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
	void Init(); 

public:
	CEdit m_editChatBanName;
	CEdit m_editChatBanMin;
	CEdit m_ebBanReason;

	CString m_strChatBanName;	
	int m_iChatBanMin;

	afx_msg void OnBnClickedOk();
	
};
