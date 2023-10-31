#pragma once
#include "afxcmn.h"



// CMachine �� ���Դϴ�.
class CHappyDoc;
class CMachine : public CFormView
{
	DECLARE_DYNCREATE(CMachine)

protected:
	CMachine();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CMachine();

public:
	enum { IDD = IDD_MACHINE };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CTreeCtrl m_treeCtrl;
	CRect treeRect;
	CString m_strSelected;

public:
	void InsertInitData();
	void ClearMachineTree();
	BYTE GetSelectedMachineID();

public:
	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
	afx_msg void OnTvnSelchangedTreeMachine(NMHDR *pNMHDR, LRESULT *pResult);
};


