#pragma once
#include "MapData.h"
#include "FileAdd.h"

// CResult 대화 상자

class CResult : public CDialogEx
{
	DECLARE_DYNAMIC(CResult)

public:
	CResult(CMapFile *pFile, CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CResult();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_RESULT };
#endif

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	BOOL PrintResult();

private:
	CMapFile* m_pCMapFile;
	CListCtrl m_listctlResult;

	std::vector<CString> vGridID;
	std::vector< GridInfo> vGridInfo;
public:
	afx_msg void OnBnClickedButtonMerge();
	afx_msg void OnBnClickedCancel();
};
