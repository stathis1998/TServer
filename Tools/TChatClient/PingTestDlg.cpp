// PingTestDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "TChatClient.h"
#include "PingTestDlg.h"
#include ".\pingtestdlg.h"


// CPingTestDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CPingTestDlg, CDialog)
CPingTestDlg::CPingTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPingTestDlg::IDD, pParent)
	, m_dwTimer(1)
	, m_pTSocket(NULL)
	, m_dwMaxPing(0)
	, m_dwLimitPing(100)
	, m_bFlag(TRUE)
	, m_bDrawing(FALSE)
{	
	lineColor[0].CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	lineColor[1].CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	lineColor[2].CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	lineColor[3].CreatePen(PS_DOT, 1, RGB(255, 0, 0));
	lineColor[4].CreatePen(PS_DOT, 1, RGB(0, 255, 0));
}

CPingTestDlg::~CPingTestDlg()
{
	ClearObject();
}

void CPingTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHK_PINGLOOP, m_chkPingLoop);
}


BEGIN_MESSAGE_MAP(CPingTestDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_SEND, OnBnClickedBtnSend)
	ON_BN_CLICKED(IDC_CHK_PINGLOOP, OnBnClickedChkPingloop)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CPingTestDlg �޽��� ó�����Դϴ�.

void CPingTestDlg::OnBnClickedBtnSend()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_bDrawing = FALSE;
	KillTimer(1);

	if(m_chkPingLoop.GetCheck() == BST_CHECKED )
	{
		UpdateData();
		m_dwTimer = GetDlgItemInt(IDC_EDIT_PINGSECOND);
		DWORD dwTimer = m_dwTimer * 1000;
		if( dwTimer == 0 )
		{
			AfxMessageBox(_T("Enter again") );
			GetDlgItem(IDC_EDIT_PINGSECOND)->SetFocus();
			return;
		}
		else if(dwTimer > 3600000)
		{
			AfxMessageBox(_T("Control to less than 60 mins"));//60���̳��� ������ �ּ���
			return;
		}

		m_vPing.clear();
		m_dwMaxPing = 0;
		m_bDrawing = TRUE;
		SetTimer(1,dwTimer,NULL);
	}
	else	
		GetDlgItem(IDC_EDIT_PINGSECOND)->EnableWindow(FALSE);

	DWORD dwTick = GetTickCount();
	if(m_pTSocket)
        m_pTSocket->SendCT_SERVICEMONITOR_ACK(dwTick);
}

void CPingTestDlg::OnBnClickedChkPingloop()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	int iState = m_chkPingLoop.GetCheck();

	switch(iState)
	{
	case BST_UNCHECKED :
		GetDlgItem(IDC_EDIT_PINGSECOND)->EnableWindow(FALSE);
		break;
	case BST_CHECKED:
		GetDlgItem(IDC_EDIT_PINGSECOND)->EnableWindow(TRUE);
		break;
	case BST_INDETERMINATE:
		GetDlgItem(IDC_EDIT_PINGSECOND)->EnableWindow(FALSE);
		break;
	}
}

