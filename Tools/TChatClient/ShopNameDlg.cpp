// ShopNameDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "TChatClient.h"
#include "ShopNameDlg.h"
#include ".\shopnamedlg.h"


// CShopNameDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CShopNameDlg, CDialog)
CShopNameDlg::CShopNameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShopNameDlg::IDD, pParent)
	, m_strNAME(_T(""))
{
}

CShopNameDlg::~CShopNameDlg()
{
}

void CShopNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SHOP_NAME, m_strNAME);
}


BEGIN_MESSAGE_MAP(CShopNameDlg, CDialog)
END_MESSAGE_MAP()


// CShopNameDlg �޽��� ó�����Դϴ�.

BOOL CShopNameDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon( hIcon, FALSE);
	SetIcon( hIcon, TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CShopNameDlg::OnOK()
{
	UpdateData();
	CDialog::OnOK();
}
