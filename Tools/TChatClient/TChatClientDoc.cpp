// TChatClientDoc.cpp : CTChatClientDoc Ŭ������ ����
//

#include "stdafx.h"
#include "TChatClient.h"

#include "TChatClientDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTChatClientDoc

IMPLEMENT_DYNCREATE(CTChatClientDoc, CDocument)

BEGIN_MESSAGE_MAP(CTChatClientDoc, CDocument)
END_MESSAGE_MAP()


// CTChatClientDoc ����/�Ҹ�

CTChatClientDoc::CTChatClientDoc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.

}

CTChatClientDoc::~CTChatClientDoc()
{
}

BOOL CTChatClientDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ �ٽ� �ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}




// CTChatClientDoc serialization

void CTChatClientDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
	}
}


// CTChatClientDoc ����

#ifdef _DEBUG
void CTChatClientDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTChatClientDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTChatClientDoc ���
