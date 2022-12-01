
// RPDataConverterDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "RPDataConverter.h"
#include "RPDataConverterDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define FILELEN 1024

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
public:
	afx_msg void OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
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


// CRPDataConverterDlg 대화 상자



CRPDataConverterDlg::CRPDataConverterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RPDATACONVERTER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRPDataConverterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST1, m_ListControl);
	DDX_Control(pDX, IDC_LIST3, m_ListBox);
}

BEGIN_MESSAGE_MAP(CRPDataConverterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	
	ON_BN_CLICKED(IDC_BUTTON1, &CRPDataConverterDlg::OnBnClickedButton1)
	
	ON_BN_CLICKED(IDC_BUTTON2, &CRPDataConverterDlg::OnBnClickedButton2)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CRPDataConverterDlg::OnNMDblclkList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CRPDataConverterDlg::OnLvnItemchangedList)
	ON_LBN_DBLCLK(IDC_LIST3, &CRPDataConverterDlg::OnLbnDblclkList)
END_MESSAGE_MAP()


// CRPDataConverterDlg 메시지 처리기

BOOL CRPDataConverterDlg::OnInitDialog()
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
	m_pMapFile = new CMapFile;

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CRPDataConverterDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CRPDataConverterDlg::OnPaint()
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
HCURSOR CRPDataConverterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CRPDataConverterDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	strPathName1 = GetUserSelectFolder(_T("선택 파일"));


	if (0 == strPathName1.GetLength())
	{
		return;
	}
		
	SetDlgItemText(IDC_EDIT1, strPathName1);

	GetFileList(strPathName1);

}

CString CRPDataConverterDlg::GetUserSelectFolder(CString strMsg)
{
	ITEMIDLIST *pidlBrowse;
	TCHAR pszPathname[1024] = { 0, };
	BROWSEINFO BrInfo;
	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.pidlRoot = NULL;
	CString strUploadDir;

	memset(&BrInfo, 0, sizeof(BrInfo));
	BrInfo.pszDisplayName = pszPathname;
	BrInfo.lpszTitle = strMsg;
	BrInfo.ulFlags = BIF_RETURNONLYFSDIRS;

	pidlBrowse = ::SHBrowseForFolder(&BrInfo);

	if (NULL != pidlBrowse)
	{
		::SHGetPathFromIDList(pidlBrowse, pszPathname);
		strUploadDir = pszPathname;  // 폴더 경로
		strUploadDir += _T("\\");
	}
	else
	{
		strUploadDir = _T("");
	}
		
	return strUploadDir;
}


void CRPDataConverterDlg::GetFileList(CString strFolder)
{
	CFileFind file;
	BOOL b = file.FindFile(strFolder + _T("*.*"));	     			// 모든 확장자를 다 사용.	
	CString strFolderItem, strFileExt, strTempString;
	BOOL Check;
	int num = 0;

	m_ListControl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES);

	if (FALSE == b)
	{
		file.Close();
		return;
	}

	while (b)
	{
		b = file.FindNextFile();
		if (TRUE == file.IsDirectory() && FALSE == file.IsDots())			// 디렉토리 발견시 
		{
			strFolderItem = file.GetFilePath();
			GetFileList(strFolderItem);						// 하위폴더를 검색하기 위해 재귀호출 발생  
		}
		strFolderItem = file.GetFilePath();
		strFileExt = strFolderItem.Mid(strFolderItem.ReverseFind('.'));		// 확장자만 추출한다. 

		if (FALSE == file.IsDots())									// 파일 탐색 필터 정의에따라 해당 StringList에 추가
		{
			strFileExt.MakeUpper();							// strFileExt 에는 확장자 (.EXE 형태) 가 들어옴. 비교위해 대문자화 함
			if (TRUE == file.IsDirectory())
			{
				continue;				// 폴더만 남는 경우는 넣으면 안됨 
			}

			strFolderItem.Replace(strPathName1, _T(""));

			m_ListControl.InsertItem(num, strFolderItem);
		}
	}
}


//Read 버튼
void CRPDataConverterDlg::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CStringArray temp;
	CString str;

	int nCount = m_ListControl.GetItemCount();
	int DataCnt = m_ListBox.GetCount();
	if (DataCnt <= 0)
	{
		AfxMessageBox(_T("데이터가 없습니다."));
	}
	
	for (int i = 0; i < DataCnt; i++)
	{
		m_ListBox.GetText(i, str);
		temp.Add(str);
	}

	/*
	for (int i = 0; i < nCount; i++)
	{
		BOOL Check = m_ListControl.GetCheck(i);
		if (TRUE == Check)
		{
			str = m_ListControl.GetItemText(i, 0);
			temp.Add(str);
		}
	}*/

	int SelDataCnt = temp.GetSize();
	m_pMapFile->setGridCnt(SelDataCnt);

	for (int i = 0; i < SelDataCnt; i++)
	{
		m_pMapFile->FileOpen(strPathName1, temp.GetAt(i), i);
	}

	CResult dlg(m_pMapFile);
	dlg.DoModal();
}




void CRPDataConverterDlg::OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.


	CString FileName;
	int cnt = m_ListControl.GetItemCount();
	for (int i = 0; i < cnt; i++)
	{
		BOOL Check = m_ListControl.GetCheck(i);
		if (TRUE == Check)
		{
			FileName = m_ListControl.GetItemText(i, 0);
			UINT idx = m_ListBox.FindStringExact(-1, FileName);
			if (idx != LB_ERR)
			{
				m_ListBox.DeleteString(idx);
			}
			else
			{
				m_ListBox.InsertString(-1, FileName);
			}
		}
	}


	*pResult = 0;
}


void CRPDataConverterDlg::OnLvnItemchangedList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString str;
	if (pNMLV->uChanged == LVIF_STATE)
	{
		if ((pNMLV->uOldState & 0x1000) && (pNMLV->uNewState & 0x2000))
		{
			int cnt = m_ListControl.GetItemCount();
			for (int i = 0; i < cnt; i++)
			{
				BOOL Check = m_ListControl.GetCheck(i);
				if (TRUE == Check)
				{
					str = m_ListControl.GetItemText(i, 0);
					UINT idx = m_ListBox.FindStringExact(-1, str);
					if (idx != LB_ERR)
					{
						continue;
					}
					else
					{
						m_ListBox.InsertString(-1, str);
					}
				}
			}
		}
		else if ((pNMLV->uOldState & 0x2000) && (pNMLV->uNewState & 0x1000))
		{
			int cnt = m_ListControl.GetItemCount();
			for (int i = 0; i < cnt; i++)
			{
				BOOL Check = m_ListControl.GetCheck(i);
				if (FALSE == Check)
				{
					str = m_ListControl.GetItemText(i, 0);
					UINT idx = m_ListBox.FindStringExact(-1, str);
					if (idx != LB_ERR)
					{
						m_ListBox.DeleteString(idx);
					}
				}
			}
		}
	}
	*pResult = 0;
}


void CRPDataConverterDlg::OnLbnDblclkList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int CurSel = m_ListBox.GetCurSel();

	if (CurSel > 0)
	{
		m_ListBox.DeleteString(CurSel);
	}

}
