// KINGPCX.cpp: implementation of the KINGPCX class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "kingimage.h"
#include "KINGPCX.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

KINGPCX::KINGPCX()
{
	color=0;
}

KINGPCX::~KINGPCX()
{
	delete color;

}

int KINGPCX::getpicture(CFile *fp)
{
	int num;
	fp->SeekToBegin();
	num=fp->Read(&pcxhead,sizeof(pcxhead));
	width=pcxhead.xmax-pcxhead.xmin+1;
	height=pcxhead.ymax-pcxhead.ymin+1;
	usedbyte=width*height;
	bitsperpixel=pcxhead.bitperpixel;
	linebytes=pcxhead.bytes_per_line;
	usedcolor=1;
	usedcolor<<=bitsperpixel;
	bitsperpixel*=pcxhead.color_planes;
	if (bitsperpixel==24)	//to modify width so that is divide by 4
		offset=((pcxhead.xmax-pcxhead.xmin+1)*3+3)/4*4;
	else
		offset=((pcxhead.xmax-pcxhead.xmin+1)+3)/4*4;
	point=new BYTE[linebytes*height*pcxhead.color_planes+height];
//	pixel=new BYTE[2];
	num=fp->GetPosition();
	int length=fp->GetLength()-num;
	if (bitsperpixel==8)
		length-=256*3;
	pixel=new BYTE[length];
	fp->Read(pixel,length);
	decodepcx(pixel);
	num=fp->GetPosition();
	fp->Seek(-256*3,CFile::end);
	if (bitsperpixel==8){
		color=new GIFCOLOR[256];
		num=fp->Read(&color[0].red,256*3);
		num=fp->GetPosition();
	}
	else	//do that to prevent error when delete it
		color=new GIFCOLOR[2];
	initbmp();
	return 0;
}

int KINGPCX::savepicture(CFile *fp)
{
//	pcxhead.bytes_per_line=width;
//	pcxhead.bytes_per_line=linebytes;
	fp->Write(&pcxhead,sizeof(pcxhead));
	int bytetotal=0;
	BYTE *output=new BYTE[width*height*bitsperpixel/8*2];
	codepcx(point,output,bytetotal);
	fp->Write(output,bytetotal);
	if (bitsperpixel==8){
		BYTE temp=0x0c;
		fp->Write(&temp,1);
		fp->Write(&color[0].red,256*3);
	}
	return 0;
}

int KINGPCX::decodepcx(BYTE *fp)
{
	int place=0;
	if (bitsperpixel!=24)
	for (int i=height-1;i>=0;i--){	//for bmp cente is left-bottom
		int readnum=0;
		place=i*offset;
		while (readnum<linebytes){	//this must be linebytes not width or offset
			if (*fp>0xc0){
				BYTE num;
				num=*(fp++)-0xc0;
				for (BYTE j=0;j<num;j++){
					point[place++]=*fp;
					readnum++;
				}
			}
			else{
				point[place++]=*fp;
				readnum++;
			}
			fp++;
		}
		for (int j=readnum;j<offset;j++)
			point[place++]=0;
	}
	else
	for (int i=height-1;i>=0;i--)
		for (int k=2;k>=0;k--){		//sort the order of GBR
		int readnum=0;
		place=i*offset+k;
		while (readnum<linebytes){
			if (*fp>0xc0){
				BYTE num;
				num=*(fp++)-0xc0;
				for (BYTE j=0;j<num;j++){
					point[place]=*fp;
					readnum++;
					place+=3;
				}
			}
			else{
				point[place]=*fp;
				readnum++;
				place+=3;
			}
			fp++;
		}
		}
	return 0;
}

