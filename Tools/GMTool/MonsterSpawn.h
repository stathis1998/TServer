#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CMonsterSpawnDlg ��ȭ �����Դϴ�.

class CMonsterSpawnDlg : public CDialog
{
	DECLARE_DYNAMIC(CMonsterSpawnDlg)

public:
	CMonsterSpawnDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMonsterSpawnDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MONSPAWN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_comboWorld;
	CComboBox m_comboAction;

	DWORD m_dwMapID;
	DWORD m_dwChannel;
	DWORD m_dwSpawnID;
	DWORD m_dwTriggerID;
	DWORD m_dwHostID;
	DWORD m_dwRHID;
	DWORD m_dwRHType;

	CListCtrl m_listMon;

	void SetWorld();
	void AddItemData(DWORD dwMonID, DWORD dwHostID, BYTE bStat, float fPosX, float fPosY, float fPosZ);
	void UpdateData();

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnSearchMonster();
	afx_msg void OnBnClickedBtnAction();
	afx_msg void OnBnClickedCancel();

};
