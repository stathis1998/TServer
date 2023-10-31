// TChatSocket.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TChatClient.h"
#include "TChatSocket.h"
#include "CryptographyExt.h"

INT64 g_4skey[KEY_COUNT] = {
	0x5193817ae183acee,
	0x3891aeacbed18ead,
	0x549aeced13de13a1,
	0x09aeb1498c1eade9,
	0x19861acea1720ae7,
	0x0139aecea89541a2,
	0x6b97253c5fbb8b06 };

CString g_strSecretKey = "A5$$8AFS13A1::-11#!..'�19716AC&�/D1;;1#";

MAPTCHATSOCKET CTChatSocket::m_mapSOCKET;


// CTChatSocket

CTChatSocket::CTChatSocket()
{
	memset( &m_target, 0x00, sizeof(SOCKADDR_IN));
	m_sock = INVALID_SOCKET;

	m_bSVR = SVR_NONE;
	m_bSvrID = 0;

	m_packet.ExpandIoBuffer(MAX_PACKET_SIZE);
	m_bValid = FALSE;
	m_pOwner = NULL;
	m_bUseCrypt = TRUE;
	m_dlCheckFile = 0;
}

CTChatSocket::~CTChatSocket()
{
	End();
}


// CTChatSocket 멤버 함수입니다.

CTChatSocket *CTChatSocket::GetTChatSocket( SOCKET sock)
{
	MAPTCHATSOCKET::iterator finder = m_mapSOCKET.find(sock);

	if( finder != m_mapSOCKET.end() )
		return (*finder).second;

	return NULL;
}

void CTChatSocket::InitTChatSocket()
{
	m_mapSOCKET.clear();
}

void CTChatSocket::ReleaseTChatSocket()
{
	m_mapSOCKET.clear();
}

void CTChatSocket::SetOwner( CWnd *pOwner)
{
	m_pOwner = pOwner;
}

BYTE CTChatSocket::Start( LPCTSTR strAddr, DWORD dwPort, BYTE bType)
{
	BOOL bAsync = TRUE;
	End();

	if(!m_pOwner)
		return FALSE;

	m_sock = WSASocket(
		AF_INET,
		bType,
		0, NULL,
		0, 0);

	if( m_sock == INVALID_SOCKET )
		return FALSE;

	if( ioctlsocket( m_sock, FIONBIO, (unsigned long *) &bAsync) == SOCKET_ERROR )
	{
		closesocket(m_sock);
		m_sock = INVALID_SOCKET;

		return FALSE;
	}
	int nSIZE = 512 * 1024;

	if( setsockopt( m_sock, SOL_SOCKET, SO_RCVBUF, (const char *) &nSIZE, sizeof(int)) == SOCKET_ERROR )
	{
		closesocket(m_sock);
		m_sock = INVALID_SOCKET;

		return FALSE;
	}

	hostent *pHostent = gethostbyname(strAddr);
	if( !pHostent )
		return FALSE;

	CString strIP = inet_ntoa(*((in_addr *) *pHostent->h_addr_list));

	memset( &m_target, 0x00, sizeof(SOCKADDR_IN));
	m_target.sin_family = AF_INET;
	m_target.sin_addr.s_addr = inet_addr(strIP);
	m_target.sin_port = htons((u_short) dwPort);

	if( WSAAsyncSelect( m_sock, m_pOwner->GetSafeHwnd(), WM_SESSION_MSG, FD_CONNECT|FD_READ|FD_CLOSE) == SOCKET_ERROR || (
		connect( m_sock, (SOCKADDR *) &m_target, sizeof(SOCKADDR_IN)) && GetLastError() != WSAEWOULDBLOCK ))
	{
		closesocket(m_sock);
		m_sock = INVALID_SOCKET;

		return FALSE;
	}

	m_mapSOCKET.insert( MAPTCHATSOCKET::value_type(
		m_sock,
		this));

	return TRUE;
}

void CTChatSocket::End()
{
	MAPTCHATSOCKET::iterator finder = m_mapSOCKET.find(m_sock);

	if( m_sock != INVALID_SOCKET )
		closesocket(m_sock);

	if( finder != m_mapSOCKET.end() )
		m_mapSOCKET.erase(finder);

	m_sock = INVALID_SOCKET;
	m_bValid = FALSE;

	memset( &m_target, 0x00, sizeof(SOCKADDR_IN));
	m_packet.Clear();

	m_dwSendNumber = 0;	
	m_dwRecvNumber = 0;
}

void CTChatSocket::Encrypt( CPacket *pPacket)
{
	pPacket->m_pHeader->m_dwNumber = ++m_dwSendNumber;
	INT64 key = g_4skey[pPacket->m_pHeader->m_dwNumber % KEY_COUNT];
	pPacket->Encrypt(key);
	pPacket->EncryptHeader(key);

	CString strSecretKey = g_strSecretKey;

	LPBYTE lpszSecretKey = (LPBYTE)(LPCTSTR)strSecretKey;
	DWORD dwSecretKey = (strSecretKey.GetLength() + 1) * sizeof(TCHAR);

	DWORD dwBufSize = DWORD(pPacket->m_pHeader->m_wSize);

	EncryptBuffer(CALG_RC4, pPacket->m_pBuf, dwBufSize, pPacket->m_pBuf, dwBufSize, lpszSecretKey, dwSecretKey);

	pPacket->m_pHeader->m_wSize = WORD(dwBufSize);
}

BYTE CTChatSocket::Decrypt(CPacket * pPacket)
{
	if(!m_bUseCrypt)
		return TRUE;

	m_dwRecvNumber++;

	INT64 key = g_4skey[m_dwRecvNumber % KEY_COUNT];

	if(!m_packet.DecryptHeader(key) ||
		m_packet.m_pHeader->m_dwNumber != m_dwRecvNumber ||
		!m_packet.Decrypt(key))
		return FALSE;

	return TRUE;
}

void CTChatSocket::Say( CPacket *pPacket)
{
	if( !IsValid() || !pPacket )
		return;

	Encrypt(pPacket);

	LPBYTE pBUF = pPacket->GetBuffer();
	int nSEND = 0;
	int nSIZE = INT(pPacket->GetSize());

	while(nSEND < nSIZE)
	{
		int nLOCAL = send( m_sock, (const char *) pBUF, nSIZE - nSEND, 0);

		if( nLOCAL != SOCKET_ERROR )
		{
			nSEND += nLOCAL;
			pBUF += nLOCAL;
		}
		else
		{
			int nERROR = WSAGetLastError();

			if( nERROR != WSAEWOULDBLOCK )
				return;
		}
	}
}

int CTChatSocket::CheckMSG()
{
	if( m_packet.GetReadBytes() < PACKET_HEADER_SIZE ||
		m_packet.GetReadBytes() < m_packet.GetSize() )
		return PACKET_INCOMPLETE;

	if(!Decrypt(&m_packet))
	{
		ATLTRACE("Decrypt Error");		
		return PACKET_INVALID;
	}

	return PACKET_COMPLETE;
}

void CTChatSocket::Flush()
{
	m_packet.Flush();
}

BYTE CTChatSocket::IsValid()
{
	return m_bValid;
}

BYTE CTChatSocket::Read( DWORD dwRead)
{
	if( dwRead == 0 )
		return FALSE;

	return m_packet.ReadBytes(dwRead);
}
