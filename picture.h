// picture.h: interface for the picture class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PICTURE_H__358E35C7_08DB_11D3_AA68_00E0CCDDAE2D__INCLUDED_)
#define AFX_PICTURE_H__358E35C7_08DB_11D3_AA68_00E0CCDDAE2D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class PICTURE  
{

public:
	PICTURE();
	virtual ~PICTURE();

	int width;
	int height;
	int bitsperpixel;
	int needbyte;
	int usedcolor;
	int usedbyte;
	BYTE *pixel;	//maybe serval point in one BYTE
	BYTE *point;

public:
	virtual int getpicture(CFile *fp);
	virtual int savepicture(CFile *fp);
	virtual int draw(CDC *pdc);

};

#endif // !defined(AFX_PICTURE_H__358E35C7_08DB_11D3_AA68_00E0CCDDAE2D__INCLUDED_)
