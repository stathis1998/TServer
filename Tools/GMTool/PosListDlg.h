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
	
	DWORD	m_dwSelectIndex;	
	BYTE	m_bLoad;

	void Init(); // ����Ʈ �ڽ� �ʱ� ����
	void LoadPosListFromData();
	void ItemDelete();
	bool ItemSelect();
	void ItemDelete_Multi(); // ����Ʈ ��Ʈ�ѿ��� ���� ���õ� ������ ����
	void ClearListItem(); // ����Ʈ ������ ��� ����
	void InsertItemToListCtr(int _iRow, USERPOS* _USERPOS);

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedPosfind();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
};
