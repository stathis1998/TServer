#pragma once
#include "afxwin.h"
#include "afxdtctl.h"


// CHelpMessage ��ȭ �����Դϴ�.

class CHelpMessage : public CDialog
{
	DECLARE_DYNAMIC(CHelpMessage)

public:
	CHelpMessage(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CHelpMessage();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_HELPINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	
	DECLARE_MESSAGE_MAP()

public:
	CComboBox m_cbWorld;
	CComboBox m_cbType;
	CDateTimeCtrl m_dtStart;
	CDateTimeCtrl m_dtEnd;

public:
	void	Init();
	void	SetShow(BYTE bShow);
	BYTE	GetWorld(BYTE& p_bOut);	

	afx_msg void OnBnClickedBtnHsend();
	afx_msg void OnCbnSelchangeCbHtype();
};
