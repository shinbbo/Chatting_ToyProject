
// ServerRemoteDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "ServerRemote.h"
#include "ServerRemoteDlg.h"
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


// CServerRemoteDlg 대화 상자



CServerRemoteDlg::CServerRemoteDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERVERREMOTE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerRemoteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CServerRemoteDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEWHEEL()

	ON_BN_CLICKED(IDCANCEL, &CServerRemoteDlg::OnBnClickedCancel)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CServerRemoteDlg 메시지 처리기

BOOL CServerRemoteDlg::OnInitDialog()
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
	m_pSeverSocket = new CServerSocket();
	m_pEventThread = NULL;
	if (NULL == m_pSeverSocket)
	{
		return FALSE;
	}

	m_Socket = m_pSeverSocket->ConnectSocket();

	while (1)
	{
	RECEVICE_AGAIN:
		int iRecv = m_pSeverSocket->Recv(m_Socket);
		if (SOCKET_ERROR == iRecv)
		{
			goto RECEVICE_AGAIN;
		}
		break;
	}
	
	m_Sec = 0;
	m_Total = 0;
	
	CString str = _T("연결중");
	SetDlgItemText(IDC_STATIC_MARK, str);

	m_pSeverSocket->DataThreadFunc();
	EventThreadFunc();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CServerRemoteDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CServerRemoteDlg::OnPaint()
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
HCURSOR CServerRemoteDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CServerRemoteDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	strcpy(m_stRemoteEvent.szBuf, "exit");
	m_pSeverSocket->SocketClose(m_stRemoteEvent);

	m_pSeverSocket->DataThreadClose();
	EventThreadClose();
	CDialogEx::OnCancel();
}

//데이터 쓰레드
void CServerRemoteDlg::EventThreadFunc()
{
	if (NULL == m_pEventThread)
	{
		m_pEventThread = AfxBeginThread(EventRunThread, this);

		if (NULL == m_pEventThread)
		{
			return;
		}

		m_pEventThread->m_bAutoDelete = FALSE;
		m_eEventThreadWorkType = EventThreadWorking::EVENT_RUNNING;
	}
}

UINT CServerRemoteDlg::EventRunThread(LPVOID pParm)
{
	CServerRemoteDlg* g_pDlg = (CServerRemoteDlg*)pParm;

	if (NULL == g_pDlg)
	{
		return 0;
	}

	g_pDlg->m_stRemoteEvent.stMouseEvent.bFlag = FALSE;
	g_pDlg->m_stRemoteEvent.stKeyboardEvent.bFlag = FALSE;

	while (EventThreadWorking::EVENT_RUNNING == g_pDlg->m_eEventThreadWorkType)
	{
		int iSend = 0;
		g_pDlg->ProgressTime();

		if (TRUE == g_pDlg->m_stRemoteEvent.stMouseEvent.bFlag)
		{
			iSend = ::send(g_pDlg->m_Socket, (char*)&g_pDlg->m_stRemoteEvent, sizeof(g_pDlg->m_stRemoteEvent), 0);

			if (SOCKET_ERROR == iSend)
			{
				return 0;
			}

			g_pDlg->m_stRemoteEvent.stMouseEvent.MouseClickEvnet = eMouseClickEvent::None;
			g_pDlg->m_stRemoteEvent.stMouseEvent.bFlag = FALSE;
		}

		if (TRUE == g_pDlg->m_stRemoteEvent.stKeyboardEvent.bFlag)
		{
			iSend = ::send(g_pDlg->m_Socket, (char*)&g_pDlg->m_stRemoteEvent, sizeof(g_pDlg->m_stRemoteEvent), 0);

			if (SOCKET_ERROR == iSend)
			{
				return 0;
			}

			g_pDlg->m_stRemoteEvent.stKeyboardEvent.bFlag = FALSE;
		}
	}

	return 0;
}

void CServerRemoteDlg::EventThreadClose()
{
	if (NULL != m_pEventThread)
	{
		m_eEventThreadWorkType = EventThreadWorking::EVENT_STOP;
		Sleep(100);

		m_pEventThread->SuspendThread();
		DWORD dwResult;
		GetExitCodeThread(m_pEventThread->m_hThread, &dwResult);

		delete m_pEventThread;
		m_pEventThread = NULL;
	}
}

