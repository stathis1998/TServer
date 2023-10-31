// AnnounceDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "GMTool.h"
#include "AnnounceDlg.h"
#include ".\announcedlg.h"

#include "GMToolDoc.h"

// CAnnounceDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CAnnounceDlg, CDialog)
CAnnounceDlg::CAnnounceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAnnounceDlg::IDD, pParent)
	, m_dwTimerMin(0)
{
}

CAnnounceDlg::~CAnnounceDlg()
{
}

void CAnnounceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHK_TIMER, m_chkTimer);
	DDX_Control(pDX, IDC_COMBO_NOTICEWORLD, m_comboWorld);
}


BEGIN_MESSAGE_MAP(CAnnounceDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_TIMESELECT, OnBnClickedBtnTimeSelect)
	ON_BN_CLICKED(IDC_BTN_TIMEREND, OnBnClickedBtnTimerEnd)	
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CAnnounceDlg �޽��� ó�����Դϴ�.


BOOL CAnnounceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	m_chkTimer.SetCheck(FALSE);
	m_strAnnounce = _T("");
	GetDlgItem(IDC_BTN_TIMEREND)->EnableWindow(FALSE);
	m_bTimer = 0;

	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW, 0);// ��޸��� ��ȭ���ڰ� ������ �½�ũ�ٿ� �� �߰� �ϱ�
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CAnnounceDlg::SetWorld()
{
	CGMToolDoc* pDoc = CGMToolDoc::GetInstance();
	if(!pDoc)
		return;

	while(m_comboWorld.GetCount())
		m_comboWorld.DeleteString(0);

	m_comboWorld.AddString(_T("Total"));//��ü
	MAPGROUP::iterator itG;
	for(itG=pDoc->m_mapGroup.begin(); itG!=pDoc->m_mapGroup.end(); itG++)
		if( (*itG).second->m_bID != 0 )
			m_comboWorld.AddString((*itG).second->m_strName);

    m_comboWorld.SetCurSel(0);
}


void CAnnounceDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CGMToolDoc* pDoc = CGMToolDoc::GetInstance();
	if(!pDoc)
		return;

	GetDlgItemText(IDC_EDIT_NOTICE, m_strAnnounce);	

	CString str = m_strAnnounce;
	str.Trim(_T(" \n\r"));
	if(str == _T(""))
	{
		MessageBox(_T("You should enter the notice"));
		GetDlgItem(IDC_EDIT_NOTICE)->SetFocus();
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

	if(m_chkTimer.GetCheck())
	{
		m_dwTimerMin = GetDlgItemInt(IDC_EDIT_TIME);
		DWORD dwTimer = m_bTimer ? m_dwTimerMin*1000 : m_dwTimerMin*60000;
		if(dwTimer > 3600000)
		{
			AfxMessageBox(_T("Control to less than 60 mins"));//60���̳��� ������ �ּ���
			return;
		}

		GetDlgItem(IDC_EDIT_NOTICE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHK_TIMER)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_NOTICEWORLD)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TIMESELECT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_TIMEREND)->EnableWindow();

		SetTimer(1, dwTimer, NULL);
	}
	else
		SetDlgItemText(IDC_EDIT1, _T(""));

	pDoc->SendCT_ANNOUNCEMENT_REQ(m_nID, m_strAnnounce);

	//OnOK();
}

void CAnnounceDlg::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(GetDlgItem(IDC_BTN_TIMEREND)->IsWindowEnabled())
	{
		AfxMessageBox(_T("Close the timer"));//Timer�� �����ϼ���
		return;
	}
	m_chkTimer.SetCheck(FALSE);
	SetDlgItemText(IDC_EDIT_TIME, _T("0"));

	OnCancel();
}

void CAnnounceDlg::OnBnClickedBtnTimerEnd()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	KillTimer(1);
	GetDlgItem(IDC_EDIT_NOTICE)->EnableWindow();
	GetDlgItem(IDC_CHK_TIMER)->EnableWindow();
	GetDlgItem(IDC_COMBO_NOTICEWORLD)->EnableWindow();
	GetDlgItem(IDC_EDIT_TIME)->EnableWindow();
	GetDlgItem(IDC_BTN_TIMESELECT)->EnableWindow();
	GetDlgItem(IDC_BTN_TIMEREND)->EnableWindow(FALSE);
	SetDlgItemText(IDC_EDIT_NOTICE, _T(""));
}

void CAnnounceDlg::OnBnClickedBtnTimeSelect()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(!m_bTimer)
		SetDlgItemText(IDC_BTN_TIMESELECT, _T("Sec"));
	else
		SetDlgItemText(IDC_BTN_TIMESELECT, _T("Min"));

	m_bTimer = !m_bTimer;
}



void CAnnounceDlg::OnTimer(UINT nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	switch(nIDEvent)
	{
	case 1:
		{
			CGMToolDoc* pDoc = CGMToolDoc::GetInstance();
			if(!pDoc)
			{
				KillTimer(1);
					GetDlgItem(IDC_EDIT_NOTICE)->EnableWindow();
					GetDlgItem(IDC_CHK_TIMER)->EnableWindow();
					GetDlgItem(IDC_COMBO_NOTICEWORLD)->EnableWindow();
					GetDlgItem(IDC_EDIT_TIME)->EnableWindow();
					GetDlgItem(IDC_BTN_TIMESELECT)->EnableWindow();
					GetDlgItem(IDC_BTN_TIMEREND)->EnableWindow(FALSE);
					SetDlgItemText(IDC_EDIT_NOTICE, _T(""));
					break;
			}
			
			pDoc->SendCT_ANNOUNCEMENT_REQ(m_nID, m_strAnnounce);
		}
		break;
	default:
		break;
	}

	CDialog::OnTimer(nIDEvent);
}
