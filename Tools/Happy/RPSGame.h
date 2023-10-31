#pragma once
#include "XListCtrl.h"
#include "afxwin.h"

// CRPSGame ��ȭ �����Դϴ�.

class CRPSGame : public CDialog
{
	DECLARE_DYNAMIC(CRPSGame)

public:
	VRPSGameData m_vRpsList;
	vector<BYTE> m_vType;
	CXListCtrl	 m_lcRPCList;
	CComboBox	 m_cbWorld;
	CComboBox	 m_cbType;

	BYTE		 m_bSelectWorld;
	BYTE		 m_bSelectType;
	BYTE		 m_bTypeCur;
	BYTE		 m_bView;

public:
	CRPSGame(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CRPSGame();
	enum { IDD = IDD_RPSGAME };

	void Init();
	void InitRPSList();
	int	 AddEventList(LPRPSGameData pData);
	void ClearList();
	void RefreshList();
	void InitWorld();
	void InitType();
	void AddType(BYTE bType);
	int GetSelectItem();
	void CheckValue(int nRow, int nCol);
	LPRPSGameData FindRPSData(BYTE bType, BYTE bRound);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeWorld();
	afx_msg void OnCbnSelchangeType();
	afx_msg void OnNMClickLcRPSList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtRpsupdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
