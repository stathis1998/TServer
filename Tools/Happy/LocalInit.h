#pragma once


// CLocalInit ��ȭ �����Դϴ�.

class CLocalInit : public CDialog
{
	DECLARE_DYNAMIC(CLocalInit)

public:
	CLocalInit(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CLocalInit();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_LOCALINIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
