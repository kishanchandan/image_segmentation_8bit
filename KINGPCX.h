// KINGPCX.h: interface for the KINGPCX class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KINGPCX_H__35659D84_09B5_11D3_AA6B_00E0CCDDAE2D__INCLUDED_)
#define AFX_KINGPCX_H__35659D84_09B5_11D3_AA6B_00E0CCDDAE2D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "picture.h"

class KINGPCX : public PICTURE  
{
public:
	KINGPCX();
	virtual ~KINGPCX();

public:
	int savepicture(CFile *fp);
	int getpicture(CFile *fp);
	int draw(CDC *pdc);

public:
	int codepcx(BYTE *fp,BYTE *output,int &totalbyte);
	PCXHEAD pcxhead;
	int offset;
	int decodepcx(BYTE *fp);
	int linebytes;
	GIFCOLOR *color;
	BITMAPUSED bmpused;
	int initbmp();
};

#endif // !defined(AFX_KINGPCX_H__35659D84_09B5_11D3_AA6B_00E0CCDDAE2D__INCLUDED_)
