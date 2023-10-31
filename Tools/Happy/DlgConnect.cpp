// DlgConnect.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Happy.h"
#include "MainFrm.h"
#include "HappyDoc.h"
#include "HappyView.h"
#include "DlgConnect.h"
#include ".\dlgconnect.h"




// CDlgConnect ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgConnect, CDialog)
CDlgConnect::CDlgConnect(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgConnect::IDD, pParent)
	, m_dwIP(0)
	, m_dwPort(0)
	, m_strUserID(_T(""))
	, m_strPassword(_T(""))
{		
}

CDlgConnect::~CDlgConnect()
{
}

void CDlgConnect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_IPAddress(pDX, IDC_IPADDRESS_IP, m_dwIP);
	DDX_Text(pDX, IDC_EDIT_PORT, m_dwPort);
	DDX_Text(pDX, IDC_EDIT_USER, m_strUserID);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
	DDX_Control(pDX, IDC_COMBO1, m_cbAccount);
}

BEGIN_MESSAGE_MAP(CDlgConnect, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


BOOL CDlgConnect::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	
	//CEdit *pEdit = (CEdit *)GetDlgItem(IDC_EDIT_PASSWORD);
	//if(pMsg->message == WM_KEYDOWN)
	//{
	//	if(pMsg->hwnd == pEdit->m_hWnd)
	//	{
	//		if(pMsg->wParam == VK_RETURN)
	//			return FALSE;
	//	}
	//}   

	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgConnect::Init()
{
	while(m_cbAccount.GetCount())
		m_cbAccount.DeleteString(0);
	
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	SetCurrentDirectory(pDoc->m_szCurrentDirectory);
	CStdioFile file;
	CFileException exFile;	
	if( file.Open( "HappyIP.txt", CFile::modeRead | CFile::typeText,&exFile ) )
	{
		ACCOUNT stAccount;
		BYTE bIndex = 0;
		while(TRUE)
		{			
			if(!file.ReadString(stAccount.m_str[bIndex]))
			{
				break;
			}			
			bIndex++;
			bIndex = bIndex % 5;
			if(bIndex == 0 )
			{
				CString str = stAccount.m_str[0];
				m_cbAccount.AddString(str);
				m_mapACCOUNT.insert(map<CString,ACCOUNT>::value_type(stAccount.m_str[0],stAccount));				
			}
		}

		file.Close();
		GetDlgItem(IDC_COMBO1)->ShowWindow(SW_SHOW);
	}
	else
	{
		TCHAR errmsg[1024];
		exFile.GetErrorMessage(errmsg,1024);

		CString strErr = errmsg;
		//AfxMessageBox(errmsg);
	}
}
void CDlgConnect::OnCbnSelchangeCombo1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString str;
	m_cbAccount.GetLBText(m_cbAccount.GetCurSel(),str);
	if(str == _T(""))
		return;

	map<CString,ACCOUNT>::iterator it = m_mapACCOUNT.find(str);
	if( it == m_mapACCOUNT.end())
		return;

	ACCOUNT stAccount = (*it).second;
	str = stAccount.m_str[1];
	CString tok,strReverseIP;
	int nPos,nPoint;
	nPos = nPoint = 0;
	tok = str.Tokenize(_T("."),nPos);
	while(tok != _T(""))
	{
		strReverseIP = tok + strReverseIP;
		if(nPoint < 3)
		{
			strReverseIP = _T(".") + strReverseIP;
			nPoint++;
		}
		tok = str.Tokenize(_T("."),nPos); //AfxExtractSubString		
	}

	m_dwIP = inet_addr(strReverseIP.GetBuffer(0));
	m_dwPort = atoi(stAccount.m_str[2].GetBuffer(0));
	m_strUserID = stAccount.m_str[3];
	m_strPassword = stAccount.m_str[4];

	UpdateData(FALSE);
}

// ��� ���̾�α״� OnInitDialog() �� ����Ǿ�߸� ��Ʈ��(m_cbAccount)�� �����ȴ�.
BOOL CDlgConnect::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	
	Init();
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
