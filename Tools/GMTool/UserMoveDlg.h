#pragma once
#include "afxwin.h"

#include "PosListDlg.h"
#include "SavePosDlg.h"

// CUserMoveDlg ��ȭ �����Դϴ�.

class CUserMoveDlg : public CDialog
{
	DECLARE_DYNAMIC(CUserMoveDlg)

public:
	CUserMoveDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CUserMoveDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_USERMOVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CPosListDlg*	m_pdlgPosList;
	CSavePosDlg*	m_pdlgSavePos;

	CString m_strUser;
	DWORD m_dwChannel;
	DWORD m_dwMapID;
	CString m_strTarget1;
	CComboBox m_comboWorld;
	FLOAT m_fPosX;
	FLOAT m_fPosY;
	FLOAT m_fPosZ;
	CString m_strPos;

public:
	void SetWorld();
	void SetEnableCoordinatesList(BOOL _bEnable); // [��ǥ�Է�] ���� �׸���� Enable �� ���ΰ�
	void SetEnableCharList(BOOL _bEnable); // [��� ĳ���ͷ��� ��ȯ] ���� �׸���� Enable �� ���ΰ�.
	void Init(); // [��ǥ�Է�] , [��� ĳ���ͷ��� ��ȯ] �κ��� ��� Disable ���·� �����.	
	void SetData(USERPOS& _UserPos);	
	void SetShowChildDlgAll(BOOL _bShow);

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioCoord();
	afx_msg void OnBnClickedRadioChar();
	afx_msg void OnBnClickedLoadpos();
	afx_msg void OnBnClickedSavePos();
	afx_msg void OnBnClickedOk();  // [Move] ��ư Ŭ������ ��
	afx_msg void OnBnClickedOk2(); // [Summon] ��ư Ŭ������ ��
	afx_msg void OnBnClickedCancel();
};
