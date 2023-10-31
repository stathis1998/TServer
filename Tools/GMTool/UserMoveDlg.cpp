// UserMoveDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "GMTool.h"
#include "UserMoveDlg.h"
#include ".\usermovedlg.h"

#include "GMToolDoc.h"

// CGMToolDlg �� �����ϱ� ����
#include "GMTool.h"
#include "GMToolDlg.h"


// CUserMoveDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CUserMoveDlg, CDialog)
CUserMoveDlg::CUserMoveDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserMoveDlg::IDD, pParent)
	, m_strUser(_T(""))
	, m_dwChannel(1)
	, m_dwMapID(0)
	, m_strTarget1(_T(""))	
{
	m_pdlgSavePos = new CSavePosDlg();
	m_pdlgSavePos->Create(IDD_SAVEPOS,CWnd::GetDesktopWindow());
	
	m_pdlgPosList = new CPosListDlg();
	m_pdlgPosList->Create(IDD_POSLISTDLG,CWnd::GetDesktopWindow());	
	m_pdlgPosList->Init();
}

CUserMoveDlg::~CUserMoveDlg()
{
	if(m_pdlgPosList)
	{
		m_pdlgPosList->DestroyWindow();
		delete m_pdlgPosList;
		m_pdlgPosList = NULL;
	}
	if(m_pdlgSavePos)
	{
		m_pdlgSavePos->DestroyWindow();
        delete m_pdlgSavePos;
		m_pdlgSavePos = NULL;
	}
}

void CUserMoveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO2, m_comboWorld);
	DDX_Text(pDX, IDC_EDIT1, m_strUser);
	DDX_Text(pDX, IDC_EDIT6, m_dwChannel);
	DDX_Text(pDX, IDC_EDIT7, m_dwMapID);	
	DDX_Text(pDX, IDC_EDIT_TARGET, m_strTarget1);
}


BEGIN_MESSAGE_MAP(CUserMoveDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO_COORD, OnBnClickedRadioCoord)
	ON_BN_CLICKED(IDC_RADIO_CHAR, OnBnClickedRadioChar)
	ON_BN_CLICKED(IDC_LOADPOS, OnBnClickedLoadpos)
	ON_BN_CLICKED(IDC_SAVE_POS, OnBnClickedSavePos)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	
	ON_BN_CLICKED(IDOK2, OnBnClickedOk2)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CUserMoveDlg �޽��� ó�����Դϴ�.

BOOL CUserMoveDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	SetEnableCoordinatesList(FALSE);
	SetEnableCharList(FALSE);

	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW, 0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CUserMoveDlg::SetWorld()
{
	CGMToolDoc* pDoc = CGMToolDoc::GetInstance();
	if(!pDoc)
		return;

	while(m_comboWorld.GetCount())
		m_comboWorld.DeleteString(0);

	m_comboWorld.AddString(_T("Choice Server"));
	MAPGROUP::iterator itG;
	for(itG=pDoc->m_mapGroup.begin(); itG!=pDoc->m_mapGroup.end(); itG++)
		if( (*itG).second->m_bID != 0 )
			m_comboWorld.AddString((*itG).second->m_strName);	

    m_comboWorld.SetCurSel(0);
}

// [��ǥ�Է�] ���� �׸���� Enable �� ���ΰ�
void CUserMoveDlg::SetEnableCoordinatesList(BOOL _bEnable)
{
	GetDlgItem(IDC_EDIT2)->EnableWindow(_bEnable); // PosX
	GetDlgItem(IDC_EDIT3)->EnableWindow(_bEnable); // PosY
	GetDlgItem(IDC_EDIT4)->EnableWindow(_bEnable); // PosZ
	GetDlgItem(IDC_EDIT6)->EnableWindow(_bEnable); // Channel
	GetDlgItem(IDC_EDIT7)->EnableWindow(_bEnable); // Map
	GetDlgItem(IDC_SAVE_POS)->EnableWindow(_bEnable); // �����ư	
	GetDlgItem(IDOK)->EnableWindow(_bEnable); // �̵���ư
	GetDlgItem(IDC_LOADPOS)->EnableWindow(_bEnable);
}

// [��� ĳ���ͷ��� ��ȯ] ���� �׸���� Enable �� ���ΰ�.
void CUserMoveDlg::SetEnableCharList(BOOL _bEnable)
{
	GetDlgItem(IDC_EDIT_TARGET)->EnableWindow(_bEnable);
	GetDlgItem(IDC_COMBO2)->EnableWindow(_bEnable);
	GetDlgItem(IDOK2)->EnableWindow(_bEnable);
}


void CUserMoveDlg::SetData(USERPOS& _UserPos)
{
	CString strTemp;
	strTemp.Format("%d",_UserPos.ChannelNum);
	SetDlgItemText(IDC_EDIT6,strTemp);	
	
	strTemp.Format("%d",_UserPos.MapID);
	SetDlgItemText(IDC_EDIT7,strTemp);

	strTemp.Format("%.2f",_UserPos.PosX);
	SetDlgItemText(IDC_EDIT2,strTemp);
	
	strTemp.Format("%.2f", _UserPos.PosY);
	SetDlgItemText(IDC_EDIT3,strTemp);
	
	strTemp.Format("%.2f", _UserPos.PosZ);
	SetDlgItemText(IDC_EDIT4,strTemp);
}

