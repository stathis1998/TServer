// CharMsg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Happy.h"
#include "CharMsg.h"
#include ".\charmsg.h"
#include "MainFrm.h"
#include "HappyDoc.h"


// CCharMsg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CCharMsg, CDialog)
CCharMsg::CCharMsg(CWnd* pParent /*=NULL*/)
	: CDialog(CCharMsg::IDD, pParent)
{
}

CCharMsg::~CCharMsg()
{
}

void CCharMsg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCharMsg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CCharMsg �޽��� ó�����Դϴ�.

void CCharMsg::OnBnClickedOk()
{
	CString strName;
	CString strMsg;

	GetDlgItemText(IDC_EDIT1, strName);
	GetDlgItemText(IDC_EDIT2, strMsg);

	strName.Trim(_T(" "));
	if(strName.IsEmpty() || strMsg.IsEmpty())
	{
		AfxMessageBox(_T("Enter again")); //�ٽ� �Է����ּ���
		SetDlgItemText(IDC_EDIT1, _T(""));
		return;
	}

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	pDoc->SendCT_CHARMSG_REQ(strName, strMsg);
	OnOK();
}
