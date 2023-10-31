#pragma once
#include "afxcmn.h"


// CDlgPatchProgress ��ȭ �����Դϴ�.

class CDlgPatchProgress : public CDialog
{
	DECLARE_DYNAMIC(CDlgPatchProgress)

public:
	CDlgPatchProgress(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgPatchProgress();

	void SetProgress(BYTE bPos);
	void SetStatus(BYTE bStatus);
	void SetFileName(CString strName);

	BYTE m_bStatus;
// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_PATCHPROGRESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_progress;
	virtual BOOL OnInitDialog();
protected:
	virtual void OnCancel();
};
