// TChatPriceDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "TChatClient.h"
#include "TChatPriceDlg.h"
#include "TChatItem.h"
#include ".\tchatpricedlg.h"


// CTChatPriceDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CTChatPriceDlg, CDialog)
CTChatPriceDlg::CTChatPriceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTChatPriceDlg::IDD, pParent)
	, m_dwRune(0)
	, m_dwLuna(0)
	, m_dwCron(0)
{
	m_pTITEM = NULL;
}

CTChatPriceDlg::~CTChatPriceDlg()
{
}

void CTChatPriceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RUNE, m_dwRune);
	DDX_Text(pDX, IDC_EDIT_LUNA, m_dwLuna);
	DDX_Text(pDX, IDC_EDIT_CRON, m_dwCron);
}


BEGIN_MESSAGE_MAP(CTChatPriceDlg, CDialog)
END_MESSAGE_MAP()


// CTChatPriceDlg �޽��� ó�����Դϴ�.

BOOL CTChatPriceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon( hIcon, FALSE);
	SetIcon( hIcon, TRUE);

	DWORD dwPrice = m_pTITEM->m_pTITEM->m_dwPrice / 2;
	m_dwRune = dwPrice / 1000000;
	m_dwLuna = (dwPrice % 1000000) / 1000;
	m_dwCron = dwPrice % 1000;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CTChatPriceDlg::OnOK()
{
	UpdateData();
	CDialog::OnOK();
}
