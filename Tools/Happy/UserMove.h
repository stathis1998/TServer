// ���·� UserMove.h

#pragma once
#include "afxwin.h"


#include "SavePos.h"
#include "PosListDlg.h"
#include "afxcmn.h"

// CUserMove ��ȭ �����Դϴ�.

class CUserMove : public CDialog
{
	DECLARE_DYNAMIC(CUserMove)

public:
	CUserMove(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CUserMove();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_USERMOVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:

	BYTE m_bType;
	CComboBox m_comboWorld;
	CListCtrl m_listctrPos;

	void SetWorld();
	
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedOk();

	afx_msg void OnBnClickedSavePos();
	afx_msg void OnBnClickedRadioCoord(); // '��ǥ�Է�' ���� ��ư�� Ŭ������ ��
	afx_msg void OnBnClickedRadioChar();  // '���ĳ����' ���� ��ư�� Ŭ������ ��
	afx_msg void OnBnClickedLoadpos(); // ���帮��Ʈ�� �����ִ� ���̾�α׸� �����ش�.

	void ShowLocationItem(BYTE bShow);	
	void Init(); // [��ǥ�Է�] , [��� ĳ���ͷ��� ��ȯ] �κ��� ��� Disable ���·� �����.
	
	void SetData(LPUSERPOS pUserPos);
	void InsertItemToListCtr(int _iRow, LPUSERPOS _USERPOS);
	void LoadPosListFromData();
	void ItemDelete();
	void ClearListItem();
	void MoveNow(LPUSERPOS pUserPos = NULL);

	BYTE FindListData(LPUSERPOS pPos);
	void DeleteListData(LPUSERPOS pPos);

	afx_msg void OnBnClickedDelPos();
	afx_msg void OnNMDblclkPosfind(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedPosfind(NMHDR *pNMHDR, LRESULT *pResult);
};
