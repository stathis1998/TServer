#pragma once


// CGuildChange ��ȭ �����Դϴ�.

class CGuildChange : public CDialog
{
	DECLARE_DYNAMIC(CGuildChange)

public:
	CGuildChange(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CGuildChange();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_GUILDCHANGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
