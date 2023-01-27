
// NewRPMapPosView.h: CNewRPMapPosView 클래스의 인터페이스
//

#pragma once
#include "File.h"


class CNewRPMapPosView : public CView
{
protected: // serialization에서만 만들어집니다.
	CNewRPMapPosView() noexcept;
	DECLARE_DYNCREATE(CNewRPMapPosView)

// 특성입니다.
public:
	//CNewRPMapPosDoc* GetDocument() const;
	CMainFrame* GetMainFrm() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CNewRPMapPosView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:


private:
	CFileRead m_pCFileRead;

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // NewRPMapPosView.cpp의 디버그 버전
inline CNewRPMapPosDoc* CNewRPMapPosView::GetDocument() const
   { return reinterpret_cast<CNewRPMapPosDoc*>(m_pDocument); }
#endif

