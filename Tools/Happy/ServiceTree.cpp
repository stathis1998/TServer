// ServiceTree.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Happy.h"

#include "MainFrm.h"
#include "HappyDoc.h"
#include "HappyView.h"
#include ".\servicetree.h"

// CServiceTree

IMPLEMENT_DYNCREATE(CServiceTree, CFormView)

CServiceTree::CServiceTree()
	: CFormView(CServiceTree::IDD)
{
	
}

CServiceTree::~CServiceTree()
{
}

void CServiceTree::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_SERVICE, m_treeCtrl);
}

BEGIN_MESSAGE_MAP(CServiceTree, CFormView)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_SERVICE, OnTvnSelchangedTreeService)
	ON_NOTIFY(TVN_BEGINDRAG, IDC_TREE_SERVICE, OnTvnBegindragTreeService)
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()


// CServiceTree �����Դϴ�.

#ifdef _DEBUG
void CServiceTree::AssertValid() const
{
	CFormView::AssertValid();
}

void CServiceTree::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CServiceTree �޽��� ó�����Դϴ�.

void CServiceTree::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

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

HTREEITEM CServiceTree::FindService(DWORD dwID, HTREEITEM hStart)
{
	/*
	if(m_treeCtrl.GetItemData(hStart) == dwID)
		return hStart;

	HTREEITEM hItem;
	HTREEITEM hChildItem ;

	if(m_treeCtrl.ItemHasChildren(hStart))
	{
		hChildItem = m_treeCtrl.GetChildItem(hStart);
		if(hChildItem != NULL)
		{
			hItem = FindService(dwID, hChildItem);
			if(hItem)
				return hItem;
		}
	}
	else
	{
		hChildItem = hStart;
		while(hChildItem)
		{
			hItem = m_treeCtrl.GetNextItem(hChildItem, TVGN_NEXT);
			if (m_treeCtrl.GetItemData(hChildItem) == dwID)
				return hChildItem;
			hChildItem = hItem;
		}
	}
	return NULL;
	*/

	HTREEITEM hitemFind, hItemChile, hItemSibling;
    hitemFind = hItemChile = hItemSibling = NULL;

	if(m_treeCtrl.GetItemData(hStart) == dwID)
		hitemFind = hStart;
    else
    {
		hItemChile = m_treeCtrl.GetChildItem(hStart);
		if(hItemChile)
			hitemFind = FindService(dwID, hItemChile);

        hItemSibling = m_treeCtrl.GetNextSiblingItem(hStart);
		if(hitemFind==NULL && hItemSibling)
			hitemFind = FindService(dwID, hItemSibling);
    }

	return hitemFind;
}

void CServiceTree::UpdateServiceTree(DWORD dwID)
{
	if(dwID == 0xFFFFFFFF)
	{
		m_treeCtrl.DeleteAllItems();
		return;
	}

	CHappyDoc * pDoc = (CHappyDoc *)GetDocument();
	if(!pDoc)
		return;

	LPHAPPYSERVICE pService = pDoc->FindService(dwID);
	if(pService)
	{
		int nImage = SelectServiceImage(pService->m_dwStatus);
		HTREEITEM hItem = FindService(pService->m_dwID, m_treeCtrl.GetRootItem());
		if(hItem)
			m_treeCtrl.SetItemImage(hItem, nImage, nImage);

		/*
		HTREEITEM hItem = m_treeCtrl.GetFirstVisibleItem();
		while(hItem)
		{
			HTREEITEM hFound = FindService(pService->m_dwID, hItem);
			if(hFound)
			{
				m_treeCtrl.SetItemImage(hFound, nImage, nImage);
			}
			hItem = m_treeCtrl.GetNextVisibleItem(hItem);
		}
		*/
	}
	UpdateWindow();
}

void CServiceTree::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	m_bIsGroup = FALSE;
}

