#pragma once
#include "afxwin.h"


// CSavePosDlg ��ȭ �����Դϴ�.

class CSavePosDlg : public CDialog
{
	DECLARE_DYNAMIC(CSavePosDlg)

public:
	CSavePosDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSavePosDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_SAVEPOS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	USERPOS m_stUserPos;
	CString m_strSaveName;

public:
	void SetUserPos(USERPOS& _UserPos);
	void Init();
	
	afx_msg void OnBnClickedOk();
	CEdit m_editSaveName;
	virtual BOOL OnInitDialog();
};
