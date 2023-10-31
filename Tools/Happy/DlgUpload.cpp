// DlgUpload.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Happy.h"
#include "DlgUpload.h"
#include "MainFrm.h"
#include "HappyDoc.h"

// CDlgUpload ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgUpload, CDialog)
CDlgUpload::CDlgUpload(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUpload::IDD, pParent)
{
}

CDlgUpload::~CDlgUpload()
{
}

void CDlgUpload::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_MACHINE, m_comboBox);
	DDX_Control(pDX, IDC_LIST_MACHINESERVICE, m_listBox);
}


BEGIN_MESSAGE_MAP(CDlgUpload, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnBnClickedButtonBrowse)
	ON_CBN_SELCHANGE(IDC_COMBO_MACHINE, OnCbnSelchangeComboMachine)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgUpload �޽��� ó�����Դϴ�.

void CDlgUpload::OnBnClickedButtonBrowse()
{
	CFileDialog dlg(TRUE);
	if(dlg.DoModal() == IDOK)
	{
		CWnd *pWnd = GetDlgItem(IDC_EDIT_FILENAME);
		ASSERT(pWnd);

		CString strPathName = dlg.GetPathName();
		pWnd->SetWindowText( strPathName );
	}
}

BOOL CDlgUpload::OnInitDialog()
{
	CDialog::OnInitDialog();

	CMainFrame * pWnd = (CMainFrame *)GetParent();
	CHappyDoc * pDoc = (CHappyDoc *)pWnd->GetFirstDocument();

	MAPMACHINE::iterator it;
	for(it=pDoc->m_mapMachine.begin(); it!=pDoc->m_mapMachine.end(); it++)
	{
		LPHAPPYMACHINE pMachine = (*it).second;
		int nIndex = m_comboBox.AddString(pMachine->m_strName);
		m_comboBox.SetItemData(nIndex, pMachine->m_bID);
	}
	m_comboBox.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CDlgUpload::OnCbnSelchangeComboMachine()
{
	int nIndex = m_comboBox.GetCurSel();
	DWORD dwID = (DWORD)m_comboBox.GetItemData(nIndex);
	CMainFrame * pWnd = (CMainFrame *)GetParent();
	CHappyDoc * pDoc = (CHappyDoc *)pWnd->GetFirstDocument();

	m_listBox.ResetContent();

	MAPSERVICE::iterator it;
	CString strAlert;
	for(it=pDoc->m_mapService.begin(); it!=pDoc->m_mapService.end(); it++)
	{
		if((*it).second->m_pMachine->m_bID == dwID)
		{
			m_listBox.AddString((*it).second->m_strName);
			if((*it).second->m_dwStatus != DCSVC_STAT_STOPPED)
				strAlert += (*it).second->m_strName + _T("\n");
		}
	}
	if(!strAlert.IsEmpty())
	{
		strAlert += _T("Refused to access to service or under execution");//���񽺿� �������� �źεǾ��ų� ������ �Դϴ�
		AfxMessageBox(strAlert);
	}
}

void CDlgUpload::OnBnClickedOk()
{
	int nIndex = m_comboBox.GetCurSel();
	if(nIndex <= 0)
		AfxMessageBox(_T("Select the machine"));//Machine�� �����ϼ���
	else
	{
		CWnd *pWnd = GetDlgItem(IDC_EDIT_FILENAME);
		pWnd->GetWindowText(m_strFile);
		
		if(m_strFile.IsEmpty())
			AfxMessageBox(_T("Select the upload file"));//Upload ������ �����ϼ���
		else
		{
			m_bMachine = (BYTE)m_comboBox.GetItemData(nIndex);
			UploadService();
			//OnOK();
		}
	}
}

void CDlgUpload::UploadService()
{
	CMainFrame * pWnd = (CMainFrame *)GetParent();
	CHappyDoc * pDoc = (CHappyDoc *)pWnd->GetFirstDocument();
	if(pDoc->m_fileUpload.Open(m_strFile,CFile::modeRead|CFile::shareExclusive))
	{
		pDoc->SendCT_SERVICEUPLOADSTART_REQ(
			m_bMachine,
			pDoc->m_fileUpload.GetFileName());

		pDoc->m_dlgUploadProgress.m_strFileName = m_strFile;
		pDoc->m_dlgUploadProgress.m_dwFileSize = (DWORD)pDoc->m_fileUpload.GetLength();

		if(IDCANCEL == pDoc->m_dlgUploadProgress.DoModal())
		{
			if(pDoc->m_fileUpload.m_hFile != CFile::hFileNull)
			{
				pDoc->SendCT_SERVICEUPLOADEND_REQ(TRUE, NULL, 0);
				pDoc->m_fileUpload.Close();
			}
		}
	}
}