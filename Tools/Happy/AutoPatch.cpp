// AutoPatch.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Happy.h"
#include "HappyDoc.h"
#include "Shlwapi.h"
#include ".\autopatch.h"

// CAutoPatch

IMPLEMENT_DYNCREATE(CAutoPatch, CFormView)

CAutoPatch::CAutoPatch()
	: CFormView(CAutoPatch::IDD)
{
}

CAutoPatch::~CAutoPatch()
{
}

void CAutoPatch::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_PREVFILE, m_treectrlPrevFile);
	DDX_Control(pDX, IDC_TREE_NEXTFILE, m_treectrlNextFile);
	DDX_Control(pDX, IDC_BUTTON_PREVFILE, m_buttonPrevFile);
	DDX_Control(pDX, IDC_BUTTON_NEXTFILE, m_buttonNextFile);
}

BEGIN_MESSAGE_MAP(CAutoPatch, CFormView)
	ON_WM_SIZE()
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_TREE_PREVFILE, OnTvnItemexpandingTreePrevfile)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_TREE_NEXTFILE, OnTvnItemexpandingTreeNextfile)
	ON_NOTIFY(NM_KILLFOCUS, IDC_TREE_PREVFILE, OnNMKillfocusTreePrevfile)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_NEXTFILE, OnTvnSelchangedTreeNextfile)
END_MESSAGE_MAP()


// CAutoPatch �����Դϴ�.

#ifdef _DEBUG
void CAutoPatch::AssertValid() const
{
	CFormView::AssertValid();
}

void CAutoPatch::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CAutoPatch �޽��� ó�����Դϴ�.

void CAutoPatch::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	if (GetSafeHwnd())
	{
		CRect rect, rect2, rect3;
		GetClientRect(&rect);
		if (m_treectrlPrevFile.GetSafeHwnd())
		{
			m_buttonPrevFile.GetWindowRect(&rect3);
			int nThin = rect3.bottom-rect3.top;
			rect2.right = rect.right;
			rect2.left = rect.left;
			rect2.bottom = (rect.bottom - rect.top)/2;
			rect2.top = rect.top+nThin*2;
			rect3.left = rect2.left;
			rect3.bottom = rect2.top;
			rect3.right = rect2.right;
			rect3.top = rect3.bottom-nThin;
			m_treectrlPrevFile.MoveWindow(&rect2);
			m_buttonPrevFile.MoveWindow(&rect3);
		}
		if(m_treectrlNextFile.GetSafeHwnd())
		{
			m_buttonNextFile.GetWindowRect(&rect3);
			int nThin = rect3.bottom-rect3.top;
			rect2.right = rect.right;
			rect2.left = rect.left;
			rect2.bottom = rect.bottom;
			rect2.top = (rect.bottom - rect.top)/2 + nThin*2;
			rect3.left = rect2.left;
			rect3.bottom = rect2.top;
			rect3.right = rect2.right;
			rect3.top = rect3.bottom-nThin;
			m_treectrlNextFile.MoveWindow(&rect2);
			m_buttonNextFile.MoveWindow(&rect3);
		}
		ShowWindow(SW_SHOW);
	}
}

void CAutoPatch::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	static int i=0;
	if(!i)
	{
		SetImageList();
		SetTreeNodes();
		i++;
		m_treectrlPrevFile.SetBkColor( RGB( 200, 200, 200 ) );
		m_treectrlNextFile.SetBkColor( RGB( 200, 200, 200 ) );
	}
}
BOOL CAutoPatch::IsDirectory(CString strPath)
//������ ����̺� ���� �˾ƺ��� �Լ�
{
	CFileStatus fStatus;
	CFile::GetStatus(strPath,fStatus);
	
	if(IsDrive(strPath))
		return TRUE;

	if(fStatus.m_attribute & 0x10)
		return TRUE;
	else
		return FALSE;
	
}
BOOL CAutoPatch::IsDrive(CTreeCtrl * pTreeCtrl, HTREEITEM hItem)
{
	CString strPath=GetItemPath(pTreeCtrl, hItem);
	if(strPath.GetLength()>3)
		return FALSE;

	return TRUE;
}

