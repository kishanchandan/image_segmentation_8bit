// KINGBMP.h: interface for the KINGBMP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KINGBMP_H__358E35CA_08DB_11D3_AA68_00E0CCDDAE2D__INCLUDED_)
#define AFX_KINGBMP_H__358E35CA_08DB_11D3_AA68_00E0CCDDAE2D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "picture.h"
/*
class BITMAPUSED
{
public:
	BITMAPINFO bmpinfo;
	RGBQUAD    color[256];
};
*/
class KINGBMP : public PICTURE  
{
public:
	KINGBMP();
	virtual ~KINGBMP();
	
public:
	int savepicture(CFile *fp);
	int getpicture(CFile *fp);
	int draw(CDC *pdc);

public:
	BITMAPFILEHEADER bmphead;
	BITMAPUSED bmpused;
	
};

#endif // !defined(AFX_KINGBMP_H__358E35CA_08DB_11D3_AA68_00E0CCDDAE2D__INCLUDED_)
