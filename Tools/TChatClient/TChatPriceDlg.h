#pragma once

class CTChatItem;

// CTChatPriceDlg ��ȭ �����Դϴ�.

class CTChatPriceDlg : public CDialog
{
	DECLARE_DYNAMIC(CTChatPriceDlg)

public:
	CTChatPriceDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTChatPriceDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_PRICE };

public:
	CTChatItem *m_pTITEM;

	DWORD m_dwRune;
	DWORD m_dwLuna;
	DWORD m_dwCron;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
