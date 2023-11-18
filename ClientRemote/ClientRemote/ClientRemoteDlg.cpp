
// ClientRemoteDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "ClientRemote.h"
#include "ClientRemoteDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CClientRemoteDlg 대화 상자



CClientRemoteDlg::CClientRemoteDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENTREMOTE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientRemoteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_IP, m_IpEdit);
}

BEGIN_MESSAGE_MAP(CClientRemoteDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CClientRemoteDlg::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_SCREEN, &CClientRemoteDlg::OnBnClickedButtonScreen)
END_MESSAGE_MAP()


// CClientRemoteDlg 메시지 처리기

BOOL CClientRemoteDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CClientRemoteDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CClientRemoteDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CClientRemoteDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//데이터 쓰레드
void CClientRemoteDlg::DataThreadFunc()
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

UINT CClientRemoteDlg::DataRunThread(LPVOID pParm)
{
	CClientRemoteDlg* g_pDlg = (CClientRemoteDlg*)pParm;
	if (NULL == g_pDlg)
	{
		return 0;
	}

	while (ThreadWorking::RUNNING == g_pDlg->m_eDataThreadWorkType)
	{
		g_pDlg->ScreenShot();
		g_pDlg->m_ClientSocket.DataSend();
	}

	return 0;
}

void CClientRemoteDlg::DataThreadClose()
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


//이벤트 쓰레드
void CClientRemoteDlg::EventThreadFunc()
{
	if (NULL == m_pEventThread)
	{
		m_pEventThread = AfxBeginThread(EventRunThread, this);

		if (NULL == m_pEventThread)
		{
			return;
		}

		m_pEventThread->m_bAutoDelete = FALSE;
		m_eEventThreadWorkType = ThreadWorking::RUNNING;
	}
}

UINT CClientRemoteDlg::EventRunThread(LPVOID pParm)
{
	CClientRemoteDlg* g_pDlg = (CClientRemoteDlg*)pParm;
	if (NULL == g_pDlg)
	{
		return 0;
	}

	SOCKET s = g_pDlg->m_ClientSocket.getSocket();

	while (ThreadWorking::RUNNING == g_pDlg->m_eEventThreadWorkType)
	{
		int iRecv = ::recv(s, (char*)&g_pDlg->m_stRemoteEvent, sizeof(g_pDlg->m_stRemoteEvent), 0);

		if (SOCKET_ERROR == iRecv)
		{
			return 0;
		}

		if (0 == (strcmp(g_pDlg->m_stRemoteEvent.szBuf, "exit")))
		{
			g_pDlg->m_ClientSocket.SocketClose();
			g_pDlg->EventThreadClose();
			g_pDlg->DataThreadClose();
		}

		if (TRUE == g_pDlg->m_stRemoteEvent.stMouseEvent.bFlag)
		{
			g_pDlg->MouseControl(g_pDlg->m_stRemoteEvent.stMouseEvent);
		}

		if (TRUE == g_pDlg->m_stRemoteEvent.stKeyboardEvent.bFlag)
		{
			g_pDlg->KeyboardControl(g_pDlg->m_stRemoteEvent.stKeyboardEvent);
		}
	}

	return 0;
}



void CClientRemoteDlg::EventThreadClose()
{
	if (NULL != m_pEventThread)
	{
		m_pEventThread->SuspendThread();
		DWORD dwResult;
		GetExitCodeThread(m_pEventThread->m_hThread, &dwResult);

		delete m_pEventThread;
		m_pEventThread = NULL;
		m_eEventThreadWorkType = ThreadWorking::STOP;
	}
}


void CClientRemoteDlg::ScreenShot()
{
	HDC dc = ::GetWindowDC(NULL);

	CImage Image;

	int cx = ::GetSystemMetrics(SM_CXSCREEN);
	int cy = ::GetSystemMetrics(SM_CYSCREEN);
	int color = ::GetDeviceCaps(dc, BITSPIXEL);

	Image.Create(cx, cy, color, 0);

	::BitBlt(Image.GetDC(), 0, 0, cx, cy, dc, 0, 0, SRCCOPY);

	Image.Save(L"Image.png", Gdiplus::ImageFormatPNG);

	::ReleaseDC(NULL, dc);

	Image.ReleaseDC();
	Image.Destroy();
}

