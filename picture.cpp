// PICTURE.cpp: implementation of the PICTURE class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "kingimage.h"
#include "PICTURE.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PICTURE::PICTURE()
{

}

PICTURE::~PICTURE()
{
	if (pixel!=0)
		delete pixel;
	pixel=0;
	if (point!=0)
		delete point;
}

int PICTURE::getpicture(CFile *fp)
{
	return 0;
}

int PICTURE::savepicture(CFile *fp)
{
	return 0;
}

int PICTURE::draw(CDC *pdc)
{
	return 0;
}