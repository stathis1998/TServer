#pragma once
#include "afxcmn.h"

// CTGroupDlg ��ȭ �����Դϴ�.

class CTGroupDlg : public CDialog
{
	DECLARE_DYNAMIC(CTGroupDlg)

public:
	CTGroupDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTGroupDlg();

public:
	CListCtrl m_cTGROUP;
	VTGROUP m_vTGROUP;
	LPTGROUP m_pTGROUP;

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_GROUP };

public:
	void ClearTGROUP();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
