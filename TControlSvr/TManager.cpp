#include "StdAfx.h"


CTManager::CTManager()
{	
	m_bUpload = FALSE;
	m_bAuthority = 0;
	m_strID = _T("");
	m_bLock = TRUE;
}

CTManager::~CTManager()
{
}

// ���·� ������ �˻��Ͽ� ����� ����
BYTE CTManager::CheckAuthority(BYTE m_bClass) 
{	
	if( m_bAuthority <= m_bClass)
		return TRUE; 
	else if( m_bAuthority <= MANAGER_SERVICE )
	{		
		SendCT_AUTHORITY_ACK();
	}

	return FALSE;
}

