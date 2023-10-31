#pragma once

class CTTextLinker
{
public:
	static const CString	LINK_DEF_TOK;

	static const CHAR		LINK_STR_START;
	static const CHAR		LINK_STR_END;
	static const DWORD		LINK_DATA_POOL_SIZE;
	static const DWORD		LINK_QUEST_COLOR;

protected:
	DWORD			m_nTextLinkDataPos;
	CTTextLinkData*	m_pTextLinkDataPool;

public:
	/// ���ο� �ؽ�Ʈ ��ũ ����Ÿ �ν��Ͻ��� ��´�.
	DWORD NewTextLinkData(CTTextLinkData** pOut);
	/// �־��� ���̵��� �ؽ�Ʈ ��ũ ����Ÿ�� ��´�.
	CTTextLinkData* GetTextLinkData(DWORD nPos) const;

public:

	/// �־��� ������ ���ڿ� ���˿� ���߾� ��ũ �ؽ�Ʈ�� ��Ʈ��ũ ���� ������ ���·� �����.
	CString MakeItemNetText(const CString& strFormat, const VDWORD& vItemID);

	CString BuildNetString( const CString& strHeader, const CString& strBody);
	CString AppendNetString( const CString& strMSG1, const CString& strMSG2);
	/// �־��� ���ڿ��� ���๮�� ������ ������.
	CString SplitTextByCrLf(const CString& strMSG, const CString& strTOK, INT& nPos, BOOL bTrimCrLf=TRUE);

protected:
	CTTextLinker();
	~CTTextLinker();

public:
	static CTTextLinker* GetInstance();
};
