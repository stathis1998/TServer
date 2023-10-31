// UserKickOutDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "GMTool.h"
#include "UserKickOutDlg.h"
#include ".\userkickoutdlg.h"
#include "GMToolDoc.h"

// CUserKickOutDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CUserKickOutDlg, CDialog)
CUserKickOutDlg::CUserKickOutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserKickOutDlg::IDD, pParent)
	,m_strUser		(_T(""))
{
}

CUserKickOutDlg::~CUserKickOutDlg()
{
}

void CUserKickOutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUserKickOutDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CUserKickOutDlg �޽��� ó�����Դϴ�.

void CUserKickOutDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	
	GetDlgItemText(IDC_EDIT1, m_strUser);
	m_strUser.Trim(_T(" "));

	if(m_strUser == _T(""))
	{
		MessageBox(MSG_CHARNAME_NULL, MB_OK);
		return;
	}

	CGMToolDoc* pDoc = CGMToolDoc::GetInstance();
	if(!pDoc)
		return;
	pDoc->SendCT_USERKICKOUT_REQ(m_strUser);
	OnOK();
}

BOOL CUserKickOutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
