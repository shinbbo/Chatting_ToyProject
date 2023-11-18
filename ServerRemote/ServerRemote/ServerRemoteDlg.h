
// ServerRemoteDlg.h: 헤더 파일
//

#pragma once
#include "ServerSocket.h"
#include "RemoteEvent.h"

enum class EventThreadWorking
{
	EVENT_STOP = 0,
	EVENT_RUNNING = 1
};


// CServerRemoteDlg 대화 상자
class CServerRemoteDlg : public CDialogEx
{
// 생성입니다.
public:
	CServerRemoteDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVERREMOTE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


public:
	void ShowScreen();

	void EventThreadFunc();
	void EventThreadClose();
	static UINT EventRunThread(LPVOID pParm);

	

	BOOL PreTranslateMessage(MSG* pMsg);
	void ProgressTime();

private:
	CServerSocket* m_pSeverSocket;
	SOCKET m_Socket;

	RemoteEvent m_stRemoteEvent;
	eMouseClickEvent m_eMouseClickEvent;

	CWinThread* m_pEventThread;
	EventThreadWorking m_eEventThreadWorkType;

	int m_Sec;
	DWORD m_Total;

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
