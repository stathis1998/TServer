// RetDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "GMTool.h"
#include "RetDlg.h"


// CRetDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CRetDlg, CDialog)
CRetDlg::CRetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRetDlg::IDD, pParent)
{
}

CRetDlg::~CRetDlg()
{
}

void CRetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LB_RET, m_lbRet);
}


BEGIN_MESSAGE_MAP(CRetDlg, CDialog)
END_MESSAGE_MAP()


// CRetDlg �޽��� ó�����Դϴ�.

void CRetDlg::SetMsg(CString str)
{
	CFont Font;
	Font.CreateFont(5,0,0,0,100,FALSE,FALSE,
		0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
		NULL,"Arial");

	m_lbRet.SetFont(&Font);

	CRect rect;
	GetClientRect(&rect);
	
	//CRect rRect;
	//GetDlgItem(IDOK)->GetWindowRect(&rRect);
	DWORD dwCharWidth = 8;	
	DWORD dwLen = str.GetLength();
	DWORD width = dwLen * dwCharWidth;
	if(width > (DWORD)rect.Width() - 5)
		width = (DWORD)rect.Width() - 5;

	m_lbRet.MoveWindow( (rect.Width() - width)/2 + 5, rect.top + 25,width + 5, 20);
	
	m_lbRet.SetWindowText(str);
}