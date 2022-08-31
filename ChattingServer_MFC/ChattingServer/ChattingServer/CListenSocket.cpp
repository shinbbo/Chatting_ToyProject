//#include "stdafx.h"
#include "pch.h"
#include "CListenSocket.h"
#include "CChildSocket.h"
#include "ChattingServerDlg.h"


CListenSocket::CListenSocket()
{
}

CListenSocket::~CListenSocket()
{
}

void CListenSocket::OnAccept(int nErrorCode)
{
	CChildSocket* pChild = new CChildSocket();														//클라이언트와 연결할 데이터 소켓 객체를 생성하여

	BOOL check = Accept(*pChild);																				//클라이언트의 접속을 허용하고 데이터 소켓과 연결한다

	if (FALSE == check)
	{
		delete pChild;
		AfxMessageBox(_T("접속 허용 실패"));
		return;
	}

	//CListenSocket 객체의 주소를 CChildSocket 객체에 알려주기 위한 함수 호출
	pChild->SetListenSocket(this);																					//CChildSocket 클래스에 사용자가 정의한 함수
	m_ptrChildSocketList.AddTail(pChild);

	CChattingServerDlg* pMain = (CChattingServerDlg*)AfxGetMainWnd();

	pMain->m_List.AddString(_T(" 서버 접속 허용"));
	pMain->m_List.SetCurSel(pMain->m_List.GetCount() - 1);

	CSocket::OnAccept(nErrorCode);
}

void CListenSocket::CloseClientSocket(CSocket* pChild)
{
	POSITION pos;
	pos = m_ptrChildSocketList.Find(pChild);

	if (NULL != pos)
	{
		pChild->ShutDown();									// 클라이언트와 연결된 데이터 소켓을 닫습니다.
		pChild->Close();											//  접속 종료
	}

	m_ptrChildSocketList.RemoveAt(pos);						//리스트에서 제거 한 후
	delete pChild;																// 메모리에서 해제
}


void CListenSocket::BroadCast(char* pszBuffer, int len)
{
	POSITION pos;
	pos = m_ptrChildSocketList.GetHeadPosition();
	CChildSocket* pChild = NULL;

	while (NULL != pos)
	{
		pChild = (CChildSocket*)m_ptrChildSocketList.GetNext(pos);

		if (NULL != pChild)
		{
			pChild->Send(pszBuffer, len * 2);
		}
	}
}
