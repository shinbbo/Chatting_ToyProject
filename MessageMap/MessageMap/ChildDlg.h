#pragma once

// CChildDlg 대화 상자

class CMessageMapDlg;
class CChildDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChildDlg)

public:
	CChildDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CChildDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_ListBoxChild;
	CMessageMapDlg* pParent;
	CEdit m_EditChild;
	CString strChild;
protected:
	afx_msg LRESULT OnUmMymessageChild(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedButtonChild();
	virtual void PostNcDestroy();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
