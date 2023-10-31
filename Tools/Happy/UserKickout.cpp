// ���·� UserKickout.cpp

// UserKickout.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Happy.h"
#include "UserKickout.h"
#include ".\userkickout.h"
#include "MainFrm.h"
#include "HappyDoc.h"


// CUserKickout ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CUserKickout, CDialog)
CUserKickout::CUserKickout(CWnd* pParent /*=NULL*/)
	: CDialog(CUserKickout::IDD, pParent)
{
}

CUserKickout::~CUserKickout()
{
}

void CUserKickout::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUserKickout, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CUserKickout �޽��� ó�����Դϴ�.

void CUserKickout::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	GetDlgItemText(IDC_EDIT1, m_strUser);
	m_strUser.Trim(_T(" "));

	if(m_strUser == _T(""))
		return;

	pDoc->SendCT_USERKICKOUT_REQ(m_strUser);
	//OnOK();
}

void CUserKickout::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	OnCancel();
}
