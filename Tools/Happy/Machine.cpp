// Machine.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Happy.h"

#include "MainFrm.h"
#include "HappyDoc.h"
#include "HappyView.h"
#include ".\machine.h"


// CMachine

IMPLEMENT_DYNCREATE(CMachine, CFormView)

CMachine::CMachine()
	: CFormView(CMachine::IDD)
{
}

CMachine::~CMachine()
{
}

void CMachine::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_MACHINE, m_treeCtrl);
}

BEGIN_MESSAGE_MAP(CMachine, CFormView)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_MACHINE, OnTvnSelchangedTreeMachine)
END_MESSAGE_MAP()


// CMachine �����Դϴ�.

#ifdef _DEBUG
void CMachine::AssertValid() const
{
	CFormView::AssertValid();
}

void CMachine::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CMachine �޽��� ó�����Դϴ�.

void CMachine::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (GetSafeHwnd())
	{
		CRect rect;
		GetClientRect(&rect);
		if (m_treeCtrl.GetSafeHwnd())
		{
			m_treeCtrl.MoveWindow(&rect);
			ShowWindow(SW_SHOW);
		}
	}
	GetClientRect(&treeRect); // ���·� Graph
}

void CMachine::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	m_strSelected = _T("");
}

void CMachine::OnDestroy()
{
	CFormView::OnDestroy();

	CImageList * pImageList = m_treeCtrl.GetImageList(TVSIL_NORMAL);
	if(pImageList)
		delete pImageList;
}

void CMachine::InsertInitData()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	m_treeCtrl.DeleteAllItems();

	CImageList * pImageList = m_treeCtrl.GetImageList(TVSIL_NORMAL);
	if(!pImageList)
	{
		pImageList = new CImageList();
		pImageList->Create(16,16,ILC_COLOR4,3,3);
		pImageList->Add(AfxGetApp()->LoadIcon(IDR_GROUP_NOMAL));
		pImageList->Add(AfxGetApp()->LoadIcon(IDR_GROUP_SELECT));
	}
	m_treeCtrl.SetImageList(pImageList, TVSIL_NORMAL);

	TCHAR szText[50];
	TVINSERTSTRUCT tv;
	tv.hInsertAfter = TVI_LAST;
	tv.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;

	MAPMACHINE::iterator itS;
	for(itS=pDoc->m_mapMachine.begin(); itS!=pDoc->m_mapMachine.end(); itS++)
	{
		LPHAPPYMACHINE pMachine = (*itS).second;
		tv.hParent = 0;
		lstrcpy(szText, pMachine->m_strName);
		tv.item.pszText = szText;
		tv.item.iImage = 0;
		tv.item.iSelectedImage = 1;
		m_treeCtrl.InsertItem(&tv);
	}
}

void CMachine::ClearMachineTree()
{
	m_treeCtrl.DeleteAllItems();
	UpdateWindow();

	m_strSelected = _T("");
}

BYTE CMachine::GetSelectedMachineID()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return 0;
	}

	MAPMACHINE::iterator it;
	for(it=pDoc->m_mapMachine.begin(); it!=pDoc->m_mapMachine.end(); it++)
	{
		if(m_strSelected == (*it).second->m_strName)
			return (*it).second->m_bID;
	}

	return 0;
}
void CMachine::OnTvnSelchangedTreeMachine(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	HTREEITEM hItem = pNMTreeView->itemNew.hItem;
	m_strSelected = m_treeCtrl.GetItemText(hItem);
	*pResult = 0;
}
