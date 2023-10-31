// ���·� Announce.h

#pragma once
#include "afxwin.h"

#define MAX_WORLD 10

// CAnnounce ��ȭ �����Դϴ�.

class CAnnounce : public CDialog
{
	DECLARE_DYNAMIC(CAnnounce)

public:
	CAnnounce(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CAnnounce();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ANNOUNCE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox	m_comboWorld;
	CString		m_strAnnounce;
	DWORD		m_nID;

	CButton m_checkTimer;
	DWORD m_dwTimerMin;

	BOOL m_bTimer;

	void SetWorld();

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBnClickedButtonTimerend();
	afx_msg void OnBnClickedButtonTimer();
};
