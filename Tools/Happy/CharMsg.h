#pragma once


// CCharMsg ��ȭ �����Դϴ�.

class CCharMsg : public CDialog
{
	DECLARE_DYNAMIC(CCharMsg)

public:
	CCharMsg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CCharMsg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_CHARMSG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
