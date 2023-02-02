
// NewRPMapPosDoc.cpp: CNewRPMapPosDoc 클래스의 구현
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
#include "MainFrm.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CNewRPMapPosDoc

IMPLEMENT_DYNCREATE(CNewRPMapPosDoc, CDocument)

BEGIN_MESSAGE_MAP(CNewRPMapPosDoc, CDocument)
	ON_COMMAND(ID_FILE_OPEN, &CNewRPMapPosDoc::OnFileOpen)
END_MESSAGE_MAP()


// CNewRPMapPosDoc 생성/소멸

CNewRPMapPosDoc::CNewRPMapPosDoc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.
	m_pCFileRead = NULL;
}

CNewRPMapPosDoc::~CNewRPMapPosDoc()
{
}

BOOL CNewRPMapPosDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CNewRPMapPosDoc serialization

void CNewRPMapPosDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CNewRPMapPosDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CNewRPMapPosDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CNewRPMapPosDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CNewRPMapPosDoc 진단

#ifdef _DEBUG
void CNewRPMapPosDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CNewRPMapPosDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CNewRPMapPosDoc 명령
void CNewRPMapPosDoc::OnFileOpen()
{
	
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd(); // (CMainFrame*)AfxGetMainWnd() : AfxGet~()를 CMainFrame이 가지고있다.

	m_pCFileRead = new CFileRead;
	if (false == m_pCFileRead->FileDialog())
	{
		return;
	}
	else
	{
		CNewRPMapPosView* pMView = (CNewRPMapPosView *)pMain->GetActiveView();	// 프레임창에 연결된 활성뷰에대한 포인터 가져옴	
		pMain->SetActiveView(pMView);	// 분할 윈도우로 나누었을 경우 특정 순간에 현재 뷰에서 다른 뷰를 활성화(포커스) 해주어야 할 경우
		pMView->Invalidate();			// 화면전체를 재표시 / 강제로 WM_PAINT메시지 발생->WM_PAINT메시지는 OnDraw()함수를 호출하여 화면을 다시 그리는 역할
	}
}



bool CNewRPMapPosDoc::LoadCheck()
{
	if (NULL != m_pCFileRead)
	{
		return true;
	}
	else
	{
		return false;
	}
}


CFileRead* CNewRPMapPosDoc::getInterface()
{
	return m_pCFileRead;
}