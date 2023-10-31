#pragma once
#include "xlistctrl.h"


// CTGiftSet ��ȭ �����Դϴ�.

class CTGiftSet : public CDialog
{
	DECLARE_DYNCREATE(CTGiftSet)

public:
	CTGiftSet(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTGiftSet();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_GIFTSET };

	void SetData();
	void InitList();
	void InsertListItem(LPCMGIFT pGift= NULL);
	void DelteListItem(int nRow);
	void ClearList();
	void CheckValues(int nRow);
	BYTE GetListData(int nRow, TCMGIFT& tGift);
	int GetSelectItem();

public:
	CXListCtrl m_lcCMGift;
	MAPCMGIFT m_mapCMGift;
	TCMGIFT m_stDefalultGift;
	BYTE m_bGroup;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedGiftadd();
	afx_msg void OnBnClickedGiftdel();
	afx_msg void OnBnClickedGiftre();
	afx_msg void OnBnClickedGiftup();

	afx_msg LRESULT OnXCtrlUpdate(WPARAM wParam, LPARAM lParam);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
