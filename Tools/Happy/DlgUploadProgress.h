#pragma once
#include "afxcmn.h"

// CDlgUploadProgress ��ȭ �����Դϴ�.

class CDlgUploadProgress : public CDialog
{
	DECLARE_DYNAMIC(CDlgUploadProgress)

public:
	CDlgUploadProgress(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgUploadProgress();
	void SetPos(DWORD dwPos);
	CString m_strFileName;
	DWORD m_dwFileSize;

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_UPLOADPROGRESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
private:
	CProgressCtrl m_progress;

	virtual BOOL OnInitDialog();
};
