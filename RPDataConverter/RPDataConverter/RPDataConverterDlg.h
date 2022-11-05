﻿
// RPDataConverterDlg.h: 헤더 파일
//

#pragma once
#include "MapData.h"
#include "Result.h"

// CRPDataConverterDlg 대화 상자
class CRPDataConverterDlg : public CDialogEx
{
// 생성입니다.
public:
	CRPDataConverterDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RPDATACONVERTER_DIALOG };
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
	afx_msg void OnBnClickedButton1();
	CString GetUserSelectFolder(CString strMsg);
	void GetFileList(CString strFolder);


private:
	CString strPathName1;
	CMapFile* m_pMapFile;
	

public:
	
	CListCtrl m_ListControl;
	afx_msg void OnBnClickedButton2();
};
