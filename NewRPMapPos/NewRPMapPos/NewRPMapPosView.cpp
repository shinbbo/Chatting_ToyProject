
// NewRPMapPosView.cpp: CNewRPMapPosView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "NewRPMapPos.h"
#endif

#include "NewRPMapPosDoc.h"
#include "NewRPMapPosView.h"
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNewRPMapPosView

IMPLEMENT_DYNCREATE(CNewRPMapPosView, CView)

BEGIN_MESSAGE_MAP(CNewRPMapPosView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CNewRPMapPosView 생성/소멸

CNewRPMapPosView::CNewRPMapPosView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.
	CenterDC.x = CenterDC.y = 0;

	m_Rect.SetRect(0, 0, (long)(::GetSystemMetrics(SM_CXFULLSCREEN)*1.5), (long)(::GetSystemMetrics(SM_CYFULLSCREEN)*1.5));
	CenterDC.x = ((m_Rect.right - m_Rect.left) / 2);
	CenterDC.y = ((m_Rect.bottom - m_Rect.top) / 2);
}

CNewRPMapPosView::~CNewRPMapPosView()
{
}

BOOL CNewRPMapPosView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CNewRPMapPosView 그리기
/*
void CNewRPMapPosView::OnDraw(CDC* pDC)
{
	CNewRPMapPosDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	std::vector< LaneDataVertex> LaneDataVertex = m_pCFileRead.getLaneDataVertex();
	int size = LaneDataVertex.size();

}*/


// CNewRPMapPosView 인쇄

BOOL CNewRPMapPosView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CNewRPMapPosView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CNewRPMapPosView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CNewRPMapPosView 진단

#ifdef _DEBUG
void CNewRPMapPosView::AssertValid() const
{
	CView::AssertValid();
}

void CNewRPMapPosView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

/*
CNewRPMapPosDoc* CNewRPMapPosView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNewRPMapPosDoc)));
	return (CNewRPMapPosDoc*)m_pDocument;
}*/
#endif //_DEBUG


CMainFrame* CNewRPMapPosView::GetMainFrm() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNewRPMapPosDoc)));
	return (CMainFrame*)m_pDocument;
}


// CNewRPMapPosView 메시지 처리기
void CNewRPMapPosView::OnDraw(CDC* pDC)
{
	CNewRPMapPosDoc* pDoc = (CNewRPMapPosDoc*)GetDocument();
	if (NULL == pDoc)
	{
		return;
	}

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.

	if (false == pDoc->LoadCheck())
	{
		return;
	}

	m_pCFileRead = pDoc->getInterface();


	std::vector< LaneDataVertex> LaneDataVertex = m_pCFileRead->getLaneDataVertex();
	int size = LaneDataVertex.size();

	for (int i = 0; i < size; i++)
	{

	}
}