void CUserMoveDlg::Init()
{
	GetDlgItem(IDC_EDIT1)->SetFocus();

	// ���� �� �����
	SetDlgItemText(IDC_EDIT1,"");
	SetDlgItemText(IDC_EDIT2,"");
	SetDlgItemText(IDC_EDIT3,"");
	SetDlgItemText(IDC_EDIT4,"");
	SetDlgItemText(IDC_EDIT_TARGET,"");

	
	// ������ư ���þȵ� ���·� �ٲٱ�
	CButton* pradioCoord = (CButton*)GetDlgItem(IDC_RADIO_COORD);
	pradioCoord->SetCheck(FALSE);

	CButton* pradioChar = (CButton*)GetDlgItem(IDC_RADIO_CHAR);
	pradioChar->SetCheck(FALSE);

	if( pradioChar->GetCheck() )
		pradioChar->SetCheck(FALSE);


	// [��ǥ�Է�],[��������� ��ȯ] �׸�� Disable
	SetEnableCoordinatesList(FALSE);
	SetEnableCharList(FALSE);
}

void CUserMoveDlg::SetShowChildDlgAll(BOOL _bShow)
{
	int nShow = 0;
	if(_bShow)
		nShow = SW_SHOW;
	else
		nShow = SW_HIDE;

	m_pdlgSavePos->ShowWindow(nShow);
	m_pdlgPosList->ShowWindow(nShow);
}

void CUserMoveDlg::OnBnClickedRadioCoord()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	SetEnableCoordinatesList(TRUE);
	SetEnableCharList(FALSE);
}

void CUserMoveDlg::OnBnClickedRadioChar()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	SetEnableCoordinatesList(FALSE);
	SetEnableCharList(TRUE);
}

void CUserMoveDlg::OnBnClickedLoadpos()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if( !m_pdlgPosList->m_bLoad )
		m_pdlgPosList->LoadPosListFromData();

	m_pdlgPosList->m_listctrPos.UpdateData();	
	m_pdlgPosList->ShowWindow(SW_SHOW);	
	m_pdlgPosList->SetFocus();
}

void CUserMoveDlg::OnBnClickedSavePos()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	USERPOS UserPos;
	CString strTmp;

	UserPos.ChannelNum = GetDlgItemInt(IDC_EDIT6);
	UserPos.MapID = GetDlgItemInt(IDC_EDIT7);

	GetDlgItemText(IDC_EDIT2,strTmp);
	UserPos.PosX = (float)atof(strTmp);
	
	GetDlgItemText(IDC_EDIT3,strTmp);
	UserPos.PosY = (float)atof(strTmp);

	GetDlgItemText(IDC_EDIT4,strTmp);
	UserPos.PosZ = (float)atof(strTmp);
	
	strcpy(UserPos.POSNAME, "");	

	m_pdlgSavePos->SetUserPos(UserPos);	
	m_pdlgSavePos->ShowWindow(SW_SHOW);
	m_pdlgSavePos->Init();
}

void CUserMoveDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CGMToolDoc* pDoc = CGMToolDoc::GetInstance();
	if(!pDoc)
		return;

	if(!UpdateData())
		return;

	m_strUser.Trim(_T(" "));
	if(m_strUser == _T(""))
	{
		MessageBox(MSG_CHARNAME_NULL, MB_OK);
		return;
	}

	CString strTmp;
	GetDlgItemText(IDC_EDIT2,strTmp);
	m_fPosX = (float)atof(strTmp);
	
	GetDlgItemText(IDC_EDIT3,strTmp);
	m_fPosY = (float)atof(strTmp);

	GetDlgItemText(IDC_EDIT4,strTmp);
	m_fPosZ = (float)atof(strTmp);

	if(!m_fPosX && !m_fPosY && !m_fPosZ)
		return;

	pDoc->SendCT_USERMOVE_REQ(m_strUser, (BYTE)m_dwChannel, (WORD)m_dwMapID, m_fPosX, m_fPosY, m_fPosZ);
	//OnOK();
}



void CUserMoveDlg::OnBnClickedOk2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CGMToolDoc* pDoc = CGMToolDoc::GetInstance();
	if(!pDoc)
		return;

	UpdateData();

	m_strUser.Trim(_T(" "));
	if(m_strUser == _T(""))
	{
		MessageBox(MSG_CHARNAME_NULL, MB_OK);
		return;
	}

	m_strTarget1.Trim(_T(" "));
	if(m_strTarget1 == _T(""))
	{
		MessageBox(MSG_CHARNAME_NULL, MB_OK);
		return;
	}

	if(!m_comboWorld.GetCurSel())
	{
		MessageBox(MSG_SVR_NOTSELECT,MB_OK);//������ ���õ��� �ʾҽ��ϴ�
		return;
	}

	CString str;
	m_comboWorld.GetLBText(m_comboWorld.GetCurSel(), str);
	MAPGROUP::iterator itG;
	for(itG=pDoc->m_mapGroup.begin(); itG!=pDoc->m_mapGroup.end(); itG++)
		if((*itG).second->m_strName == str)
			break;

	if( itG == pDoc->m_mapGroup.end() )
	{
		MessageBox(MSG_NOT_FOUNTSERVER,MB_OK);
		return;
	}

	BYTE bID = (*itG).second->m_bID;
	
	pDoc->SendCT_USERPOSITION_REQ(bID, m_strUser, m_strTarget1);
}

void CUserMoveDlg::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	SetShowChildDlgAll(FALSE);
	OnCancel();
}
