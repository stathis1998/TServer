// EventDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Happy.h"
#include "HappyDoc.h"
#include "MainFrm.h"
#include "EventDlg.h"

// CEventDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CEventDlg, CDialog)
CEventDlg::CEventDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEventDlg::IDD, pParent),
	m_pdlg(NULL)
{
}

CEventDlg::~CEventDlg()
{
}

void CEventDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_lcEvent);
}


BEGIN_MESSAGE_MAP(CEventDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_ADDEVENT, OnBnClickedBtnAddEvent)
	ON_BN_CLICKED(IDC_BTN_DELEVENT, OnBnClickedBtnDeleteEvent)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnNMDblclkList1)
	ON_BN_CLICKED(IDC_BTN_UPDATE, OnBnClickedBtnUpdate)
END_MESSAGE_MAP()


// CEventDlg �޽��� ó�����Դϴ�.

void CEventDlg::OnBnClickedBtnAddEvent()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//CEventInfo dlg;
	m_pdlg = new CEventInfo(this);
	m_pdlg->SetEventID(0,EK_ADD);
	m_pdlg->DoModal();
	delete m_pdlg;
	m_pdlg = NULL;
}

void CEventDlg::OnBnClickedBtnDeleteEvent()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	DWORD dwIndex = ItemSelect();
	if( !dwIndex )
		return;

	MAPEVENTINFO::iterator it = pDoc->m_mapEventInfo.find(dwIndex);//stEVENT.m_dwIndex);
	if( it == pDoc->m_mapEventInfo.end() )	
		return;


	if( (*it).second.m_bState )
	{
		
		if( AfxMessageBox(_T("This event is already in progress! Do you still wish to delete?"),MB_YESNO) != IDYES)
			return;
	}
	else
	{
		if( AfxMessageBox(_T("Are you Sure you want to delete?"),MB_YESNO) != IDYES)
			return;
	}
	//(*it).second.m_dStartDate = 0;
	//(*it).second.m_dEndDate = 0;

	pDoc->SendCT_EVENTCHANGE_REQ(EK_DEL,&(*it).second);	
}

BOOL CEventDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	Init();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CEventDlg::Init()
{
	// ����Ʈ ��Ʈ���� ��� �÷� ����
	for(int i = 0; i < MAX_EVENTLIST_COLUMN ; i++)
        m_lcEvent.DeleteColumn(0);

	
	LV_COLUMN lvColumn;
	char*list[MAX_EVENTLIST_COLUMN]={"Index", "Title","Kind","Value (%)","State"};
	int nWidth[MAX_EVENTLIST_COLUMN]={40,200,100,60,50};
	
	for(int i = 0; i< MAX_EVENTLIST_COLUMN; i++)
	{
		lvColumn.mask		= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvColumn.fmt		= LVCFMT_CENTER;
		lvColumn.pszText	= list[i];
		lvColumn.iSubItem	= i;
		lvColumn.cx			= nWidth[i];

		m_lcEvent.InsertColumn(i,&lvColumn);
		//m_listctrPos.InsertColumn(0,"NAME",LVCFMT_CENTER,180);
	}
	
	// �ϳ��� ���� ��ü�� ���õǵ��� ����
	m_lcEvent.SetExtendedStyle(LVS_EX_FULLROWSELECT);
}

void CEventDlg::AddListData(LPEVENTINFO pData)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	CString strTmp;
	LV_ITEM lvItem;
	INT nCount = m_lcEvent.GetItemCount();

	lvItem.mask		= LVIF_TEXT;
	lvItem.iItem	= nCount; // Row �ε���
	
	for(int iCol = 0; iCol < MAX_EVENTLIST_COLUMN ; iCol++)
	{
		switch(iCol)
		{
		case 0 : strTmp.Format("%d",pData->m_dwIndex);	break;
		case 1 : strTmp.Format("%s",pData->m_strTitle ); break;
		case 2 : 
			{
				CString strEventName = _T("");
				MAPBYTESTRING::iterator it =  pDoc->m_mapEventName.find(pData->m_bID);
				if( it != pDoc->m_mapEventName.end())
					strEventName = (*it).second;

				strTmp.Format("%s",strEventName);
			}
			break;
		case 3 : strTmp.Format("%d",pData->m_wValue);	break;
		case 4 : 
			{			
				if(pData->m_bState)
					strTmp.Format("%s",_T("Run"));
				else
					strTmp.Format("%s",_T("-"));
			}
			break;
		}
		
		lvItem.iSubItem = iCol ; // �÷� �ε���
		lvItem.pszText	= strTmp.GetBuffer(0);
		
		if( iCol == 0 )
			m_lcEvent.InsertItem(&lvItem);
		else
            m_lcEvent.SetItem(&lvItem);
	}
}

void CEventDlg::ClearListItem()
{
	int iCount = m_lcEvent.GetItemCount();
	for(int i = 0; i < iCount; i++)	
		m_lcEvent.DeleteItem(0);	
    
	m_lcEvent.DeleteAllItems();
}

DWORD CEventDlg::ItemSelect()
{
	// ���õ� ������ ã��	
	DWORD dwID = 0;
	int iCount = m_lcEvent.GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		if( m_lcEvent.GetItemState(i, LVIS_SELECTED) != 0 )
		{			
			CString str = _T("");
			str = m_lcEvent.GetItemText(i,0);
			dwID = (DWORD)atoi(str);

			break;
		}
	}

	return dwID;
}
BOOL CEventDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if( pMsg->message == WM_KEYDOWN) 
	{
		switch(pMsg->wParam)
		{
		case VK_RETURN:
		case VK_ESCAPE:
			return FALSE;

		}
	}
	else if(pMsg->message == WM_KEYUP)
	{
		switch(pMsg->wParam)
		{
		case VK_UP:
		case VK_DOWN:
			ItemSelect();
			break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}


void CEventDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

	DWORD dwEventIndex = ItemSelect();
	if(!dwEventIndex)
		return;

	//CEventInfo dlg;
	m_pdlg = new CEventInfo(this);
	m_pdlg->SetEventID(dwEventIndex,EK_UPDATE);
	m_pdlg->DoModal();
	delete m_pdlg;
	m_pdlg = NULL;
}

void CEventDlg::OnBnClickedBtnUpdate()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	pDoc->SendCT_EVENTLIST_REQ();
	pDoc->SendCT_CASHITEMLIST_REQ();
}