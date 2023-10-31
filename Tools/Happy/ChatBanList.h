#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CChatBanList ��ȭ �����Դϴ�.

class CChatBanList : public CDialog
{
	DECLARE_DYNAMIC(CChatBanList)

public:
	CChatBanList(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CChatBanList();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_CHATBANLIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_lcChatBanList;

	MAPBANLIST	m_mapBANLIST;

public:
	void Init();
	void InsertToListCtrl(DWORD dwID,CString strName, __time64_t time, WORD wMin, CString strReason,CString strOPName);
	void ClearListCtrl();
	void ItemDelete();
	void ClearAllItem();
	void SelectItem();

public:
	afx_msg void OnBnClickedBtnViewbanlist();
	afx_msg void OnBnClickedBtnDeleteban();
	afx_msg void OnBnClickedBtnClearbanlist();
	afx_msg void OnBnClickedCancel();	
	afx_msg void OnNMClickLcBanlist(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);	
	CEdit m_ebBanReason;
};
