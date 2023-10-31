// DlgUploadProgress.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Happy.h"
#include "DlgUploadProgress.h"
#include ".\dlguploadprogress.h"

// CDlgUploadProgress ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgUploadProgress, CDialog)
CDlgUploadProgress::CDlgUploadProgress(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUploadProgress::IDD, pParent)
{
}

CDlgUploadProgress::~CDlgUploadProgress()
{

}

void CDlgUploadProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_SERVICEUPLOAD, m_progress);
}


BEGIN_MESSAGE_MAP(CDlgUploadProgress, CDialog)
END_MESSAGE_MAP()


// CDlgUploadProgress �޽��� ó�����Դϴ�.

BOOL CDlgUploadProgress::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_progress.SetRange(0, 100);
	CEdit * pEdit = (CEdit *)GetDlgItem(IDC_EDIT_CURFILENAME);
	CString strText;
	strText.Format("%s(0/%d)",m_strFileName,m_dwFileSize);
	pEdit->SetWindowText(strText);

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
void CDlgUploadProgress::SetPos(DWORD dwPos)
{
	m_progress.SetPos(dwPos);
}
