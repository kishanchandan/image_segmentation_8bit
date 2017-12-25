// KINGGIF.cpp: implementation of the KINGGIF class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "kingimage.h"
#include "KINGGIF.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

KINGGIF::KINGGIF()
{
	existglobe=0;
	existlocal=0;
	usedcolor=0;
	index=0;
	leftbits=8;
	place=0;
	usedmask[0]=1;
	for (int i=1;i<8;i++)
		usedmask[i]=(usedmask[i-1]<<1)+1;
}

KINGGIF::~KINGGIF()
{
	delete globe_gifcolor;
	delete local_gifcolor;
}

int KINGGIF::getpicture(CFile *fp)
{
	int num;
	fp->SeekToBegin();
	num=fp->Read(&gifhead,GIFHEADLENGTH);
	if ( (gifhead.copyright[0]!='G') || (gifhead.copyright[1]!='I') || (gifhead.copyright[2]!='F') )
		return -1;
	bitsperpixel=(gifhead.globe_flag&0x07)+1;
	if (gifhead.globe_flag&0x80){
		existglobe=1;
		usedcolor=1;
		usedcolor<<=bitsperpixel;
		_init=usedcolor;
		_end=usedcolor+1;
		globe_gifcolor=new GIFCOLOR[usedcolor];
		fp->Seek(GIFHEADLENGTH,CFile::begin);
		num=fp->Read(&globe_gifcolor[0].red,sizeof(GIFCOLOR)*usedcolor);
		num=fp-> GetPosition();
	}
	else
		globe_gifcolor=new GIFCOLOR[2];

	int offset=fp->Seek(GIFHEADLENGTH+usedcolor*sizeof(GIFCOLOR),CFile::begin);
	BYTE next;
	fp->Read(&next,1);
	num=fp-> GetPosition();
	int end=0;
	int ret;
	while (end==0)
		switch	(next)
	{
		case ';':
			end=1;break;
		case ',':
			fp->Read(&gifdatahead,GIFLOCALHEADLENGTH);
			offset+=GIFLOCALHEADLENGTH;
			width=(gifdatahead.width+3)/4*4;
			height=gifdatahead.height;
			usedbyte=width*height;
			needbyte=width*height;
			pixel=new BYTE[needbyte];
			point=new BYTE[usedbyte];
			if (gifdatahead.local_flag&0x80){
				existlocal=1;
				usedcolor=1;
				usedcolor<<=bitsperpixel;
				_init=usedcolor;
				_end=usedcolor+1;
				local_gifcolor=new GIFCOLOR[usedcolor];
				fp->Seek(offset,CFile::begin);
				num=fp->Read(&local_gifcolor[0].red,sizeof(GIFCOLOR)*usedcolor);
				offset=offset+sizeof(GIFCOLOR)*usedcolor;
			}
			else
				local_gifcolor=new GIFCOLOR[2];
			num=fp->GetLength()-fp->GetPosition()-1;
			fp->Seek(offset,CFile::begin);
			fp->Read(pixel,num);
			temp=num;
			ret=decodelzw(pixel);
			if (ret<0)
				return -1;
			num=fp->Read(&next,1);
			if (num==0)
				return -1;
			while (next==0){
				num=fp->Read(&next,1);
				if (num==0)
					return -1;
			}
			end=1;
			break;
		case '!':
			break;
		default: 
			exit(-2);
	}
	return 0;
}

int KINGGIF::savepicture(CFile *fp)
{
	fp->SeekToBegin();
	fp->Write(&gifhead,GIFHEADLENGTH);
	if (existglobe==1){
		fp->Write(&globe_gifcolor[0].red,sizeof(GIFCOLOR)*usedcolor);
	}
//	fp->Seek(GIFHEADLENGTH,CFile::begin);
	BYTE next=',';
	fp->Write(&next,1);
//	fp->Seek(GIFHEADLENGTH+1,CFile::begin);
	fp->Write(&gifdatahead,GIFLOCALHEADLENGTH-1);
//	fp->Seek(GIFHEADLENGTH+GIFLOCALHEADLENGTH,CFile::begin);
	if (existlocal==1)
		fp->Write(&local_gifcolor[0].red,sizeof(GIFCOLOR)*usedcolor);
	BYTE *code=new BYTE[usedbyte*2];
	int reallen=0;
//	fp->Seek(GIFHEADLENGTH+GIFLOCALHEADLENGTH+sizeof(GIFCOLOR)*usedcolor+1,CFile::begin);
	codelzw(code,&reallen);
	fp->Write(code,reallen);
	next=';';
	fp->Write(&next,1);
	delete code;
	return 0;
}

