#pragma once
#include "MapData.h"

// CFileAdd 대화 상자

class CFileMerge : public CDialogEx
{
	DECLARE_DYNAMIC(CFileMerge)

public:
	CFileMerge(CMapFile* pMapFile, CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CFileMerge();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ADD };
#endif

public:
	void FileMerge(TCHAR* szBuffer);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSave();

private:
	CMapFile* m_pMapFile;
	CEdit m_Edit;
	FILE* fp;
};
