// CharMsgDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "GMTool.h"
#include "CharMsgDlg.h"
#include ".\charmsgdlg.h"
#include "GMToolDoc.h"

// CCharMsgDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CCharMsgDlg, CDialog)
CCharMsgDlg::CCharMsgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCharMsgDlg::IDD, pParent)
{
}

CCharMsgDlg::~CCharMsgDlg()
{
}

void CCharMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCharMsgDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CCharMsgDlg �޽��� ó�����Դϴ�.

// Send ��ư ������ ��
void CCharMsgDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString strName;
	CString strMsg;

	GetDlgItemText(IDC_EDIT_CHARNAME, strName);
	GetDlgItemText(IDC_EDIT_CHARMSG, strMsg);

	strName.Trim(_T(" "));
	if(strName.IsEmpty() || strMsg.IsEmpty())
	{
		MessageBox(MSG_CHARNAME_NULL, MB_OK);
		SetDlgItemText(IDC_EDIT_CHARNAME, _T(""));
		return;
	}

	CGMToolDoc* pDoc = CGMToolDoc::GetInstance();
	if(!pDoc)
		return;
	pDoc->SendCT_CHARMSG_REQ(strName, strMsg);	
}

BOOL CCharMsgDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW, 0); // ��޸��� ��ȭ���ڰ� ������ �½�ũ�ٿ� �� �߰� �ϱ�
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
