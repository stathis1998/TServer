// PosListDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Happy.h"
#include "PosListDlg.h"

#include "MainFrm.h"
#include "HappyDoc.h"
#include ".\poslistdlg.h"

// CPosListDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CPosListDlg, CDialog)
CPosListDlg::CPosListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPosListDlg::IDD, pParent)
	, m_dwSelectIndex	(0)
{	
}

CPosListDlg::~CPosListDlg()
{
}

void CPosListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listctrPos);
}


BEGIN_MESSAGE_MAP(CPosListDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_DELETE, OnBnClickedDelete)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnNMDblclkList1)
	ON_BN_CLICKED(IDC_POSFIND, OnBnClickedPosfind)
END_MESSAGE_MAP()


// CPosListDlg �޽��� ó�����Դϴ�.


// ����Ʈ ��Ʈ�� �ʱ� ����
void CPosListDlg::Init()
{
	// ����Ʈ ��Ʈ���� ��� �÷� ����
	for(int i = 0; i < MAX_POSLIST_COLUMN ; i++)
        m_listctrPos.DeleteColumn(0);

	
	LV_COLUMN lvColumn;
	char*list[MAX_POSLIST_COLUMN]={"Name", "Channel", "Map", "X", "Y", "Z"};
	int nWidth[MAX_POSLIST_COLUMN]={150,70,60,90,90,90};
	
	for(int i = 0; i< MAX_POSLIST_COLUMN; i++)
	{
		lvColumn.mask		= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvColumn.fmt		= LVCFMT_CENTER;
		lvColumn.pszText	= list[i];
		lvColumn.iSubItem	= i;
		lvColumn.cx			= nWidth[i];

		m_listctrPos.InsertColumn(i,&lvColumn);
		//m_listctrPos.InsertColumn(0,"NAME",LVCFMT_CENTER,180);
	}
	
	// �ϳ��� ���� ��ü�� ���õǵ��� ����
	m_listctrPos.SetExtendedStyle(LVS_EX_FULLROWSELECT);
}

void CPosListDlg::InsertItemToListCtr(int _iRow, LPUSERPOS _USERPOS)
{	
	CString strTmp;
	LV_ITEM lvItem;

	lvItem.mask		= LVIF_TEXT;
	lvItem.iItem	= _iRow; // Row �ε���	
	
	for(int iCol = 0; iCol < MAX_POSLIST_COLUMN ; iCol++)
	{
		switch(iCol)
		{
		case 0 : strTmp.Format("%s",_USERPOS->strName);		break;
		case 1 : strTmp.Format("%d",_USERPOS->bChannelNum ); break;
		case 2 : strTmp.Format("%d",_USERPOS->wMapID );		break;
		case 3 : strTmp.Format("%.2f",_USERPOS->PosX );		break;
		case 4 : strTmp.Format("%.2f",_USERPOS->PosY );		break;
		case 5 : strTmp.Format("%.2f",_USERPOS->PosZ );		break;
		}
		
		lvItem.iSubItem = iCol ; // �÷� �ε���
		lvItem.pszText	= strTmp.GetBuffer(0);
		
		if( iCol == 0 )
			m_listctrPos.InsertItem(&lvItem);
		else
            m_listctrPos.SetItem(&lvItem);
	}
	m_listctrPos.SetItemData(_iRow, (DWORD)_USERPOS);
}

void CPosListDlg::LoadPosListFromData()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	ClearListItem();

	SETUSERPOS::iterator it;
	int i = 0;
	for( it = pDoc->m_setUSERPOS.begin(); it != pDoc->m_setUSERPOS.end(); it++)
	{
		InsertItemToListCtr(i,(*it));
		i ++;
	}

	m_listctrPos.UpdateData();
}


void CPosListDlg::ItemDelete()
{
	if(AfxMessageBox("Are you sure you want to delete?",MB_YESNO) == IDNO ) //���� �����Ͻðڽ��ϱ�
		return;
	

	// Document ����Ʈ ���
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}	

	// ���õ� ������ ã��	
	int iCount = m_listctrPos.GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		if( m_listctrPos.GetItemState(i, LVIS_SELECTED) != 0 )
		{
			LPUSERPOS pPos = (LPUSERPOS)m_listctrPos.GetItemData(i);
			DeleteListData(pPos);
		}
	}
}

