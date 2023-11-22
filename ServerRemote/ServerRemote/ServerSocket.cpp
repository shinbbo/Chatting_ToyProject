#include "pch.h"
#include "ServerSocket.h"
#include "ServerRemoteDlg.h"


CServerSocket::CServerSocket()
{
	m_pDataThread = NULL;
}

CServerSocket::~CServerSocket()
{

}

SOCKET CServerSocket::ConnectSocket()
{
	WSADATA wsaData;
	int iRes = ::WSAStartup(MAKEWORD(2, 2), &wsaData);
	
	if (ERROR_SUCCESS != iRes)
	{
		return NULL;
	}

	m_ServerSocket = ::socket(PF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN addr;
	memset(&addr, 0, sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(5000);

	iRes = ::bind(m_ServerSocket, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN));
	if (SOCKET_ERROR == iRes)
	{
		return NULL;
	}

	iRes = ::listen(m_ServerSocket, SOMAXCONN);
	if (SOCKET_ERROR == iRes)
	{
		return NULL;
	}

	SOCKADDR_IN clientAddr;
	memset(&clientAddr, 0, sizeof(clientAddr));
	int iAddrLen = sizeof(clientAddr);
	int iReceive = 0;

	if ((hClient = ::accept(m_ServerSocket, (SOCKADDR*)&clientAddr, &iAddrLen)) == INVALID_SOCKET)
	{
		return NULL;
	}

	if (AfxMessageBox(_T("연결 요청을 수락하겠습니까?"), MB_OKCANCEL | MB_ICONINFORMATION) == IDCANCEL)
	{
		return NULL;
	}

	return hClient;
}


//데이터 쓰레드
void CServerSocket::DataThreadFunc()
{
	if (NULL == m_pDataThread)
	{
		m_pDataThread = AfxBeginThread(DataRunThread, this);

		if (NULL == m_pDataThread)
		{
			return;
		}

		m_pDataThread->m_bAutoDelete = FALSE;
		m_eDataThreadWorkType = ThreadWorking::RUNNING;
	}
}

UINT CServerSocket::DataRunThread(LPVOID pParm)
{
	CServerSocket* g_pDlg = (CServerSocket*)pParm;
	CServerRemoteDlg* pMainDlg = (CServerRemoteDlg*)::AfxGetMainWnd();

	if (NULL == g_pDlg || NULL == pMainDlg)
	{
		return 0;
	}

	while (ThreadWorking::RUNNING == g_pDlg->m_eDataThreadWorkType)
	{
		bool bFlag = g_pDlg->DataRecv();
		if (true == bFlag)
		{
			pMainDlg->ShowScreen();
		}
	}

	return 0;
}

void CServerSocket::DataThreadClose()
{
	if (NULL != m_pDataThread)
	{
		m_eDataThreadWorkType = ThreadWorking::STOP;
		Sleep(100);

		m_pDataThread->SuspendThread();
		DWORD dwResult;
		GetExitCodeThread(m_pDataThread->m_hThread, &dwResult);

		delete m_pDataThread;
		m_pDataThread = NULL;
	}
}

bool CServerSocket::DataRecv()
{
	fp = fopen("Image.png", "wb");
	if (NULL == fp)
	{
		return false;
	}

	int iDataLen = 0;
	int iRecv = ::recv(hClient, (char*)&iDataLen, sizeof(iDataLen), 0);
	if (SOCKET_ERROR == iRecv)
	{
		TRACE("iDataLen : %d\n", iDataLen);
		return false;
	}

	int iTotalBytes = 0;
	char szBuf[4096] = { 0, };

	while (iTotalBytes < iDataLen)
	{
		int iBytesRecv = ::recv(hClient, szBuf, sizeof(szBuf), 0);
		if (SOCKET_ERROR == iBytesRecv)
		{
			TRACE("iBytesRecv : %d\n", iBytesRecv);
			return false;
		}

		//받은 데이터 크기만큼 파일 쓰기
		fwrite(szBuf, sizeof(char), iBytesRecv, fp);
		iTotalBytes += iBytesRecv;
	}

	fclose(fp);
	
	return true;
}

int CServerSocket::Recv(SOCKET s)
{
	char szbuf[32] = { 0, };

	return ::recv(s, szbuf, sizeof(szbuf), 0);
}


void CServerSocket::SocketClose(RemoteEvent _RemoteEvent)
{
	::send(hClient, (char*)&_RemoteEvent, sizeof(_RemoteEvent), 0);

	char buf[256] = { 0, };
	int iRecv = ::recv(hClient, buf, sizeof(buf), 0);
	if (0 < iRecv)
	{
		shutdown(hClient, SD_BOTH);
		closesocket(hClient);
	}

	closesocket(m_ServerSocket);
	WSACleanup();
}