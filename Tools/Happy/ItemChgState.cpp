// ItemChgState.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Happy.h"
#include "MainFrm.h"
#include "HappyDoc.h"
#include "ItemChgState.h"
#include ".\itemchgstate.h"


// CItemChgState ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CItemChgState, CDialog)
CItemChgState::CItemChgState(CWnd* pParent /*=NULL*/)
	: CDialog(CItemChgState::IDD, pParent)	
{
}

CItemChgState::~CItemChgState()
{
}

void CItemChgState::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EB_ITINPUT, m_ebInput);
	DDX_Control(pDX, IDC_SAVE_POS, m_lblFindType);
	DDX_Control(pDX, IDC_LC_ITLIST, m_lcFindList);	
	DDX_Control(pDX, IDC_COMBO1, m_cbWorld);
}


BEGIN_MESSAGE_MAP(CItemChgState, CDialog)
	ON_BN_CLICKED(IDC_RA_ITID, OnBnClickedRaItid)
	ON_BN_CLICKED(IDC_RB_ITNAME, OnBnClickedRbItname)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_FINDITEM, OnBnClickedBtnFinditem)
END_MESSAGE_MAP()


// CItemChgState �޽��� ó�����Դϴ�.


void CItemChgState::OnBnClickedRaItid()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_bFindType = FALSE;
	SetDlgItemText(IDC_SAVE_POS,_T("Item ID"));
}

void CItemChgState::OnBnClickedRbItname()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_bFindType = TRUE;
	SetDlgItemText(IDC_SAVE_POS,_T("Item Name"));
}

void CItemChgState::OnBnClickedOk() //ChangeState
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
		
	ItemMultiSelect();
	//OnOK();
}

void CItemChgState::OnBnClickedBtnFinditem()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	ClearListItem();

	CString strItemName;
	DWORD dwID;

	if(!m_bFindType)
	{	
		strItemName = SQL_GARBAGE;
		dwID = GetDlgItemInt(IDC_EB_ITINPUT);
		if(!dwID)
		{
			MessageBox("Wrong Item ID",MB_OK);
			return;
		}
	}
	else
	{
		dwID = 0;
		GetDlgItemText(IDC_EB_ITINPUT,strItemName);		
		if(strItemName == _T("") )
		{
			MessageBox("Wrong Item Name",MB_OK);
			return;
		}				
		strItemName = SQL_LIKE(strItemName);
	}

	m_bWorldID = m_cbWorld.GetCurSel();
	if(m_bWorldID == 0)
	{
		MessageBox("Wrong World",MB_OK);
			return;
	}

	// Document ����Ʈ ���
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	pDoc->SendCT_ITEMFIND_REQ(WORD(dwID),strItemName,m_bWorldID);
}

void CItemChgState::Init()
{
	// ID �� �⺻��������
	CButton* pBtn1 = (CButton*)GetDlgItem(IDC_RA_ITID);
	pBtn1->SetCheck(TRUE);

	CButton* pBtn2 = (CButton*)GetDlgItem(IDC_RB_ITNAME);
	pBtn2->SetCheck(FALSE);

	OnBnClickedRaItid();

	m_bFindType = FALSE;
	SetDlgItemText(IDC_EB_ITINPUT,_T(""));
	ClearListItem();
	
	SetWorld();
}

void CItemChgState::InitFindListCtrl()
{
		// ����Ʈ ��Ʈ���� ��� �÷� ����
	for(int i = 0; i < MAX_ITEMFIND_COLUMN ; i++)
        m_lcFindList.DeleteColumn(0);

	
	LV_COLUMN lvColumn;
	char*list[MAX_ITEMFIND_COLUMN]={"Item ID", "State", "Name"};
	int nWidth[MAX_ITEMFIND_COLUMN]={70,40,160};
	
	for(int i = 0; i< MAX_ITEMFIND_COLUMN; i++)
	{
		lvColumn.mask		= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvColumn.fmt		= LVCFMT_LEFT;
		lvColumn.pszText	= list[i];
		lvColumn.iSubItem	= i;
		lvColumn.cx			= nWidth[i];

		m_lcFindList.InsertColumn(i,&lvColumn);
		//m_listctrPos.InsertColumn(0,"NAME",LVCFMT_CENTER,180);
	}
	
	// �ϳ��� ���� ��ü�� ���õǵ��� ����
	m_lcFindList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
}

void CItemChgState::InsertItemToListCtr(ITEMSTATE* pITEMSTATE, int _iRow)
{
	CString strTmp;
	LV_ITEM lvItem;
	int iRowCount = _iRow;

	if(iRowCount == -1 )
		iRowCount = m_lcFindList.GetItemCount();
	
	lvItem.mask		= LVIF_TEXT;
	lvItem.iItem	= iRowCount; // Row �ε���
	
	for(int iCol = 0; iCol < MAX_ITEMFIND_COLUMN ; iCol++)
	{
		switch(iCol)
		{
		case 0 : strTmp.Format("%d",pITEMSTATE->m_wItemID);		break;
		case 1 : strTmp.Format("%d",pITEMSTATE->m_bInitState ); break;
		case 2 : strTmp.Format("%s",pITEMSTATE->m_strName );	break;		
		}
		
		lvItem.iSubItem = iCol ; // �÷� �ε���
		lvItem.pszText	= strTmp.GetBuffer(0);
		
		if( iCol == 0 )
			m_lcFindList.InsertItem(&lvItem);
		else
            m_lcFindList.SetItem(&lvItem);
	}
}

