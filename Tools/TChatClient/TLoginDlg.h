#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CTLoginDlg ��ȭ �����Դϴ�.

class CTLoginDlg : public CDialog
{
	DECLARE_DYNAMIC(CTLoginDlg)

public:
	CIPAddressCtrl m_cIPAddr;
	CString m_strIPAddr;

	CString m_strID;
	CString m_strPW;
	DWORD m_dwPort;
	BYTE m_bPWEncrypt;
	

public:
	CTLoginDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTLoginDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_LOGIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	
	CButton m_chkPWEncrypt;
};
