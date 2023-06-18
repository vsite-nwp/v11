#pragma once

class Cv11View : public CView
{
protected: // create from serialization only
	Cv11View();
	DECLARE_DYNCREATE(Cv11View)

// Attributes
public:
	Cv11Doc* GetDocument() const;
	CRect rect;

	COLORREF colour;
	byte shape; 

	COLORREF preview_colour;
	byte preview_shape;
// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~Cv11View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg LRESULT OnHighlight(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnShape();
	afx_msg void OnColour();
};

#ifndef _DEBUG  // debug version in v11View.cpp
inline Cv11Doc* Cv11View::GetDocument() const
   { return reinterpret_cast<Cv11Doc*>(m_pDocument); }
#endif

