// IPChangeDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "GMTool.h"
#include "IPChangeDlg.h"
#include ".\ipchangedlg.h"

#include "GMTool.h"
#include "GMToolDlg.h"

// CIPChangeDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CIPChangeDlg, CDialog)
CIPChangeDlg::CIPChangeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIPChangeDlg::IDD, pParent)	
{
}

CIPChangeDlg::~CIPChangeDlg()
{
}

void CIPChangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, m_ctrIPAddr);
}


BEGIN_MESSAGE_MAP(CIPChangeDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CIPChangeDlg �޽��� ó�����Դϴ�.

void CIPChangeDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	DWORD dwPort = GetDlgItemInt(IDC_EDIT_PORT);
	if( dwPort <= 0 )
	{
		MessageBox(MSG_ENTER_AGAIN,MB_OK);
		return;
	}

	CString strIP;
	m_ctrIPAddr.GetWindowText(strIP);
	if(strIP == _T("0.0.0.0") )
	{
		MessageBox(MSG_ENTER_AGAIN,MB_OK);
		return;
	}

	CGMToolApp *pApp = (CGMToolApp *)AfxGetApp(); // App ������ ����
	CGMToolDlg *pdlgGMTool = (CGMToolDlg *) pApp->GetMainWnd(); // ���������� �� ����
	if(pdlgGMTool)
	{
		pdlgGMTool->SetIPPort(strIP,dwPort);
		pdlgGMTool->SetFocus();
	}

	OnOK();
}