BOOL CAutoPatch::IsDrive(CString strPath)
{
	CString strDriveLetter;

	strDriveLetter=PickDriveLetter(strPath);

	if(strDriveLetter.GetLength()>3)
		return FALSE;

	return TRUE;
}
HTREEITEM CAutoPatch::AddTreeItem(CTreeCtrl * pTreeCtrl, CString strPath)
{
	CString strDir,strTitle;
	HTREEITEM hItem,hFound;
	
	if(!IsDirectory(strPath))//������ �ƴϸ� �׳� ��������
		return NULL;

	strDir=GetDir(strPath);//�θ������� ��θ� ã�Ƽ�
	ProperPath(strDir);
	hFound=FindItem(pTreeCtrl, strDir);//�ش� �θ� �������� ã��
	if(hFound)//������
	{
		strTitle=GetFileTitle(strPath);
		hItem=pTreeCtrl->InsertItem(strTitle,CLOSED_FOLDER,OPEN_FOLDER,hFound);
		return hItem;//�ڽ� ���������� ����
	}
	return NULL;
}
HTREEITEM CAutoPatch::AddTreeItem(CTreeCtrl * pTreeCtrl, HTREEITEM hParent,LPCTSTR szText,int nIcon,int nSelectedIcon)
{
	//Ʈ�� �������� �ø��� �Լ�

	TV_INSERTSTRUCT tvStruct;

	tvStruct.hParent=hParent;
	tvStruct.hInsertAfter=TVI_LAST;
	tvStruct.item.iImage=nIcon;
	tvStruct.item.iSelectedImage=nSelectedIcon;
	tvStruct.item.cchTextMax=::lstrlen(szText);
	tvStruct.item.pszText=(LPSTR)szText;
	tvStruct.item.mask=TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_PARAM;

	return pTreeCtrl->InsertItem(&tvStruct);
}
void CAutoPatch::AddChildFolderItem(CTreeCtrl * pTreeCtrl, HTREEITEM hParent)
//�Ϻ� ������ ã�Ƽ� Ʈ���信 ��带 �ø��� �κ�
{
	HTREEITEM hItem;
	CFileFind finder;
	CString strPath;

	strPath=GetItemPath(pTreeCtrl, hParent);
	ProperPath(strPath);
	strPath+="*.*";

	if(finder.FindFile(strPath))
	{
		BOOL bFlag=TRUE;
		while(bFlag)
		{
			bFlag=finder.FindNextFile();
			if(finder.IsDirectory() && !finder.IsDots())
			{
				hItem=AddTreeItem(pTreeCtrl, hParent,finder.GetFileTitle(),CLOSED_FOLDER,OPEN_FOLDER);
				SetButtonState(pTreeCtrl, hItem);
			}
		}
	}
	finder.Close();
}
CString CAutoPatch::GetFileTitle(CString strPath)
//������ ��ü����߿��� �̸��� �����ϴ� �Լ�
{
	CString strTitle;
	if(strPath.GetLength()==3)
		return strPath;

	if(strPath.Right(1)=="\\")
		strPath=strPath.Left(strPath.GetLength()-1);

	int nPos=strPath.ReverseFind('\\');
	strTitle=strPath.Mid(nPos+1);
	
	return strTitle;
}
void CAutoPatch::ProperPath(CString &strPath)
{
	if(strPath.Right(1)!="\\")
		strPath+="\\";
}
HICON CAutoPatch::GetIconHandle(int nIndex)
//Shell32.dll���� �ش� �������� �ڵ��� ���� �Լ�
{
	CString strPath;

	strPath=GetSystemDir();//������ �ý��� ���丮�� ���ϰ�
	ProperPath(strPath);
	strPath=strPath+"Shell32.dll";//Shell32.dll������ �������� ����

	HICON hIconSmall,hIconLarge;

	::ExtractIconEx(strPath,nIndex,&hIconLarge,&hIconSmall,1);
	
	return hIconSmall;//���� �������� ��ȯ
}
CString CAutoPatch::GetSystemDir() const
//�������� �ý��� ������ ��θ� ��ȯ
{
	TCHAR szPath[MAX_PATH];
	::GetSystemDirectory(szPath,MAX_PATH);

	return szPath;
}
void CAutoPatch::SetImageList()//�̹��� ����Ʈ �ʱ�ȭ
//Shell32.dll���� �������� �����ؼ� �̹̱� ����Ʈ�� �ִ� �Լ�
{
	m_imageList.Create(16,16,ILC_COLOR8|ILC_MASK,1,1);
	HICON hIcon;

	hIcon=GetIconHandle(3);
	m_imageList.Add(hIcon);//CLOSED_FOLDER
	::DestroyIcon(hIcon);
	hIcon=GetIconHandle(4);
	m_imageList.Add(hIcon);//OPEN_FOLDER
	::DestroyIcon(hIcon);
	hIcon=GetIconHandle(6);
	m_imageList.Add(hIcon);//FLOPPY_DRIVE
	::DestroyIcon(hIcon);
	hIcon=GetIconHandle(8);
	m_imageList.Add(hIcon);//FIXED_DRIVE
	::DestroyIcon(hIcon);
	hIcon=GetIconHandle(9);
	m_imageList.Add(hIcon);//NET_DRIVE
	::DestroyIcon(hIcon);
	hIcon=GetIconHandle(11);
	m_imageList.Add(hIcon);//CDROM_DRIVE
	::DestroyIcon(hIcon);
	hIcon=GetIconHandle(15);
	m_imageList.Add(hIcon);//MY_COM
	::DestroyIcon(hIcon);
	hIcon=GetIconHandle(32);
	m_imageList.Add(hIcon);//RECYCLE_BIN
	::DestroyIcon(hIcon);
	hIcon=GetIconHandle(34);
	m_imageList.Add(hIcon);//DESKTOP
	::DestroyIcon(hIcon);
	hIcon=GetIconHandle(35);
	m_imageList.Add(hIcon);//CONTROL_PANEL
	::DestroyIcon(hIcon);
	m_treectrlPrevFile.SetImageList(&m_imageList,TVSIL_NORMAL);
	m_treectrlNextFile.SetImageList(&m_imageList,TVSIL_NORMAL);
	m_imageList.Detach();
}
void CAutoPatch::SetTreeNodes()//���ʷ� Ʈ���信 ��带 �ø��� �κ�
{
	/*LPSHELLFOLDER lpsf = NULL;
	LPITEMIDLIST lpi_MyComputer = NULL;
	TCHAR pszName[MAX_PATH];//, pszName2[MAX_PATH];
	SHGetDesktopFolder(&lpsf);
	SHGetSpecialFolderLocation(NULL, CSIDL_DRIVES, &lpi_MyComputer);

	LPMALLOC pMalloc;
	SHGetMalloc(&pMalloc);
	STRRET str;
	HRESULT hr;
	hr = lpsf->GetDisplayNameOf(lpi_MyComputer, SHGDN_NORMAL, &str);

	if (SUCCEEDED(hr))
	{
		hr = StrRetToBuf(&str, lpi_MyComputer, pszName, MAX_PATH);
		if (SUCCEEDED(hr))
		{
			if (str.uType == STRRET_WSTR) // �� if ���� �뵵�� �� �𸣰���.
				pMalloc->Free(str.pOleStr);
		}
	}

	if (FAILED(hr))
	{
		_tcscpy(pszName, _T("My Computer"));
	}

	lpsf->Release();
	pMalloc->Free(lpi_MyComputer);

	m_strMyComputer = pszName;

	HTREEITEM hItem;

	hItem=AddTreeItem(&m_treectrlPrevFile, NULL,m_strMyComputer,MY_COM,MY_COM);
	SetDrives(&m_treectrlPrevFile, hItem);//����̺긦 �˻��ؼ� �ø�

	hItem=AddTreeItem(&m_treectrlNextFile, NULL,m_strMyComputer,MY_COM,MY_COM);
	SetDrives(&m_treectrlNextFile, hItem);//����̺긦 �˻��ؼ� �ø�*/
}

