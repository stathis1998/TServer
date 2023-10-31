#pragma once
#include "afxwin.h"


// CAnnounceDlg ��ȭ �����Դϴ�.

class CAnnounceDlg : public CDialog
{
	DECLARE_DYNAMIC(CAnnounceDlg)

public:
	CAnnounceDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CAnnounceDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ANNOUNCE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CButton m_chkTimer;
	DWORD m_dwTimerMin;
	CComboBox m_comboWorld;

	CString m_strAnnounce;
	DWORD	m_nID;
	BOOL	m_bTimer;

	void	SetWorld();

	afx_msg void OnBnClickedBtnTimeSelect();
	afx_msg void OnBnClickedBtnTimerEnd();	
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
};
