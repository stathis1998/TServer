// PosListDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "GMTool.h"
#include "PosListDlg.h"
#include ".\poslistdlg.h"

#include "GMToolDoc.h"
#include "GMTool.h"
#include "GMToolDlg.h"

// CPosListDlg ��ȭ �����Դϴ�.
IMPLEMENT_DYNAMIC(CPosListDlg, CDialog)
CPosListDlg::CPosListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPosListDlg::IDD, pParent)
	,m_dwSelectIndex	(0)
	,m_bLoad			(FALSE)
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
	ON_BN_CLICKED(IDC_POSFIND, OnBnClickedPosfind)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnNMDblclkList1)
END_MESSAGE_MAP()


// CPosListDlg �޽��� ó�����Դϴ�.

void CPosListDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	ItemSelect();
	ShowWindow(SW_HIDE);


	//OnOK();
}

void CPosListDlg::OnBnClickedDelete()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	ItemDelete();
}

void CPosListDlg::OnBnClickedPosfind()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.	
	CString strTemp;
	strTemp.Empty();
	GetDlgItemText(IDC_EDIT_POSFIND,strTemp);
	if (strTemp.IsEmpty() )
		return;


	// Document ����Ʈ ���
	CGMToolDoc* pDoc = CGMToolDoc::GetInstance();
	if(!pDoc)
		return ;

	char szTmp[100];
	strcpy(szTmp,strTemp.GetBuffer(0) );
	
	LISTUSERPOS::iterator it = pDoc->m_listUSERPOS.begin();
	DWORD dwIndex = 0;
	while(dwIndex < m_dwSelectIndex )
	{
		it++;
		dwIndex++;
	}

	if(it == pDoc->m_listUSERPOS.end() )
	{
		m_dwSelectIndex = 0;
		it = pDoc->m_listUSERPOS.begin();
	}

	for( ; it != pDoc->m_listUSERPOS.end(); it++, m_dwSelectIndex++)
	{
		if( strstr( (*it)->POSNAME, szTmp) )
		{
			m_listctrPos.EnsureVisible(m_dwSelectIndex,TRUE);
			m_listctrPos.SetItemState(m_dwSelectIndex,LVIS_SELECTED,LVIS_SELECTED);
			m_listctrPos.SetFocus();
			m_dwSelectIndex++;
			return;
		}
	}

	if( it == pDoc->m_listUSERPOS.end() )
	{
		m_dwSelectIndex = 0;
		MessageBox(MSG_NOT_FOUND,MB_OK);
	}

	m_listctrPos.SetFocus();
}

void CPosListDlg::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	
	ShowWindow(SW_HIDE);

	//OnCancel();
}



// ����Ʈ ��Ʈ�� �ʱ� ����
void CPosListDlg::Init()
{
	// ����Ʈ ��Ʈ���� ��� �÷� ����
	for(int i = 0; i < MAX_POSLIST_COLUMN ; i++)
        m_listctrPos.DeleteColumn(i);

	
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

void CPosListDlg::InsertItemToListCtr(int _iRow,USERPOS* _USERPOS)
{	
	CString strTmp;
	LV_ITEM lvItem;

	lvItem.mask		= LVIF_TEXT;
	lvItem.iItem	= _iRow; // Row �ε���
	
	for(int iCol = 0; iCol < MAX_POSLIST_COLUMN ; iCol++)
	{
		switch(iCol)
		{
		case 0 : strTmp.Format("%s",_USERPOS->POSNAME);		break;
		case 1 : strTmp.Format("%d",_USERPOS->ChannelNum ); break;
		case 2 : strTmp.Format("%d",_USERPOS->MapID );		break;
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
	
}

void CPosListDlg::LoadPosListFromData()
{
	// Document ����Ʈ ���
	CGMToolDoc* pDoc = CGMToolDoc::GetInstance();
	if(!pDoc)
		return ;

	ClearListItem();

	int k = m_listctrPos.GetItemCount();

	LISTUSERPOS::iterator it;
	int i = 0;
	for( it = pDoc->m_listUSERPOS.begin(); it != pDoc->m_listUSERPOS.end(); it++)
	{
		InsertItemToListCtr(i,(*it) );
		i += 1;
	}

	m_listctrPos.UpdateData();
	m_bLoad = TRUE;
}


void CPosListDlg::ItemDelete()
{
	if(MessageBox(MSG_DELETE_Q,_T("DELETE"),MB_YESNO) == IDNO) // AfxMessageBox �� ���� ������ȭ���� ���� ��.
	{
		m_listctrPos.SetFocus();
		return;
	}
	
	// Document ����Ʈ ���
	CGMToolDoc* pDoc = CGMToolDoc::GetInstance();
	if(!pDoc)
		return;
	

	// ���õ� ������ ã��	
	int iCount = m_listctrPos.GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		if( m_listctrPos.GetItemState(i, LVIS_SELECTED) != 0 )
		{
			CString strName;
			strName = m_listctrPos.GetItemText(i,0);



			char szName[100];
			int len = strName.GetLength();
			strcpy(szName,strName.GetBuffer(0));

			LISTUSERPOS::iterator it;
			it = find_if( pDoc->m_listUSERPOS.begin(), pDoc->m_listUSERPOS.end(), CPosEqual(szName) );
			if( it != pDoc->m_listUSERPOS.end() )
			{
				delete (*it);
				pDoc->m_listUSERPOS.erase(it);

				// list control ����
				m_listctrPos.DeleteItem(i);
				m_listctrPos.UpdateData();
				break;
			}
			break;
		}
	}

	m_listctrPos.SetFocus();
}


void CPosListDlg::ItemDelete_Multi()
{
	POSITION pos;
	while(pos = m_listctrPos.GetFirstSelectedItemPosition() )
	{
		int iSelectItem = m_listctrPos.GetNextSelectedItem(pos);
		m_listctrPos.DeleteItem(iSelectItem);
	}

	m_listctrPos.UpdateData();
}

bool CPosListDlg::ItemSelect()
{
	// Document ����Ʈ ���
	CGMToolDoc* pDoc = CGMToolDoc::GetInstance();
	if(!pDoc)
		return FALSE;

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
			strcpy(UserPos.POSNAME, strTemp);			
			
			strTemp = m_listctrPos.GetItemText(i,1);
			UserPos.ChannelNum = atoi(strTemp);

			strTemp = m_listctrPos.GetItemText(i,2);
			UserPos.MapID = atoi(strTemp);

			strTemp = m_listctrPos.GetItemText(i,3);
			UserPos.PosX = (float)atof(strTemp);

			strTemp = m_listctrPos.GetItemText(i,4);
			UserPos.PosY = (float)atof(strTemp);

			strTemp = m_listctrPos.GetItemText(i,5);
			UserPos.PosZ = (float)atof(strTemp);
			
			CGMToolApp *pApp = (CGMToolApp *)AfxGetApp(); // App ������ ����			
			CGMToolDlg *pdlgGMTool = (CGMToolDlg *) pApp->GetMainWnd(); // ���������� �� ����
			if(pdlgGMTool)
                pdlgGMTool->m_pdlgUserMove->SetData(UserPos);		

			return true;
		}
	}

	return false;
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

void CPosListDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

	bool bFind = ItemSelect();
	
	if( bFind) 
	{
		ShowWindow(SW_HIDE);
		//OnOK();		
	}
}

BOOL CPosListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
