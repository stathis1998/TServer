// GMToolDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "GMTool.h"
#include "GMToolDlg.h"
#include ".\gmtooldlg.h"

#include "GMToolDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGMToolDlg ��ȭ ����



CGMToolDlg::CGMToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGMToolDlg::IDD, pParent)
	, m_strID(_T(""))
	, m_strPW(_T(""))
	, m_bDisconnect(TRUE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
	m_pdlgCharMsg = new CCharMsgDlg();
	m_pdlgCharMsg->Create(IDD_CHARMSG,CWnd::GetDesktopWindow());

	m_pdlgUserMove = new CUserMoveDlg();
	m_pdlgUserMove->Create(IDD_USERMOVE,CWnd::GetDesktopWindow()); 

	m_pdlgChatBan = new CChatBanDlg();
	m_pdlgChatBan->Create(IDD_CHATBAN,CWnd::GetDesktopWindow());	

	m_pdlgUserKickOut = new CUserKickOutDlg();
	m_pdlgUserKickOut->Create(IDD_USERKICKOUT,CWnd::GetDesktopWindow());

	m_pdlgIPChange = new CIPChangeDlg();
	m_pdlgIPChange->Create(IDD_IP,CWnd::GetDesktopWindow() );

	m_pdlgAnnounce = new CAnnounceDlg();
	m_pdlgAnnounce->Create(IDD_ANNOUNCE,CWnd::GetDesktopWindow() );

	m_pdlgMonsterSpawn = new CMonsterSpawnDlg();
	m_pdlgMonsterSpawn->Create(IDD_MONSPAWN,CWnd::GetDesktopWindow() );
	

	// CWnd::GetDesktopWindow() : ��Ŀ�� ��ȭ���ڸ� ������ ������ �ϱ�-> ������ �½�ũ �ٿ� ��ȭ���ڰ� ������
	// �½�ũ �ٿ� ��ȭ���� �Ȼ���� �ҷ���  �ش� ��ȭ������ OnInitDialog() �� �Ʒ� �ڵ� �߰��ؾ���.
	// ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW, 0);
	
}

void CGMToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGMToolDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_CONNECT, OnBnClickedBtnConnect)
	ON_BN_CLICKED(IDC_BTN_CHARMSG, OnBnClickedBtnCharmsg)
	ON_BN_CLICKED(IDC_BTN_MOVE, OnBnClickedBtnMove)
	ON_BN_CLICKED(IDC_BTN_BAN, OnBnClickedBtnBan)
	ON_BN_CLICKED(IDC_BTN_KICKOUT, OnBnClickedBtnKickout)
	ON_BN_CLICKED(IDC_BTN_DISCONNECT, OnBnClickedBtnDisconnect)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_CHGIP, OnBnClickedBtnChgip)
	ON_BN_CLICKED(IDC_BTN_ANNOUNCE, OnBnClickedBtnAnnounce)
	ON_BN_CLICKED(IDC_BTN_MONSTER, OnBnClickedBtnMonster)
END_MESSAGE_MAP()


// CGMToolDlg �޽��� ó����

BOOL CGMToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	// �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	SetEnableMenu(MANAGER_COUNT);

	m_dwPort = DEF_PORT;	
	m_strIP = DEF_IP;
	LoadConfig();

	SetConfigText(m_strIP,m_dwPort);

	return TRUE;  // ��Ʈ�ѿ� ���� ��Ŀ���� �������� ���� ��� TRUE�� ��ȯ�մϴ�.
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸����� 
// �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
// �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CGMToolDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�. 
HCURSOR CGMToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CGMToolDlg::OnBnClickedBtnDisconnect()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(m_bDisconnect)
		return;

	int nRet = AfxMessageBox(_T("Are you sure you want to disconnect?"),MB_YESNO); // ���� ������ �����ϰڽ��ϱ�?
	if(nRet != IDYES)
	{
		m_bDisconnect = FALSE;
		return;
	}
	m_bDisconnect = TRUE;
	
	GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_CHGIP)->EnableWindow(TRUE);
	
	SetShowChildDlgAll(FALSE);

	CGMToolDoc * pDoc = CGMToolDoc::GetInstance();
	if(!pDoc)
		return ;

	pDoc->DisconnectServer();
	m_pdlgUserMove->m_pdlgPosList->m_bLoad = FALSE;
	SetEnableMenu(MANAGER_COUNT);
	SaveConfig();

	// ���� ������ Ÿ��Ʋ �ٲٱ�	
	CString strTitle = _T(" - GMTool ");
	CWnd* pWnd = AfxGetMainWnd();	
	pWnd->SetWindowText(strTitle);
	
}

void CGMToolDlg::OnBnClickedBtnConnect()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	SetConfigText(m_strIP,m_dwPort);

	CGMToolDoc * pDoc = CGMToolDoc::GetInstance();
	if(!pDoc)
		return;

	GetDlgItemText(IDC_EDIT_ID,m_strID);
	GetDlgItemText(IDC_EDIT_PW,m_strPW);
	pDoc->SetUserData(m_strID,m_strPW);
	pDoc->SessionStart(m_strIP,m_dwPort);
}


void CGMToolDlg::LoadConfig()
{
	FILE* pf = fopen(CONFIG_FILE,"rt");
	if( !pf )
		return;

	char strIP[50]={0,};
	DWORD dwPort;
	if( fscanf(pf,"%s %d",strIP,&dwPort) == EOF )
	{
		fclose(pf);
		return;
	}

	m_strIP = strIP;
	m_dwPort = dwPort;
	fclose(pf);
}