int KINGGIF::decodelzw(BYTE *fp)
{
//	temp=num;
	int place=0;
	BYTE *nowpoint;
	nowpoint=point;
	inittable();
	BYTE *now;
	now=fp;
	readbits=*(now++)+1;
	int pre=-1;
	int end=0;
	while (end==0){
	int next=getcode(&now);
	if (next>top)
		return -1;
	if (next==_init){
		pre=-1;
		inittable();
	}
	else
		if (next==_end)
			end=1;
		else{
			if (next<top){
				if (pre!=-1) 
					inserttable(pre,next);
			}
			else
				inserttable(pre,pre);
		pre=next;
//		if (usedcolor!=256)
			for (int i1=0;i1<table[next].GetSize();i1++){
				*(nowpoint++)=(BYTE)table[next].GetAt(i1);
				place++;
				if (place==gifdatahead.width)
					for (int ii=gifdatahead.width;ii<width;ii++)
						*(nowpoint++)=place=0;
			}
		}
	}
//	}
	initbmp();
	return 0;
}

void KINGGIF::inittable()
{
	nowmax=usedcolor<<1;
	readbits=bitsperpixel+1;
	int i;
	for (i=0;i<usedcolor;i++){
		table[i].RemoveAll();
		table[i].Add(i);
	}
	for (i=usedcolor;i<4096;i++)
		table[i].RemoveAll();
	top=usedcolor+2;
	for (i=0;i<256;i++)
		quick[i].RemoveAll();
/*	for (i=0;i<256;i++)
		for (int j=0;j<256;j++)
			quick2[i][j].RemoveAll();
*/
}





int KINGGIF::inserttable(int pre, int next)
{
		table[top].RemoveAll();
		table[top].Copy(table[pre]);
		table[top].Add(table[next].GetAt(0));
	top++;
	if (top==nowmax){
		readbits++;
		nowmax<<=1;
		int whythis=readbits;
		if (whythis==13)
			readbits--;
	}
	return 0;
}

int KINGGIF::getcode(BYTE **now)
{
	int needbits=readbits;
	int total=0;
	int power=0;
	preleftbits=leftbits;
	while (index==0)	
		index=*((*now)++);
	while (needbits>leftbits){//
		int thisterm=0;
		thisterm=(**now)>>(8-leftbits);
		total+=(thisterm<<power);
		power+=leftbits;
		needbits-=leftbits;
		(*now)++;
		index--;
		while (index==0)
			index=*((*now)++);
		leftbits=8;
	}
	int thisterm=0;
//	int mask=0xff;
//	mask>>=(leftbits-needbits);
	thisterm=(**now)&usedmask[7-leftbits+needbits];
	total=total+(thisterm<<power);
	leftbits=leftbits-needbits;
	if (leftbits==0){
		leftbits=8;
		(*now)++;
		index--;
		while (index==0)
			index=*((*now)++);
	}
	return total;
}

