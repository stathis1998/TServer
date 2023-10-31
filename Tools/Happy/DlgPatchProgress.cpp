// DlgPatchProgress.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Happy.h"
#include "DlgPatchProgress.h"
#include ".\dlgpatchprogress.h"
#include "MainFrm.h"
#include "HappyDoc.h"


// CDlgPatchProgress ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgPatchProgress, CDialog)
CDlgPatchProgress::CDlgPatchProgress(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPatchProgress::IDD, pParent)
{
	m_bStatus = 0xFF;
}

CDlgPatchProgress::~CDlgPatchProgress()
{
}

void CDlgPatchProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_PATCH, m_progress);
}

void CDlgPatchProgress::SetProgress(BYTE bPos)
{
	m_progress.SetPos(bPos);
}

void CDlgPatchProgress::SetFileName(CString strName)
{
	CEdit * pEdit = (CEdit *)GetDlgItem(IDC_EDIT_PATCHFILENAME);
	pEdit->SetWindowText(strName);
}

void CDlgPatchProgress::SetStatus(BYTE bStatus)
{
	CEdit * pEdit = (CEdit *)GetDlgItem(IDC_EDIT_PATCHSTATUS);
	CString strStatus;
	switch(bStatus)
	{
	case 0: 				strStatus = _T("The patch has been successfully completed"); break; //"��ġ�� ���������� �Ϸ� �Ǿ����ϴ�."
	case 1: 				strStatus = _T("Zipping the files..."); break; //���� ���� ��..
	case 2: 				strStatus = _T("Uploading the files..."); break; //���� ���ε� ��...
	case 3: 				strStatus = _T("The error occurred during the patch"); break; //��ġ�� ������ �߻��߽��ϴ�.
	case _FTP_CONNECT:		strStatus = _T("Connecting to a FTP server "); break;//FTP������ �������Դϴ�.....
	case _ERR_TEMPDIR:		strStatus = _T("Creating a temporary directory error occurred"); break; //�ӽ� ���丮 ������ ������ �߻��߽��ϴ�.
	case _ERR_CURRENTDIR:	strStatus = _T("During set the current directory the error occurred"); break; //���� ���丮 ������ ������ �߻��߽��ϴ�.
	case _ERR_ZIP:			strStatus = _T("Generating a Zip file error occurred"); break; //Zip ���� ������ ������ �߻��߽��ϴ�.
	case _ERR_EMPTY:		strStatus = _T("IP/PORT/ACCOUNT/PASSWORD is empty"); break; //IP/PORT/ACCOUNT/PASSWORD�� ����ֽ��ϴ�.
	case _ERR_ADDRORACCOUNT:strStatus = _T("Reconfirm the IP/PORT/ACCOUNT/PASSWORD "); break; //IP/PORT/ACCOUNT/PASSWORD�� ��Ȯ�� ���ֽʽÿ�.
	case _ERR_THREADFAIL:	strStatus = _T("Failed to create a thread"); break; //Thread ������ ���� �Ͽ����ϴ�.
	case _ERR_DBWRITE:		strStatus = _T("The error has occurred during the Update DB"); break; //DB�� Update�� ������ �߻��߽��ϴ�.
	}
	m_bStatus = bStatus;
	pEdit->SetWindowText(strStatus);
}

BEGIN_MESSAGE_MAP(CDlgPatchProgress, CDialog)
END_MESSAGE_MAP()


// CDlgPatchProgress �޽��� ó�����Դϴ�.

BOOL CDlgPatchProgress::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_progress.SetRange(0,100);
	m_progress.SetPos(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CDlgPatchProgress::OnCancel()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if(m_bStatus == 0 ||
		m_bStatus == 3 ||
		m_bStatus ==_ERR_TEMPDIR ||		
		m_bStatus ==_ERR_CURRENTDIR ||
		m_bStatus ==_ERR_ZIP ||	
		m_bStatus ==_ERR_EMPTY ||		
		m_bStatus ==_ERR_ADDRORACCOUNT ||
		m_bStatus ==_ERR_THREADFAIL ||
		m_bStatus ==_ERR_DBWRITE
		)
	{
		if(m_bStatus == 0 && pDoc->m_bPatchType == 2)
			pDoc->m_dlgPreVersion->Init();//ShowWindow(SW_SHOW);
		CDialog::OnCancel();
	}
}
