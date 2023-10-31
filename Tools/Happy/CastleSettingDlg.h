#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "afxdtctl.h"


// CCastleSettingDlg ��ȭ �����Դϴ�.

class CCastleSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CCastleSettingDlg)

public:
	CCastleSettingDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CCastleSettingDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_CASTLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_lcCastleInfo;
	CComboBox m_cbCastleID;
	CComboBox m_comboWorld;
	CDateTimeCtrl m_datetimepicker;

	MAPCASTLEINFO m_mapCastleInfo;
	BYTE	m_bSelectWorld;

public:
	void Init();
	void InitCastleInfoListControl();
	void InsertItemToListCtr(CASTLEINFO stCASTLEINFO, int _iRow = -1);
	void InsertCastleID(WORD wID);
	void ClearListItem();
	void ClearCastleIDCombo();
	void SetWorld();
	void ChangeCastleInfo(WORD wCastleID,DWORD dwDefGuildID,CString strDefGuild,DWORD dwAtkGuildID,CString strAtkGuild,__time64_t tTime);
	void SetCastleState();
	void ClearCastleState();
	void SetEnableCastleInfoGroup(BYTE bEnable);
	void SetEnableCastleStatusGroup(BYTE bEnable);

	afx_msg void OnBnClickedBtnCastleinfo();
	afx_msg void OnBnClickedBtnChgGuild();
	afx_msg void OnBnClickedBtnChgStatus();	
protected:
	virtual void OnOK();
};