void CAutoPatch::SetDrives(CTreeCtrl * pTreeCtrl, HTREEITEM& hParent)//����̺긦 �˻��ϴ� �κ�
{
	int nIndex=0;
	DWORD dwDrives;
	char cDrive;
	CString strDrive;
	CString strVolumeName;
	int nIcon;
	SHFILEINFO shInfo;
	HTREEITEM hItem;

	dwDrives=::GetLogicalDrives();//���� �����ϴ� ����̺��� ���¸� ��Ʈ������ ǥ��

	while(dwDrives)
	{
		if (dwDrives & 1)
		{
			cDrive='A'+nIndex;
			strDrive=CString(cDrive)+":\\";
			nIcon=GetDriveIcon(strDrive);
			SHGetFileInfo(strDrive,0,&shInfo,sizeof(SHFILEINFO),SHGFI_TYPENAME);
			strVolumeName=GetVolumeName(strDrive,shInfo.szTypeName);//�帮�̺��� �����̸��� ����
			hItem=AddTreeItem(pTreeCtrl, hParent,strVolumeName,nIcon,nIcon);
			SetButtonState(pTreeCtrl, hItem);
		}
		nIndex++;
		dwDrives>>=1;
	}
}
void CAutoPatch::SetButtonState( CTreeCtrl * pTreeCtrl, HTREEITEM hItem)//�Ϻ� ���丮�� �ִ��� 
//�˾Ƴ��� ����� ���¸� ǥ����
{
	CFileFind finder;
	CString strPath;

	strPath=GetItemPath(pTreeCtrl, hItem);
	ProperPath(strPath);
	strPath+="*.*";
	if(finder.FindFile(strPath)==TRUE)
	{
		BOOL bFind=TRUE;
		while(bFind)
		{
			bFind=finder.FindNextFile();
			if(finder.IsDirectory() && !finder.IsDots())
			{
				AddTreeItem(pTreeCtrl, hItem,"",CLOSED_FOLDER,OPEN_FOLDER);//������ ���� �������� ����
				break;
			}
		}
	}
	finder.Close();
}
int CAutoPatch::GetDriveIcon(CString& strDrive )//����̺� �������ε����� ����
{
	int nType,nIcon;

	nType=::GetDriveType((LPCTSTR)strDrive);

	switch(nType)
	{
	case DRIVE_REMOVABLE:
		nIcon=FLOPPY_DRIVE;
		break;
	case DRIVE_REMOTE:
	case DRIVE_RAMDISK:
		nIcon=NET_DRIVE;
		break;
	case DRIVE_FIXED:
		nIcon=FIXED_DRIVE;
		break;
	case DRIVE_CDROM:
		nIcon=CDROM_DRIVE;
	}
	return nIcon;
}

