#pragma once

// CInstallBar ��ȭ �����Դϴ�.

class CInstallBar : public CDialogBar
{
	DECLARE_DYNAMIC(CInstallBar)

public:
	CInstallBar(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CInstallBar();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_INSTALLBAR };

	void OnUpdateUpdateEnable(CCmdUI *pCmdUI){pCmdUI->Enable(m_bEnableUpdate);}
	void OnUpdateOpenEnable(CCmdUI *pCmdUI)	{pCmdUI->Enable(m_bEnableOpen);}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	BYTE	m_bEnableUpdate;
	BYTE	m_bEnableOpen;
};
