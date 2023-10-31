// HappyGroup.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Happy.h"
#include "HappyGroup.h"
#include ".\happygroup.h"


// CHappyGroup

IMPLEMENT_DYNCREATE(CHappyGroup, CFormView)

CHappyGroup::CHappyGroup()
	: CFormView(CHappyGroup::IDD)
{
}

CHappyGroup::~CHappyGroup()
{
}

void CHappyGroup::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_GROUP, m_tabCtrl);
}

void CHappyGroup::OnAddTab(CString strName, CWnd * pWnd)
{
	static int nPage=0;
	m_tabCtrl.AddTabPage(strName, nPage++, pWnd);
}

BEGIN_MESSAGE_MAP(CHappyGroup, CFormView)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_GROUP, OnTcnSelchangeTabGroup)
END_MESSAGE_MAP()


// CHappyGroup �����Դϴ�.

#ifdef _DEBUG
void CHappyGroup::AssertValid() const
{
	CFormView::AssertValid();
}

void CHappyGroup::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CHappyGroup �޽��� ó�����Դϴ�.

void CHappyGroup::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (GetSafeHwnd())
	{
		CRect rect;
		GetClientRect(&rect);
		if (m_tabCtrl.GetSafeHwnd())
		{
			m_tabCtrl.MoveWindow(&rect);
			m_tabCtrl.OnMoveWindow();
		}
	}
}

int CHappyGroup::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.
	return 0;
}

void CHappyGroup::OnTcnSelchangeTabGroup(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
}
