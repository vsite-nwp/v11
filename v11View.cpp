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
	ON_COMMAND(ID_SHAPE, &Cv11View::OnShape)
	ON_COMMAND(ID_COLOR, &Cv11View::OnColour)
	ON_REGISTERED_MESSAGE(AFX_WM_ON_HIGHLIGHT_RIBBON_LIST_ITEM, OnHighlight)
	ON_WM_CONTEXTMENU()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// Cv11View construction/destruction

Cv11View::Cv11View() 
{
	shape = 0;
	preview_shape = shape;
	colour = RGB(0, 0, 0);
	preview_colour = colour;
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
	CPen pen(PS_SOLID, 1, colour);
	CPen* usePen = pDC->SelectObject(&pen);

	switch (shape) 
	{
	case 0:
		pDC->Rectangle(rect);
		break;
	case 1:
		pDC->Ellipse(rect);
		break;
	case 2:
		pDC->RoundRect(rect, CPoint(10, 20));
		break;
	}

	pDC->SelectObject(usePen);
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

void Cv11View::OnColour()
{
	CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arr;
	((CMainFrame*)AfxGetMainWnd())->m_wndRibbonBar.GetElementsByID(ID_COLOR, arr);
	CMFCRibbonColorButton* ribbon_colour = (CMFCRibbonColorButton*)arr.GetAt(0);

	colour = ribbon_colour->GetColor();
	preview_colour = colour;

	Invalidate();
}

void Cv11View::OnShape()
{
	CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arr;
	((CMainFrame*)AfxGetMainWnd())->m_wndRibbonBar.GetElementsByID(ID_SHAPE, arr);
	CMFCRibbonGallery* ribbon_gallery = (CMFCRibbonGallery*)arr.GetAt(0);

	shape = ribbon_gallery->GetSelectedItem();
	preview_shape = shape;

	Invalidate();
}

void Cv11View::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRectTracker rectTracker;

	if (rectTracker.TrackRubberBand(this, point)) 
	{
		rect = rectTracker.m_rect;
	}

	Invalidate();
}

LRESULT Cv11View::OnHighlight(WPARAM wp, LPARAM lp)
{
	int i = static_cast<int>(wp);

	if (i == -1) 
	{
		shape = preview_shape;
		colour = preview_colour;
		Invalidate();
		return 0;
	}

	CMFCRibbonBaseElement* ribbon_element = reinterpret_cast<CMFCRibbonBaseElement*>(lp);
	int id = ribbon_element->GetID();

	switch (id) 
	{
	case ID_SHAPE:
		shape = i;
		break;
	case ID_COLOR: 
		CMFCRibbonColorButton* highlight_colour = static_cast<CMFCRibbonColorButton*>(ribbon_element);
		colour = highlight_colour->GetHighlightedColor();
		break;
	}

	Invalidate();
	return 0;
}