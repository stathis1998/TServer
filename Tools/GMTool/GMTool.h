// GMTool.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error PCH���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����Ͻʽÿ�.
#endif

#include "resource.h"		// �� ��ȣ


// CGMToolApp:
// �� Ŭ������ ������ ���ؼ��� GMTool.cpp�� �����Ͻʽÿ�.
//

class CGMToolApp : public CWinApp
{
public:
	CGMToolApp();

// ������
	public:
	virtual BOOL InitInstance();

// ����

	DECLARE_MESSAGE_MAP()
};

extern CGMToolApp theApp;
