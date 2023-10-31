#include "stdafx.h"

//#include "TMiniDump.h"
//#include <string>

//using namespace std;

enum BSUMDRET
{
	eDUMP_SUCCEEDED ,
	eDBGHELP_NOT_FOUND ,
	eDBGHELP_MISSING_EXPORTS ,
	eBAD_PARAM ,
	eOPEN_DUMP_FAILED ,
	eMINIDUMPWRITEDUMP_FAILED ,
	eGETTHREADCONTEXT_FAILED ,
	eREADPROCMEM_FAILED ,
	eINVALID_ERROR 
};

typedef BOOL (WINAPI *MINIDUMPWRITEDUMP)(
	HANDLE hProcess, DWORD dwPid, HANDLE hFile, MINIDUMP_TYPE DumpType,
	CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
	CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
	CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam );

BOOL CalculateBeginningOfCallInstruction ( UINT_PTR & dwRetAddr );
BSUMDRET OnMemoryDump();

extern "C" void * _ReturnAddress ( void ) ;

MINIDUMP_TYPE	g_eDumpType = MiniDumpNormal;
std::string		g_strUserData = "tttttt";
HWND			g_hWnd = NULL;
BOOL			g_bSaveMessage = FALSE;
CTMiniDump		theTMiniDump;
CString			g_strErrorMsg;

CTMiniDump::CTMiniDump()
{
	// ���ܰ� �߻��ϸ� ���ͷ���
	::SetUnhandledExceptionFilter(TopLevelExceptionFilter);
}

CTMiniDump::~CTMiniDump()
{
}

