// ���·� UserKickout.h

#pragma once


// CUserKickout ��ȭ �����Դϴ�.

class CUserKickout : public CDialog
{
	DECLARE_DYNAMIC(CUserKickout)

public:
	CUserKickout(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CUserKickout();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_USERKICKOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strUser;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
