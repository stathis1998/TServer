#pragma once
#include "afxcmn.h"


// CIPChangeDlg ��ȭ �����Դϴ�.

class CIPChangeDlg : public CDialog
{
	DECLARE_DYNAMIC(CIPChangeDlg)

public:
	CIPChangeDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CIPChangeDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_IP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CIPAddressCtrl m_ctrIPAddr;
	afx_msg void OnBnClickedOk();
};