int KINGGIF::initbmp()
{
	bmpused.bmpinfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
//	bmpused.bmpinfo.bmiHeader.biWidth=(width+3)/4*4;
	bmpused.bmpinfo.bmiHeader.biWidth=width;
	bmpused.bmpinfo.bmiHeader.biHeight=-height;
	bmpused.bmpinfo.bmiHeader.biPlanes=1;
	bmpused.bmpinfo.bmiHeader.biBitCount=8;//bitsperpixel;
	bmpused.bmpinfo.bmiHeader.biCompression=0;
	bmpused.bmpinfo.bmiHeader.biSizeImage=0;
	bmpused.bmpinfo.bmiHeader.biXPelsPerMeter=0;
	bmpused.bmpinfo.bmiHeader.biYPelsPerMeter=0;
	bmpused.bmpinfo.bmiHeader.biClrUsed=usedcolor;
	bmpused.bmpinfo.bmiHeader.biClrImportant=0;
	if (existlocal==1){
	bmpused.bmpinfo.bmiColors[0].rgbBlue=local_gifcolor[0].blue;
	bmpused.bmpinfo.bmiColors[0].rgbGreen=local_gifcolor[0].green;
	bmpused.bmpinfo.bmiColors[0].rgbRed=local_gifcolor[0].red;
	bmpused.bmpinfo.bmiColors[0].rgbReserved=0;
	for (int i=0;i<usedcolor-1;i++){
		bmpused.color[i].rgbBlue=local_gifcolor[i+1].blue;
		bmpused.color[i].rgbGreen=local_gifcolor[i+1].green;
		bmpused.color[i].rgbRed=local_gifcolor[i+1].red;
		bmpused.color[i].rgbReserved=0;
	}
	}
	else{
		bmpused.bmpinfo.bmiColors[0].rgbBlue=globe_gifcolor[0].blue;
		bmpused.bmpinfo.bmiColors[0].rgbGreen=globe_gifcolor[0].green;
		bmpused.bmpinfo.bmiColors[0].rgbRed=globe_gifcolor[0].red;
		bmpused.bmpinfo.bmiColors[0].rgbReserved=0;
		for (int i=0;i<usedcolor-1;i++){
			bmpused.color[i].rgbBlue=globe_gifcolor[i+1].blue;
			bmpused.color[i].rgbGreen=globe_gifcolor[i+1].green;
			bmpused.color[i].rgbRed=globe_gifcolor[i+1].red;
			bmpused.color[i].rgbReserved=0;
		}
	}
	

	return 0;
}

int KINGGIF::draw(CDC *pdc)
{
//	int ret=::SetDIBitsToDevice(pdc->m_hDC,0,0,bmpused.bmpinfo.bmiHeader.biWidth,height,0,0,0,height,point,&bmpused.bmpinfo,DIB_RGB_COLORS);
	int ret=::SetDIBitsToDevice(pdc->m_hDC,0,0,width,height,0,0,0,height,point,&bmpused.bmpinfo,DIB_RGB_COLORS);
	return 0;
}


int KINGGIF::codelzw(BYTE * fp,int *length)
{
	BYTE *now;
	BYTE *needcode;
	needcode=point;
//	DWORD t1,t0,t2,t3,t4,t5,t6,t7,t8,t9;
//	t0=GetTickCount();
	CWordArray pre;
	CWordArray next;
	int left=usedbyte;
	int place=0;
//	int delay1=-1;
//	int delay2=-1;
	now=fp;
//	BYTE skip
	leftbits=8;
	readbits=8;
//	*now=bitsperpixel;
	writecode(&now,length,bitsperpixel);
//	*now=skip;
	skip=0xfe;
	writecode(&now,length,skip);
	skip=0xfe;
//	*now=_init;
	inittable();
	writecode(&now,length,_init);
	int last=-1;
	int here=-1;
	findstart=usedcolor+2;
//	t1= GetTickCount()-t0;
	while (usedbyte>0){
		if (place>=gifdatahead.width)
			for (int jj=gifdatahead.width;jj<width;jj++){
				needcode++;
				place=0;
				usedbyte--;
			}
		next.RemoveAll();
		next.Add(*(needcode++));
		place++;
		usedbyte--;
		if (last!=-1){
			aim.RemoveAll();
			aim.Copy(pre);
			aim.Append(next);
		}
		else{
			aim.RemoveAll();
			aim.Copy(next);
		}
		here=find();
		if ( here>=0 ){
			pre.RemoveAll();
			pre.Copy(aim);
			last=here;
			findstart=here+1;
		}
		else{
			findstart=usedcolor+2;
			if (top==0x1000){
				writecode(&now,length,last);
				writecode(&now,length,_init);	
				needcode--;
				place--;
				usedbyte++;
				inittable();
				last=-1;
				here=-1;
				pre.RemoveAll();
			}
			else{
			writecode(&now,length,last);
			inserttable2( last , *(needcode-1) );
			last=*(needcode-1);
			pre.RemoveAll();
			pre.Copy(next);
			}
		}
	}
	writecode(&now,length,last);
	writecode(&now,length,_end);
	if (skip!=0)
		*(now-0xfe-1+skip)=0xfe-skip;
	*(now++)=0;
	(*length)++;
	return 0;
}

