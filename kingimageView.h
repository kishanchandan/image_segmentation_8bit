// kingimageView.h : interface of the CKingimageView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_KINGIMAGEVIEW_H__358E35BF_08DB_11D3_AA68_00E0CCDDAE2D__INCLUDED_)
#define AFX_KINGIMAGEVIEW_H__358E35BF_08DB_11D3_AA68_00E0CCDDAE2D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CKingimageView : public CScrollView
{
protected: // create from serialization only
	CKingimageView();
	DECLARE_DYNCREATE(CKingimageView)

// Attributes
public:
	CKingimageDoc* GetDocument();

//	void Processing();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKingimageView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CKingimageView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CKingimageView)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnProcess();
	afx_msg void Onany();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNewmenu();
	afx_msg void OnRegionDetection();
	afx_msg void OnAnswer1Createnegativeimage();
	afx_msg void OnAnswer1Applyhistogramequalization();
	afx_msg void OnAnswer2Labelshapes();
};

#ifndef _DEBUG  // debug version in kingimageView.cpp
inline CKingimageDoc* CKingimageView::GetDocument()
   { return (CKingimageDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KINGIMAGEVIEW_H__358E35BF_08DB_11D3_AA68_00E0CCDDAE2D__INCLUDED_)
