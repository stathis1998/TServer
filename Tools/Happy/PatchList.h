#pragma once
#include "afxcmn.h"



// CPatchList �� ���Դϴ�.

class CPatchList : public CFormView
{
	DECLARE_DYNCREATE(CPatchList)

protected:
	CPatchList();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CPatchList();

public:
	void UpdatePatchList();
	enum { IDD = IDD_PATCHLIST };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CListCtrl m_listCtrl;
	virtual void OnInitialUpdate();
};


