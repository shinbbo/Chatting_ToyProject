
// ClientRemoteDlg.h: 헤더 파일
//

#pragma once
#include "ClientSocket.h"
#include "RemoteEvent.h"

enum class ThreadWorking
{
	STOP = 0,
	RUNNING = 1
};

// CClientRemoteDlg 대화 상자
class CClientRemoteDlg : public CDialogEx
{
// 생성입니다.
public:
	CClientRemoteDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENTREMOTE_DIALOG };
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
	void DataThreadFunc();
	static UINT DataRunThread(LPVOID pParam);
	void DataThreadClose();

	void EventThreadFunc();
	static UINT EventRunThread(LPVOID pParm);
	void EventThreadClose();

	void MouseControl(MouseEvent _MouseEvent);
	void KeyboardControl(KeyboardEvent _KeyboardEvent);
	void ScreenShot();

private:
	CClientSocket m_ClientSocket;

	CWinThread* m_pDataThread;
	CWinThread* m_pEventThread;
	
	ThreadWorking m_eDataThreadWorkType;
	ThreadWorking m_eEventThreadWorkType;

	RemoteEvent m_stRemoteEvent;
	CEdit m_IpEdit;
public:
	afx_msg void OnBnClickedButtonConnect();
	afx_msg void OnBnClickedButtonScreen();
	afx_msg void OnDestroy();
};
