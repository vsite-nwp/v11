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
	ON_COMMAND(ID_SHAPE, &Cv11View::OnChangeDesign)
	ON_COMMAND(ID_COLOR, &Cv11View::OnChangeColor)
	ON_WM_LBUTTONDOWN()
	ON_REGISTERED_MESSAGE(AFX_WM_ON_HIGHLIGHT_RIBBON_LIST_ITEM, &Cv11View::OnHighlightRibbonListItem)
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &Cv11View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// Cv11View construction/destruction

Cv11View::Cv11View() {
	color = RGB(0, 0, 0);
	shape = 0;
	previewColor = RGB(0, 0, 0);
	previewShape = 0;
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

void Cv11View::OnDraw(CDC* pDC)
{
	CPen pen;
	pen.CreatePen(PS_SOLID, 6, color);
	pDC->SelectObject(pen);
	POINT pn = { 50, 50 };
	if (shape == 0)
		pDC->Rectangle(&rect);
	if (shape == 1)
		pDC->Ellipse(&rect);
	if (shape == 2)
		pDC->RoundRect(&rect, pn);
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


void Cv11View::OnLButtonDown(UINT f, CPoint p) {
	CRectTracker track;
	if (track.TrackRubberBand(this, p)) {
		rect = track.m_rect;
		Invalidate();
	}
}

// Cv11View message handlers

void Cv11View::OnChangeColor() {
	CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arr;
	((CMainFrame*)AfxGetMainWnd())->m_wndRibbonBar.GetElementsByID(ID_COLOR, arr);
	CMFCRibbonColorButton* ribbonColorButton = (CMFCRibbonColorButton*)arr.GetAt(0);
	color = ribbonColorButton->GetColor();
	previewColor = color;
	Invalidate();
}

void Cv11View::OnChangeDesign() {
	CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arr;
	((CMainFrame*)AfxGetMainWnd())->m_wndRibbonBar.GetElementsByID(ID_SHAPE, arr);
	CMFCRibbonGallery* ribbonGallery = (CMFCRibbonGallery*)arr.GetAt(0);
	shape = ribbonGallery->GetSelectedItem();
	previewShape = shape;
	Invalidate();
}

LRESULT Cv11View::OnHighlightRibbonListItem(WPARAM wp, LPARAM lp) {
	int i = (int)wp;
	CMFCRibbonBaseElement* pElement = (CMFCRibbonBaseElement*)lp;
	UINT id = pElement->GetID();
	if (id == ID_COLOR) {
		if (i == -1)
		{
			color = previewColor;
		}
		else {
			CMFCRibbonColorButton* colorBtn = (CMFCRibbonColorButton*)pElement;
			color = colorBtn->GetHighlightedColor();
		}
	}
	if (id == ID_SHAPE) {
		if (i == -1)
		{
			shape = previewShape;
		}
		else {
			shape = i;
		}
	}
	Invalidate();
	return 0;
}