LONG WINAPI CTMiniDump::TopLevelExceptionFilter(_EXCEPTION_POINTERS *pException)
{
	LONG lResult = EXCEPTION_CONTINUE_SEARCH;

	char szDllHelpPath[MAX_PATH] = {0};
	HMODULE hDLL = NULL;

	// DLL �ε�
	if(::GetModuleFileName(NULL, szDllHelpPath, _MAX_PATH))
	{
		char *pSlash = ::strrchr(szDllHelpPath, '\\');
		if( pSlash )
		{
			::lstrcpy(pSlash + 1, "DBGHELP.DLL");
			hDLL = ::LoadLibrary(szDllHelpPath);
		}
	}

	if( NULL == hDLL )
		return lResult;
	
	// DLL���� MiniDumpWriteDump�Լ� ������
	MINIDUMPWRITEDUMP pMiniDumpWriteDump = (MINIDUMPWRITEDUMP)::GetProcAddress(hDLL, "MiniDumpWriteDump");
	if( pMiniDumpWriteDump == NULL )
		return lResult;

	SYSTEMTIME time;
	::GetLocalTime(&time);
	
	CString strTime;
	strTime.Format("%d-%d-%d-%d-%d-%d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	
	CString strFaultReason = GetFaultReason(pException);

	// ������ ���� �̸� ����
	char szFilename[MAX_PATH];
	::GetModuleFileName(NULL, szFilename, MAX_PATH);
	CString strFileName = szFilename;

 	int iPos = strFileName.ReverseFind('.');
	if( iPos > 0 )
		strFileName = strFileName.Left(iPos);

	strFileName += "(" + strTime + ")" + ".dmp";

	// ���Ͽ� ���
	HANDLE hFile = CreateFile(strFileName, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if( INVALID_HANDLE_VALUE == hFile )
		return lResult;

	HANDLE hProcess = GetCurrentProcess();
	DWORD dwProcessID = GetCurrentProcessId();
	DWORD dwThreadID = GetCurrentThreadId();

	MINIDUMP_EXCEPTION_INFORMATION sExceptionInfo;
	sExceptionInfo.ThreadId = dwThreadID;
	sExceptionInfo.ExceptionPointers = pException;
	sExceptionInfo.ClientPointers = FALSE;

	MINIDUMP_USER_STREAM_INFORMATION sUserInfo; 
	MINIDUMP_USER_STREAM m_vUseData[2];

	// �߰�����
	m_vUseData[0].Type = 0;
	m_vUseData[0].Buffer = (PVOID)((LPCSTR)strFaultReason);
	m_vUseData[0].BufferSize = strFaultReason.GetLength()+1;

	m_vUseData[1].Type = 1;
	m_vUseData[1].Buffer = (PVOID)g_strUserData.c_str();
	m_vUseData[1].BufferSize = (ULONG)g_strUserData.size();

	sUserInfo.UserStreamCount = 2; 
	sUserInfo.UserStreamArray = m_vUseData;

	// �������
	if( pMiniDumpWriteDump( hProcess, dwProcessID, hFile, g_eDumpType, &sExceptionInfo, &sUserInfo, NULL) )
		lResult = EXCEPTION_EXECUTE_HANDLER;
	
	CloseHandle(hFile);

	return lResult;
}



void CTMiniDump::SetOption(_MINIDUMP_TYPE nDumpType)
{
	g_eDumpType = nDumpType;
}
void CTMiniDump::SetUserInfo(const char *pszTemp)
{
	g_strUserData = pszTemp;
}
void CTMiniDump::SetHWND(HWND hWnd)
{
	g_hWnd = hWnd;
}

char* CTMiniDump::GetFaultReason(_EXCEPTION_POINTERS* pExPtrs)
{
	if( NULL == pExPtrs || NULL == pExPtrs->ExceptionRecord || NULL ==  pExPtrs->ExceptionRecord->ExceptionCode )
		return "Unknown";

	if (::IsBadReadPtr(pExPtrs, sizeof(EXCEPTION_POINTERS))) 
		return "BAD EXCEPTION POINTERS";

	// ������ ���� �ڵ��� �׳� ��ȯ�� �� �ִ�.
	//http://msdn.microsoft.com/library/default.asp?url=/library/en-us/debug/base/exception_record_str.asp
	switch( pExPtrs->ExceptionRecord->ExceptionCode )
	{
	case EXCEPTION_ACCESS_VIOLATION:         return "EXCEPTION_ACCESS_VIOLATION";		// �߸��� ����
	case EXCEPTION_DATATYPE_MISALIGNMENT:    return "EXCEPTION_DATATYPE_MISALIGNMENT";  // ���ĵ������� ����Ÿ����
	case EXCEPTION_BREAKPOINT:               return "EXCEPTION_BREAKPOINT";				// �극��ũ����Ʈ�� �ɷ�����
	case EXCEPTION_SINGLE_STEP:              return "EXCEPTION_SINGLE_STEP";			// �ϳ��� ��ɾ �����Ҷ�
	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:    return "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";	// �迭�� �Ѿ�����
	case EXCEPTION_FLT_DENORMAL_OPERAND:     return "EXCEPTION_FLT_DENORMAL_OPERAND";	// ����ȭ�� �����ʴ� ������
	case EXCEPTION_FLT_DIVIDE_BY_ZERO:       return "EXCEPTION_FLT_DIVIDE_BY_ZERO";		// float 0���γ���
	case EXCEPTION_FLT_INEXACT_RESULT:       return "EXCEPTION_FLT_INEXACT_RESULT";		// �Ҽ��� ��Ȯ�ϰ� ��Ÿ��������
	case EXCEPTION_FLT_INVALID_OPERATION:    return "EXCEPTION_FLT_INVALID_OPERATION";	// ��Ÿ����
	case EXCEPTION_FLT_OVERFLOW:             return "EXCEPTION_FLT_OVERFLOW";			// float�����÷ο�
	case EXCEPTION_FLT_STACK_CHECK:          return "EXCEPTION_FLT_STACK_CHECK";		// ���ÿ����� ��ġ�ų� �ʹ��������
	case EXCEPTION_FLT_UNDERFLOW:            return "EXCEPTION_FLT_UNDERFLOW";			// ���� ������
	case EXCEPTION_INT_DIVIDE_BY_ZERO:       return "EXCEPTION_INT_DIVIDE_BY_ZERO";		// int�� 0���� ����
	case EXCEPTION_INT_OVERFLOW:             return "EXCEPTION_INT_OVERFLOW";			// int�����÷ο�
	case EXCEPTION_PRIV_INSTRUCTION:         return "EXCEPTION_PRIV_INSTRUCTION";		// ��ɾ ���� �� �� ������
	case EXCEPTION_IN_PAGE_ERROR:            return "EXCEPTION_IN_PAGE_ERROR";			// page�� �ε� �� �� ���µ� �����Ѱ��
	case EXCEPTION_ILLEGAL_INSTRUCTION:      return "EXCEPTION_ILLEGAL_INSTRUCTION";	// �߸��� ��ɾ�
	case EXCEPTION_NONCONTINUABLE_EXCEPTION: return "EXCEPTION_NONCONTINUABLE_EXCEPTION";// �����Ҽ� ���µ� ��� �����ų ���
	case EXCEPTION_STACK_OVERFLOW:           return "EXCEPTION_STACK_OVERFLOW";			// ���ÿ����÷ο�
	case EXCEPTION_INVALID_DISPOSITION:      return "EXCEPTION_INVALID_DISPOSITION";	// �߸��ȱ��(?)���� C������ �߻����� ����
	case EXCEPTION_GUARD_PAGE:               return "EXCEPTION_GUARD_PAGE";				// �����������ϴ� page�� ����
	case EXCEPTION_INVALID_HANDLE:           return "EXCEPTION_INVALID_HANDLE";			// �߸��� �ڵ�
	case 0xE06D7363:                         return "Microsoft C++ Exception";			// Visual C++ ����
	default:
		break;
	}

	return "Unknown";
}

void CTMiniDump::SnapMemoryDump()
{
	/*
	// http://windowssdk.msdn.microsoft.com/en-us/library/ms680519.aspx
	typedef enum _MINIDUMP_TYPE {
	MiniDumpNormal                         = 0x0000,	// ��� �������� ������ ���ð��� ����
	MiniDumpWithDataSegs                   = 0x0001,	// �ε�� ��ƫ�� ����Ÿ���� ����
	MiniDumpWithFullMemory                 = 0x0002,	// ���μ����� �޸� ��ü�� ����
	MiniDumpWithHandleData                 = 0x0004,
	MiniDumpFilterMemory                   = 0x0008,
	MiniDumpScanMemory                     = 0x0010,
	MiniDumpWithUnloadedModules            = 0x0020,
	MiniDumpWithIndirectlyReferencedMemory = 0x0040,
	MiniDumpFilterModulePaths              = 0x0080,
	MiniDumpWithProcessThreadData          = 0x0100,
	MiniDumpWithPrivateReadWriteMemory     = 0x0200,
	} MINIDUMP_TYPE;
	*/
	MINIDUMP_TYPE ePreDumpType = g_eDumpType;
	g_eDumpType = MiniDumpWithFullMemory;
	g_bSaveMessage = FALSE;
	Sleep(1000);
	OnMemoryDump();
	Sleep(1000);
	g_bSaveMessage = TRUE;
	g_eDumpType = ePreDumpType;
}


#define SNAPPROLOG(Cntx)                                                   \
	__asm push  EBP														   \
	__asm mov   EBP , ESP												   \
	__asm sub   ESP , __LOCAL_SIZE										   \
	__asm mov   Cntx.Eax , EAX                                             \
	__asm mov   Cntx.Ebx , EBX                                             \
	__asm mov   Cntx.Ecx , ECX                                             \
	__asm mov   Cntx.Edx , EDX                                             \
	__asm mov   Cntx.Edi , EDI                                             \
	__asm mov   Cntx.Esi , ESI                                             \
	__asm xor   EAX , EAX                                                  \
	__asm mov   AX , GS                                                    \
	__asm mov   Cntx.SegGs , EAX                                           \
	__asm mov   AX , FS                                                    \
	__asm mov   Cntx.SegFs , EAX                                           \
	__asm mov   AX , ES                                                    \
	__asm mov   Cntx.SegEs , EAX                                           \
	__asm mov   AX , DS                                                    \
	__asm mov   Cntx.SegDs , EAX                                           \
	__asm mov   AX , CS                                                    \
	__asm mov   Cntx.SegCs , EAX                                           \
	__asm mov   AX , SS                                                    \
	__asm mov   Cntx.SegSs , EAX                                           \
	__asm mov  EAX , DWORD PTR [EBP]                                       \
	__asm mov  Cntx.Ebp , EAX                                              \
	__asm mov  EAX , EBP                                                   \
	__asm add  EAX , 8                                                     \
	__asm mov  Cntx.Esp , EAX                                              \
	__asm push ESI                                                         \
	__asm push EDI                                                         \
	__asm push EBX                                                         \
	__asm push ECX                                                         \
	__asm push EDX

#define SNAPEPILOG(eRetVal)                                                \
	__asm pop     EDX											           \
	__asm pop     ECX                                                      \
	__asm pop     EBX                                                      \
	__asm pop     EDI                                                      \
	__asm pop     ESI                                                      \
	__asm mov     EAX , eRetVal									           \
	__asm mov     ESP , EBP												   \
	__asm pop     EBP											           \
	__asm ret                     

#define COPYKEYCONTEXTREGISTERS(stFinalCtx,stInitialCtx)               \
	stFinalCtx.Eax   = stInitialCtx.Eax   ;                                \
	stFinalCtx.Ebx   = stInitialCtx.Ebx   ;                                \
	stFinalCtx.Ecx   = stInitialCtx.Ecx   ;                                \
	stFinalCtx.Edx   = stInitialCtx.Edx   ;                                \
	stFinalCtx.Edi   = stInitialCtx.Edi   ;                                \
	stFinalCtx.Esi   = stInitialCtx.Esi   ;                                \
	stFinalCtx.SegGs = stInitialCtx.SegGs ;                                \
	stFinalCtx.SegFs = stInitialCtx.SegFs ;                                \
	stFinalCtx.SegEs = stInitialCtx.SegEs ;                                \
	stFinalCtx.SegDs = stInitialCtx.SegDs ;                                \
	stFinalCtx.SegCs = stInitialCtx.SegCs ;                                \
	stFinalCtx.SegSs = stInitialCtx.SegSs ;                                \
	stFinalCtx.Ebp   = stInitialCtx.Ebp

#define k_CALLNEARBACK  5
#define k_CALLFARBACK   6

BOOL CalculateBeginningOfCallInstruction ( UINT_PTR & dwRetAddr )
{
	BOOL bRet = TRUE ;

	__try
	{
		BYTE * pBytes = (BYTE*)dwRetAddr ;

		if ( 0xE8 == *(pBytes - k_CALLNEARBACK) )
		{
			dwRetAddr -= k_CALLNEARBACK ;
		}
		else if ( 0xFF == *(pBytes - k_CALLFARBACK) )
		{
			dwRetAddr -= k_CALLFARBACK ;
		}
		else
		{
			bRet = FALSE ;
		}
	}
	__except ( EXCEPTION_EXECUTE_HANDLER )
	{
		bRet = FALSE ;
	}
	return ( bRet ) ;
}

BSUMDRET __declspec ( naked ) OnMemoryDump()
{
	_EXCEPTION_POINTERS sDumpInfo;

	CONTEXT stInitialCtx ;
	CONTEXT stFinalCtx ;

	BSUMDRET    eRet ;
	BOOL        bRetVal ;

	eRet = eDUMP_SUCCEEDED ;

	SNAPPROLOG ( stInitialCtx ) ;

	ZeroMemory ( &stFinalCtx , sizeof ( CONTEXT ) ) ;

	stFinalCtx.ContextFlags = 
		CONTEXT_FULL               |
		CONTEXT_CONTROL            |
		CONTEXT_DEBUG_REGISTERS    |
		CONTEXT_EXTENDED_REGISTERS |
		CONTEXT_FLOATING_POINT       ;

	bRetVal = GetThreadContext ( GetCurrentThread ( ) ,&stFinalCtx);
	if ( TRUE == bRetVal )
	{
		COPYKEYCONTEXTREGISTERS ( stFinalCtx , stInitialCtx ) ;

		UINT_PTR dwRetAddr = (UINT_PTR)_ReturnAddress() ;
		bRetVal = CalculateBeginningOfCallInstruction ( dwRetAddr );
		if ( TRUE == bRetVal )
		{
			stFinalCtx.Eip = (DWORD)dwRetAddr ;

			sDumpInfo.ContextRecord = &stFinalCtx;
			CTMiniDump::TopLevelExceptionFilter( &sDumpInfo );
			eRet = eDUMP_SUCCEEDED;
		}
		else
		{
			eRet = eGETTHREADCONTEXT_FAILED;
		}
	}

	SNAPEPILOG ( eRet ) ;
}



