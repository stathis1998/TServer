// HappyView.h : iCHappyView Ŭ������ �������̽�
//


#pragma once

#include "HappyDoc.h"
#include "HSession.h"
#include "DlgConnect.h"
#include "DlgUpload.h"
#include "GMToolDlg.h"
#include "ChatBanList.h"

class CHappyView : public CView
{
protected: // serialization������ ��������ϴ�.
	CHappyView();
	DECLARE_DYNCREATE(CHappyView)

public:
	CDlgConnect m_dlgConnect;
	CDlgUpload m_dlgUpload;
	CGMToolDlg *m_dlgGMTool;
	CChatBanList* m_dlgChatBanList;
	
// �۾�

// Ư��
public:
	CHappyDoc* GetDocument() const;

// ������
	public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ����
public:
	virtual ~CHappyView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �޽��� �� �Լ��� �����߽��ϴ�.
protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnServerConnect();
	afx_msg void OnServiceStart();
	afx_msg void OnServiceStop();
	afx_msg void OnServerDisconnect();
	afx_msg void OnServiceAllView();
	afx_msg void OnServiceUpload();
	afx_msg void OnCuruserGraph();
	afx_msg void OnGmtool();
	afx_msg void OnServcieLog();
	afx_msg void OnUserweb();
	afx_msg void OnServiceAutoStart();
	afx_msg void OnServiceDataClear();
	afx_msg void OnExportHappydata();
	afx_msg void OnUseradminChatbanlist();
};

#ifndef _DEBUG  // HappyView.cpp�� ����� ����
inline CHappyDoc* CHappyView::GetDocument() const
   { return reinterpret_cast<CHappyDoc*>(m_pDocument); }
#endif