void CGMToolDlg::SaveConfig()
{
	FILE* pf = fopen(CONFIG_FILE,"wt");
	if( !pf )
		return;

	fprintf(pf,"%s %d",m_strIP, m_dwPort);
	fclose(pf);
}

void CGMToolDlg::SetConfigText(CString strIP,DWORD dwPort)
{	
	CString strInfo;
	strInfo.Format("[IP] %s  [PORT] %d",m_strIP,m_dwPort);
	SetDlgItemText(IDC_STATIC_INFO,strInfo);
}

void CGMToolDlg::SetIPPort(CString strIP, DWORD dwPort)
{
	m_strIP = strIP;
	m_dwPort = dwPort;
	SetConfigText(m_strIP,m_dwPort);
}

void CGMToolDlg::SetEnableMenu(BYTE bLevel)
{
	GetDlgItem(IDC_BTN_BAN)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_KICKOUT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_CHARMSG)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_MOVE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_ANNOUNCE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_MONSTER)->EnableWindow(FALSE);

	switch(bLevel)
	{
	case MANAGER_ALL:
	case MANAGER_CONTROL:
	case MANAGER_USER:
	case MANAGER_SERVICE:
	case MANAGER_GMLEVEL1:
		{
			GetDlgItem(IDC_BTN_ANNOUNCE)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_MONSTER)->EnableWindow(TRUE);			
		}
		
	case MANAGER_GMLEVEL2:
		{
			GetDlgItem(IDC_BTN_BAN)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_KICKOUT)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_CHARMSG)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_MOVE)->EnableWindow(TRUE);
		}
		
	default:
		break;
	}
}

void CGMToolDlg::OnBnClickedBtnCharmsg()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_pdlgCharMsg->ShowWindow(SW_SHOW);
}

void CGMToolDlg::OnBnClickedBtnMove()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_pdlgUserMove->Init();
	m_pdlgUserMove->SetWorld();	
	m_pdlgUserMove->ShowWindow(SW_SHOW);	
}

void CGMToolDlg::OnBnClickedBtnBan()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_pdlgChatBan->Init();
	m_pdlgChatBan->ShowWindow(SW_SHOW);	
}

void CGMToolDlg::OnBnClickedBtnKickout()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_pdlgUserKickOut->ShowWindow(SW_SHOW);
}

void CGMToolDlg::OnBnClickedBtnChgip()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_pdlgIPChange->ShowWindow(SW_SHOW);
	m_pdlgIPChange->m_ctrIPAddr.SetFocus();
}

void CGMToolDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	OnBnClickedBtnDisconnect();
	
	if(!m_bDisconnect)
		return;

	if(m_pdlgCharMsg)
	{
		m_pdlgCharMsg->DestroyWindow();
		delete m_pdlgCharMsg;
		m_pdlgCharMsg = NULL;
	}

	if(m_pdlgUserMove)
	{
		m_pdlgUserMove->DestroyWindow();
		delete m_pdlgUserMove;
		m_pdlgUserMove = NULL;
	}

	if(m_pdlgChatBan)
	{
		m_pdlgChatBan->DestroyWindow();
		delete m_pdlgChatBan;
		m_pdlgChatBan = NULL;
	}

	if(m_pdlgUserKickOut)
	{
		m_pdlgUserKickOut->DestroyWindow();
		delete m_pdlgUserKickOut;
		m_pdlgUserKickOut = NULL;
	}

	if(m_pdlgIPChange)
	{
		m_pdlgIPChange->DestroyWindow();
		delete m_pdlgIPChange;
		m_pdlgIPChange = NULL;
	}

	if(m_pdlgAnnounce)
	{
		m_pdlgAnnounce->DestroyWindow();
		delete m_pdlgAnnounce;
		m_pdlgAnnounce = NULL;
	}

	if(m_pdlgMonsterSpawn)
	{
		m_pdlgMonsterSpawn->DestroyWindow();
		delete m_pdlgMonsterSpawn;
		m_pdlgMonsterSpawn = NULL;
	}


	OnOK();
}

void CGMToolDlg::SetShowChildDlgAll(BOOL _bShow)
{
	int nShow = 0;
	if(_bShow)
		nShow = SW_SHOW;
	else
		nShow = SW_HIDE;

	m_pdlgCharMsg->ShowWindow(nShow);
	m_pdlgUserMove->SetShowChildDlgAll(_bShow);
	m_pdlgUserMove->ShowWindow(nShow);
	m_pdlgChatBan->ShowWindow(nShow);
	m_pdlgUserKickOut->ShowWindow(nShow);
	m_pdlgIPChange->ShowWindow(nShow);
	m_pdlgAnnounce->ShowWindow(nShow);
	m_pdlgMonsterSpawn->ShowWindow(nShow);
}


BOOL CGMToolDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	// ����Ʈ �ڽ����� ����Ű ����.
	// ����Ʈ �ڽ� �Ӽ����� Password �� TRUE �� Multiline(true),want return(true) �Ӽ��� ����ȵ�
	// �ڵ忡�� ����Ű ����ȵǵ��� ����

	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_EDIT_PW); 
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->hwnd == pEdit->m_hWnd)
		{
			if(pMsg->wParam == VK_RETURN)
				return FALSE;
		}
	}   

	return CDialog::PreTranslateMessage(pMsg);
}

void CGMToolDlg::OnBnClickedBtnAnnounce()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_pdlgAnnounce->ShowWindow(SW_SHOW);
	m_pdlgAnnounce->SetWorld();
}

void CGMToolDlg::OnBnClickedBtnMonster()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_pdlgMonsterSpawn->ShowWindow(SW_SHOW);
	m_pdlgMonsterSpawn->SetWorld();
}
