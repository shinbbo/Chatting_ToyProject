// FileAdd.cpp: 구현 파일
//

#include "pch.h"
#include "RPDataConverter.h"
#include "FileAdd.h"
#include "afxdialogex.h"


// CFileAdd 대화 상자

IMPLEMENT_DYNAMIC(CFileMerge, CDialogEx)

CFileMerge::CFileMerge(CMapFile* pMapData, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ADD, pParent)
{
	m_pMapFile = pMapData;
}

CFileMerge::~CFileMerge()
{
}

void CFileMerge::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SAVEPATH, m_Edit);
}


BEGIN_MESSAGE_MAP(CFileMerge, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CFileMerge::OnBnClickedButtonSave)
END_MESSAGE_MAP()


// CFileAdd 메시지 처리기
void CFileMerge::FileMerge(TCHAR* szBuffer)
{
	std::vector<CString> vGridID;
	vGridID = m_pMapFile->getGridIdList();

	std::vector<CString> vFilePath;
	vFilePath = m_pMapFile->getFilePath();
	fp = m_pMapFile->getFilePoint();
	CString a = szBuffer;
	_wfopen_s(&fp, a + L"\\merge.bin", L"wb");
	if (NULL == fp)
	{
		return;
	}

	for (int i = 0; i < vGridID.size(); i++)
	{
		std::vector<int> size = m_pMapFile->getFileSize();
		fwrite(m_pMapFile->data, sizeof(MapData), size[i], fp);
	}

	fclose(fp);
}

void CFileMerge::OnBnClickedButtonSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BROWSEINFO BrInfo;
	TCHAR szBuffer[512];                                      // 경로저장 버퍼 

	::ZeroMemory(&BrInfo, sizeof(BROWSEINFO));
	::ZeroMemory(szBuffer, 512);

	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.lpszTitle = _T("파일이 저장될 폴더를 선택하세요");
	BrInfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_RETURNONLYFSDIRS;
	LPITEMIDLIST pItemIdList = ::SHBrowseForFolder(&BrInfo);
	::SHGetPathFromIDList(pItemIdList, szBuffer);				// 파일경로 읽어오기

	CString str;
	str.Format(_T("%s"), szBuffer);
	m_Edit.SetWindowTextW(str);

	FileMerge(szBuffer);
}




