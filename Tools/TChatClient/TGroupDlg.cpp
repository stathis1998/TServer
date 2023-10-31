// TGroupDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "TChatClient.h"
#include "TGroupDlg.h"
#include ".\tgroupdlg.h"


// CTGroupDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CTGroupDlg, CDialog)
CTGroupDlg::CTGroupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTGroupDlg::IDD, pParent)
{
	m_pTGROUP = NULL;
	ClearTGROUP();
}

CTGroupDlg::~CTGroupDlg()
{
	ClearTGROUP();
}

void CTGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_GROUP, m_cTGROUP);
}


BEGIN_MESSAGE_MAP(CTGroupDlg, CDialog)
END_MESSAGE_MAP()


// CTGroupDlg �޽��� ó�����Դϴ�.

void CTGroupDlg::ClearTGROUP()
{
	while(!m_vTGROUP.empty())
	{
		delete m_vTGROUP.back();
		m_vTGROUP.pop_back();
	}
}

BOOL CTGroupDlg::OnInitDialog()
{
	static CString strSTATE[4] = {
		CString(_T("Maintenance")),
		CString(_T("Normal")),
		CString(_T("Congestion")),
		CString(_T("Saturation"))};
	CDialog::OnInitDialog();
	CRect rect;

	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon( hIcon, FALSE);
	SetIcon( hIcon, TRUE);
	m_cTGROUP.DeleteAllItems();

	m_cTGROUP.GetClientRect(&rect);
	m_cTGROUP.InsertColumn( 0, _T("Server"), LVCFMT_CENTER, rect.Width() / 2);
	m_cTGROUP.InsertColumn( 1, _T("Status"), LVCFMT_CENTER, rect.Width() / 4);
	m_cTGROUP.InsertColumn( 2, _T("Count"), LVCFMT_CENTER, rect.Width() / 4);

	m_cTGROUP.SetExtendedStyle(
		LVS_EX_FULLROWSELECT|
		LVS_EX_GRIDLINES);

	for( int i=0; i<INT(m_vTGROUP.size()); i++)
	{
		int nIndex = m_cTGROUP.InsertItem( i, m_vTGROUP[i]->m_strNAME);
		CString strCOUNT;

		m_cTGROUP.SetItemText( nIndex, 1, strSTATE[m_vTGROUP[i]->m_bStatus]);
		strCOUNT.Format( _T("%d"), m_vTGROUP[i]->m_bCount);
		m_cTGROUP.SetItemText( nIndex, 2, strCOUNT);

		m_cTGROUP.SetItemData( nIndex, (DWORD_PTR) m_vTGROUP[i]);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CTGroupDlg::OnOK()
{
	POSITION pos = m_cTGROUP.GetFirstSelectedItemPosition();

	if(pos)
	{
		int nIndex = m_cTGROUP.GetNextSelectedItem(pos);

		if( nIndex >= 0 && nIndex < INT(m_vTGROUP.size()) )
			m_pTGROUP = m_vTGROUP[nIndex];
	}

	CDialog::OnOK();
}
