#pragma once
#include "afxwin.h"

// CDlgUpload ��ȭ �����Դϴ�.

class CDlgUpload : public CDialog
{
	DECLARE_DYNAMIC(CDlgUpload)

public:
	CDlgUpload(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgUpload();

	CString m_strFile;
	BYTE m_bMachine;

	void UploadService();
// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_UPLOAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonBrowse();
	virtual BOOL OnInitDialog();
	CComboBox m_comboBox;
	afx_msg void OnCbnSelchangeComboMachine();
	CListBox m_listBox;
	afx_msg void OnBnClickedOk();
};
