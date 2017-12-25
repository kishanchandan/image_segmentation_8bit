// kingimageDoc.h : interface of the CKingimageDoc class
//
/////////////////////////////////////////////////////////////////////////////
#include "picture.h"
#include "kingbmp.h"
#include "kingpcx.h"
#include "kinggif.h"
#include "kingjpg.h"


#if !defined(AFX_KINGIMAGEDOC_H__358E35BD_08DB_11D3_AA68_00E0CCDDAE2D__INCLUDED_)
#define AFX_KINGIMAGEDOC_H__358E35BD_08DB_11D3_AA68_00E0CCDDAE2D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CKingimageDoc : public CDocument
{
protected: // create from serialization only
	CKingimageDoc();
	DECLARE_DYNCREATE(CKingimageDoc)

// Attributes
public:

// Operations
public:
	int imagetype;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKingimageDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CKingimageDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	PICTURE	*_picture;
	KINGBMP *_bmp;
	KINGPCX *_pcx;
	KINGGIF *_gif;
	KINGJPG *_jpg;
	int returnkey;
// Generated message map functions
protected:
	//{{AFX_MSG(CKingimageDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KINGIMAGEDOC_H__358E35BD_08DB_11D3_AA68_00E0CCDDAE2D__INCLUDED_)
