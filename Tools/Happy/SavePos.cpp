// SavePos.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Happy.h"
#include "SavePos.h"
#include ".\savepos.h"

#include "MainFrm.h"
#include "HappyDoc.h"

// CSavePos ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CSavePos, CDialog)
CSavePos::CSavePos(CWnd* pParent /*=NULL*/)
	: CDialog(CSavePos::IDD, pParent)
{
}

CSavePos::~CSavePos()
{
}


BEGIN_MESSAGE_MAP(CSavePos, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

// CSavePos �޽��� ó�����Դϴ�.
void CSavePos::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString strTemp;

	GetDlgItemText(IDC_EDIT_SAVENAME, strTemp);

	if( strTemp.IsEmpty())
		AfxMessageBox(_T("You should enter the name"), MB_OK); // �̸��� �Է��ؾ� �մϴ�
	else if(m_pstrSaveName)
	{
		m_pstrSaveName->Format("%s",strTemp);
		OnOK();
		return;
	}
	OnCancel();
}