void CServerRemoteDlg::ProgressTime()
{
	if (m_Sec == 0)
	{
		m_Sec = GetTickCount64();
	}
	m_Total = GetTickCount64() - m_Sec;

	CString strTime;
	strTime.Format(_T("%02d:%02d"), (m_Total / 1000 / 60), ((m_Total / 1000) % 60));

	SetDlgItemText(IDC_EDIT_TIME, strTime);
}

void CServerRemoteDlg::ShowScreen()
{
	CWnd* pWnd = (CWnd*)GetDlgItem(IDC_STATIC_PIC);
	CStatic* pPicBox = (CStatic*)GetDlgItem(IDC_STATIC_PIC);
	CDC* dc = pWnd->GetWindowDC();

	CRect rcPicBox;
	pPicBox->GetClientRect(rcPicBox);

	if (!m_Image.IsNull())
	{
		m_Image.Destroy();
	}

	HRESULT hResult = m_Image.Load(_T("Image.png"));
	if (FAILED(hResult))
	{
		CString str = _T("ERROR : Failed to load");
		str += _T("\n");
		TRACE(str);
	}

	::SetStretchBltMode(dc->m_hDC, HALFTONE);
	m_Image.StretchBlt(dc->m_hDC, 0, 0, rcPicBox.Width(), rcPicBox.Height(), SRCCOPY);

	ReleaseDC(dc);
	m_Image.Destroy();
}

void CServerRemoteDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CWnd* pCtl = GetDlgItem(IDC_STATIC_PIC);
	if (NULL == pCtl)
	{
		return;
	}

	//PictureControl
	CRect rectCtl;
	pCtl->GetWindowRect(&rectCtl);
	ScreenToClient(&rectCtl);
	pCtl->MoveWindow(rectCtl.left, rectCtl.top, cx - 2 * rectCtl.left, cy - rectCtl.top - rectCtl.left, TRUE);

	//Edit Control
	CRect rcEdit, EditRect;
	GetClientRect(rcEdit);
	GetDlgItem(IDC_EDIT_TIME)->GetWindowRect(EditRect);
	ScreenToClient(EditRect);
	GetDlgItem(IDC_EDIT_TIME)->SetWindowPos(NULL, rcEdit.Width() / 2, rcEdit.top + 10, 0, 0, SWP_NOSIZE);

	//Static
	CRect rcStatic, StaticRect;
	GetClientRect(rcStatic);
	GetDlgItem(IDC_STATIC)->GetWindowRect(StaticRect);
	ScreenToClient(StaticRect);
	GetDlgItem(IDC_STATIC)->SetWindowPos(NULL, (rcStatic.Width() / 2) - 50, rcStatic.top + 10, 0, 0, SWP_NOSIZE);

	//Cancle Button
	CRect rcCancle, CancleRect;
	GetClientRect(rcCancle);
	GetDlgItem(IDCANCEL)->GetWindowRect(CancleRect);
	ScreenToClient(CancleRect);
	GetDlgItem(IDCANCEL)->SetWindowPos(NULL, rcCancle.Width() - 70, rcCancle.top + 10, 0, 0, SWP_NOSIZE);

}


void CServerRemoteDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rc;
	CWnd* pPictureControl = GetDlgItem(IDC_STATIC_PIC);

	ClientToScreen(&point);
	::ScreenToClient(pPictureControl->m_hWnd, &point);
	int iWidth = 0, iHeight = 0;

	if (NULL != pPictureControl)
	{
		pPictureControl->GetClientRect(&rc);
		iWidth = rc.Width();
		iHeight = rc.Height();
	}

	m_stRemoteEvent.stMouseEvent.pos.x = UINT((point.x * GetSystemMetrics(SM_CXSCREEN)) / iWidth);
	m_stRemoteEvent.stMouseEvent.pos.y = UINT((point.y * GetSystemMetrics(SM_CYSCREEN)) / iHeight);

	CDialogEx::OnMouseMove(nFlags, point);
}


void CServerRemoteDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_stRemoteEvent.stMouseEvent.MouseClickEvnet = eMouseClickEvent::LButtonDown;
	m_stRemoteEvent.stMouseEvent.bFlag = TRUE;

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CServerRemoteDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_stRemoteEvent.stMouseEvent.MouseClickEvnet = eMouseClickEvent::LButtonUp;
	m_stRemoteEvent.stMouseEvent.bFlag = TRUE;

	CDialogEx::OnLButtonUp(nFlags, point);
}


void CServerRemoteDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_stRemoteEvent.stMouseEvent.MouseClickEvnet = eMouseClickEvent::RButtonDown;
	m_stRemoteEvent.stMouseEvent.bFlag = TRUE;

	CDialogEx::OnRButtonDown(nFlags, point);
}


BOOL CServerRemoteDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (zDelta <= 0)
	{
		m_stRemoteEvent.stMouseEvent.MouseClickEvnet = eMouseClickEvent::WheelDown;
		m_stRemoteEvent.stMouseEvent.bFlag = TRUE;
	}
	else
	{
		m_stRemoteEvent.stMouseEvent.MouseClickEvnet = eMouseClickEvent::WheelUp;
		m_stRemoteEvent.stMouseEvent.bFlag = TRUE;
	}

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}