CString CAutoPatch::GetItemPath(CTreeCtrl * pTreeCtrl, HTREEITEM hItem)//Ʈ���������� ��θ� ����
{
	HTREEITEM hParent;
	CString strTitle;
	CString strPath;
	CString strParent;
	CString strDrive;
	int nPos;

	strTitle=pTreeCtrl->GetItemText(hItem);
	
	if(strTitle.Find(":")!=-1)
	{
		strPath=PickDriveLetter(strTitle);//����̺� ���
		return strPath;
	}
	
	if(strTitle == m_strMyComputer)
	{
		strPath="";
		return strPath;
	}

	hParent=pTreeCtrl->GetParentItem(hItem);
	strPath=pTreeCtrl->GetItemText(hItem);

	while(hParent && (pTreeCtrl->GetItemText(hParent)!= m_strMyComputer ))
	{
		strParent=pTreeCtrl->GetItemText(hParent);

		if ((nPos=strParent.Find(":"))!=-1)
			strParent=strParent.Mid(nPos-1,2);

		ProperPath(strParent);
		strPath=strParent+strPath;
		hItem=hParent;
		hParent=pTreeCtrl->GetParentItem(hItem);
	}
	return strPath;
}
CString CAutoPatch::GetSpecialFolderPath(int nFolder)
//Ư�������� ��θ� ��ȯ
{
	TCHAR szPath[MAX_PATH];

	if(::SHGetSpecialFolderPath(0,szPath,nFolder,FALSE))
		return szPath;
	return "";
}
CString CAutoPatch::PickDriveLetter(CString strVolumeName)
//���� �̸����� ����̺� �̸��� �����ϴ� �Լ�
{
	CString strDriveLetter;
	int nPos=strVolumeName.Find(":");

	strDriveLetter=strVolumeName.Mid(nPos-1,2);
	strDriveLetter+="\\";

	return strDriveLetter;
}
CString CAutoPatch::GetDir(CString strPath)
//�ش� ��ο��� �θ� ������ ��θ� ��ȯ��
{
	CString strDir=strPath;
	if(strDir.Right(1)=="\\")
		strDir=strDir.Left(strDir.GetLength()-1);
	int nPos=strDir.ReverseFind('\\');
	strDir=strDir.Left(nPos);

	return strDir;
}
HTREEITEM CAutoPatch::FindItem(CTreeCtrl * pTreeCtrl, CString strPath)
//�ش� ��θ� ������ �ִ� �������� ã��
{
	CString strTreePath;

	HTREEITEM hItem=pTreeCtrl->GetFirstVisibleItem();

	while(hItem)
	{
		strTreePath=GetItemPath(pTreeCtrl, hItem);
		ProperPath(strTreePath);
		ProperPath(strPath);
		if(strTreePath==strPath)
			return hItem;
		hItem=pTreeCtrl->GetNextVisibleItem(hItem);
	}
	return NULL;
}
CString CAutoPatch::GetVolumeName(CString  strDrive,CString strTypeName)
//����̺��� ���� �̸��� ��ȯ
{
	TCHAR szVolumeName[MAX_PATH]={0};
	CString strFullName;

	::GetVolumeInformation(strDrive,szVolumeName,MAX_PATH,0,0,0,0,0);
	int nDrive=lstrlen(szVolumeName);

	if(nDrive>0)
		strFullName=szVolumeName;
	else
		strFullName=strTypeName;
	strFullName+=" ("+strDrive+")";

	return strFullName;
}
void CAutoPatch::DeleteAllChildren(CTreeCtrl * pTreeCtrl, HTREEITEM hItem)
//�Ϻγ�带 ��� �����ϴ� �Լ�
{
	HTREEITEM hChildItem,hNextItem;

	if(pTreeCtrl->ItemHasChildren(hItem)==FALSE)
		return;

	hChildItem=pTreeCtrl->GetChildItem(hItem);
	while(hChildItem && (hNextItem=pTreeCtrl->GetNextSiblingItem(hChildItem)))
	{
		pTreeCtrl->DeleteItem(hChildItem);
		hChildItem=hNextItem;
	}
	pTreeCtrl->DeleteItem(hChildItem);
}
void CAutoPatch::OnTvnItemexpandingTreePrevfile(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	HTREEITEM hItem=pNMTreeView->itemNew.hItem;
	HTREEITEM hParent;

	if(pNMTreeView->action=TVE_EXPAND)//��ĥ��
	{
		hParent=m_treectrlPrevFile.GetParentItem(hItem);
		if(hParent)
		{
			DeleteAllChildren(&m_treectrlPrevFile, hItem);//�ڽ� ������ �� �����(���̾����� �����)
			AddChildFolderItem(&m_treectrlPrevFile, hItem);//���� �ڽ� �������� �ø�
		}
	}
	else//���� ��
	{
		DeleteAllChildren(&m_treectrlPrevFile, hItem);
		SetButtonState(&m_treectrlPrevFile, hItem);
	}
	m_treectrlPrevFile.SelectItem(hItem);

	*pResult = 0;
}

