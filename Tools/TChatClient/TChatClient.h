// TChatClient.h : TChatClient ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error PCH���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����Ͻʽÿ�.
#endif

#include "resource.h"       // �� ��ȣ


// CTChatClientApp:
// �� Ŭ������ ������ ���ؼ��� TChatClient.cpp�� �����Ͻʽÿ�.
//

class CTChatClientApp : public CWinApp
{
public:
	CTChatClientApp();


// ������
public:
	virtual BOOL InitInstance();

// ����
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CTChatClientApp theApp;
