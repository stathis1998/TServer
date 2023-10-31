#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CLuckyEventDlg ��ȭ �����Դϴ�.

class CLuckyEventDlg : public CDialog
{
	DECLARE_DYNAMIC(CLuckyEventDlg)

public:
	CLuckyEventDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CLuckyEventDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_LUCKYEVENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_lcLkEvent;
	CComboBox m_cbLkDay;
	CComboBox m_cbLKWorld;
	MAPBYTESTRING m_mapDay;
	WORD m_wSelectItemID;

	void Init();
	void ClearDlg();
	void ClearListItem();
	void InsertItemToListCtr(LPLUCKYEVENT pLKEVENT,int _iRow = -1 );
	void ItemSelect();
	void UpdateListData(LPLUCKYEVENT pLKEVENT);
	void DeleteListData(WORD wID);
	void FindDay(BYTE bDay, CString& strReturn);
	BYTE FindDay(CString str);
	void ClearCtrl();
	void ClearData();
	BYTE GetWorldGroup();
	BYTE ReadDlgText(LUCKYEVENT& stLKEVENT);

	afx_msg void OnBnClickedBtnLkdel();
	afx_msg void OnBnClickedBtnLkadd();
	afx_msg void OnBnClickedBtnLkUpdate();
	afx_msg void OnBnClickedBtnLkRefresh();
	afx_msg void OnNMClickLcLuckyevent(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeCbLkday();
	afx_msg void OnBnClickedChkLkann();
};