//마우스 이벤트
void CClientRemoteDlg::MouseControl(MouseEvent _MouseEvent)
{
	switch (_MouseEvent.MouseClickEvnet)
	{
	case eMouseClickEvent::LButtonDown:
		SetCursorPos(_MouseEvent.pos.x, _MouseEvent.pos.y);
		Sleep(200);
		::mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		
		_MouseEvent.MouseClickEvnet = eMouseClickEvent::None;
		break;
	case eMouseClickEvent::LButtonUp:
		SetCursorPos(_MouseEvent.pos.x, _MouseEvent.pos.y);
		Sleep(200);
		::mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

		_MouseEvent.MouseClickEvnet = eMouseClickEvent::None;
		break;
	case eMouseClickEvent::RButtonDown:
		SetCursorPos(_MouseEvent.pos.x, _MouseEvent.pos.y);
		Sleep(200);
		::mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
		Sleep(200);
		::mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

		_MouseEvent.MouseClickEvnet = eMouseClickEvent::None;
		break;
	case eMouseClickEvent::LButtonDouble:
		SetCursorPos(_MouseEvent.pos.x, _MouseEvent.pos.y);
		
		::mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		::mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		Sleep(10);
		::mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
		::mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

		_MouseEvent.MouseClickEvnet = eMouseClickEvent::None;
		break;
	case eMouseClickEvent::WheelDown:
		SetCursorPos(_MouseEvent.pos.x, _MouseEvent.pos.y);
		::mouse_event(MOUSEEVENTF_WHEEL, 0, 0, -120, 0);
		Sleep(200);
		
		_MouseEvent.MouseClickEvnet = eMouseClickEvent::None;
		break;
	case eMouseClickEvent::WheelUp:
		SetCursorPos(_MouseEvent.pos.x, _MouseEvent.pos.y);
		::mouse_event(MOUSEEVENTF_WHEEL, 0, 0, 120, 0);
		Sleep(200);

		_MouseEvent.MouseClickEvnet = eMouseClickEvent::None;
		break;
	default:
		break;
	}
}


//키보드 이벤트
void CClientRemoteDlg::KeyboardControl(KeyboardEvent _KeyboardEvent)
{
	if (VK_SHIFT == _KeyboardEvent.iKey[0])
	{
		keybd_event(_KeyboardEvent.iKey[0], MapVirtualKey(_KeyboardEvent.iKey[0], MAPVK_VK_TO_VSC), KEYEVENTF_SCANCODE, 0);
		keybd_event(_KeyboardEvent.iKey[1], MapVirtualKey(_KeyboardEvent.iKey[1], MAPVK_VK_TO_VSC), KEYEVENTF_SCANCODE, 0);
		Sleep(1);
		keybd_event(_KeyboardEvent.iKey[1], MapVirtualKey(_KeyboardEvent.iKey[1], MAPVK_VK_TO_VSC), KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP, 0);
		keybd_event(_KeyboardEvent.iKey[0], MapVirtualKey(_KeyboardEvent.iKey[0], MAPVK_VK_TO_VSC), KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP, 0);
	}
	else if(VK_CONTROL == _KeyboardEvent.iKey[0])
	{
		keybd_event(_KeyboardEvent.iKey[0], MapVirtualKey(_KeyboardEvent.iKey[0], MAPVK_VK_TO_VSC), KEYEVENTF_SCANCODE, 0);
		keybd_event(_KeyboardEvent.iKey[1], MapVirtualKey(_KeyboardEvent.iKey[1], MAPVK_VK_TO_VSC), KEYEVENTF_SCANCODE, 0);
		Sleep(1);
		keybd_event(_KeyboardEvent.iKey[1], MapVirtualKey(_KeyboardEvent.iKey[1], MAPVK_VK_TO_VSC), KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP, 0);
		keybd_event(_KeyboardEvent.iKey[0], MapVirtualKey(_KeyboardEvent.iKey[0], MAPVK_VK_TO_VSC), KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP, 0);
	}
	else
	{
		keybd_event(_KeyboardEvent.iKey[1], 0, KEYEVENTF_EXTENDEDKEY, 0);
		Sleep(1);
		keybd_event(_KeyboardEvent.iKey[1], 0, KEYEVENTF_KEYUP, 0);
	}
}


void CClientRemoteDlg::OnBnClickedButtonConnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString IpStr;
	m_IpEdit.GetWindowTextW(IpStr);

	if (TRUE != m_ClientSocket.SocketConnect(IpStr))
	{
		AfxMessageBox(_T("Socket Connect Error"));
		return;
	}
}


void CClientRemoteDlg::OnBnClickedButtonScreen()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szBuf[32] = "연결중";
	int iSend = m_ClientSocket.Send(szBuf);
	if (SOCKET_ERROR == iSend)
	{
		return;
	}

	DataThreadFunc();
	EventThreadFunc();
}
