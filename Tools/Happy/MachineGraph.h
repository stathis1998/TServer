#pragma once



// CMachineGraph �� ���Դϴ�.

class CMachineGraph : public CFormView
{
	DECLARE_DYNCREATE(CMachineGraph)

protected:
	CMachineGraph();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CMachineGraph();

public:
	enum { IDD = IDD_MACHINEGRAPH };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	BOOL m_bFlag;
	CDC MemDC;
	CBitmap *pOldBitmap, MemBitmap;

	CPen m_lineColor[3];
	CRect m_rectBack;
	CRect m_rectCPU;
	CRect m_rectMEM;
	CRect m_rectNET;

public:
	void UpdateMachineGraph(BYTE bMachineID);
	void DrawBackGround();
	void DrawRect();
	void ClearGraph();
	
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


