#pragma once
#include "afxcmn.h"
#include "afxwin.h"

// CMonsterSpawn ��ȭ �����Դϴ�.

class CMonsterSpawn : public CDialog
{
	DECLARE_DYNAMIC(CMonsterSpawn)

public:
	CMonsterSpawn(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMonsterSpawn();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MONSTERSPAWN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	DWORD m_dwMapID;
	DWORD m_dwSpawnID;

	DWORD m_dwTriggerID;
	DWORD m_dwHostID;
	DWORD m_dwRHID;
	DWORD m_dwRHType;

	CComboBox m_comboWorld;
	void SetWorld();

	CListCtrl m_listMon;
	void AddItemData(DWORD dwMonID, DWORD dwHostID, BYTE bStat, float fPosX, float fPosY, float fPosZ);
	
	CComboBox m_comboAction;

	DWORD m_dwChannel;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
