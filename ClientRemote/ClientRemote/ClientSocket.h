#pragma once
#include <afxwin.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)

class CClientSocket : public CWnd
{
public:
	CClientSocket();
	~CClientSocket();

	bool SocketConnect(CString _ip);
	void DataSend();
	int Send(const char* str);
	SOCKET getSocket();
	void SocketClose();

private:
	SOCKET m_ClientSocket;
	FILE* fp;

	CWinThread* m_pEventThread;
};

