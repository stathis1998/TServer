// InstallBar.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Happy.h"
#include "InstallBar.h"
#include ".\installbar.h"


// CInstallBar ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CInstallBar, CDialogBar)
CInstallBar::CInstallBar(CWnd* pParent /*=NULL*/)
	: CDialogBar()
{
}

CInstallBar::~CInstallBar()
{
}

void CInstallBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CInstallBar, CDialogBar)
	ON_UPDATE_COMMAND_UI( IDC_BTN_IUPDATE, OnUpdateUpdateEnable )
	ON_UPDATE_COMMAND_UI( IDC_BTN_IOPEN, OnUpdateOpenEnable )
END_MESSAGE_MAP()