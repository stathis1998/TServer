#pragma once
#include "afxcmn.h"


// CPosListDlg ��ȭ �����Դϴ�.

class CPosListDlg : public CDialog
{
	DECLARE_DYNAMIC(CPosListDlg)

public:
	CPosListDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPosListDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_POSLISTDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listctrPos;
	DWORD m_dwSelectIndex;

	void Init(); // ����Ʈ �ڽ� �ʱ� ����
	void LoadPosListFromData();
	void ItemDelete();
	bool ItemSelect();
	void ClearListItem(); // ����Ʈ ������ ��� ����
	void InsertItemToListCtr(int _iRow, LPUSERPOS _USERPOS);

	BYTE FindListData(LPUSERPOS pPos);
	void DeleteListData(LPUSERPOS pPos);

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedPosfind();
};
