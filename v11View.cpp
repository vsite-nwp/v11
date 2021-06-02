#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "v11.h"
#endif

#include "v11Doc.h"
#include "v11View.h"
#include "mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cv11View

IMPLEMENT_DYNCREATE(Cv11View, CView)

BEGIN_MESSAGE_MAP(Cv11View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &Cv11View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()

	ON_COMMAND(ID_COLOR, Cv11View::OnColor)
	ON_COMMAND(ID_SHAPE, Cv11View::OnShape)
	ON_REGISTERED_MESSAGE(AFX_WM_ON_HIGHLIGHT_RIBBON_LIST_ITEM, Cv11View::OnHighlightRibbonListItem)
END_MESSAGE_MAP()

// Cv11View construction/destruction
Cv11View::Cv11View() {
	color_draw_ref = 0;
	view_color_ref = 0;
	shape_draw = 0;
	shape_view = 0;
}

Cv11View::~Cv11View()
{
}

BOOL Cv11View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Cv11View drawing

void Cv11View::OnDraw(CDC* cdc) {
	CPen cPen;
	cPen.CreatePen(PS_SOLID, 4, color_draw_ref);
	cdc->SelectObject(cPen);
	CPoint cPoint(50, 50);

	switch (shape_draw) {

	case 0:
		cdc->Rectangle(cRect);
		break;

	case 1:
		cdc->Ellipse(cRect);
		break;

	case 2:
		cdc->RoundRect(cRect, cPoint);
		break;
	default:
		break;
	}

}


// Cv11View printing


void Cv11View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL Cv11View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void Cv11View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void Cv11View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void Cv11View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void Cv11View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// Cv11View diagnostics

#ifdef _DEBUG
void Cv11View::AssertValid() const
{
	CView::AssertValid();
}

void Cv11View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cv11Doc* Cv11View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cv11Doc)));
	return (Cv11Doc*)m_pDocument;
}
#endif //_DEBUG


// Cv11View message handlers

void Cv11View::OnColor() {
	CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arr;
	((CMainFrame*)AfxGetMainWnd())->m_wndRibbonBar.GetElementsByID(ID_COLOR, arr);
	CMFCRibbonColorButton* pGallery = (CMFCRibbonColorButton*)arr.GetAt(0);
	view_color_ref = pGallery->GetColor();
	color_draw_ref = view_color_ref;
	Invalidate();
}

void Cv11View::OnShape() {
	CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> cArray;
	((CMainFrame*)AfxGetMainWnd())->m_wndRibbonBar.GetElementsByID(ID_SHAPE, cArray);
	CMFCRibbonGallery* pGallery = (CMFCRibbonGallery*)cArray.GetAt(0);
	shape_view = pGallery->GetSelectedItem();
	shape_draw = shape_view;
	Invalidate();
}

void Cv11View::OnLButtonDown(UINT nFlags, CPoint point) {
	CRectTracker tracker;
	if (tracker.TrackRubberBand(this, point)) {
		cRect = tracker.m_rect;
		Invalidate();
	}
}


LRESULT Cv11View::OnHighlightRibbonListItem(WPARAM wparam, LPARAM lparam) {
	int index = (int)wparam;
	CMFCRibbonBaseElement* cmfcBaseElement = (CMFCRibbonBaseElement*)lparam;
	UINT uint = cmfcBaseElement->GetID();
	switch (uint) {
	case ID_COLOR:
		if (index < 0) {
			color_draw_ref = view_color_ref;
		}
		else {
			CMFCRibbonColorButton* colorButton = (CMFCRibbonColorButton*)cmfcBaseElement;
			color_draw_ref = colorButton->GetHighlightedColor();
		}
		break;


	case ID_SHAPE:
		if (index < 0) {
			shape_draw = shape_view;
		}
		else {
			shape_draw = index;
		}
		break;
	}


	Invalidate();
	return 0;
}