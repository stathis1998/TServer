#pragma once
#include "afxcmn.h"

// CTCharDlg ��ȭ �����Դϴ�.

class CTCharDlg : public CDialog
{
	DECLARE_DYNAMIC(CTCharDlg)

public:
	CTCharDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTCharDlg();

public:
	LPTCHARACTER m_pTCHAR;
	VTCHARACTER m_vTCHAR;
	CListCtrl m_cTCHAR;

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_CHAR };

public:
	void ClearTCHAR();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