BOOL CServerRemoteDlg::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYDOWN == pMsg->message)
	{
		m_stRemoteEvent.stKeyboardEvent.iKey[0] = 0;
		BOOL bShiftPressed = GetKeyState(VK_SHIFT) & 0x8000;
		BOOL bCtrlPressed = GetKeyState(VK_SHIFT) & 0x8000;
		if (bShiftPressed)
		{
			m_stRemoteEvent.stKeyboardEvent.iKey[0] = VK_SHIFT;

			switch (pMsg->wParam)
			{
			case 'A':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'A';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'B':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'B';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'C':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'C';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'D':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'D';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'E':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'E';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'F':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'F';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'G':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'G';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'H':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'H';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'I':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'I';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'J':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'J';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'K':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'K';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'L':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'L';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'M':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'M';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'N':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'N';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'O':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'O';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'P':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'P';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'Q':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'Q';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'R':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'R';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'S':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'S';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'T':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'T';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'U':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'U';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'V':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'V';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'W':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'W';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'X':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'X';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'Y':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'Y';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'Z':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'Z';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case '1':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = '1';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case '2':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = '2';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case '3':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = '3';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case '4':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = '4';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case '5':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = '5';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case '6':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = '6';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case '7':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = '7';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case '8':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = '8';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case '9':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = '9';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case '0':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = '0';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_OEM_1:		//;:
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_OEM_1;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_OEM_2:		//?/
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_OEM_2;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_OEM_3:		//`~
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_OEM_3;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_OEM_4:		//[{
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_OEM_4;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_OEM_5:		//\|
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_OEM_5;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_OEM_6:		//]}
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_OEM_6;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_OEM_7:		//"
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_OEM_7;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_OEM_PLUS:	//+=
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_OEM_PLUS;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_OEM_MINUS:	//_-
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_OEM_MINUS;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_OEM_COMMA:	//,<
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_OEM_COMMA;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_OEM_PERIOD:	//.>
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_OEM_PERIOD;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			}
		}

		else if (bCtrlPressed)
		{
			m_stRemoteEvent.stKeyboardEvent.iKey[0] = VK_CONTROL;

			switch (pMsg->wParam)
			{
			case 'A':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'A';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'C':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'C';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'E':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'E';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'D':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'D';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'F':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'F';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'N':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'N';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'P':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'P';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'S':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'S';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'T':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'T';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'V':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'V';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'W':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'W';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'X':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'X';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'Y':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'Y';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'Z':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'Z';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			}
		}

		else
		{
			switch (pMsg->wParam)
			{
			case 'A':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'A';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;

			case 'B':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'B';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'C':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'C';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'D':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'D';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'E':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'E';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'F':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'F';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'G':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'G';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'H':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'H';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'I':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'I';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'J':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'J';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'K':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'K';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'L':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'L';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'M':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'M';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'N':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'N';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'O':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'O';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'P':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'P';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'Q':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'Q';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'R':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'R';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'S':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'S';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'T':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'T';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'U':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'U';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'V':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'V';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'W':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'W';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'X':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'X';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'Y':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'Y';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case 'Z':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = 'Z';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case '1':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = '1';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case '2':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = '2';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case '3':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = '3';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case '4':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = '4';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case '5':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = '5';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case '6':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = '6';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case '7':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = '7';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case '8':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = '8';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case '9':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = '9';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case '0':
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = '0';
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_UP:			//위쪽 화살표
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_UP;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_DOWN:		//아래쪽 화살표
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_DOWN;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_LEFT:		//왼쪽 화살표
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_LEFT;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_RIGHT:		//오른쪽 화살표
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_RIGHT;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_SPACE:		//SpaceBar
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_SPACE;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				return TRUE;
				break;
			case VK_PRIOR:		//Page Up
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_PRIOR;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_NEXT:		//Page Down
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_NEXT;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_END:		//End
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_END;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_HOME:		//Home
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_HOME;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_INSERT:		//Insert
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_INSERT;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_DELETE:		//Delete
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_DELETE;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_LWIN:		//왼쪽 Window
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_LWIN;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_RWIN:		//오른쪽 Window
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_RWIN;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_BACK:		//BackSpace
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_BACK;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_TAB:		//Tab
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_TAB;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_RETURN:		//Enter
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_RETURN;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				return TRUE;
				break;
			case VK_SHIFT:		//Shift
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_SHIFT;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_MENU:		//Alt
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_MENU;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_CAPITAL:	//Caps Lock
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_CAPITAL;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_HANGUL:		//한글
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_HANGUL;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_ESCAPE:		//Esc
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_ESCAPE;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				return TRUE;
				break;
			case VK_MULTIPLY:	//숫자키패드 *
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_MULTIPLY;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_ADD:		//숫자키패드 +
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_ADD;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_SUBTRACT:	//숫자키패드 -
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_SUBTRACT;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_DECIMAL:	//숫자키패드 .
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_DECIMAL;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_DIVIDE:		//숫자키패드 /
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_DIVIDE;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_NUMPAD0:	//숫자키패드 0
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_NUMPAD0;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_NUMPAD1:	//숫자키패드 1
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_NUMPAD1;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_NUMPAD2:	//숫자키패드 2
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_NUMPAD2;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_NUMPAD3:	//숫자키패드 3
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_NUMPAD3;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_NUMPAD4:	//숫자키패드 4
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_NUMPAD4;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_NUMPAD5:	//숫자키패드 5
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_NUMPAD5;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_NUMPAD6:	//숫자키패드 6
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_NUMPAD6;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_NUMPAD7:	//숫자키패드 7
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_NUMPAD7;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_NUMPAD8:	//숫자키패드 8
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_NUMPAD8;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_NUMPAD9:	//숫자키패드 9
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_NUMPAD9;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_OEM_1:		//;:
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_OEM_1;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_OEM_2:		//?/
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_OEM_2;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_OEM_3:		//`~
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_OEM_3;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_OEM_4:		//[{
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_OEM_4;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_OEM_5:		//\|
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_OEM_5;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_OEM_6:		//]}
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_OEM_6;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_OEM_7:		//"
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_OEM_7;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_OEM_PLUS:	//+=
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_OEM_PLUS;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_OEM_MINUS:	//_-
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_OEM_MINUS;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_OEM_COMMA:	//,<
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_OEM_COMMA;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
			case VK_OEM_PERIOD:	//.>
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_OEM_PERIOD;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
				/*
			case VK_SNAPSHOT: // PrtScr
				m_stRemoteEvent.stKeyboardEvent.iKey[1] = VK_SNAPSHOT;
				m_stRemoteEvent.stKeyboardEvent.bFlag = TRUE;
				break;
				*/
			default:
				break;
			}
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}






