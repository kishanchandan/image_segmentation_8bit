// kingimage.h : main header file for the KINGIMAGE application
//

#if !defined(AFX_KINGIMAGE_H__358E35B5_08DB_11D3_AA68_00E0CCDDAE2D__INCLUDED_)
#define AFX_KINGIMAGE_H__358E35B5_08DB_11D3_AA68_00E0CCDDAE2D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CKingimageApp:
// See kingimage.cpp for the implementation of this class
//

class CKingimageApp : public CWinApp
{
public:
	CKingimageApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKingimageApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CKingimageApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KINGIMAGE_H__358E35B5_08DB_11D3_AA68_00E0CCDDAE2D__INCLUDED_)
