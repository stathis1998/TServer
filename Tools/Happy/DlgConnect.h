#pragma once
#include "afxwin.h"
#include "afxcmn.h"

typedef struct tagACCOUNT
{
	CString m_str[5];
	tagACCOUNT()
	{
		for(BYTE i = 0; i < 4; i++)
			m_str[i] = _T("");
	}
} ACCOUNT;

// CDlgConnect ��ȭ �����Դϴ�.

class CDlgConnect : public CDialog
{
	DECLARE_DYNAMIC(CDlgConnect)

public:
	CDlgConnect(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgConnect();

public:

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_SERVERCONNECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	DWORD m_dwIP;
	DWORD m_dwPort;
	CString m_strUserID;
	CString m_strPassword;
	map<CString,ACCOUNT> m_mapACCOUNT;	
	CComboBox m_cbAccount;


public:
	void Init();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnCbnSelchangeCombo1();
	virtual BOOL OnInitDialog();
};
