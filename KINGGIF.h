// KINGGIF.h: interface for the KINGGIF class.
//
//////////////////////////////////////////////////////////////////////
//there are something to mention for GIF
//1.after the data header,there is a byte describe next how
//  many bits to read for next data,it is always the bitsperpixel
//  in the gif header,and actual read bits is it add 1.when the
//  table top reach some point,it should increased by 1,but it 
//  should not above 12(can equal,when it is equal,at code you
//  should init table and write a init code at 12 bits,while at
//  decode the next data you read is always init data
//2.after it,there is a synchronize data,it indicate there are 
//  how many data between it and next synchronize data,you can
//  set it 0xfe or 0xff,and the last synchronize data indicate
//  how many data between it and the end data(not include it ,
//  include end data)
//3.after it,is a init data,and code.at the end of code,you should
//  always write a 0
//4.because the different order of write code and write table of
//  code and decode,there is a different of when to add readbits
//  of code and decode.
//5.like the pcx,it should extend to 4,this data can be 0
//6.like the pcx,when display use bmp,the height of bmp info
//  should be negitive.


#if !defined(AFX_KINGGIF_H__35659D83_09B5_11D3_AA6B_00E0CCDDAE2D__INCLUDED_)
#define AFX_KINGGIF_H__35659D83_09B5_11D3_AA6B_00E0CCDDAE2D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "picture.h"


class KINGGIF : public PICTURE  
{
public:
	KINGGIF();
	virtual ~KINGGIF();

public:
	int savepicture(CFile *fp);
	int getpicture(CFile *fp);
	void inittable();


public:
	int find();
	int writecode(BYTE **fp,int *length,int place);
	int codelzw(BYTE *fp,int *length);
	int getcode(BYTE **now);
	int inserttable(int pre,int next);
	int inserttable2(int pre, int next);
	int draw(CDC *pdc);
	int existglobe;
	BYTE usedmask[8];
	int existlocal;
	GIFHEAD gifhead;
	GIFCOLOR *local_gifcolor;
	GIFCOLOR *globe_gifcolor;
	GIFDATAHEAD gifdatahead;
	BITMAPUSED bmpused;
	int initbmp();

	CWordArray table[4096];
	int top;
	int readbits;	//indicate one time read bits
	int leftbits;	//indicate how many bits not read in here byte
	int _end;
	int _init;
	int decodelzw(BYTE *fp);
	int place;
	int nowmax;
	int temp;
	int findstart;
	BYTE skip;
	int preleftbits;
	BYTE index;
	CWordArray aim;
	CWordArray quick[256];	//use for fast find
//	CWordArray quick2[256][256];
//		DWORD t1,t2;

};

#endif // !defined(AFX_KINGGIF_H__35659D83_09B5_11D3_AA6B_00E0CCDDAE2D__INCLUDED_)
