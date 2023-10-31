#pragma once
#include "afxcmn.h"


class CTChatClientView;

// CTChatShopDlg ��ȭ �����Դϴ�.

class CTChatShopDlg : public CDialog
{
	DECLARE_DYNAMIC(CTChatShopDlg)

public:
	CTChatShopDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTChatShopDlg();

public:
	CTChatClientView *m_pHost;
	CString m_strTSHOPNAME;

	CListCtrl m_cInvenList;
	CListCtrl m_cItemList;
	CListCtrl m_cShopList;

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_INVEN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnLvnItemchangedListInven(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonToshop();
	afx_msg void OnBnClickedButtonToinven();
protected:
	virtual void OnOK();
};
