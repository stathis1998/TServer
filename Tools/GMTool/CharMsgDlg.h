#pragma once


// CCharMsgDlg ��ȭ �����Դϴ�.

class CCharMsgDlg : public CDialog
{
	DECLARE_DYNAMIC(CCharMsgDlg)

public:
	CCharMsgDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CCharMsgDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_CHARMSG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