void CAutoPatch::OnTvnItemexpandingTreeNextfile(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	HTREEITEM hItem=pNMTreeView->itemNew.hItem;
	HTREEITEM hParent;

	if(pNMTreeView->action=TVE_EXPAND)//��ĥ��
	{
		hParent=m_treectrlNextFile.GetParentItem(hItem);
		if(hParent)
		{
			DeleteAllChildren(&m_treectrlNextFile, hItem);//�ڽ� ������ �� �����(���̾����� �����)
			AddChildFolderItem(&m_treectrlNextFile, hItem);//���� �ڽ� �������� �ø�
		}
	}
	else//���� ��
	{
		DeleteAllChildren(&m_treectrlNextFile, hItem);
		SetButtonState(&m_treectrlNextFile, hItem);
	}
	m_treectrlNextFile.SelectItem(hItem);

	*pResult = 0;
}

void CAutoPatch::OnNMKillfocusTreePrevfile(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	HTREEITEM hItem=pNMTreeView->itemOld.hItem;

	*pResult = 0;
}

BYTE CAutoPatch::OnCompare()
{
	CHappyDoc * pDoc = (CHappyDoc *)GetDocument();

	m_strPrevFolder = GetItemPath(&m_treectrlPrevFile, m_treectrlPrevFile.GetSelectedItem());
	m_strNextFolder = GetItemPath(&m_treectrlNextFile, m_treectrlNextFile.GetSelectedItem());

	pDoc->m_strNextFolder = m_strNextFolder;
	pDoc->m_dwPatchSize = 0;

	if( !m_strPrevFolder.IsEmpty() &&
		!m_strNextFolder.IsEmpty())
	{
		CHappyApp *pApp = (CHappyApp *)AfxGetApp();
		pApp->BeginWaitCursor();

		SearchDifference(m_strPrevFolder, m_strNextFolder);
		SearchNew(m_strPrevFolder, m_strNextFolder);

		pApp->EndWaitCursor();
		return TRUE;
	}
	return FALSE;
}

