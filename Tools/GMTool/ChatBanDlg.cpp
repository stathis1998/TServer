// ChatBanDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "GMTool.h"
#include "ChatBanDlg.h"
#include ".\chatbandlg.h"

#include "GMToolDoc.h"

// CChatBanDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CChatBanDlg, CDialog)
CChatBanDlg::CChatBanDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChatBanDlg::IDD, pParent)
	, m_strChatBanName(_T(""))
{
	m_pdlgRet = new CRetDlg();
	m_pdlgRet->Create(IDD_RET);
}

CChatBanDlg::~CChatBanDlg()
{
	if(m_pdlgRet)
	{
		m_pdlgRet->DestroyWindow();
		delete m_pdlgRet;
		m_pdlgRet = NULL;
	}
}

void CChatBanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHATBAN_CHAR, m_editChatBanName);
	DDX_Control(pDX, IDC_EB_BANREASON, m_ebBanReason);
	DDX_Control(pDX, IDC_CHATBAN_MIN, m_ebBanTime);
}


BEGIN_MESSAGE_MAP(CChatBanDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CChatBanDlg �޽��� ó�����Դϴ�.

void CChatBanDlg::Init()
{
	//m_editChatBanName.SetFocus();
	m_strChatBanName = _T("");
	m_iChatBanMin = 0;
	
	m_ebBanTime.SetWindowText(_T(""));
	m_editChatBanName.SetWindowText(_T(""));
	m_ebBanReason.SetWindowText(_T(""));
	m_ebBanReason.LimitText(200);	
}

void CChatBanDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData();
	CString strMin;
	CString strReason;
	GetDlgItemText(IDC_EB_BANREASON,strReason);
	GetDlgItemText(IDC_CHATBAN_CHAR,m_strChatBanName);
	GetDlgItemText(IDC_CHATBAN_MIN,strMin);
	
	if( m_strChatBanName == _T(""))
	{
		MessageBox(_T("Enter the name"), MB_OK);// �̸��� �Է��ؾ� �մϴ�
		return;
	}		
	if( strMin == _T("") )
	{
		MessageBox(_T("Enter the minutes"), MB_OK);// �ð��� �Է��ؾ� �մϴ�
		return;
	}
	if( strReason == _T(""))
	{
		AfxMessageBox(_T("Enter the reason"), MB_OK); // ������ �Է��ؾ� �մϴ�
		return;
	}

	m_iChatBanMin = (INT)atoi(strMin);

	CGMToolDoc* pDoc = CGMToolDoc::GetInstance();
	if(!pDoc)
		return;
	pDoc->SendCT_CHATBAN_REQ(m_strChatBanName,(WORD)m_iChatBanMin,strReason);


	//OnOK();
}

BOOL CChatBanDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CChatBanDlg::ShowDlgRet(BYTE bShow, BYTE bRet)
{
	if(m_pdlgRet)
	{
		CString strRet,strName,strMin;
		if(bRet)
		{
			GetDlgItemText(IDC_CHATBAN_CHAR,strName);
			GetDlgItemText(IDC_CHATBAN_MIN,strMin);
			strRet.Format(_T("%s was banned for %s minutes"),strName,strMin);
		}
		else
			strRet.Format(_T("Enter the correct name"));	
		
		m_pdlgRet->SetMsg(strRet);
		m_pdlgRet->ShowWindow(bShow);
	}
}
