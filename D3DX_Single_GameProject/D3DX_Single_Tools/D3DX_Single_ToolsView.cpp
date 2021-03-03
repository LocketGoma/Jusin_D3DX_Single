
// D3DX_Single_ToolsView.cpp: CD3DXSingleToolsView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "D3DX_Single_Tools.h"
#endif

#include "D3DX_Single_ToolsDoc.h"
#include "D3DX_Single_ToolsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CD3DXSingleToolsView

IMPLEMENT_DYNCREATE(CD3DXSingleToolsView, CView)

BEGIN_MESSAGE_MAP(CD3DXSingleToolsView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CD3DXSingleToolsView 생성/소멸

CD3DXSingleToolsView::CD3DXSingleToolsView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CD3DXSingleToolsView::~CD3DXSingleToolsView()
{
}

BOOL CD3DXSingleToolsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CD3DXSingleToolsView 그리기

void CD3DXSingleToolsView::OnDraw(CDC* /*pDC*/)
{
	CD3DXSingleToolsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CD3DXSingleToolsView 인쇄

BOOL CD3DXSingleToolsView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CD3DXSingleToolsView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CD3DXSingleToolsView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CD3DXSingleToolsView 진단

#ifdef _DEBUG
void CD3DXSingleToolsView::AssertValid() const
{
	CView::AssertValid();
}

void CD3DXSingleToolsView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CD3DXSingleToolsDoc* CD3DXSingleToolsView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CD3DXSingleToolsDoc)));
	return (CD3DXSingleToolsDoc*)m_pDocument;
}
#endif //_DEBUG


// CD3DXSingleToolsView 메시지 처리기
