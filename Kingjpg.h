// KINGJPG.h: interface for the KINGJPG class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KINGJPG_H__3119FCF3_284E_11D3_AA98_00E0CCDDAE2D__INCLUDED_)
#define AFX_KINGJPG_H__3119FCF3_284E_11D3_AA98_00E0CCDDAE2D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "picture.h"

class KINGJPG : public PICTURE  
{
public:
	KINGJPG();
	virtual ~KINGJPG();

public:
	int  getpicture(CFile *fp);
	int  savepicture(CFile *fp);
	int  anti_entropy(int *place,BYTE *rorate_time,short int *after_anti_entropy,int looptime);
	int  anti_quantizer(short int *after_anti_quantizer,short int *after_anti_entropy);
	int  anti_color(int x,int y,short int *after_anti_dct);
	int  l_rotate(int *orignal,BYTE *now,BYTE *rotate_time,int *place);
	int  get_pic();
	int  color(int xnow,int ynow,short int *before_dct);
	int  quantizer(short int *before_entropy);
	int  entropy(BYTE *codedata,int *place,BYTE *rotate_time,short int *before_entropy,int looptime);
	int  codejpg(BYTE *codedata,int headlength,int *reallength);
	int  init_ac_huff();
	int  init_dc_huff();
	int  writecode(int length2,int orignal,BYTE *codedata,BYTE *rotate_time,int *place);
	int  draw(CDC *pdc);
	int  initbmp();

public:
	BYTE quantize[2][64];		//Á¿»¯¾ØÕó
	BYTE quantize_num;
	BYTE *huff_dc_table[4];
	BYTE *huff_ac_table[4];
	int  huff_dc_table_num[4];
	int  huff_ac_table_num[4];
	BYTE huff_dc_start_table[4][16];
	BYTE huff_ac_start_table[4][16];
	BYTE first_dc[4];
	BYTE first_ac[4];
	int  max_dc_ac_length[8][16];
	int  start_dc_ac_length[8][16];
	BYTE dc_number;
	BYTE ac_number;
	BYTE existerror;
	BYTE scale[3];
	BYTE usedquan[3];
	BYTE *used_dc_table;
	BYTE *used_ac_table;
	BYTE csnumber;
	BYTE now;
	int  place;
	int  xuse_length,yuse_length;
	int  dc_before;
	int  dc_y_before;
	int  dc_cb_before;
	int  dc_cr_before;
	int  _start[2],_end[2];
	int  headlength;
	BYTE ac_huff_type;
	BYTE *display;

public:
	BITMAPINFO bmpinfo;
};

#endif // !defined(AFX_KINGJPG_H__3119FCF3_284E_11D3_AA98_00E0CCDDAE2D__INCLUDED_)