void CAutoPatch::SearchNew(LPCTSTR strPrev, LPCTSTR strNext)
{
	CHappyDoc * pDoc = (CHappyDoc *)GetDocument();
	if(pDoc && pDoc->m_bSameFileExist)
		return;

	WIN32_FIND_DATA FileData;
	if(SetCurrentDirectory(strNext))
	{//Search new files
		HANDLE hSearch = FindFirstFile(_T("*"), &FileData);
		if(hSearch==INVALID_HANDLE_VALUE) return;
		do
		{
			if(FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if( 0 != strcmp(FileData.cFileName, _T(".")) &&
					0 != strcmp(FileData.cFileName, _T("..")))
				{
					CString strFolder1 = strPrev;
					CString strFolder2 = strNext;

					strFolder1 += _T("\\"); strFolder2 += _T("\\");
					strFolder1 += FileData.cFileName; strFolder2 += FileData.cFileName;
					
					SearchNew(strFolder1, strFolder2);		
				}
			}
			else
			{
				CString strFile1 = strPrev;
				CString strFile2 = strNext;

				strFile1 += _T("\\"); strFile2 += _T("\\");
				strFile1 += FileData.cFileName; strFile2 += FileData.cFileName;
			
				AddPatchFile(CheckExist(strFile1, strFile2));
			}
		}while(FindNextFile(hSearch, &FileData) && !pDoc->m_bSameFileExist);

		FindClose(hSearch);
	}
}

