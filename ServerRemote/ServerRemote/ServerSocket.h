#pragma once
#include <afxwin.h>
#include "RemoteEvent.h"
#pragma warning(disable:4996)
enum class ThreadWorking
{
	STOP = 0,
	RUNNING = 1
};


class CServerSocket : public CWnd
{
public:
	CServerSocket();
	~CServerSocket();

	SOCKET ConnectSocket();
	bool DataRecv();
	int Recv(SOCKET socket);
	void SocketClose(RemoteEvent _RemoteEvent);
	

	void DataThreadFunc();
	static UINT DataRunThread(LPVOID pParam);
	void DataThreadClose();

private:
	SOCKET m_ServerSocket;
	SOCKET hClient;
	FILE* fp;

	CWinThread* m_pDataThread;
	ThreadWorking m_eDataThreadWorkType;
};

