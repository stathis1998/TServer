#pragma once
#include "afxcmn.h"

// CTChannelDlg ��ȭ �����Դϴ�.

class CTChannelDlg : public CDialog
{
	DECLARE_DYNAMIC(CTChannelDlg)

public:
	CTChannelDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTChannelDlg();

public:
	LPTCHANNEL m_pTCHANNEL;
	CListCtrl m_cTCHANNEL;
	VTCHANNEL m_vTCHANNEL;

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_CHANNEL };

public:
	void ClearTCHANNEL();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
