#pragma once
#include "afxwin.h"


// CRetDlg ��ȭ �����Դϴ�.

class CRetDlg : public CDialog
{
	DECLARE_DYNAMIC(CRetDlg)

public:
	CRetDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CRetDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_RET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_lbRet;

public:
	void SetMsg(CString str);
};
