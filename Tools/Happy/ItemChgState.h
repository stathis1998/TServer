#pragma once
#include "afxwin.h"
#include "afxcmn.h"



// CItemChgState ��ȭ �����Դϴ�.

class CItemChgState : public CDialog
{
	DECLARE_DYNAMIC(CItemChgState)

public:
	CItemChgState(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CItemChgState();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ITEMSTATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:

	CEdit m_ebInput;
	CStatic m_lblFindType;
	CListCtrl m_lcFindList;
	CComboBox m_cbWorld;

	BYTE	m_bFindType;
	BYTE	m_bEnable;
	BYTE	m_bMaxFindListColumn;
	BYTE	m_bWorldID;

public:
	void Init();
	void SetWorld();
	void InitFindListCtrl();
	void InsertItemToListCtr(ITEMSTATE* pITEMSTATE, int _iRow = -1);
	void ChangeItemState(WORD wItemID, BYTE bInitState);
	bool ItemSelect();
	void ItemMultiSelect();
	void ClearListItem();
	void ResultItemState(WORD wCount);

public:
	afx_msg void OnBnClickedRaItid();
	afx_msg void OnBnClickedRbItname();	
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnFinditem();	
	
};
