#pragma once


// CShopNameDlg ��ȭ �����Դϴ�.

class CShopNameDlg : public CDialog
{
	DECLARE_DYNAMIC(CShopNameDlg)

public:
	CShopNameDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CShopNameDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_SHOP_NAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strNAME;
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
