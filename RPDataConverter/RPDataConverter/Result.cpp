// Result.cpp: 구현 파일
//

#include "pch.h"
#include "RPDataConverter.h"
#include "Result.h"
#include "afxdialogex.h"


// CResult 대화 상자

IMPLEMENT_DYNAMIC(CResult, CDialogEx)

CResult::CResult(CMapFile *pFile, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_RESULT, pParent)
{
	m_pCMapFile = pFile;
}

CResult::~CResult()
{
}

void CResult::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RESULT, m_listctlResult);
}


BEGIN_MESSAGE_MAP(CResult, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_MERGE, &CResult::OnBnClickedButtonMerge)
END_MESSAGE_MAP()

BOOL CResult::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect rect;
	m_listctlResult.GetClientRect(&rect);

	m_listctlResult.InsertColumn(0, _T("파일 이름"), LVCFMT_CENTER, 120);
	m_listctlResult.InsertColumn(1, _T("도로 데이터"), LVCFMT_CENTER, rect.Width() - 250);
	m_listctlResult.InsertColumn(2, _T("차로 데이터"), LVCFMT_CENTER, rect.Width() - 250);

	m_listctlResult.SetExtendedStyle(LVS_EX_GRIDLINES);

	if (FALSE == PrintResult())
	{
		return FALSE;
	}

	return TRUE;
}

// CResult 메시지 처리기
BOOL CResult::PrintResult()
{
	std::vector<CString> vGridID;
	std::vector< GridInfo> vGridInfo;	

	vGridID = m_pCMapFile->getGridIdList();
	vGridInfo = m_pCMapFile->getGridInfo();
	
	CString str;

	for (int i = 0; i < vGridID.size(); i++)
	{
		m_listctlResult.InsertItem(i, vGridID[i]);

		str.Format(_T("%d"), vGridInfo[i].iNumOfRoad);
		m_listctlResult.SetItemText(i, 1, str);

		str.Format(_T("%d"), vGridInfo[i].iNumOfLane);
		m_listctlResult.SetItemText(i, 2, str);
	}

	return TRUE;
}


void CResult::OnBnClickedButtonMerge()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileMerge dlg(m_pCMapFile);
	dlg.DoModal();
}
