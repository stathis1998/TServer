#pragma once
#include "afxwin.h"


// CDlgServiceLog ��ȭ �����Դϴ�.

class CDlgServiceLog : public CDialog
{
	DECLARE_DYNAMIC(CDlgServiceLog)

public:
	CDlgServiceLog(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgServiceLog();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLGSERVICELOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();

public:
	CComboBox m_comboTime;

	BYTE m_bStat;
	BYTE m_bMin;
};
