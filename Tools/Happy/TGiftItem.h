#pragma once
#include "afxwin.h"
#include "TGiftSet.h"


// CTGiftItem ��ȭ �����Դϴ�.

class CTGiftItem : public CDialog
{
	DECLARE_DYNAMIC(CTGiftItem)

public:
	CTGiftItem(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTGiftItem();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_GIFTTOOL };

	void	Init();
	void	ShowSetting(BYTE bShow);
	void	ReturnMsg(BYTE bRet);
	BYTE	GetWorld(BYTE& p_bOut);
	LPMAPCMGIFT	GetGiftItemMap();
	void	CreateGiftSet();

public:
	CComboBox m_cbWorld;
	CTGiftSet* m_pDlgGiftSet;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedGiftSet();
	afx_msg void OnBnClickedOk();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
