// stdafx.h : �� ������� �ʰ� ���� ����ϴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Windows ������� ���� ������ �ʴ� ������ ���ܽ�ŵ�ϴ�.
#endif

// �Ʒ� ������ �÷������� �켱�ϴ� �÷����� ������� �ϴ� ��� ���� ���Ǹ� �����Ͻʽÿ�.
// �ٸ� �÷����� ���Ǵ� �ش� ���� �ֽ� ������ MSDN�� �����Ͻʽÿ�.
#ifndef WINVER				// Windows 95 �� Windows NT 4 ���� ���������� ����� ����� �� �ֽ��ϴ�.
#define WINVER 0x0601		// Windows 98�� Windows 2000 ���� ������ �µ��� ������ ������ ������ �ֽʽÿ�.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// �Ϻ� CString �����ڴ� ��������� ����˴ϴ�.

// MFC�� ���� �κа� ���� ������ ��� �޽����� ���� ����⸦ �����մϴ�.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC �ٽ� �� ǥ�� ���� ���
#include <afxext.h>         // MFC �ͽ��ټ�

#include <afxdtctl.h>		// Internet Explorer 4 ���� ��Ʈ�ѿ� ���� MFC ����
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Windows ���� ��Ʈ�ѿ� ���� MFC ����
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>		// MFC ���� �ͽ��ټ�

//////////////////////////////////////////////////////////////////////

#include <queue>
#include <map>
#include <vector>
#include <list>
using namespace std;

#include "Packet.h"
#include "CTProtocol.h"


//

#define DEF_PORT						3615
#define DEF_IP							_T("83.141.22.175")
#define CONFIG_FILE						_T("GMTool.txt")
#define POSLIST_FILE					_T("UserPosList.txt")
#define MAX_POSLIST_COLUMN				6
#define MSG_CONNECT_FAIL				_T("Failed to connect\nCheck the connection information out")
#define MSG_CONNECT_CLOSE				_T("the connection is Closed")
#define MSG_SVR_NOTSELECT				_T("Server is not selected")
#define MSG_CHARNAME_NULL				_T("You should enter the name")
#define MSG_AUTHORITY_NO				_T("You don't have the authority")
#define MSG_NOT_FOUND					_T("Not Fount")
#define MSG_NOT_FOUNTSERVER				_T("Not Fount Server")
#define MSG_DELETE_Q					_T("Are you sure you want to delete?")
#define MSG_ENTER_AGAIN					_T("Enter again")

#define ACTION_AT_DEAD					9
#define MAX_MONSPAWN_STAT_COLUMN		6
//
typedef struct tagUSERPOS				USERPOS, *LPUSERPOS;
typedef struct tagHAPPYGROUP			HAPPYGROUP, *LPHAPPYGROUP;

typedef list< LPUSERPOS >				LISTUSERPOS, *LPLISTUSERPOS;
typedef map<BYTE, LPHAPPYGROUP>			MAPGROUP, *LPMAPGROUP;



//

typedef enum MANAGER_CLASS
{	
	MANAGER_ALL = 1, // ��� ���
	MANAGER_CONTROL , // ��ġ/���ε�
	MANAGER_USER , // ��������/��������/��ġ�̵�
	MANAGER_SERVICE , // ���� ON/OFF	
	MANAGER_GMLEVEL1 , // GMTool �� ��� ���(ĳ���� �޼���,�̵�,ä�ñ���,����)
	MANAGER_GMLEVEL2 ,  // GMTool �� ĳ���� �޼���, �̵� ��ɸ�.
	MANAGER_COUNT

} *LPMANAGER_CLASS;


struct tagUSERPOS
{
	char POSNAME[50];	
	DWORD ChannelNum;
	DWORD MapID;
	float PosX;
	float PosY;
	float PosZ;

	tagUSERPOS::tagUSERPOS()
	{
		strcpy(POSNAME,"");
	}
};

struct tagHAPPYGROUP
{
	BYTE m_bID;
	CString m_strName;
};

class CPosEqual
{
public:
	CPosEqual(char* _str)
	{
		strcpy(m_str, _str);
	};

	bool operator()(LPUSERPOS _USERPOS) const {
		if( !strcmp(m_str,_USERPOS->POSNAME) )
			return TRUE;
		else
			return FALSE;
	};
public:
	char m_str[100];
};