BOOL CPingTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	m_chkPingLoop.SetCheck(BST_UNCHECKED);
	GetDlgItem(IDC_EDIT_PINGSECOND)->EnableWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CPingTestDlg::OnTimer(UINT nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	switch(nIDEvent)
	{
	case 1:
		{
			DWORD dwTick = GetTickCount();
			if(!m_pTSocket)
			{
				KillTimer(1);
				break;
			}

			m_pTSocket->SendCT_SERVICEMONITOR_ACK(dwTick);
		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CPingTestDlg::SetIPPort(CString strIP, DWORD dwPort)
{
	CString strPort;
	strPort.Format("%d",dwPort);
	SetDlgItemText(IDC_TEXT_IP,strIP);
	SetDlgItemText(IDC_TEXT_PORT,strPort);
}

void CPingTestDlg::SetPing(DWORD dwPing)
{
	CString strPing;
	strPing.Format("%u",dwPing);
	SetDlgItemText(IDC_TEXT_PING,strPing);
	
	if(m_dwMaxPing < dwPing )
		m_dwMaxPing = dwPing;

	CString strMaxPing;
	strMaxPing.Format("%u",m_dwMaxPing);
	SetDlgItemText(IDC_TEXT_MAXPING, strMaxPing);
	

	if(m_chkPingLoop.GetCheck() == BST_CHECKED )
	{
		m_vPing.push_back(dwPing);
		if(m_vPing.size() >= MAXPINGCOUNT )
			m_vPing.erase(m_vPing.begin() );
		DrawGraph();
	}
}

void CPingTestDlg::SetSocket(CTChatSocket* pTSocket)
{
	if(pTSocket)
		m_pTSocket = pTSocket;
}
void CPingTestDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	KillTimer(1);
	Init();

	OnOK();
}

void CPingTestDlg::ClearObject()
{
	MemDC.DeleteDC();
	MemBitmap.DeleteObject();

	for(BYTE i=0; i<MAXCOLOR; i++)
		lineColor[i].DeleteObject();
}

void CPingTestDlg::DrawBackGround()
{
	if(!MemDC)
		return;

	MemDC.FillSolidRect(0, 0, m_rectGraph.Width(), m_rectGraph.Height(), RGB(255,255,255));
	
	CString strPing;

	strPing.Format(_T("%d"), m_dwLimitPing);
	MemDC.TextOut(15, m_rectPing.top - 9, strPing);

	strPing.Format(_T("%u"),m_dwLimitPing * 3 / 4 );
	MemDC.TextOut(15, m_rectPing.bottom-m_rectPing.Height() * 3 / 4 - 7 , strPing);

	strPing.Format(_T("%u"),m_dwLimitPing / 2);
	MemDC.TextOut(15, m_rectPing.bottom-m_rectPing.Height()/2 - 7, strPing );

	strPing.Format(_T("%u"),m_dwLimitPing / 4 );
	MemDC.TextOut(15, m_rectPing.bottom-m_rectPing.Height()/4 - 7, strPing );

	MemDC.TextOut(32, m_rectPing.bottom - 10, _T("0"));
}

void CPingTestDlg::DrawRect()
{
	if(!MemDC)
		return;
	
	MemDC.FillSolidRect(m_rectPing.left-1, m_rectPing.top-1, m_rectPing.Width() + 2, m_rectPing.Height() + 2 , RGB(200,200,200));

	CPen LineColor(PS_SOLID, 1, RGB(255, 255, 255));
	CPen *oldPen = MemDC.SelectObject(&LineColor);	


	for(BYTE i = 1; i <= 20; i++) 
	{
		MemDC.MoveTo(m_rectPing.left, m_rectPing.bottom - i*m_rectPing.Height()/20 ); 
		MemDC.LineTo(m_rectPing.right, m_rectPing.bottom - i*m_rectPing.Height()/20 );
	}

	MemDC.SelectObject(oldPen);
	LineColor.DeleteObject();
}
void CPingTestDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	// �׸��� �޽����� ���ؼ��� CDialog::OnPaint()��(��) ȣ������ ���ʽÿ�.

	if(m_bFlag)
	{
        MemDC.CreateCompatibleDC(&dc);
		MemBitmap.CreateCompatibleBitmap(&dc, m_rectGraph.Width(), m_rectGraph.Height());
		pOldBitmap = MemDC.SelectObject(&MemBitmap);
		DrawBackGround();
		DrawRect();
		m_bFlag = FALSE;
	}
	dc.BitBlt(m_rectGraph.left, m_rectGraph.top, m_rectGraph.Width(), m_rectGraph.Height(), &MemDC, 0,0, SRCCOPY);
}

void CPingTestDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

	GetClientRect(&m_rectBack);

	m_rectGraph.top = 10;
	m_rectGraph.left = 50;
	m_rectGraph.right = m_rectBack.right - 30;
	m_rectGraph.bottom = m_rectBack.bottom - 80;

	m_rectPing.top = m_rectGraph.top +10;
	m_rectPing.left = 50;
	m_rectPing.right = m_rectGraph.right - 70;
	m_rectPing.bottom = m_rectGraph.bottom - 20;


	if(!m_bFlag)
	{
		CClientDC dc(this);
		MemDC.SelectObject(pOldBitmap);
		MemBitmap.DeleteObject();
    	MemBitmap.CreateCompatibleBitmap(&dc, m_rectGraph.Width(), m_rectGraph.Height());
		pOldBitmap = MemDC.SelectObject(&MemBitmap);
		DrawBackGround();
		DrawRect();
		Invalidate(FALSE);
	}
}

BYTE CPingTestDlg::DrawPingGraph(BYTE bColor)
{
	CPen *oldPen;	
	oldPen = MemDC.SelectObject(&lineColor[bColor]);

	int x = m_rectPing.right-2;
	double y = (double)m_rectPing.Height() / (double)m_dwLimitPing ;

	int iPingCount = (int)m_vPing.size() - 1;
	for(int i = iPingCount ; i >= 0 ; i-- )
	{
		if(i == 0 ) 
			break;

        if( m_vPing[i] >= m_dwLimitPing || m_vPing[i-1] >= m_dwLimitPing )
			return 1;

		MemDC.MoveTo(x, (int)(m_rectPing.bottom - m_vPing[i] * y));
		x -= XINTERVAL;
		if(x < m_rectPing.left )
			x = m_rectPing.left;

		MemDC.LineTo(x,  (int)(m_rectPing.bottom -  m_vPing[i-1] * y));

		if( x == m_rectPing.left )
			break;
	}

	MemDC.SelectObject(oldPen);

	return 0;
}

void CPingTestDlg::DrawGraph()
{
	DrawBackGround();
	DrawRect();

	BYTE bResult = 0;

	bResult = DrawPingGraph();

	switch(bResult)
	{
	case 0:
		Invalidate(FALSE);
		break;
	case 1:
		m_dwLimitPing *= 2;
		DrawGraph();
		break;
	default:
		break;
	}	
}

void CPingTestDlg::Init()
{
	m_dwMaxPing = 0;
	m_dwLimitPing = 100;
	m_vPing.clear();
	m_chkPingLoop.SetCheck(BST_CHECKED);
	GetDlgItem(IDC_EDIT_PINGSECOND)->EnableWindow(TRUE);
	SetDlgItemText(IDC_EDIT_PINGSECOND,_T("1"));
}