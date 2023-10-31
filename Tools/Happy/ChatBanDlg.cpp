// ChatBanDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Happy.h"
#include "ChatBanDlg.h"
#include ".\chatbandlg.h"

#include "MainFrm.h"
#include "HappyDoc.h"

// CChatBanDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CChatBanDlg, CDialog)
CChatBanDlg::CChatBanDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChatBanDlg::IDD, pParent)
	, m_strChatBanName(_T(""))
	, m_iChatBanMin(0)
{
}

CChatBanDlg::~CChatBanDlg()
{
}

void CChatBanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHATBAN_CHAR, m_editChatBanName);
	DDX_Text(pDX, IDC_CHATBAN_CHAR, m_strChatBanName);
	DDX_Control(pDX, IDC_CHATBAN_MIN, m_editChatBanMin);
	DDX_Text(pDX, IDC_CHATBAN_MIN, m_iChatBanMin);
	DDX_Control(pDX, IDC_EB_BANREASON, m_ebBanReason);
}


BEGIN_MESSAGE_MAP(CChatBanDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CChatBanDlg �޽��� ó�����Դϴ�.

void CChatBanDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData();
	CString strMin=_T("");
	CString strReason=_T("");
	GetDlgItemText(IDC_CHATBAN_MIN,strMin);
	GetDlgItemText(IDC_EB_BANREASON,strReason);
	
	if( m_strChatBanName == _T(""))
	{
		AfxMessageBox(_T("You should enter the name"), MB_OK); // �̸��� �Է��ؾ� �մϴ�
		return;
	}

	if( strMin == _T("") )
	{
		AfxMessageBox(_T("You should enter the minute"), MB_OK); // �ð�(��)�� �Է��ؾ� �մϴ�
		return;
	}

	if( strReason == _T(""))
	{
		AfxMessageBox(_T("You should enter the reason"), MB_OK); // ������ �Է��ؾ� �մϴ�
		return;
	}

	m_iChatBanMin = (INT)atoi(strMin);

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}
	pDoc->SendCT_CHATBAN_REQ(m_strChatBanName,(WORD)m_iChatBanMin,strReason);
	
	//OnOK();
}

void CChatBanDlg::Init()
{
	//m_editChatBanName.SetFocus();
	m_strChatBanName = _T("");
	m_iChatBanMin = 10;
	SetDlgItemText(IDC_EB_BANREASON,_T(""));
	m_ebBanReason.LimitText(200);

	UpdateData(0);
}
