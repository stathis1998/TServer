// LocalInit.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Happy.h"
#include "LocalInit.h"


// CLocalInit ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CLocalInit, CDialog)
CLocalInit::CLocalInit(CWnd* pParent /*=NULL*/)
	: CDialog(CLocalInit::IDD, pParent)
{
}

CLocalInit::~CLocalInit()
{
}

void CLocalInit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLocalInit, CDialog)
END_MESSAGE_MAP()


// CLocalInit �޽��� ó�����Դϴ�.
