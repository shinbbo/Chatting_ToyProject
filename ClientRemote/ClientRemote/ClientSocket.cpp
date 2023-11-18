#include "pch.h"
#include "ClientSocket.h"

CClientSocket::CClientSocket()
{
	fp = NULL;
	m_pEventThread = NULL;
}

CClientSocket::~CClientSocket()
{

}

bool CClientSocket::SocketConnect(CString _ip)
{
	bool bFalg = true;
	WSADATA wsaData;
	int iRes = ::WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (ERROR_SUCCESS != iRes)
	{
		bFalg = false;
	}

	m_ClientSocket = ::socket(AF_INET, SOCK_STREAM, 0);

	if (INVALID_SOCKET == m_ClientSocket)
	{
		AfxMessageBox(_T("Socket Error"));
		bFalg = false;
	}

	CStringA temp = (CStringA)_ip;
	char* ip = temp.GetBuffer();

	SOCKADDR_IN addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = inet_addr(ip);
	addr.sin_port = htons(5000);

	SOCKET clientSocket;

	if (clientSocket = ::connect(m_ClientSocket, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		AfxMessageBox(_T("Connect Error"));
		bFalg = false;
	}

	if (AfxMessageBox(_T("Connect"), MB_OKCANCEL | MB_ICONINFORMATION) == IDOK)
	{
		bFalg = true;
	}

	return bFalg;
}

void CClientSocket::DataSend()
{
	fp = fopen("Image.png", "rb");
	if (NULL == fp)
	{
		return;
	}

	//Image.png 전체 데이터 크기
	int iDataLen = 0;

	fseek(fp, 0, SEEK_END);
	iDataLen = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	int iSend = ::send(m_ClientSocket, (char*)&iDataLen, sizeof(iDataLen), 0);

	if (SOCKET_ERROR == iSend)
	{
		return;
	}

	int iTotalBytes = 0;
	char szBuf[4096] = { 0 };

	while (iTotalBytes < iDataLen)
	{
		int iBytesSend = fread(szBuf, sizeof(char), sizeof(szBuf), fp);
		iSend = ::send(m_ClientSocket, szBuf, iBytesSend, 0);
		if (SOCKET_ERROR == iSend)
		{
			return;
		}
		iTotalBytes += iBytesSend;
	}

	fclose(fp);
	return;
}

int CClientSocket::Send(char* str)
{
	return ::send(m_ClientSocket, str, sizeof(str), 0);
}

SOCKET CClientSocket::getSocket()
{
	return m_ClientSocket;
}

void CClientSocket::SocketClose()
{
	closesocket(m_ClientSocket);
	WSACleanup();
	AfxGetMainWnd()->PostMessageW(WM_COMMAND, ID_APP_EXIT, 0);
}