LPPATCHFILE CAutoPatch::CheckExist(LPCTSTR strFile1, LPCTSTR strFile2)
{
	CFile file1, file2;
	CFileException ex;

	if(!file2.Open(strFile2, CFile::modeRead))
		return NULL;

	LPPATCHFILE pPatchFile = NULL;
	if(file1.Open(strFile1, CFile::modeRead, &ex))
		file1.Close();
	else
	{
		if( ex.m_lOsError == ERROR_FILE_NOT_FOUND ||
			ex.m_lOsError == ERROR_PATH_NOT_FOUND )
		{
			FILETIME ftLastWrite;
			DWORD nSize = (DWORD)file2.GetLength();
			//if(!nSize) // ������ 0
			//	return NULL;

			GetFileTime((void *)file2.m_hFile, NULL, NULL, &ftLastWrite);

			CString strRelPath;
			strRelPath = strFile2;
			strRelPath = strRelPath.Right(strRelPath.GetLength()-m_strNextFolder.GetLength());
			
			pPatchFile = new PATCHFILE();
			pPatchFile->m_strFullFile = strFile2;
			pPatchFile->m_strRelFile = strRelPath;
			pPatchFile->m_dwSize = nSize;
			pPatchFile->m_time.dwHighDateTime = ftLastWrite.dwHighDateTime;
			pPatchFile->m_time.dwLowDateTime = ftLastWrite.dwLowDateTime;
		}
	}

	file2.Close();

	return pPatchFile;
}
void CAutoPatch::SearchDifference(LPCTSTR strPrev, LPCTSTR strNext)
{
	CHappyDoc * pDoc = (CHappyDoc *)GetDocument();
	if(pDoc && pDoc->m_bSameFileExist)
		return;

	WIN32_FIND_DATA FileData;
	if(SetCurrentDirectory(strPrev))
	{//Search modified files
		HANDLE hSearch = FindFirstFile(_T("*"), &FileData);
		if(hSearch==INVALID_HANDLE_VALUE) return;

		do
		{
			if(FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if( 0 != strcmp(FileData.cFileName, _T(".")) &&
					0 != strcmp(FileData.cFileName, _T("..")))
				{
					CString strFolder1 = strPrev;
					CString strFolder2 = strNext;

					strFolder1 += _T("\\"); strFolder2 += _T("\\");
					strFolder1 += FileData.cFileName; strFolder2 += FileData.cFileName;
					
					SearchDifference(strFolder1, strFolder2);					
				}
			}
			else
			{
				CString strFile1 = strPrev;
				CString strFile2 = strNext;

				strFile1 += _T("\\"); strFile2 += _T("\\");
				strFile1 += FileData.cFileName; strFile2 += FileData.cFileName;

				AddPatchFile(CompareFiles(strFile1, strFile2));
			}
		}while(FindNextFile(hSearch, &FileData) && !pDoc->m_bSameFileExist);


		FindClose(hSearch);		
	}
}
LPPATCHFILE CAutoPatch::CompareFiles(LPCTSTR strFile1, LPCTSTR strFile2)
{
	CFile file1, file2;

	LPPATCHFILE pPatchFile = NULL;

	if( !file2.Open(strFile2, CFile::modeRead)||
		!file1.Open(strFile1, CFile::modeRead) )
		return NULL;

	FILETIME ftLastWrite;
	DWORD nSize = (DWORD)file2.GetLength();
	//if(!nSize) // ������ 0
	//	return NULL;

	GetFileTime((void *)file2.m_hFile, NULL, NULL, &ftLastWrite);

	CString strRelPath;
	strRelPath = strFile2;
	strRelPath = strRelPath.Right(strRelPath.GetLength()-m_strNextFolder.GetLength());
	
	pPatchFile = new PATCHFILE();
	pPatchFile->m_strFullFile = strFile2;
	pPatchFile->m_strRelFile = strRelPath;
	pPatchFile->m_dwSize = nSize;
	pPatchFile->m_time.dwHighDateTime = ftLastWrite.dwHighDateTime;
	pPatchFile->m_time.dwLowDateTime = ftLastWrite.dwLowDateTime;

	DWORD dwFileSize = (DWORD)file1.GetLength();
	if(dwFileSize != file2.GetLength())
	{//������ ����� �ٸ��ٸ�
		file1.Close(); file2.Close();
		return pPatchFile;
	}

	LPBYTE Data1 = new BYTE[dwFileSize];
	LPBYTE Data2 = new BYTE[dwFileSize];

	file1.Read( Data1, dwFileSize);
	file2.Read( Data2, dwFileSize);

	file1.Close(); 
	file2.Close();

	for( DWORD i = 0; i < dwFileSize; ++i )
		if( Data1[i] != Data2[i] ) 
		{
			delete [] Data1;
			delete [] Data2;

			return pPatchFile;
		}

	delete [] Data1;
	delete [] Data2;

	delete pPatchFile;
	return NULL;
}
void CAutoPatch::AddPatchFile(LPPATCHFILE pFile)
{
	if(pFile)
	{
		SeparatePath(pFile);
		CHappyDoc * pDoc = (CHappyDoc *)GetDocument();
		pDoc->m_dwPatchSize += pFile->m_dwSize;
		pDoc->AddPatchFile(pFile);
	}
}
void CAutoPatch::SeparatePath(LPPATCHFILE pFile)
{
	int nSlash, nLastSlash = -1;

	while((nSlash = pFile->m_strRelFile.Find(_T("\\"), nLastSlash+1))>=0) 
		nLastSlash = nSlash;

	pFile->m_strPath = pFile->m_strRelFile.Left( nLastSlash );
	pFile->m_strFileName = pFile->m_strRelFile.Right( pFile->m_strRelFile.GetLength()-nLastSlash-1);

	nLastSlash = -1;
	while((nSlash = pFile->m_strRelFile.Find(_T("."), nLastSlash+1))>=0) 
		nLastSlash = nSlash;

	int nLen = pFile->m_strRelFile.GetLength();
	pFile->m_strOriFileName = pFile->m_strRelFile.Mid(0,nLastSlash);
}

void CAutoPatch::OnTvnSelchangedTreeNextfile(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
}