int KINGGIF::writecode(BYTE * * fp, int * length,int place)
{
	static int remain=0;
	int needbits=readbits;
	while (leftbits<needbits){
		BYTE temp=place&usedmask[leftbits-1];
		place>>=leftbits;
		temp<<=(8-leftbits);
		remain+=temp;
		*((*fp)++)=remain;
		skip--;
		(*length)++;
		if (skip==0){
			skip=0xfe;
			*((*fp)++)=skip;
			(*length)++;
		}
		remain=0;
		needbits-=leftbits;
		leftbits=8;
	}
//	place&=mask[needbits-1];
	place<<=(8-leftbits);
	remain+=place;
	leftbits=8-needbits;
	if (leftbits==0){
		*((*fp)++)=remain;
		skip--;
		(*length)++;
		if (skip==0){
			skip=0xfe;
			*((*fp)++)=skip;
			(*length)++;
		}
		leftbits=8;
		remain=0;
	}
	return 0;
}

int KINGGIF::find()
{
	if (aim.GetSize()==1)
		return aim.GetAt(0);
	int length1;
	length1=aim.GetSize();
	short int first;
//	short int second;
	first=aim.GetAt(0);
/*	if (length1>2){
		second=aim.GetAt(1);
		for (int i=0;i<quick2[first][second].GetSize();i++){
			short int tab;
			tab=quick2[first][second].GetAt(i);
			if (table[tab].GetSize()==length1){
				int j=2;
				while (j<length1){
					if ( table[tab].GetAt(j)!=aim.GetAt(j)) j=length1+1;
					j++;
				}
				if (j==length1)
					return tab;
			}
		}
	}
	else
*/		for (int i=0;i<quick[first].GetSize();i++){
			short int tab;
			tab=quick[first].GetAt(i);
			if (table[tab].GetSize()==length1){
				int j=1;
				while (j<length1){
					if ( table[tab].GetAt(j)!=aim.GetAt(j)) j=length1+1;
					j++;
				}
				if (j==length1)
					return tab;
			}
		}

/*  	for (int i=findstart;i<top;i++)
		if (table[i].GetSize()==length1){
			int j=0;
			while (j<length1){
				if ( table[i].GetAt(j)!=aim.GetAt(j)) j=length1+1;
				j++;
			}
			if (j==length1)
				return i;
//			for (j=0;j<length1;j++)
//				if ( table[i].GetAt(j)!=aim.GetAt(j))	break;
//			return i;
		}
*/
	
	return -1;
}

int KINGGIF::inserttable2(int pre, int next)
{
//	if (usedcolor==256)
//		table[top]=table[pre]+table[next].GetAt(0)+table[next].GetAt(1);
//	else

	table[top].RemoveAll();
	table[top].Copy(table[pre]);
	table[top].Add(table[next].GetAt(0));
//	short int first;
//	first=table[top].GetAt(0);
//	quick[first].Add(top);
//	if (table[top].GetSize()>2)
//		quick2[first][table[top].GetAt(1)].Add(top);
	top++;
	if (top==nowmax+1){
/*		if (readbits==0x09)
			t1=GetTickCount();
		else{
			t2=GetTickCount()-t1;
			t1=GetTickCount();
		}
*/		readbits++;
		nowmax<<=1;
		int whythis=readbits;
		if (whythis==13)
			readbits--;
	}
	return 0;
}
