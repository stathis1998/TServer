#pragma once
#include "afxwin.h"
#include "afxcmn.h"

// CServiceList �� ���Դϴ�.

class CServiceList : public CFormView
{
	DECLARE_DYNCREATE(CServiceList)

protected:
	CServiceList();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CServiceList();

void AddItemData(LPHAPPYSERVICE pService);
void AddItemDataUpdate(LPHAPPYSERVICE pService, int listCnt); // 06/4/14 ���·� List Data Update

public:
	void UpdateServiceList(DWORD dwID);	

	enum { IDD = IDD_SERVICELIST };
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

public:
	virtual void OnInitialUpdate();
};


