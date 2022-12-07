// ChildDlg.cpp: 구현 파일
//

#include "pch.h"
#include "MessageMap.h"
#include "ChildDlg.h"
#include "MessageMapDlg.h"
#include "afxdialogex.h"


// CChildDlg 대화 상자

IMPLEMENT_DYNAMIC(CChildDlg, CDialogEx)

CChildDlg::CChildDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{
	this->pParent = (CMessageMapDlg*)pParent;
}

CChildDlg::~CChildDlg()
{
}

void CChildDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_ListBoxChild);
	DDX_Control(pDX, IDC_EDIT2, m_EditChild);
}


BEGIN_MESSAGE_MAP(CChildDlg, CDialogEx)
	ON_MESSAGE(UM_MYMESSAGE, &CChildDlg::OnUmMymessageChild)
	ON_BN_CLICKED(IDC_BUTTON_CHILD, &CChildDlg::OnBnClickedButtonChild)
	ON_BN_CLICKED(IDOK, &CChildDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CChildDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CChildDlg 메시지 처리기


afx_msg LRESULT CChildDlg::OnUmMymessageChild(WPARAM wParam, LPARAM lParam)
{
	MessageBox(_T("Receive Message"), _T("ChildDlg"), MB_OK);
	m_ListBoxChild.AddString(pParent->str);
	return 0;
}


void CChildDlg::OnBnClickedButtonChild()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_EditChild.GetWindowTextW(strChild);

	::SendMessage(pParent->m_hWnd, UM_MYMESSAGE, 0, (LPARAM)&strChild);

}



void CChildDlg::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CDialogEx::PostNcDestroy();
	delete this;
}


void CChildDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	PostNcDestroy();
}


void CChildDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	PostNcDestroy();
}
