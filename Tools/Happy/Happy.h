// Happy.h : Happy ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error PCH���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����Ͻʽÿ�.
#endif

#include "resource.h"       // �� ��ȣ


// CHappyApp:
// �� Ŭ������ ������ ���ؼ��� Happy.cpp�� �����Ͻʽÿ�.
//

class CHappyApp : public CWinApp
{
public:
	CHappyApp();
	void Beep();

// ������
public:
	virtual BOOL InitInstance();

// ����
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CHappyApp theApp;
