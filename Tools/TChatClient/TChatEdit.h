#pragma once

class CTChatClientView;

// CTChatEdit

class CTChatEdit : public CEdit
{
	DECLARE_DYNAMIC(CTChatEdit)

public:
	static const DWORD CHAT_MODE_CMD[];					///< ä�� ��� ���

public:
	CTChatClientView *m_pHost;

public:
	BYTE CheckTCMD();

public:
	CTChatEdit();
	virtual ~CTChatEdit();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};