bool CItemChgState::ItemSelect()
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


	VITEMSTATE vItemState;
	CString strMsg;
	// ���õ� ������ ã��
	int iCount = m_lcFindList.GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		if( m_lcFindList.GetItemState(i, LVIS_SELECTED) != 0 )
		{			
			// ���õ� ������ ����
			CString strTemp;
			ITEMSTATE ItemState;

			//wItemID
			strTemp = m_lcFindList.GetItemText(i,0);
			ItemState.m_wItemID = atoi(strTemp);

			// bInitState
			strTemp = m_lcFindList.GetItemText(i,1);
			ItemState.m_bInitState = !(atoi(strTemp));

			// ItemName
			strTemp = m_lcFindList.GetItemText(i,2);
			ItemState.m_strName = strTemp;

			vItemState.push_back(ItemState);

			strMsg.Format("Change %s state to %d", ItemState.m_strName, ItemState.m_bInitState);
			if(MessageBox(strMsg) == IDOK)
				pDoc->SendCT_ITEMSTATE_REQ(&vItemState,m_bWorldID);

			return true;
		}
	}

	return false;
}

void CItemChgState::ClearListItem()
{
	int iCount = m_lcFindList.GetItemCount();
	for(int i = 0; i < iCount; i++)	
		m_lcFindList.DeleteItem(i);	
    
	m_lcFindList.DeleteAllItems();
}

void CItemChgState::ChangeItemState(WORD wItemID, BYTE bInitState)
{
	CString strInit;
	CString strTemp;
	strInit.Format("%d",bInitState);

	int iCount = m_lcFindList.GetItemCount();
	for(int i = 0; i < iCount; i++)
	{		
		strTemp = m_lcFindList.GetItemText(i,0);		
		if(wItemID == atoi(strTemp) )
		{
			m_lcFindList.SetItemText(i,1,strInit);
			break;
		}
	}
	
	UpdateData(TRUE);
}

void CItemChgState::ItemMultiSelect()
{
	// Document ����Ʈ ���
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return ;
	}


	VITEMSTATE vItemState;

	// ���õ� �������� ����� �޾ƿ´�.
    int count = m_lcFindList.GetSelectedCount();

    // ���� ó�� ���õ� �������� ��ġ���� ��´�.
    POSITION pos = m_lcFindList.GetFirstSelectedItemPosition();
    int index = 0; 

    for (int i = 0; i < count; i++)
    {
        // ���� ���õ� �������� �������� ��´�.
        index = m_lcFindList.GetNextSelectedItem(pos); 

       // �ش� �����ۿ� ���� ó�� 
		if( m_lcFindList.GetItemState(index, LVIS_SELECTED) != 0 )
		{			
			// ���õ� ������ ����
			CString strTemp;
			ITEMSTATE ItemState;

			//wItemID
			strTemp = m_lcFindList.GetItemText(index,0);
			ItemState.m_wItemID = atoi(strTemp);

			// bInitState
			strTemp = m_lcFindList.GetItemText(index,1);
			ItemState.m_bInitState = !(atoi(strTemp));

			// ItemName
			strTemp = m_lcFindList.GetItemText(index,2);
			ItemState.m_strName = strTemp;

			vItemState.push_back(ItemState);			
		}
    }

	if( vItemState.size() == 0)
		return;

	//CString strResult=_T("");
	//CString strMsg = _T("");
	//for(WORD n = 0; n < (WORD)vItemState.size() ; n++)
	//{
	//	strMsg.Format(_T("Change %s state to %d\r\n"), vItemState.at(n).m_strName, vItemState.at(n).m_bInitState);
	//	strResult += strMsg;
	//}
	
	CString strMsg = _T("Change Item State ? ");
	if(MessageBox(strMsg,_T("Item State Change"),MB_YESNO) == IDYES)
		pDoc->SendCT_ITEMSTATE_REQ(&vItemState,m_bWorldID);

}

void CItemChgState::ResultItemState(WORD wCount)
{
	CString str;
	str.Format("%d Items Success",wCount);
	//SetDlgItemText(IDC_LB_STATERE,str);
}

void CItemChgState::SetWorld()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	while(m_cbWorld.GetCount())
		m_cbWorld.DeleteString(0);

	m_cbWorld.AddString(_T("Choose Server"));//��ü
	MAPGROUP::iterator itG;
	for(itG=pDoc->m_mapGroup.begin(); itG!=pDoc->m_mapGroup.end(); itG++)
		if( (*itG).second->m_bID != 0 )
			m_cbWorld.AddString((*itG).second->m_strName);

    m_cbWorld.SetCurSel(0);
	m_bWorldID = 0;
}