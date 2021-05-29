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
	ON_COMMAND(ID_COLOR, &Cv11View::OnColorRibbonButtonClicked)
	ON_COMMAND(ID_SHAPE, &Cv11View::OnShapeGalleryButtonClicked)
	ON_REGISTERED_MESSAGE(AFX_WM_ON_HIGHLIGHT_RIBBON_LIST_ITEM, RibbonHighlightPreview)
END_MESSAGE_MAP()

// Cv11View construction/destruction

Cv11View::Cv11View() 
{
	this->shape = 0;
	this->color = 0;

	this->shape_preview = 0;
	this->color_preview = 0;
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
	HPEN pen = CreatePen(PS_SOLID, 5, this->color);
	pDC->SelectObject(pen);
	switch (this->shape)
	{
	case 0:
		pDC->Rectangle(this->rc);
		break;
	case 1:
		pDC->Ellipse(this->rc);
		break;
	case 2:
		POINT roundRectCorners;
		roundRectCorners.x = 30; roundRectCorners.y = 30; //Round rectangle corner values
		pDC->RoundRect(this->rc, roundRectCorners);
	}
	DeleteObject(pen);
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



void Cv11View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnLButtonDown(nFlags, point);
	CRectTracker trackRectangle;
	trackRectangle.TrackRubberBand(this, point, true);
	rc = trackRectangle.m_rect;
	Invalidate(true);
}


void Cv11View::OnColorRibbonButtonClicked()
{
	// TODO: Add your command handler code here
	CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arr;
	((CMainFrame*)AfxGetMainWnd())->m_wndRibbonBar.GetElementsByID(ID_COLOR, arr); //Get color button by ID
	CMFCRibbonColorButton* pColor = (CMFCRibbonColorButton*)arr.GetAt(0);

	this->color = pColor->GetColor();
	this->color_preview = pColor->GetColor();
	Invalidate(true);
}


void Cv11View::OnShapeGalleryButtonClicked()
{
	// TODO: Add your command handler code here
	CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arr; //Store ribbon elements
	((CMainFrame*)AfxGetMainWnd())->m_wndRibbonBar.GetElementsByID(ID_SHAPE, arr); //Get gallery in ribbon by ID
	CMFCRibbonGallery* pGallery = (CMFCRibbonGallery*)arr.GetAt(0);


	this->shape = pGallery->GetSelectedItem();
	this->shape_preview = pGallery->GetSelectedItem();
	Invalidate(true);
}


LRESULT Cv11View::RibbonHighlightPreview(WPARAM wparam, LPARAM lparam)
{
	int preview_index = (int)wparam;
	CMFCRibbonBaseElement* highlighted_element = (CMFCRibbonBaseElement*)lparam;
	CMFCRibbonColorButton* highlighted_color = (CMFCRibbonColorButton*)lparam;

	switch (highlighted_element->GetID())
	{
	case ID_SHAPE:
		if (preview_index != -1) this->shape = preview_index;
		else this->shape = this->shape_preview;

		break;
		
	case ID_COLOR:
		if (preview_index != -1) this->color = highlighted_color->GetHighlightedColor();
		else this->color = this->color_preview;
	
		break;
	}

	Invalidate();
	return 0;
}
