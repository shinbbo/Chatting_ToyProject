#pragma once
#include "afxsock.h"

class CListenSocket :
	public CSocket
{
public:
	CListenSocket();
	~CListenSocket();

public:
	CPtrList m_ptrChildSocketList;
	virtual void OnAccept(int nErrorCode);
	void CloseClientSocket(CSocket* pChild);
	void BroadCast(char* pszBuffer, int len);
};

