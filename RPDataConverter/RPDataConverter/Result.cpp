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
END_MESSAGE_MAP()

BOOL CResult::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	PrintResult();



	return TRUE;
}

// CResult 메시지 처리기
BOOL CResult::PrintResult()
{
	std::vector<CString> vGridID;
	vGridID = m_pCMapFile->getGridIdList();
	CString str;

	for (int i = 0; i < vGridID.size(); i++)
	{
		//str.Format(_T("%s"), vGridID[i]);
		m_listctlResult.InsertItem(0, vGridID[i]);
	}

	return TRUE;
}
