// ChildDlg.cpp: 구현 파일
//

#include "pch.h"
#include "MessageMap.h"
#include "ChildDlg.h"
#include "afxdialogex.h"


// CChildDlg 대화 상자

IMPLEMENT_DYNAMIC(CChildDlg, CDialogEx)

CChildDlg::CChildDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CChildDlg::~CChildDlg()
{
}

void CChildDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CChildDlg, CDialogEx)
END_MESSAGE_MAP()


// CChildDlg 메시지 처리기
