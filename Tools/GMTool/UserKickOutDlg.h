#pragma once


// CUserKickOutDlg ��ȭ �����Դϴ�.

class CUserKickOutDlg : public CDialog
{
	DECLARE_DYNAMIC(CUserKickOutDlg)

public:
	CUserKickOutDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CUserKickOutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_USERKICKOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strUser;
public:

	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