int KINGPCX::initbmp()
{
	
	bmpused.bmpinfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	bmpused.bmpinfo.bmiHeader.biWidth=pcxhead.xmax-pcxhead.xmin+1;
//	bmpused.bmpinfo.bmiHeader.biWidth=linebytes;
	bmpused.bmpinfo.bmiHeader.biHeight=height;
	bmpused.bmpinfo.bmiHeader.biPlanes=1;//pcxhead.color_planes;
	bmpused.bmpinfo.bmiHeader.biBitCount=bitsperpixel;
//	bmpinfo.bmiHeader.biPlanes=pcxhead.color_planes;
//	bmpinfo.bmiHeader.biBitCount=8;//bitsperpixel;
	bmpused.bmpinfo.bmiHeader.biCompression=0;
	bmpused.bmpinfo.bmiHeader.biSizeImage=0;
	bmpused.bmpinfo.bmiHeader.biXPelsPerMeter=0;
	bmpused.bmpinfo.bmiHeader.biYPelsPerMeter=0;
	bmpused.bmpinfo.bmiHeader.biClrUsed=0;
	bmpused.bmpinfo.bmiHeader.biClrImportant=0;
	if (pcxhead.color_planes==1){
		bmpused.bmpinfo.bmiColors[0].rgbBlue=color[0].blue;
		bmpused.bmpinfo.bmiColors[0].rgbGreen=color[0].green;
		bmpused.bmpinfo.bmiColors[0].rgbRed=color[0].red;
		bmpused.bmpinfo.bmiColors[0].rgbReserved=0;
		for (int i=0;i<usedcolor-1;i++){
			bmpused.color[i].rgbBlue=color[i+1].blue;
			bmpused.color[i].rgbGreen=color[i+1].green;
			bmpused.color[i].rgbRed=color[i+1].red;
			bmpused.color[i].rgbReserved=0;
		}
	}
	return 0;
}

int KINGPCX::draw(CDC *pdc)
{
	
	int ret=::SetDIBitsToDevice(pdc->m_hDC,0,0,pcxhead.xmax-pcxhead.xmin+1,height,0,0,0,height,point,&bmpused.bmpinfo,DIB_RGB_COLORS);
	return 0;
}

int KINGPCX::codepcx(BYTE * ffp,BYTE *output,int &totalbyte)
{
	int place=0;
	BYTE *fp;
	fp=ffp;
	if (bitsperpixel==24){
		for (int i=height-1;i>=0;i--){
			for (int k=2;k>=0;k--){
				fp=ffp+i*offset+k;
				int num=0;
				int count=1;
				while (num<linebytes){
					if (*fp == *(fp+3) ){
						count ++;
						if (count == 0x3f){
							if (num!=linebytes-1){
								*(output+totalbyte++)=0xff;
								*(output+totalbyte++)=*fp;
								num++;//here add for next is not +3 but +6;
								fp+=3;
							}
							else{
								*(output+totalbyte++)=0xfe;
								*(output+totalbyte++)=*fp;
								num++;
							}
							count=1;
						}
					}
					else{
						if (count != 1){
							*(output+totalbyte++)=0xc0+count;
							*(output+totalbyte++)=*fp;
						}
						else
							if ( *fp >= 0xc0 ){
								*(output+totalbyte++)=0xc1;
								*(output+totalbyte++)=*fp;
							}
							else
								*(output+totalbyte++)=*fp;
						count=1;
					}
					fp+=3;
					num++;
				}
				if (count != 1){
					if (count !=2 ){
						*(output+totalbyte++)=0xc0+count-1;
						*(output+totalbyte++)=*fp;
					}
					else
						if (*fp<0xc0)
							*(output+totalbyte++)=*fp;
						else{
							*(output+totalbyte++)=0xc1;
							*(output+totalbyte++)=*fp;
						}
				}
			}
		}
	}
	else{
		for (int i=height-1;i>=0;i--){
				fp=ffp+i*offset;
				int num=0;
				int count=1;
				while (num<linebytes){
					if (*fp == *(fp+1) ){
						count ++;
						if (count == 0x3f){
							if (num!=linebytes-1){
								*(output+totalbyte++)=0xff;
								*(output+totalbyte++)=*fp;
								num++;
								fp++;
							}
							else{
								*(output+totalbyte++)=0xfe;
								*(output+totalbyte++)=*fp;
								num++;
							}
							count=1;
						}
					}
					else{
						if (count!=1){
							*(output+totalbyte++)=count+0xc0;
							*(output+totalbyte++)=*fp;
						}
						else
							if ( *fp >=0xc0 ){
								*(output+totalbyte++)=count+0xc0;
								*(output+totalbyte++)=*fp;
							}
							else
								*(output+totalbyte++)=*fp;
						count=1;
					}
					fp++;
					num++;
				}
				if (count != 1){
					if ( count ==2 )
						if (*(fp-1)>=0xc0){
							*(output+totalbyte++)=0xc0+count-1;
							*(output+totalbyte++)=*(fp-1);
						}
						else
							*(output+totalbyte++)=*(fp-1);
					else{
						*(output+totalbyte++)=0xc0+count-1;
						*(output+totalbyte++)=*(fp-1);
					}
				}
		}
	}
	return 0;
}
