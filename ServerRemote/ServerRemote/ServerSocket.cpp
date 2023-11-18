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
	char szBuffer[128] = { 0, };
	int iReceive = 0;

	if ((hClient = ::accept(m_ServerSocket, (SOCKADDR*)&clientAddr, &iAddrLen)) == INVALID_SOCKET)
	{
		return NULL;
	}

	if (AfxMessageBox(_T("���� ��û�� �����ϰڽ��ϱ�?"), MB_OKCANCEL | MB_ICONINFORMATION) == IDCANCEL)
	{
		return NULL;
	}

	return hClient;
}


//������ ������
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
		TRACE("DataRecv : %d\n", bFlag);
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
		m_pDataThread->SuspendThread();
		DWORD dwResult;
		GetExitCodeThread(m_pDataThread->m_hThread, &dwResult);

		delete m_pDataThread;
		m_pDataThread = NULL;
		m_eDataThreadWorkType = ThreadWorking::STOP;
	}
}

bool CServerSocket::DataRecv()
{
	int iDataLen = 0;
	int iRecv = ::recv(hClient, (char*)&iDataLen, sizeof(iDataLen), 0);
	if (SOCKET_ERROR == iRecv || 0 >= iDataLen)
	{
		TRACE("iDataLen : %d\n", iDataLen);
		return false;
	}

	fp = fopen("Image.png", "wb");
	if (NULL == fp)
	{
		return false;
	}

	int iTotalBytes = 0;
	char szBuf[4096] = { 0 };

	while (iTotalBytes < iDataLen)
	{
		int iBytesRecv = ::recv(hClient, szBuf, sizeof(szBuf), 0);
		if (SOCKET_ERROR == iBytesRecv)
		{
			TRACE("iBytesRecv : %d\n", iBytesRecv);
			return false;
		}

		//���� ������ ũ�⸸ŭ ���� ����
		fwrite(szBuf, sizeof(char), iBytesRecv, fp);
		iTotalBytes += iBytesRecv;
	}

	fclose(fp);
	
	return true;
}

int CServerSocket::Recv(SOCKET s)
{
	char buf[32] = { 0 };

	return ::recv(s, buf, sizeof(buf), 0);
}


void CServerSocket::SocketClose(RemoteEvent _RemoteEvent)
{
	::send(hClient, (char*)&_RemoteEvent, sizeof(_RemoteEvent), 0);
	shutdown(hClient, SD_BOTH);

	char buf[256];
	int iRecv = ::recv(hClient, buf, sizeof(buf), 0);

	closesocket(hClient);
	closesocket(m_ServerSocket);
	WSACleanup();
}