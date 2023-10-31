#pragma once

// CPatchBar ��ȭ �����Դϴ�.

class CPatchBar : public CDialogBar
{
	DECLARE_DYNAMIC(CPatchBar)

public:
	CPatchBar(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPatchBar();

	void GetFtp(CString *strAddr, CString *strUser, CString *strPasswd, WORD *wPort);
	void SetFtp(CString *strAddr, CString *strUser, DWORD *wPort);
// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_PATCHBAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnUpdateCompareShow(CCmdUI *pCmdUI);		// 06/02/01 �ڻ� 
	afx_msg void OnUpdatePatchUpdateShow(CCmdUI *pCmdUI);	// 06/02/01 �ڻ� 

public:
	BYTE	m_bEnableCompare;								// Compare ��ư Ȱ��ȭ ����
	BYTE	m_bEnablePatchUpdate;							// PatchUpdate ��ư Ȱ��ȭ ����	

	DWORD m_dwIP;
	DWORD m_dwPort;
	CString m_strUserID;
	CString m_strPassword;
};
