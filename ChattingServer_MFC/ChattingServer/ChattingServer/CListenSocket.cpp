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
	CChildSocket* pChild = new CChildSocket();														//Ŭ���̾�Ʈ�� ������ ������ ���� ��ü�� �����Ͽ�

	BOOL check = Accept(*pChild);																				//Ŭ���̾�Ʈ�� ������ ����ϰ� ������ ���ϰ� �����Ѵ�

	if (FALSE == check)
	{
		delete pChild;
		AfxMessageBox(_T("���� ��� ����"));
		return;
	}

	//CListenSocket ��ü�� �ּҸ� CChildSocket ��ü�� �˷��ֱ� ���� �Լ� ȣ��
	pChild->SetListenSocket(this);																					//CChildSocket Ŭ������ ����ڰ� ������ �Լ�
	m_ptrChildSocketList.AddTail(pChild);

	CChattingServerDlg* pMain = (CChattingServerDlg*)AfxGetMainWnd();

	pMain->m_List.AddString(_T(" ���� ���� ���"));
	pMain->m_List.SetCurSel(pMain->m_List.GetCount() - 1);

	CSocket::OnAccept(nErrorCode);
}

void CListenSocket::CloseClientSocket(CSocket* pChild)
{
	POSITION pos;
	pos = m_ptrChildSocketList.Find(pChild);

	if (NULL != pos)
	{
		pChild->ShutDown();									// Ŭ���̾�Ʈ�� ����� ������ ������ �ݽ��ϴ�.
		pChild->Close();											//  ���� ����
	}

	m_ptrChildSocketList.RemoveAt(pos);						//����Ʈ���� ���� �� ��
	delete pChild;																// �޸𸮿��� ����
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
