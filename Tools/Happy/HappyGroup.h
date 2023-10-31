#pragma once
#include "afxcmn.h"
#include "TabCtrlHappy.h"

// CHappyGroup �� ���Դϴ�.

class CHappyGroup : public CFormView
{
	DECLARE_DYNCREATE(CHappyGroup)

protected:
	CHappyGroup();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CHappyGroup();
public:
	void OnAddTab(CString strName, CWnd* pWnd);

public:
	enum { IDD = IDD_HAPPYGROUP };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CTabCtrlHappy m_tabCtrl;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTcnSelchangeTabGroup(NMHDR *pNMHDR, LRESULT *pResult);
};