bool CPosListDlg::ItemSelect()
{
	// Document ����Ʈ ���
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return false;
	}

	// ���õ� ������ ã��
	int iCount = m_listctrPos.GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		if( m_listctrPos.GetItemState(i, LVIS_SELECTED) != 0 )
		{
			// ���õ� ������ ����
			
			CString strTemp;
			USERPOS UserPos;

			strTemp = m_listctrPos.GetItemText(i,0);
			UserPos.strName= strTemp;			
			
			strTemp = m_listctrPos.GetItemText(i,1);
			UserPos.bChannelNum = atoi(strTemp);

			strTemp = m_listctrPos.GetItemText(i,2);
			UserPos.wMapID = atoi(strTemp);

			strTemp = m_listctrPos.GetItemText(i,3);
			UserPos.PosX = (float)atof(strTemp);

			strTemp = m_listctrPos.GetItemText(i,4);
			UserPos.PosY = (float)atof(strTemp);

			strTemp = m_listctrPos.GetItemText(i,5);
			UserPos.PosZ = (float)atof(strTemp);

			pDoc->m_pdlgGMTool->m_dlgUserMove->SetData(&UserPos);

			return true;
		}
	}

	return false;

}
void CPosListDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	ItemSelect();

	ShowWindow(SW_HIDE);
	//ClearListItem();

	//OnOK();
}

void CPosListDlg::OnBnClickedDelete()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	ItemDelete();
}


void CPosListDlg::ClearListItem()
{
	int iCount = m_listctrPos.GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		m_listctrPos.DeleteItem(i);
	}
    
	m_listctrPos.DeleteAllItems();
}
void CPosListDlg::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	ShowWindow(SW_HIDE);
	//ClearListItem();
	//OnCancel();
}

// �������� ����Ŭ������ ��
void CPosListDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

	bool bFind = ItemSelect();
	
	if( bFind) 
	{
		ShowWindow(SW_HIDE);
		//ClearListItem();
		//OnOK();
		
	}	
}

void CPosListDlg::OnBnClickedPosfind()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString strTemp;
	GetDlgItemText(IDC_EDIT_POSFIND,strTemp);
	if (strTemp.IsEmpty() )
		return;

	// Document ����Ʈ ���
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return ;
	}

	char szTmp[100];
	strcpy(szTmp,strTemp.GetBuffer(0) );
	
	SETUSERPOS::iterator it = pDoc->m_setUSERPOS.begin();
	DWORD dwIndex = 0;
	while(dwIndex < m_dwSelectIndex )
	{
		it++;
		dwIndex++;
	}

	if(it == pDoc->m_setUSERPOS.end() )
	{
		m_dwSelectIndex = 0;
		it = pDoc->m_setUSERPOS.begin();
	}

	for( ; it != pDoc->m_setUSERPOS.end(); it++, m_dwSelectIndex++)
	{
		if( strstr( (*it)->strName, szTmp) )
		{
			m_listctrPos.EnsureVisible(m_dwSelectIndex,TRUE);
			m_listctrPos.SetItemState(m_dwSelectIndex,LVIS_SELECTED,LVIS_SELECTED);
			m_listctrPos.SetFocus();
			m_dwSelectIndex++;
			return;
		}
	}

	if( it == pDoc->m_setUSERPOS.end() )
	{
		m_dwSelectIndex = 0;
		AfxMessageBox("Not Found    ",MB_OK);
	}
}

BYTE CPosListDlg::FindListData(LPUSERPOS pPos)
{
	if(!pPos)
		return FALSE;

	// Document ����Ʈ ���
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return FALSE;
	}
	if(pDoc->m_setUSERPOS.empty())
		return FALSE;

	SETUSERPOS::iterator it = pDoc->m_setUSERPOS.find(pPos);
	if(it != pDoc->m_setUSERPOS.end())
		return TRUE;

	return FALSE;
}

void CPosListDlg::DeleteListData(LPUSERPOS pPos)
{
	if(!pPos)
		return ;

	// Document ����Ʈ ���
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return ;
	}

	if(pDoc->m_setUSERPOS.empty())
		return ;

	SETUSERPOS::iterator it = pDoc->m_setUSERPOS.find(pPos);
	if(it != pDoc->m_setUSERPOS.end())
	{
		delete (*it);
		pDoc->m_setUSERPOS.erase(it);
	}
}