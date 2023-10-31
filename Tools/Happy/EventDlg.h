#pragma once
#include "afxcmn.h"
#include "EventInfo.h"

// CEventDlg ��ȭ �����Դϴ�.
class CEventDlg : public CDialog
{
	DECLARE_DYNAMIC(CEventDlg)

public:
	CEventDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CEventDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_EVENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_lcEvent;
	CEventInfo* m_pdlg;

	MAPEVENTINFO m_mapEventInfo;

	void Init();
	void AddListData(LPEVENTINFO pData);
	void ClearListItem();
	DWORD ItemSelect();

	afx_msg void OnBnClickedBtnAddEvent();
	afx_msg void OnBnClickedBtnDeleteEvent();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnUpdate();
};