void CServiceTree::InsertInitData()
{
	CHappyDoc * pDoc = (CHappyDoc *)GetDocument();
	if(!pDoc) return;

	m_treeCtrl.DeleteAllItems();

	CImageList * pImageList = m_treeCtrl.GetImageList(TVSIL_NORMAL);
	if(!pImageList)
	{
		pImageList = new CImageList();
		pImageList->Create(16,16,ILC_COLOR4,3,3);
		pImageList->Add(AfxGetApp()->LoadIcon(IDR_SERVICE_STOP));
		pImageList->Add(AfxGetApp()->LoadIcon(IDR_SERVICE_RUN));
		pImageList->Add(AfxGetApp()->LoadIcon(IDR_SERVICE_PENDING));
		pImageList->Add(AfxGetApp()->LoadIcon(IDR_SERVICE_NOTCONTROL));
		pImageList->Add(AfxGetApp()->LoadIcon(IDR_GROUP_NOMAL));
		pImageList->Add(AfxGetApp()->LoadIcon(IDR_GROUP_SELECT));
		pImageList->Add(AfxGetApp()->LoadIcon(IDR_WORLD_NOMAL));
		pImageList->Add(AfxGetApp()->LoadIcon(IDR_WORLD_SELECT));
	}
	m_treeCtrl.SetImageList(pImageList, TVSIL_NORMAL);

	HTREEITEM hGroup[255]={NULL};
	HTREEITEM hSvrType[255][255]={NULL};
	TCHAR szText[50];
	TVINSERTSTRUCT tv;
	HTREEITEM hItem;
	tv.hInsertAfter = TVI_LAST;
	tv.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;

	MAPSERVICE::iterator itS;
	for(itS=pDoc->m_mapService.begin(); itS!=pDoc->m_mapService.end(); itS++)
	{
		LPHAPPYSERVICE pSvr = (*itS).second;
		lstrcpy(szText, pSvr->m_strName);
		tv.item.pszText = szText;
		if(pSvr->m_pSvrType)
		{
			if(!hGroup[pSvr->m_pGroup->m_bID])
			{
				tv.hParent = 0;
				lstrcpy(szText, pSvr->m_pGroup->m_strName);
				tv.item.pszText = szText;
				tv.item.iImage = IMAGE_WORLD_NOMAL;
				tv.item.iSelectedImage = IMAGE_WORLD_SELECTED;
				tv.item.lParam = pSvr->m_dwID & 0x00FF0000;
				hItem = m_treeCtrl.InsertItem(&tv);
				hGroup[pSvr->m_pGroup->m_bID] = hItem;
			}

			if(!hSvrType[pSvr->m_pGroup->m_bID][pSvr->m_pSvrType->m_bType])
			{
				tv.hParent = hGroup[pSvr->m_pGroup->m_bID];
				lstrcpy(szText, pSvr->m_pSvrType->m_strName);
				tv.item.pszText = szText;
				tv.item.iImage = IMAGE_GROUP_NOMAL;
				tv.item.iSelectedImage = IMAGE_GROUP_SELECTED;
				tv.item.lParam = pSvr->m_dwID & 0x00FFFF00;
				hItem = m_treeCtrl.InsertItem(&tv);
				hSvrType[pSvr->m_pGroup->m_bID][pSvr->m_pSvrType->m_bType] = hItem;
			}

			tv.hParent = hSvrType[pSvr->m_pGroup->m_bID][pSvr->m_pSvrType->m_bType];
			lstrcpy(szText, pSvr->m_strName);
			tv.item.pszText = szText;
			tv.item.iImage = SelectServiceImage(pSvr->m_dwStatus);
			tv.item.iSelectedImage = tv.item.iImage;
			tv.item.lParam = pSvr->m_dwID;
			HTREEITEM hITEM = m_treeCtrl.InsertItem(&tv);
			m_treeCtrl.SetItemData(hITEM, pSvr->m_dwID);
		}
	}

	///////////////////////////////////////////////
	// ���·� Graph
	m_hDragItem = NULL;
	m_DragFlag = 0;	
	m_Cursor = LoadCursor(0, IDC_ARROW);
	///////////////////////////////////////////////
}

int CServiceTree::SelectServiceImage(DWORD dwStatus)
{
	int image;
	switch(dwStatus)
	{
	case DCSVC_STAT_RUNNING: image = IMAGE_SERVCIE_RUNNING; break;
	case DCSVC_STAT_STOPPED: image = IMAGE_SERVICE_STOP; break;
	case DCSVC_STAT_START_PENDING:
	case DCSVC_STAT_STOP_PENDING:
	case DCSVC_STAT_CONTINUE_PENDING:
	case DCSVC_STAT_PENDING:
		image = IMAGE_SERVICE_PENDING;
		break;
	case DCSVC_STAT_CANNOTCONTROL:
	case DCSVC_STAT_PAUSED:
	default:
		image = IMAGE_SERVICE_NOTCONTROL;
		break;
	}
	return image;
}

