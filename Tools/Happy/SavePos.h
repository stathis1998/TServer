#pragma once
#include "afxwin.h"


// CSavePos ��ȭ �����Դϴ�.

class CSavePos : public CDialog
{
	DECLARE_DYNAMIC(CSavePos)

public:
	CSavePos(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSavePos();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_SAVEPOS };

protected:
	DECLARE_MESSAGE_MAP()
public:

	CString* m_pstrSaveName;
	afx_msg void OnBnClickedOk();
};
