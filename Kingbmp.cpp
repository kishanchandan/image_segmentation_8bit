// KINGBMP.cpp: implementation of the KINGBMP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "kingimage.h"
#include "kingbmp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

KINGBMP::KINGBMP()
{

}

KINGBMP::~KINGBMP()
{
	delete point;
	point=0;
	delete pixel;
	pixel=0;
}

int KINGBMP::getpicture(CFile *fp)
{
	int num;
	fp->SeekToBegin();
	num=fp->Read(&bmphead,sizeof(bmphead));
	num=fp->Read(&bmpused.bmpinfo.bmiHeader,sizeof(bmpused.bmpinfo.bmiHeader));
	width=bmpused.bmpinfo.bmiHeader.biWidth;	//modify it to make it can divide by 4
	height=bmpused.bmpinfo.bmiHeader.biHeight;
	bitsperpixel=bmpused.bmpinfo.bmiHeader.biBitCount;
	needbyte=width*height;

	switch (bmpused.bmpinfo.bmiHeader.biBitCount)
	{
	case 1:
		if (bmpused.bmpinfo.bmiHeader.biClrUsed==0){
			fp->Read(&(bmpused.bmpinfo.bmiColors[0]),2*4);
			usedcolor=2;
		}
		else{
			fp->Read(&(bmpused.bmpinfo.bmiColors[0]),1*4);
			usedcolor=1;
		}
		needbyte=(needbyte+7)/8;
		break;
	case 4:
		if (bmpused.bmpinfo.bmiHeader.biClrUsed==0){
			fp->Read(&(bmpused.bmpinfo.bmiColors[0]),16*4);
			usedcolor=16;
		}
		else{
			fp->Read(&(bmpused.bmpinfo.bmiColors[0]),bmpused.bmpinfo.bmiHeader.biClrUsed*4);
			usedcolor=bmpused.bmpinfo.bmiHeader.biClrUsed;
		}
		needbyte=(needbyte+1)/2;
		break;
	case 8:
		if (bmpused.bmpinfo.bmiHeader.biClrUsed==0){
			num=fp->Read(&(bmpused.bmpinfo.bmiColors[0]),256*4);
			usedcolor=256;
		}
		else{
			num=fp->Read(&(bmpused.bmpinfo.bmiColors[0]),bmpused.bmpinfo.bmiHeader.biClrUsed*4);
			usedcolor=bmpused.bmpinfo.bmiHeader.biClrUsed;
		}
		break;
	case 24:
		needbyte*=3;
		needbyte=(width*3+3)/4*4*height;
		break;
	default: 
		AfxMessageBox("file error");
		exit(-1);
	}
	pixel=new BYTE[needbyte+1];
	num=fp->Read(pixel,needbyte);
	num=fp->GetPosition();
	int i,j,place;
	BYTE data;
	switch (bmpused.bmpinfo.bmiHeader.biBitCount)
	{
	case 1:
		point=new BYTE[needbyte*8];
		place=0;
		for (i=0;i<needbyte;i++){
			data=pixel[i];
			for (j=0;j<8;j++){
				if (data==data/2*2)
					point[place++]=0;
				else
					point[place++]=1;
				data/=2;
			}
		}
		usedbyte=needbyte*3;
		break;
	case 4:
		point=new BYTE[needbyte*2];
		place=0;
		for (i=0;i<needbyte;i++){
			data=pixel[i];
			point[place++]=data/16;
			point[place++]=data%16;
		}
		usedbyte=needbyte+needbyte;
		break;
	case 8:
		point=new BYTE[needbyte+1];
		for (i=0;i<needbyte;i++)
			point[i]=pixel[i];
		usedbyte=needbyte;
		break;
	case 24:
		point=new BYTE[needbyte];
		for (i=0;i<needbyte;i++)
			point[i]=pixel[i];
		usedbyte=needbyte;
		break;
	}

	return 0;
}

int KINGBMP::savepicture(CFile *fp)
{
	
	fp->SeekToBegin();
	fp->Write(&bmphead,sizeof(bmphead));
	fp->Write(&bmpused.bmpinfo.bmiHeader,sizeof(bmpused.bmpinfo.bmiHeader));	
	BYTE *writepixel;
	int i,j,place;
	
	switch (bitsperpixel)
	{
	case 1:
		if (bmpused.bmpinfo.bmiHeader.biClrUsed==0)
			fp->Write(&(bmpused.bmpinfo.bmiColors[0]),2*4);
		else
			fp->Write(&(bmpused.bmpinfo.bmiColors[0]),1*4);
		writepixel=new BYTE[needbyte];
		place=0;
		for (i=0;i<needbyte;i++)
			for (j=0;j<8;j++)
				writepixel[i]=writepixel[i]+writepixel[i]+point[place++];
		fp->Write(writepixel,needbyte);
		break;
	
	case 4:
		if (bmpused.bmpinfo.bmiHeader.biClrUsed==0)
			fp->Write(&(bmpused.bmpinfo.bmiColors[0]),16*4);
		else
			fp->Write(&(bmpused.bmpinfo.bmiColors[0]),bmpused.bmpinfo.bmiHeader.biClrUsed*4);
		place=0;
		for (i=0;i<needbyte;i++)
			writepixel[i]=point[place++]*16+point[place++];
		fp->Write(writepixel,needbyte);
		break;

	case 8:
		writepixel=new BYTE[10];
		if (bmpused.bmpinfo.bmiHeader.biClrUsed==0)
			fp->Write(&(bmpused.bmpinfo.bmiColors[0]),256*4);
		else
			fp->Write(&(bmpused.bmpinfo.bmiColors[0]),bmpused.bmpinfo.bmiHeader.biClrUsed*4);
		fp->Write(point,needbyte);
		break;

	case 24:
		writepixel=new BYTE[10];
		fp->Write(point,needbyte);
		break;
	
	}
	
	delete writepixel;
//	fp->Close();
	return 0;
}

int KINGBMP::draw(CDC *pdc)
{
	
	int ret=::SetDIBitsToDevice(pdc->m_hDC,0,0,width,height,0,0,0,height,point,&bmpused.bmpinfo,DIB_RGB_COLORS);
	return 0;
}