void CServiceTree::OnDestroy()
{
	CFormView::OnDestroy();
	CImageList * pList = m_treeCtrl.GetImageList(TVSIL_NORMAL);
	if(pList)
		delete pList;
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

void CServiceTree::OnTvnSelchangedTreeService(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	CHappyDoc * pDoc = (CHappyDoc *)GetDocument();
	pDoc->m_bViewAllService = 0;
	pDoc->m_dwSelectedService = (DWORD)pNMTreeView->itemNew.lParam;
	HTREEITEM hItemNew = m_treeCtrl.GetParentItem(pNMTreeView->itemNew.hItem);
	HTREEITEM hItemOld = m_treeCtrl.GetParentItem(pNMTreeView->itemOld.hItem);
	HTREEITEM hWorldNew = hItemNew ? m_treeCtrl.GetParentItem(hItemNew) : NULL;
	HTREEITEM hWorldOld = hItemOld ? m_treeCtrl.GetParentItem(hItemOld) : NULL;

	if(hItemNew && !hWorldNew)
		m_bIsGroup = TRUE;
	else
		m_bIsGroup = FALSE;

	if(hItemNew && hItemNew == hItemOld)
	{
		if(hWorldNew)
			return;

		m_treeCtrl.SetItemImage(pNMTreeView->itemOld.hItem, IMAGE_GROUP_NOMAL, IMAGE_GROUP_NOMAL);
		m_treeCtrl.SetItemImage(pNMTreeView->itemNew.hItem, IMAGE_GROUP_SELECTED, IMAGE_GROUP_SELECTED);
	}
	else
	{
		if(hWorldOld)
		{
			m_treeCtrl.SetItemImage(hWorldOld, IMAGE_WORLD_NOMAL, IMAGE_WORLD_NOMAL);
			m_treeCtrl.SetItemImage(hItemOld, IMAGE_GROUP_NOMAL, IMAGE_GROUP_NOMAL);
		}
		else if(hItemOld)
		{
			m_treeCtrl.SetItemImage(hItemOld, IMAGE_WORLD_NOMAL, IMAGE_WORLD_NOMAL);
			m_treeCtrl.SetItemImage(pNMTreeView->itemOld.hItem, IMAGE_GROUP_NOMAL, IMAGE_GROUP_NOMAL);
		}
		else
			m_treeCtrl.SetItemImage(pNMTreeView->itemOld.hItem, IMAGE_WORLD_NOMAL, IMAGE_WORLD_NOMAL);

		if(hWorldNew)
		{
			m_treeCtrl.SetItemImage(hWorldNew, IMAGE_WORLD_SELECTED, IMAGE_WORLD_SELECTED);
			m_treeCtrl.SetItemImage(hItemNew, IMAGE_GROUP_SELECTED, IMAGE_GROUP_SELECTED);
		}
		else if(hItemNew)
		{
			m_treeCtrl.SetItemImage(hItemNew, IMAGE_WORLD_SELECTED, IMAGE_WORLD_SELECTED);
			m_treeCtrl.SetItemImage(pNMTreeView->itemNew.hItem, IMAGE_GROUP_SELECTED, IMAGE_GROUP_SELECTED);
		}
		else
			m_treeCtrl.SetItemImage(pNMTreeView->itemNew.hItem, IMAGE_WORLD_SELECTED, IMAGE_WORLD_SELECTED);
	}

	*pResult = 0;

	CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
	if(pFrm == NULL)
		return;
	CServiceList * pList = (CServiceList *)pFrm->m_wndSplitter.GetView(pFrm->m_nList);
	pList->UpdateServiceList(0);
}
//////////////////////////////////////////////////////////////////////////
// ���·� Graph Ʈ�������ۿ��� ������ �巡�� �ؼ� �׷����� �߰�
void CServiceTree::OnTvnBegindragTreeService(NMHDR *pNMHDR, LRESULT *pResult)
{
	CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
	CHappyDoc * pDoc = (CHappyDoc *)GetDocument();

	if( pFrm->GetCurView() == 1 &&	pFrm->GetGraphCheck() )
	{
		LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
		
		m_hDragItem = pNMTreeView->itemNew.hItem; // ���� ���õ� ������ �ڵ��� ���

		if( m_treeCtrl.GetParentItem(m_hDragItem) == NULL ) // ���� �巡�׽� ����
			return;

		m_DragFlag = 1;	
		m_Cursor = LoadCursor(0, IDC_WAIT);
		SetCursor(m_Cursor);
		SetCapture();	
	}

	*pResult = 0;
}

void CServiceTree::OnLButtonUp(UINT nFlags, CPoint point)
{	
	if( m_DragFlag )
	{
        ReleaseCapture(); // ���콺 �޽��� ĸ�� ����� �����Ѵ�.
		m_Cursor = LoadCursor(0, IDC_ARROW);
		m_DragFlag = 0;
		m_hDragItem = NULL;
	}

	CFormView::OnLButtonUp(nFlags, point);
}

BOOL CServiceTree::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	SetCursor(m_Cursor);
	return TRUE;
	//return CFormView::OnSetCursor(pWnd, nHitTest, message);	
}

