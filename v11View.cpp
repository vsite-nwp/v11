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
	ON_COMMAND(ID_SHAPE, &Cv11View::OnShapeClick)
	ON_COMMAND(ID_COLOR, &Cv11View::OnColorCLick)
	ON_COMMAND(ID_FILL, &Cv11View::OnFillClick)
	ON_WM_LBUTTONDOWN()
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_REGISTERED_MESSAGE(AFX_WM_ON_HIGHLIGHT_RIBBON_LIST_ITEM, &Cv11View::OnHighlightRibbonListItem)
END_MESSAGE_MAP()

// Cv11View construction/destruction

Cv11View::Cv11View() {
	color = 0;
	shape = 0;
	fill = 0;

	tempColor = 0;
	tempShape = 0;
	tempFill = 0;
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
	CPen p;
	p.CreatePen(PS_SOLID, 10, color);
	CPen* pOld = pDC->SelectObject(&p);

	CBrush b;
	b.CreateSolidBrush(fill);
	CBrush* bOld = pDC->SelectObject(&b);

	switch (shape) {
	case 0:
		pDC->Rectangle(rc);
		break;
	case 1:
		pDC->Ellipse(rc);
		break;
	case 2:
		pDC->RoundRect(rc, CPoint(100, 100));
		break;
	}

	pDC->SelectObject(pOld);
	pDC->SelectObject(bOld);

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

void Cv11View::OnLButtonDown(UINT nFlags, CPoint point) {
	CRectTracker tracker;
	if (tracker.TrackRubberBand(this, point)) {
		this->rc = tracker.m_rect;
	}
	Invalidate();
}

void Cv11View::OnShapeClick() {
	CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arr;
	((CMainFrame*)AfxGetMainWnd())->m_wndRibbonBar.GetElementsByID(ID_SHAPE, arr);
	CMFCRibbonGallery* pGallery = (CMFCRibbonGallery*)arr.GetAt(0);
	shape = pGallery->GetSelectedItem();
	tempShape = shape;
	Invalidate();
}

void Cv11View::OnColorCLick() {
	CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arr;
	((CMainFrame*)AfxGetMainWnd())->m_wndRibbonBar.GetElementsByID(ID_COLOR, arr);
	CMFCRibbonColorButton* pColor = (CMFCRibbonColorButton*)arr.GetAt(0);
	color = pColor->GetColor();
	tempColor = color;
	Invalidate();


}void Cv11View::OnFillClick() {
	CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arr;
	((CMainFrame*)AfxGetMainWnd())->m_wndRibbonBar.GetElementsByID(ID_FILL, arr);
	CMFCRibbonColorButton* pColor = (CMFCRibbonColorButton*)arr.GetAt(0);
	fill = pColor->GetColor();
	tempFill = fill;
	Invalidate();
}

LRESULT Cv11View::OnHighlightRibbonListItem(WPARAM wp, LPARAM lp)
{
	int index = (int)wp;
	CMFCRibbonBaseElement* pElem = (CMFCRibbonBaseElement*)lp;
	UINT id = pElem->GetID();

	switch (id) {
	case ID_SHAPE:

		if (index == -1)
			shape = tempShape;
		else
			shape = index;
		Invalidate();
		break;


	case ID_COLOR:
		
		if (index == -1)
			color = tempColor;
		else {
			CMFCRibbonColorButton* rcButton = (CMFCRibbonColorButton*)pElem;
			color = rcButton->GetHighlightedColor();
		}
		Invalidate();
		break;


	case ID_FILL:
		
		if (index == -1)
			fill = tempFill;
		else {
			CMFCRibbonColorButton* rcButton = (CMFCRibbonColorButton*)pElem;
			fill = rcButton->GetHighlightedColor();
		}
		Invalidate();
		break;


	default:
		break;
	}
	return 0;
}