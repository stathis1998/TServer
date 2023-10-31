// ���·� Announce.cpp

// Announce.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Happy.h"
#include "Announce.h"
#include ".\announce.h"
#include "MainFrm.h"
#include "HappyDoc.h"

// CAnnounce ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CAnnounce, CDialog)
CAnnounce::CAnnounce(CWnd* pParent /*=NULL*/)
	: CDialog(CAnnounce::IDD, pParent)
	, m_dwTimerMin(0)
{
}

CAnnounce::~CAnnounce()
{
}

void CAnnounce::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_comboWorld);
	DDX_Control(pDX, IDC_CHECK1, m_checkTimer);
	DDX_Text(pDX, IDC_EDIT_TIMER, m_dwTimerMin);
}


BEGIN_MESSAGE_MAP(CAnnounce, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_TIMEREND, OnBnClickedButtonTimerend)
	ON_BN_CLICKED(IDC_BUTTON_TIMER, OnBnClickedButtonTimer)
END_MESSAGE_MAP()


// CAnnounce �޽��� ó�����Դϴ�.

BOOL CAnnounce::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_checkTimer.SetCheck(FALSE);
	m_strAnnounce = _T("");
	GetDlgItem(IDC_BUTTON_TIMEREND)->EnableWindow(FALSE);
	m_bTimer = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CAnnounce::SetWorld()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	while(m_comboWorld.GetCount())
		m_comboWorld.DeleteString(0);

	m_comboWorld.AddString(_T("Total"));//��ü
	MAPGROUP::iterator itG;
	for(itG=pDoc->m_mapGroup.begin(); itG!=pDoc->m_mapGroup.end(); itG++)
		if( (*itG).second->m_bID != 0 )
			m_comboWorld.AddString((*itG).second->m_strName);

    m_comboWorld.SetCurSel(0);
}

void CAnnounce::OnBnClickedOk()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	GetDlgItemText(IDC_EDIT1, m_strAnnounce);
	

	CString str = m_strAnnounce;
	str.Trim(_T(" \n\r"));
	if(str == _T(""))
	{
		AfxMessageBox(_T("You should enter the notice"));
		return;
	}

	if( !m_comboWorld.GetCurSel() )
		m_nID = 0;
	else
	{
		m_comboWorld.GetLBText(m_comboWorld.GetCurSel(), str);

		MAPGROUP::iterator itG;
		for(itG=pDoc->m_mapGroup.begin(); itG!=pDoc->m_mapGroup.end(); itG++)
			if((*itG).second->m_strName == str)
				break;
			
		m_nID = (*itG).second->m_bID;
	}

	if(m_checkTimer.GetCheck())
	{
		UpdateData();
		DWORD dwTimer = m_bTimer ? m_dwTimerMin*1000 : m_dwTimerMin*60000;
		if(dwTimer > 3600000)
		{
			AfxMessageBox(_T("Control to less than 60 mins"));//60���̳��� ������ �ּ���
			return;
		}

		GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK1)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TIMER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_TIMER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_TIMEREND)->EnableWindow();

		SetTimer(1, dwTimer, NULL);
	}
	else
		SetDlgItemText(IDC_EDIT1, _T(""));
	pDoc->SendCT_ANNOUNCEMENT_REQ(m_nID, m_strAnnounce);
		
	//OnOK();
}

void CAnnounce::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(GetDlgItem(IDC_BUTTON_TIMEREND)->IsWindowEnabled())
	{
		AfxMessageBox(_T("Close the timer"));//Timer�� �����ϼ���
		return;
	}
	m_checkTimer.SetCheck(FALSE);
	SetDlgItemText(IDC_EDIT_TIMER, _T("0"));
	OnCancel();
}

void CAnnounce::OnTimer(UINT nIDEvent)
{
	switch(nIDEvent)
	{
	case 1:
		{
			CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
			CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
			if(!pDoc)
			{
				pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
				if(!pDoc)
				{
					KillTimer(1);
					GetDlgItem(IDC_EDIT1)->EnableWindow();
					GetDlgItem(IDC_CHECK1)->EnableWindow();
					GetDlgItem(IDC_COMBO1)->EnableWindow();
					GetDlgItem(IDC_EDIT_TIMER)->EnableWindow();
					GetDlgItem(IDC_BUTTON_TIMER)->EnableWindow();
					GetDlgItem(IDC_BUTTON_TIMEREND)->EnableWindow(FALSE);
					SetDlgItemText(IDC_EDIT1, _T(""));
					break;
				}
			}
			pDoc->SendCT_ANNOUNCEMENT_REQ(m_nID, m_strAnnounce);
		}
		break;
	default:
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

void CAnnounce::OnBnClickedButtonTimerend()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	KillTimer(1);
	GetDlgItem(IDC_EDIT1)->EnableWindow();
	GetDlgItem(IDC_CHECK1)->EnableWindow();
	GetDlgItem(IDC_COMBO1)->EnableWindow();
	GetDlgItem(IDC_EDIT_TIMER)->EnableWindow();
	GetDlgItem(IDC_BUTTON_TIMER)->EnableWindow();
	GetDlgItem(IDC_BUTTON_TIMEREND)->EnableWindow(FALSE);
	SetDlgItemText(IDC_EDIT1, _T(""));
}

void CAnnounce::OnBnClickedButtonTimer()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!m_bTimer)
		SetDlgItemText(IDC_BUTTON_TIMER, _T("Sec"));
	else
		SetDlgItemText(IDC_BUTTON_TIMER, _T("Min"));

	m_bTimer = !m_bTimer;
}
