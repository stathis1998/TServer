// TChatClientDoc.h : CTChatClientDoc Ŭ������ �������̽�
//


#pragma once

class CTChatClientDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CTChatClientDoc();
	DECLARE_DYNCREATE(CTChatClientDoc)

// Ư��
public:

// �۾�
public:

// ������
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ����
public:
	virtual ~CTChatClientDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �޽��� �� �Լ��� �����߽��ϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};


