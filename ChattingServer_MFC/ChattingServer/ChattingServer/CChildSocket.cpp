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


//���� �� Ŭ���̾�Ʈ�� ���� �ּҸ� m_pListenSocket�� ����
void CChildSocket::SetListenSocket(CAsyncSocket* pSocket)
{
	m_pListenSocket = pSocket;
}


//Ŭ���̾�Ʈ ������ ������ �Ǹ� ȣ��Ǵ� �Լ�
void CChildSocket::OnClose(int nErrorCode)
{
	CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;
	pServerSocket->CloseClientSocket(this);

	CSocket::OnClose(nErrorCode);
}



//Ŭ���̾�Ʈ�� �����κ��� �����͸� ������ �Ǹ� �ڵ����� ȣ��Ǵ� �Լ�
void CChildSocket::OnReceive(int nErrorCode)
{
	CString temp = _T("");
	CString strIPAdress = _T("");

	UINT uPortNumber = 0;

	char szBuffer[1024];
	::ZeroMemory(szBuffer, 1024);

	//����� Ŭ���̾�Ʈ�� IP �ּҿ� ��Ʈ ��ȣ�� �˾Ƴ���.
	GetPeerName(strIPAdress, uPortNumber);

	//������ �����͸� �����Ѵ�.
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
