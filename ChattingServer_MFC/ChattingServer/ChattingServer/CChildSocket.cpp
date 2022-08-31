#include "pch.h"
#include "CChildSocket.h"
#include "CListenSocket.h"
#include "ChattingServerDlg.h"


CChildSocket::CChildSocket()
{
}

CChildSocket::~CChildSocket()
{
}


//연결 된 클라이언트의 소켓 주소를 m_pListenSocket에 저장
void CChildSocket::SetListenSocket(CAsyncSocket* pSocket)
{
	m_pListenSocket = pSocket;
}


//클라이언트 소켓이 닫히게 되면 호출되는 함수
void CChildSocket::OnClose(int nErrorCode)
{
	CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;
	pServerSocket->CloseClientSocket(this);

	CSocket::OnClose(nErrorCode);
}



//클라이언트가 서버로부터 데이터를 보내게 되면 자동으로 호출되는 함수
void CChildSocket::OnReceive(int nErrorCode)
{
	CString temp = _T("");
	CString strIPAdress = _T("");

	UINT uPortNumber = 0;

	char szBuffer[1024];
	::ZeroMemory(szBuffer, 1024);

	//연결된 클라이언트의 IP 주소와 포트 번호를 알아낸다.
	GetPeerName(strIPAdress, uPortNumber);

	//실제로 데이터를 수신한다.
	int len = 0;
	if ((len = Receive(szBuffer, 1024)) > 0)
	{
		CChattingServerDlg* pMain = (CChattingServerDlg*)AfxGetMainWnd();

		temp.Format(_T("[%s] : %s"), strIPAdress, szBuffer);

		pMain->m_List.AddString(temp);
		pMain->m_List.SetCurSel(pMain->m_List.GetCount() - 1);

		CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;
		pServerSocket->BroadCast(szBuffer, len);
	}

	CSocket::OnReceive(nErrorCode);
}
