#pragma once
#include "afxcmn.h"


// CPrograssDlg ��ȭ �����Դϴ�.

class CPrograssDlg : public CDialog
{
	DECLARE_DYNAMIC(CPrograssDlg)

public:
	CPrograssDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPrograssDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_INSTALL };
protected:
	DWORD m_dwTotal;
	DWORD m_dwCurrent;
	CProgressCtrl m_hProgressBar;

public:
	void SetTitle(CString strTitle)		{	SetWindowText(strTitle);}
	void SetText(CString strText)		{	SetDlgItemText(IDC_STATIC_NAME, strText);}
	void ShowComplete(BYTE bShow = SW_SHOW){GetDlgItem(IDC_BT_COMPLITE)->ShowWindow(bShow);}

	void SetProgressRange(DWORD dwMax);
	void SetProgressVal(DWORD dwVal);
	void AddProgressVal(DWORD dwAdd)	{	SetProgressVal(m_dwCurrent + dwAdd);}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtComplite(){ShowWindow(SW_HIDE);}
};
