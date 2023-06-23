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
	ON_REGISTERED_MESSAGE(AFX_WM_ON_HIGHLIGHT_RIBBON_LIST_ITEM, &Cv11View::OnHighlightRibbonListItem)
	ON_COMMAND(ID_SHAPE, &Cv11View::OnShape)
	ON_COMMAND(ID_COLOR, &Cv11View::OnColor)
END_MESSAGE_MAP()

// Cv11View construction/destruction

Cv11View::Cv11View() 
	: m_currColor(0)
	, m_prevColor(0)
	, m_currShape(0)
	, m_prevShape(0)
{}

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
	pen.CreatePen(PS_SOLID, 2, m_currColor);
	pDC->SelectObject(pen);
	switch (m_currShape)
	{
	case 0:
		pDC->Rectangle(&m_rc);
		break;
	case 1:
		pDC->Ellipse(&m_rc);
		break;
	case 2:
		pDC->RoundRect(&m_rc, POINT{100, 100} /* arrbitary point */);
		break;
	default:
		// unhandeled shape, do nothing
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

void Cv11View::OnLButtonDown(UINT /* nFlags */, CPoint point)
{
	CRectTracker rectTracker;
	if (rectTracker.TrackRubberBand(this, point))
		m_rc = rectTracker.m_rect;
	
	Invalidate();
}

void Cv11View::OnShape()
{
	CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arr;
	((CMainFrame*)AfxGetMainWnd())->m_wndRibbonBar.GetElementsByID(ID_SHAPE, arr);
	CMFCRibbonGallery* pGallery = (CMFCRibbonGallery*)arr.GetAt(0);
	m_prevShape = m_currShape;
	m_currShape = pGallery->GetSelectedItem();

	Invalidate();
}

void Cv11View::OnColor()
{
	CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*> arr;
	((CMainFrame*)AfxGetMainWnd())->m_wndRibbonBar.GetElementsByID(ID_COLOR, arr);
	CMFCRibbonColorButton* pGallery = (CMFCRibbonColorButton*)arr.GetAt(0);
	m_prevColor = m_currColor;
	m_currColor = pGallery->GetColor();

	Invalidate();
}

void Cv11View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

LRESULT Cv11View::OnHighlightRibbonListItem(WPARAM wp, LPARAM lp)
{
	int index = (int)wp;
	CMFCRibbonBaseElement* pElem = (CMFCRibbonBaseElement*)lp;
	UINT id = pElem->GetID(); // button id (ID_SHAPE, ID_COLOR)

	switch (id)
	{
	case ID_COLOR:
	{
		if (index == -1)
		{
			m_currColor = m_prevColor;
		}
		else
		{
			CMFCRibbonColorButton* colorBtn = (CMFCRibbonColorButton*)pElem;
			m_currColor = colorBtn->GetHighlightedColor();
		}
			Invalidate();
		break;
	}
	case ID_SHAPE:
	{
		if (index == -1)
		{
			m_currShape = m_prevShape;
		}
		else
		{
			m_currShape = index;
		}
			Invalidate();
		break;
	}
	default:
	{
		// unhandeled, do nothing
		break;
	}
	}

	return 0;
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