void CServiceTree::OnMouseMove(UINT nFlags, CPoint point)
{	
	if(m_DragFlag)
	{		
		if(point.x > treeRect.right)
			ReleaseCapture(); // ���콺 �޽��� ĸ�� ����� �����Ѵ�.		
	}

	CFormView::OnMouseMove(nFlags, point);
}
//////////////////////////////////////////////////////////////////////////

// �ʼ����� ���� �� �Ŀ� ���콺 ������ Ŭ������ �� �޴� ����
void CServiceTree::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

	HTREEITEM hSelectedItem = m_treeCtrl.GetSelectedItem();

	// ���弭�� �������� �������� ��� ����
	if( !hSelectedItem || m_bIsGroup )
		return;

	// ������ �������� �θ� �������(�� ��������� �������� �������� ���) ����
	if( !m_treeCtrl.GetParentItem(hSelectedItem) )
		return;

	// �� ���� �������� ������ �Ŀ� ���õ� �� ���� ������ ���� �ȿ��� ��Ŭ�� �ߴ��� üũ
	CPoint  p;
	GetCursorPos(&p);
	::ScreenToClient(m_treeCtrl.m_hWnd, &p);
	HTREEITEM hHitItem = m_treeCtrl.HitTest(p);
	
	if( hHitItem != hSelectedItem )
		return;

	// �ʼ����� ���� ��Ʈ�Ѱ�������(��� or ���������) üũ
	int nImg,nSelectedImg;
	nImg = nSelectedImg = 0;
	m_treeCtrl.GetItemImage(hSelectedItem,nImg,nSelectedImg);
	if( nSelectedImg == IMAGE_SERVICE_NOTCONTROL )
		return;

	m_dwSelectedMapSvrID = (DWORD)m_treeCtrl.GetItemData(hSelectedItem);


	// �޴� ����
	CMenu menu;
	menu.CreatePopupMenu();

	menu.AppendMenu(MF_STRING, IDS_MAPSVRCONNECT, "ReConnect");
	//menu.AppendMenu(MF_SEPARATOR);
	//menu.AppendMenu(MF_STRING, IDS_MAPSVRDISCONNECT, "DisConnect");

	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, 
		point.x, point.y, GetParent());

}

// �˾��޴����� [ReConnect] �� �������� �� ����Ǵ� �Լ�
// CTabCtrlHappy::OnCommand() �Լ����� �� �Լ��� ȣ���Ѵ�.
void CServiceTree::MapSvrConnect()
{	
	CHappyDoc * pDoc = (CHappyDoc *)GetDocument();
	pDoc->SendCT_RECONNECT_REQ(m_dwSelectedMapSvrID);
}

// �˾��޴����� [DisConnect] �� �������� �� ����Ǵ� �Լ�
void CServiceTree::MapSvrDisConnect()
{
	CHappyDoc* pDoc = (CHappyDoc*)GetDocument();
	pDoc->SendCT_DISCONNECT_REQ(m_dwSelectedMapSvrID);

}
