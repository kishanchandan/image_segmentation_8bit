// KINGJPG.cpp: implementation of the KINGJPG class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "kingimage.h"
#include "KINGJPG.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


BYTE ZIGZAG[64]={
	0 ,1 ,5 ,6 ,14,15,27,28,
	2 ,4 ,7 ,13,16,26,29,42,
	3 ,8 ,12,17,25,30,41,43,
	9 ,11,18,24,31,40,44,53,
	10,19,23,32,39,45,52,54,
	20,22,33,38,46,51,56,60,
	21,34,37,47,50,56,59,61,
	35,36,48,49,57,58,62,63
};

BYTE ANTIZIGZAG[64]={
	0 ,1 ,8 ,16,9 ,2 ,3 ,10,
	17,24,32,25,18,11,4 ,5 ,
	12,19,26,33,40,48,41,34,
	27,20,13,6 ,7 ,14,21,28,
	35,42,49,56,57,50,43,36,
	29,22,15,23,30,37,44,51,
	58,59,52,45,38,31,39,46,
	53,60,61,54,47,55,62,63
};

extern int r_cr[256];
extern int g_cb[256];
extern int g_cr[256];
extern int b_cb[256];
extern int y_r[256];
extern int y_g[256];
extern int y_b[256];
extern int cb_r[256];
extern int cb_g[256];
extern int cb_b[256];
extern int cr_g[256];
extern int cr_b[256];

BYTE y_ac[19]={0,0xb5,0x10,0,2,1,3,3,2,4,3,5,5,4,4,0,0,1,0x7d};
BYTE cbcr_ac[19]={0,0xb5,0x11,0,2,1,2,4,4,3,4,7,5,4,4,0,1,2,0x77};

BYTE y_dc[19]={0,0x1f,0x00,0,1,5,1,1,1,1,1,1,0,0,0,0,0,0,0};
BYTE cbcr_dc[19]={0,0x1f,0x01,0,0,3,1,1,1,1,1,1,1,1,1,0,0,0,0};

BYTE y_dc_table[]={0,1,2,3,4,5,6,7,8,9,10,11};
BYTE cbcr_dc_table[]={0,1,2,3,4,5,6,7,8,9,10,11};

BYTE y_ac_table[]={0x01,0x02,0x03,0x00,0x04,0x11,0x05,0x12,
				   0x21,0x31,0x41,0x06,0x13,0x51,0x61,0x07,
				   0x22,0x71,0x14,0x32,0x81,0x91,0xa1,0x08,
				   0x23,0x42,0xb1,0xc1,0x15,0x52,0xd1,0xf0,
				   0x24,0x33,0x62,0x72,0x82,0x09,0x0a,0x16,
				   0x17,0x18,0x19,0x1a,0x25,0x26,0x27,0x28,
				   0x29,0x2a,0x34,0x35,0x36,0x37,0x38,0x39,
				   0x3a,0x43,0x44,0x45,0x46,0x47,0x48,0x49,
				   0x4a,0x53,0x54,0x55,0x56,0x57,0x58,0x59,
				   0x5a,0x63,0x64,0x65,0x66,0x67,0x68,0x69,
				   0x6a,0x73,0x74,0x75,0x76,0x77,0x78,0x79,
				   0x7a,0x83,0x84,0x85,0x86,0x87,0x88,0x89,
				   0x8a,0x92,0x93,0x94,0x95,0x96,0x97,0x98,
				   0x99,0x9a,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,
				   0xa8,0xa9,0xaa,0xb2,0xb3,0xb4,0xb5,0xb6,
				   0xb7,0xb8,0xb9,0xba,0xc2,0xc3,0xc4,0xc5,
				   0xc6,0xc7,0xc8,0xc9,0xca,0xd2,0xd3,0xd4,
				   0xd5,0xd6,0xd7,0xd8,0xd9,0xda,0xe1,0xe2,
				   0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,0xea,
				   0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa};

BYTE cbcr_ac_table[]={0x00,0x01,0x02,0x03,0x11,0x04,0x05,0x21,
				      0x31,0x06,0x12,0x41,0x51,0x07,0x61,0x71,
				      0x13,0x22,0x32,0x81,0x08,0x14,0x42,0x91,
				      0xa1,0xb1,0xc1,0x09,0x23,0x33,0x52,0xf0,
				      0x15,0x62,0x72,0xd1,0x0a,0x16,0x24,0x34,
				      0xe1,0x25,0xf1,0x17,0x18,0x19,0x1a,0x26,
				      0x27,0x28,0x29,0x2a,0x35,0x36,0x37,0x38,
				      0x39,0x3a,0x43,0x44,0x45,0x46,0x47,0x48,
				      0x49,0x4a,0x53,0x54,0x55,0x56,0x57,0x58,
				      0x59,0x5a,0x63,0x64,0x65,0x66,0x67,0x68,
				      0x69,0x6a,0x73,0x74,0x75,0x76,0x77,0x78,
				      0x79,0x7a,0x82,0x83,0x84,0x85,0x86,0x87,
				      0x88,0x89,0x8a,0x92,0x93,0x94,0x95,0x96,
				      0x97,0x98,0x99,0x9a,0xa2,0xa3,0xa4,0xa5,
				      0xa6,0xa7,0xa8,0xa9,0xaa,0xb2,0xb3,0xb4,
				      0xb5,0xb6,0xb7,0xb8,0xb9,0xba,0xc2,0xc3,
				      0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xd2,
				      0xd3,0xd4,0xd5,0xd6,0xd7,0xd8,0xd9,0xda,
				      0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,
				      0xea,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa};
//int exp2[12]={1,2,4,8,16,32,64,128,256,512,1024,2048};

extern int jpeg_IDCT(short int *p,short int *q);
extern int jpeg_DCT(short int *p,short int *q);
extern void copy_byte(BYTE *to,BYTE *from,int total);
extern int scale_pic(short int *p,int,int,short int *,int,int,BYTE);
extern int scale_pic2(short int *p,int,int,short int *,int,int,BYTE);
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

KINGJPG::KINGJPG()
{
	quantize_num=0;
	dc_number=0;
	ac_number=0;
	ac_huff_type=0;
	used_dc_table=0;
	used_ac_table=0;
	for (int i=0;i<4;i++){
		huff_dc_table[i]=0;
		huff_ac_table[i]=0;
	}
}

KINGJPG::~KINGJPG()
{
	delete used_dc_table;
	delete used_ac_table;
	delete display;
	for (int i=0;i<4;i++){
		delete huff_dc_table[i];
		delete huff_ac_table[i];
	}
}

int KINGJPG::getpicture(CFile *fp)
{
	unsigned long filelength;
	filelength=fp->GetLength();
	pixel=new BYTE[filelength+1];
	if (pixel==0)
		return -3;
	unsigned long readlength;
	int ret=0;
	readlength=fp->Read(pixel,filelength);

	BOOL atend;
	atend=FALSE;
	place=2;
	BYTE before,now;
	now=0;
	if ( ( *pixel != 0xff )&&( *(pixel+1) != 0xd8 ) ) 
		return -2;
	while (!atend)
	{
		before=now;
		now=*(pixel+place++);
		int i;
		int step;
		BYTE csnow;
		int length;
		int total;
		if (before==0xff)
			switch (now){

			case 0xe0:	//APP UNIT
				place=place+*(pixel+place+1);	
				before=0;
				break;

			case 0xfe:	//COM UNIT
				place=place+*(pixel+place+1);	
				before=0;
				break;

			case 0xdb:	//DQT UNIT
				place++;
				length=*(pixel+place++);
				if ( length==0x84 )	//two quantize is at same place
				{
					copy_byte(&quantize[0][0],pixel+place+1,0x40);
					copy_byte(&quantize[1][0],pixel+place+0x42,0x40);
					place=place+0x82;
				}
				else
				{
					copy_byte(&quantize[quantize_num][0],pixel+place+length-66,64);
					place+=length-2;
					quantize_num++;
				}
				before=0;
				break;

			case 0xc0:	//SOF0 LABEL
				place+=3;
				height=*(pixel+place)*256+*(pixel+place+1);
				width=*(pixel+place+2)*256+*(pixel+place+3);
				xuse_length=(width-1)/16*16+16;
				yuse_length=(height-1)/16*16+16;
				point=new BYTE[yuse_length*xuse_length*3];
				place+=5;
				for (i=0;i<3;i++){
					scale[i]=*(pixel+place+1+3*i);
					usedquan[i]=*(pixel+place+2+3*i);
				}
				place+=9;
				before=0;
				break;

			case 0xc4:	//DHT UNIT
				length=*(pixel+place)*256+*(pixel+place+1);
				step=2;
				while (step<length)
				{
					if (*(pixel+place+step)/16==0)	//DC TABLE
					{
						if (dc_number==0)
							_start[0]=place;
						total=0;
						for (i=0;i<16;i++){
							huff_dc_start_table[dc_number][i]=total;
							total+=*(pixel+place+step+1+i);
						}
						max_dc_ac_length[dc_number][0]=0;
						start_dc_ac_length[dc_number][0]=0;
						int number;
						int start=0;
						int scale=2;
						int last=0;
						if (huff_dc_start_table[dc_number][1]!=0)
							start=2;
						for (i=1;i<15;i++)
						{
							start_dc_ac_length[dc_number][i]=start;
							number=huff_dc_start_table[dc_number][i+1]-huff_dc_start_table[dc_number][i];
							max_dc_ac_length[dc_number][i]=start_dc_ac_length[dc_number][i]+number-1;
							if (number-1<0)
							{
								max_dc_ac_length[dc_number][i]=0;
								start_dc_ac_length[dc_number][i]=0;
							}
							if (number==0)
								scale*=2;
							else
							{
								scale=2;
								last=i;
							}
							if (last!=0)
								start=(max_dc_ac_length[dc_number][last]+1)*scale;
							else
								start=0;
						}
						i=0;
						while (huff_dc_start_table[dc_number][i]==0)
							i++;
						first_dc[dc_number]=i;
						start_dc_ac_length[dc_number][15]=start;
						max_dc_ac_length[dc_number][15]=start+number-1;
						huff_dc_table[dc_number]=new BYTE[total];
						copy_byte(&huff_dc_table[dc_number][0],pixel+place+step+17,total);
						huff_dc_table_num[dc_number]=total;
						dc_number++;
						step=step+17+total;
					}
					else	//AC TABLE
					{
						total=0;
						for (i=0;i<16;i++){
							huff_ac_start_table[ac_number][i]=total;
							total+=*(pixel+place+step+1+i);
						}
						max_dc_ac_length[ac_number+4][0]=0;
						start_dc_ac_length[ac_number+4][0]=0;
						int number;
						int start=0;
						int scale=2;
						int last=0;
						if (huff_ac_start_table[ac_number][1]!=0)
							start=2;
						for (i=1;i<15;i++)
						{
							start_dc_ac_length[ac_number+4][i]=start;
							number=huff_ac_start_table[ac_number][i+1]-huff_ac_start_table[ac_number][i];
							max_dc_ac_length[ac_number+4][i]=start_dc_ac_length[ac_number+4][i]+number-1;
							if (number-1<0)
							{
								max_dc_ac_length[ac_number+4][i]=0;
								start_dc_ac_length[ac_number+4][i]=0;
							}
							if (number==0)
								scale*=2;
							else
							{
								scale=2;
								last=i;
							}
							if (last!=0)
								start=(max_dc_ac_length[ac_number+4][last]+1)*scale;
							else
								start=0;
						}
						i=0;
						while (huff_ac_start_table[ac_number][i]==0)
							i++;
						first_ac[ac_number]=i;
						start_dc_ac_length[ac_number+4][15]=start;
						number=total-huff_ac_start_table[ac_number][15];
						max_dc_ac_length[ac_number+4][15]=65535;
						huff_ac_table[ac_number]=new BYTE[total];
						copy_byte(&huff_ac_table[ac_number][0],pixel+place+step+17,total);
						huff_ac_table_num[ac_number]=total;
						ac_number++;
						step=step+17+total;
						if (ac_number==2)
							_end[1]=place+step;
					}
				}
				place=place+length;
				now=0;
				break;

			case 0xda:	//SOS UNIT
				length=*(pixel+place+1);
				csnumber=(length-6)/2;
				used_dc_table=new BYTE[csnumber];
				used_ac_table=new BYTE[csnumber];
				for (csnow=0;csnow<csnumber;csnow++){
					used_dc_table[csnow]=pixel[place+3+1+csnow*2]/16;
					used_ac_table[csnow]=pixel[place+3+1+csnow*2]%16;
				}
				place+=length;
				before=0;
				headlength=place;
				ret=get_pic();
				if (ret<0)
					return -1;
				break;
			case 0xd9:atend=TRUE;
				break;

			case 0:	
				break;

			default:
				return -1;
			}
	}
	return 1;
}

int	KINGJPG::get_pic()
{
	int x_size;
	int y_size;
	x_size=xuse_length/8/(scale[0]/16);
	y_size=yuse_length/8/(scale[0]%16);
	int xnow,ynow;
	BYTE rotate_time;
	rotate_time=0;
	dc_before=0;
	dc_y_before=0;
	dc_cb_before=0;
	dc_cr_before=0;
	int looptime=(scale[0]/16)*(scale[0]%16)+2;
	short int *after_anti_entropy;
	short int *after_anti_dct;
	short int *after_anti_quantizer;
	after_anti_entropy=new short int[looptime*64];
	after_anti_dct=new short int[looptime*64];
	after_anti_quantizer=new short int[looptime*64];
	now=*(pixel+place);
	DWORD d1,d2,d3,d4;
	int ret;
	for (ynow=0;ynow<y_size;ynow++){
		for (xnow=0;xnow<x_size;xnow++){
		ret=anti_entropy(&place,&rotate_time,after_anti_entropy,looptime);
		d2=GetTickCount()-d1;
		if (ret<0)
			return -1;
		anti_quantizer(after_anti_quantizer,after_anti_entropy);
//		d1=GetTickCount();
//		for (int test=0;test<100;test++)
		for (int i=0;i<looptime;i++)
			jpeg_IDCT(after_anti_quantizer+i*64,after_anti_dct+i*64);
//		d3=GetTickCount();
//		for (int test=0;test<100;test++)
		anti_color(xnow,ynow,after_anti_dct);
//		d4=GetTickCount()-d3;
	}
	}
	delete after_anti_entropy;
	delete after_anti_dct;
	delete after_anti_quantizer;
	initbmp();
	return 1;
}


int	KINGJPG::anti_entropy(int *place,BYTE *rotate_time,short int *after_anti_entropy,int looptime)
{

	int i,j;
	int orignal;
	BYTE r,v_length;
	int  v;
	BYTE tablenum;
	BYTE length;
	BYTE whyking;
	for (i=0;i<looptime;i++){
		tablenum=*(used_dc_table+0);
		if (i==looptime-1)
			tablenum=*(used_dc_table+2);
		if (i==looptime-2)
			tablenum=*(used_dc_table+1);
		whyking=first_dc[tablenum];
		j=0;
		int king;
		while (j<64){
		length=1;
		orignal=0;
		l_rotate(&orignal,&now,rotate_time,place);
		while ((orignal>max_dc_ac_length[tablenum][length-1]) || ( (length<whyking)&&(orignal==0) ) )
		{
		l_rotate(&orignal,&now,rotate_time,place);
		length++;
		if (length>17)
			return -1;
		}		//end while
		
		
		
		if (j==0)	//is dc
		{

		dc_before=dc_y_before;
		if (i==looptime-1)
			dc_before=dc_cr_before;
		if (i==looptime-2)
			dc_before=dc_cb_before;

		int dc_place;
		king=length;
		dc_place=huff_dc_start_table[tablenum][length-1]+orignal-start_dc_ac_length[tablenum][length-1];
		length=huff_dc_table[tablenum][dc_place];
		orignal=0;
		for (r=0;r<length;r++)
			l_rotate(&orignal,&now,rotate_time,place);
		int temp=1;
		for (r=0;r<length-1;r++)
			temp*=2;
		if ( orignal >= temp )	//diffdc > 0
			*(after_anti_entropy+i*64+j)=dc_before+orignal;
		else
			*(after_anti_entropy+i*64+j)=dc_before-(temp*2-orignal-1);

		if ((orignal==0) && (length==0) )
			*(after_anti_entropy+i*64+j)=dc_before;
		
		if (i==looptime-1)
			dc_cr_before=*(after_anti_entropy+i*64+j);
		else
			if (i==looptime-2)
				dc_cb_before=*(after_anti_entropy+i*64+j);
			else
				dc_y_before=*(after_anti_entropy+i*64+j);

		tablenum=*(used_ac_table+0)+4;
		if (i==looptime-1)
			tablenum=*(used_ac_table+2)+4;
		if (i==looptime-2)
			tablenum=*(used_ac_table+1)+4;
		j++;
		whyking=first_ac[tablenum-4];
		}
		else	//is ac
		{
		int ac_place;
		ac_place=huff_ac_start_table[tablenum-4][length-1]+orignal-start_dc_ac_length[tablenum][length-1];
		length=huff_ac_table[tablenum-4][ac_place];
		if (length==0)
			while (j<64)	*(after_anti_entropy+i*64+j++)=0;
		else	{
		r=length/16;
		v_length=length%16;
		v=0;
		int jj;
		for (jj=0;jj<v_length;jj++)
			l_rotate(&v,&now,rotate_time,place);
		int temp=1;
		for (jj=0;jj<v_length-1;jj++)
			temp*=2;
		if (v_length==0)
			temp=0;
		for (jj=0;jj<r;jj++)
			*(after_anti_entropy+i*64+j++)=0;
		if ( v >= temp )	//diffac <0
			*(after_anti_entropy+i*64+j++)=v;
		else
			*(after_anti_entropy+i*64+j++)=-(temp*2-v-1);

		}
		}
		}	//end for 0 to 64
	}
	return 1;
}

int KINGJPG::anti_quantizer(short int *after_anti_quantizer,short int *after_anti_entropy)
{
	int looptime;
	looptime=(scale[0]/16)*(scale[0]%16)+2;
	int ii;
	short int temp[64];
	for (int tablenum=0;tablenum<looptime-2;tablenum++)	//first
		for (ii=0;ii<64;ii++)
		*(after_anti_quantizer+tablenum*64+ANTIZIGZAG[ii])=*(after_anti_entropy+tablenum*64+ii)*quantize[usedquan[0]][ii];
	for (ii=0;ii<64;ii++)		//second
		*(after_anti_quantizer+(looptime-2)*64+ANTIZIGZAG[ii])=*(after_anti_entropy+(looptime-2)*64+ii)*quantize[usedquan[1]][ii];
	for (ii=0;ii<64;ii++)		//third
		*(after_anti_quantizer+(looptime-1)*64+ANTIZIGZAG[ii])=*(after_anti_entropy+(looptime-1)*64+ii)*quantize[usedquan[2]][ii];
	//before is ordered by zig-zag
/*	for (tablenum=0;tablenum<looptime;tablenum++)
	{
		for (ii=0;ii<64;ii++)
			temp[ii]=*(after_anti_entropy+tablenum*64+ii);
		int i=1;
		int total=1;
		int add=1;
		for (ii=0;ii<64;ii++)
		{
			if (ii+1>total)	
				if (add==1)	total=total+(++i);
				else
					total=total+(--i);
			int xplace;
			int yplace;
			if (i%2==0)
			{
				if ((add==1)||(i==8))
					yplace=(i-1)-(total-1-ii);
				else
					yplace=7-(total-1-ii);
				if ((add==1)||(i==8))
					xplace=i-1-yplace;
				else
					xplace=16-i-1-yplace;
			}
			else
			{
				if ((add==1)||(i==8))
					xplace=(i-1)-(total-1-ii);
				else
					xplace=7-(total-1-ii);
				if ((add==1)||(i==8))
					yplace=i-1-xplace;
				else
					yplace=16-i-1-xplace;
			}
			*(after_anti_entropy+tablenum*64+yplace*8+xplace)=temp[ii];
			if (i==8) add=0;
		}
	}
*/	return 1;
}

int KINGJPG::anti_color(int x,int y,short int *after_anti_dct)
{
	int looptime;
	int xscale=scale[0]/16;
	int yscale=scale[0]%16;
	looptime=(scale[0]/16)*(scale[0]%16)+2;
	short int *cb=new short int[64*xscale*yscale];
	short int *cr=new short int[64*xscale*yscale];
//	scale_pic(after_anti_dct+(looptime-2)*64,8,8,cb,8*xscale,8*yscale,0);
//	scale_pic(after_anti_dct+(looptime-1)*64,8,8,cr,8*xscale,8*yscale,0);
	scale_pic2(after_anti_dct+(looptime-2)*64,8,8,cb,xscale,yscale,0);
	scale_pic2(after_anti_dct+(looptime-1)*64,8,8,cr,xscale,yscale,0);
	int place1,place2,place3,place4,place5=0;
/*	for (int xx=0;xx<xscale;xx++)
	for (int yy=0;yy<yscale;yy++)
		for (int yyy=0;yyy<8;yyy++)
		for (int xxx=0;xxx<8;xxx++){
			int temp;
			place1=(y*8*yscale+yy*8+yyy)*xuse_length+x*8*xscale+xx*8+xxx;
			place1*=3;
			place2=(yy*xscale+xx)*64+yyy*8+xxx;
			place3=yyy+(yy<<3);
			place4=xxx+(xx<<3);
			place5=(place3<<3)*xscale+place4;
//			temp=0.5+ *(after_anti_dct+place2) + (*(cr+place5)-128)*1.40200;	//r=y+cr*1.400200
			temp=*(after_anti_dct+place2) + ((r_cr[*(cr+place5)]+32768)>>16);	//r=y+cr*1.400200
			//here change the red and blue color
			*(point+place1)=(BYTE)temp;
			if (temp>255)
				*(point+place1)=255;
			if (temp<0)
				*(point+place1)=0;
			
//			temp=0.5+ *(after_anti_dct+place2) - (*(cr+place5)-128)*0.71414 - (*(cb+place5)-128)*0.34414;	//g=y-0.71414*cr-0.34414*cb
			temp=*(after_anti_dct+place2) + ((g_cb[*(cb+place5)]+g_cr[*(cr+place5)]+32768)>>16);	//g=y-0.71414*cr-0.34414*cb
			*(point+place1+1)=(BYTE)temp;
			if (temp>255)
				*(point+place1+1)=255;
			if (temp<0)
				*(point+place1+1)=0;
			
//			temp=0.5+ *(after_anti_dct+place2) + (*(cb+place5)-128)*1.77200;
			temp=*(after_anti_dct+place2) + ((b_cb[*(cb+place5)])>>16);
			*(point+place1+2)=(BYTE)temp;
			if (temp>255)
				*(point+place1+2)=255;
			if (temp<0)
				*(point+place1+2)=0;
			
		}
*/
	int xplace=(x*xscale)<<3;
	int yplace=(y*yscale)<<3;
	for (int xx=0;xx<xscale;xx++)
	for (int yy=0;yy<yscale;yy++)
	{
		place2=(yy*xscale+xx)<<6;
		place1=(yplace+yy*8)*xuse_length+xplace+(xx<<3);
		place3=(yy*xscale<<6)+(xx<<3);
		place5=place3;
		for (int yyy=0;yyy<8;yyy++){
			int place=place1;
		for (int xxx=0;xxx<8;xxx++){
			int temp;
			int place1;
			place1=place;
			place1=place1+place1+place1;
			temp=*(after_anti_dct+place2) + ((r_cr[*(cr+place5)]+32768)>>16);	//r=y+cr*1.400200
			//here change the red and blue color
			*(point+place1)=(BYTE)temp;
			if (temp>255)
				*(point+place1)=255;
			if (temp<0)
				*(point+place1)=0;

			temp=*(after_anti_dct+place2) + ((g_cb[*(cb+place5)]+g_cr[*(cr+place5)]+32768)>>16);	//g=y-0.71414*cr-0.34414*cb
			*(point+place1+1)=(BYTE)temp;
			if (temp>255)
				*(point+place1+1)=255;
			if (temp<0)
				*(point+place1+1)=0;
			temp=*(after_anti_dct+place2) + ((b_cb[*(cb+place5)])>>16);
			*(point+place1+2)=(BYTE)temp;
			if (temp>255)
				*(point+place1+2)=255;
			if (temp<0)
				*(point+place1+2)=0;
			place2++;
			place++;
			place5++;
		}
		place1+=xuse_length;
		place5+=((xscale-1)<<3);
		}
	
	}
	delete cb;
	delete cr;
	return 1;
}

int KINGJPG::l_rotate(int *orignal,BYTE *now,BYTE *rotate_time,int *place)
{
	if (*now >= 128)	//most high is 1
	{
		*orignal=*orignal*2+1;
		*now=((*now)&0x7f)<<1;
	}
	else			//most high is 0
	{
		*orignal=*orignal*2;
		*now=(*now)<<1;
	}
	*(rotate_time)=*(rotate_time)+1;
	if (*(rotate_time)==8)
	{
		*(rotate_time)=0;
		*(place)=*(place)+1;
		*now=*(pixel+*place);
		if ( ( *now == 0xff) && ( *(pixel+*place+1)!=0 ) )
		{
			int king;
			king++;
		}
		if ( (*now == 0) && (*(pixel+*place-1)==0xff) )
		{
			*(place)=*(place)+1;
			*now=*(pixel+*place);
		}
	}
	return 1;
}

int KINGJPG::draw(CDC *pdc)
{
	int ret=::SetDIBitsToDevice(pdc->m_hDC,0,0,width,height,0,0,0,height,display,&bmpinfo,DIB_RGB_COLORS);
	return 0;
}

int KINGJPG::initbmp()
{
	bmpinfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	bmpinfo.bmiHeader.biWidth=width;
	bmpinfo.bmiHeader.biHeight=0-height;
	bmpinfo.bmiHeader.biPlanes=1;//pcxhead.color_planes;
	bmpinfo.bmiHeader.biBitCount=24;
	bmpinfo.bmiHeader.biCompression=0;
	bmpinfo.bmiHeader.biSizeImage=0;
	bmpinfo.bmiHeader.biXPelsPerMeter=0;
	bmpinfo.bmiHeader.biYPelsPerMeter=0;
	bmpinfo.bmiHeader.biClrUsed=0;
	bmpinfo.bmiHeader.biClrImportant=0;
	int xuse;
	xuse=(width*3+3)/4*4;
	display=new BYTE[xuse*height];
	int place1=0;
	int place=0;
	if (xuse==width*3)
	for (int i=0;i<height;i++){
		for (int j=0;j<width;j++){
			*(display+place++)=*(point+place1+2);
			*(display+place++)=*(point+place1+1);
			*(display+place++)=*(point+place1);
			place1+=3;
		}
		place1=(i+1)*3*xuse_length;
	}
	else
	for (int i=0;i<height;i++){
		int j;
		for (j=0;j<width;j++){
			*(display+place++)=*(point+place1+2);
			*(display+place++)=*(point+place1+1);
			*(display+place++)=*(point+place1);
			place1+=3;
		}
		for (j=width*3;j<xuse;j++)
			*(display+place++)=0;
		place1=(i+1)*3*xuse_length;
	}
	return 0;
}

int KINGJPG::savepicture(CFile *fp)
{
		init_dc_huff();
		init_ac_huff();
		BYTE *codedata=new BYTE[width*height];
		if (codedata==0)
			return -1;
		int reallength=0;
		int ret=codejpg(codedata,headlength,&reallength);
		if (ret<0)
		{
			delete codedata;
			return -1;
		}

		fp->Write(pixel,_start[0]);
		fp->Write(y_dc,19);
		fp->Write(y_dc_table,sizeof(y_dc_table));

		BYTE add;
		add=0xff;
		fp->Write(&add,1);
		add=0xc4;
		fp->Write(&add,1);
		fp->Write(cbcr_dc,19);
		fp->Write(cbcr_dc_table,sizeof(cbcr_dc_table));

		add=0xff;
		fp->Write(&add,1);
		add=0xc4;
		fp->Write(&add,1);
		fp->Write(y_ac,19);
		fp->Write(y_ac_table,sizeof(y_ac_table));

		add=0xff;
		fp->Write(&add,1);
		add=0xc4;
		fp->Write(&add,1);
		fp->Write(cbcr_ac,19);
		fp->Write(cbcr_ac_table,sizeof(cbcr_ac_table));

		
		fp->Write(pixel+_end[1],headlength-_end[1]);
		fp->Write(codedata,reallength);
		delete codedata;
		BYTE end=0x00;
		fp->Write(&end,1);
		end=0xff;
		fp->Write(&end,1);
		end=0xd9;
		fp->Write(&end,1);
		return 0;
}

int		KINGJPG::codejpg(BYTE *codedata,int headlength,int *reallength)
{
//here should extend the picture so that its xlength and ylength
//can be divide by 16.
//but we only decode the picture that was code from jpg,so we 
//have not do this.
//for the other type we should add it
	int x_size;
	int y_size;
	x_size=xuse_length/8/(scale[0]/16);
	y_size=yuse_length/8/(scale[0]%16);
	int xnow,ynow;
	BYTE rotate_time;
	rotate_time=0;
	dc_before=0;
	dc_y_before=0;
	dc_cb_before=0;
	dc_cr_before=0;
	place=headlength;
	int looptime=(scale[0]/16)*(scale[0]%16)+2;
	short int *before_entropy;
	short int *before_dct;
	before_entropy=new short int[looptime*64];
	before_dct=new short int[looptime*64];
	now=*(pixel+place);
	int ret;
	for (ynow=0;ynow<y_size;ynow++){
		for (xnow=0;xnow<x_size;xnow++){
		color(xnow,ynow,before_dct);
		for (int i=0;i<looptime;i++)
			jpeg_DCT(before_dct+64*i,before_entropy+64*i);
		quantizer(before_entropy);
		ret=entropy(codedata,reallength,&rotate_time,before_entropy,looptime);
		if (ret<0)
			return -1;
		}
	}
	delete before_entropy;
	delete before_dct;
	return 1;
}

int		KINGJPG::color(int xnow,int ynow,short int *before_dct)
{
	int looptime;
	int xscale=scale[0]/16;
	int yscale=scale[0]%16;
	looptime=(scale[0]/16)*(scale[0]%16)+2;
	short int *cb=new short int[256];
	short int *cr=new short int[256];
	for (int y=0;y<8*yscale;y++)
		for (int x=0;x<8*xscale;x++){
			int place1,place2,place3,place4;
			place1=(ynow*8*yscale+y)*xuse_length+xnow*8*xscale+x;
			place1*=3;
			place2=(y%8)*8+x%8;
			place3=(y/8*2+x/8)*64+place2;
			place4=y*16+x;
//			*(before_dct+place3)=0.299*(*(point+place1))+0.587*(*(point+place1+1))+0.114*(*(point+place1+2));
			*(before_dct+place3)=(y_r[*(point+place1)]+y_g[*(point+place1+1)]+y_b[*(point+place1+2)])>>16;
//			*(cb+place4) = 128 - 0.15847*(*(point+place1))-0.33126*(*(point+place1+1))+0.5*(*(point+place1+2));
			*(cb+place4) = 128 + ((cb_r[*(point+place1)]+cb_g[*(point+place1+1)]+cb_b[*(point+place1+2)])>>16);
//			*(cr+place4) = 128 + 0.5*(*(point+place1))-0.41869*(*(point+place1+1))-0.08131*(*(point+place1+2));
			*(cr+place4) = 128 + ((cb_b[*(point+place1)]+cr_g[*(point+place1+1)]+cr_b[*(point+place1+2)])>>16);
		}
	scale_pic(cb,8*xscale,8*yscale,before_dct+(looptime-2)*64,8,8,0);
	scale_pic(cr,8*xscale,8*yscale,before_dct+(looptime-1)*64,8,8,0);
//	scale_pic2(cb,8*xscale,8*yscale,before_dct+(looptime-2)*64,-xscale,-yscale,0);
//	scale_pic2(cr,8*xscale,8*yscale,before_dct+(looptime-1)*64,-xscale,-yscale,0);

	delete cb;
	delete cr;
	return 0;
}

int		KINGJPG::quantizer(short int *before_entropy)
{
	int looptime;
	looptime=(scale[0]/16)*(scale[0]%16)+2;
	short int temp[64];
	int place=0;
	int tablenum;
	tablenum=usedquan[0];
	int huffnum=0;
	for (int i=0;i<looptime;i++){
		place=i*64;
		int j;
		for (j=0;j<64;j++){
			int data;
			data=*(before_entropy+place);
			temp[ZIGZAG[j]]=data;
			place++;
		}
		place=i*64;
		for (j=0;j<64;j++)
			*(before_entropy+place++)=0.5+((double)temp[j])/quantize[tablenum][j];
		if (i==looptime-2)
			tablenum=usedquan[1];
		if (i==looptime-1)
			tablenum=usedquan[2];
	}
	return 0;
}

int		KINGJPG::entropy(BYTE *codedata,int *place,BYTE *rotate_time,short int *before_entropy,int looptime)
{
	int i,j;
	int orignal;
	BYTE r,v_length;
	int  v;
	BYTE tablenum;
	BYTE length;
	BYTE whyking;
	int  diffdc;
	int  absdc; 
	for (i=0;i<looptime;i++){
		tablenum=*(used_dc_table+0);
		if (i==looptime-1)
			tablenum=*(used_dc_table+2);
		if (i==looptime-2)
			tablenum=*(used_dc_table+1);
		whyking=first_dc[tablenum];
		j=0;
//dc code
		dc_before=dc_y_before;
		if (i==looptime-2)
			dc_before=dc_cb_before;
		if (i==looptime-1)
			dc_before=dc_cr_before;

		diffdc=*(before_entropy+i*64)-dc_before;
		if (diffdc>0)
			absdc=diffdc;
		else
			absdc=-diffdc;
		int length=0;
		int big=1;
		while (absdc>=big){
			length++;
			big<<=1;
		}
		if (diffdc<0)
			diffdc=big-absdc-1;
		int search;
		int jj;
		for (jj=0;jj<huff_dc_table_num[tablenum];jj++)
			if (length==huff_dc_table[tablenum][jj])
				break;
		if (jj==huff_dc_table_num[tablenum])
			return -1;
		search=jj;
		int length3=1;
		while ((search>=huff_dc_start_table[tablenum][length3-1])&&(length3<17))
			length3++;
		if (search>huff_dc_table_num[tablenum])
			return -1;
		length3--;
		orignal=start_dc_ac_length[tablenum][length3-1]+search-huff_dc_start_table[tablenum][length3-1];
		int length2=1;
		while ( ((orignal>max_dc_ac_length[tablenum][length2-1]) || ((length2<whyking)&&(orignal==0)))  &&(length2<17))
			length2++;
		if (length2>=17)
			return -1;
		writecode(length2,orignal,codedata,rotate_time,place);
		writecode(length,diffdc,codedata,rotate_time,place);
		if (i==looptime-1)
			dc_cr_before=*(before_entropy+i*64);
		else
			if (i==looptime-2)
				dc_cb_before=*(before_entropy+i*64);
			else
				dc_y_before=*(before_entropy+i*64);
		
		tablenum=*(used_ac_table+0)+4;
		if (i==looptime-1)
			tablenum=*(used_ac_table+2)+4;
		if (i==looptime-2)
			tablenum=*(used_ac_table+1)+4;
		j++;

		whyking=first_ac[tablenum-4];


//end dc
		while (j<64)	{
			int qq=j;
			while ( (*(before_entropy+i*64+qq)==0) && (qq<64) )
				qq++;
			if (qq==64)	//the remain is all 0
			{
				int search;
				for (jj=0;jj<huff_ac_table_num[tablenum-4];jj++)
					if (0==huff_ac_table[tablenum-4][jj])
						break;
				if (jj==huff_ac_table_num[tablenum-4])
					return -1;
				search=jj;
				int length3=1;
				while ((search>=huff_ac_start_table[tablenum-4][length3-1])&&(length3<17))
					length3++;
				if (search>huff_ac_table_num[tablenum-4])
					return -1;
				length3--;
				orignal=start_dc_ac_length[tablenum][length3-1]+search-huff_ac_start_table[tablenum-4][length3-1];
				int length2=1;
				while (((orignal>max_dc_ac_length[tablenum][length2-1]) || ((length2<whyking)&&(orignal==0)))  &&(length2<17))
					length2++;
				if (length2>=17)
					return -1;
				writecode(length2,orignal,codedata,rotate_time,place);
				j=64;
			}

			else
			{

			if (qq-j>=16)//at least 16 ge 0
			{
				int search;
				for (jj=0;jj<huff_ac_table_num[tablenum-4];jj++)
					if (0xf0==huff_ac_table[tablenum-4][jj])
						break;
				if (jj==huff_ac_table_num[tablenum-4])
					return -1;
				search=jj;
				int length3=1;
				while ((search>=huff_ac_start_table[tablenum-4][length3-1])&&(length3<17))
					length3++;
				if (search>huff_ac_table_num[tablenum-4])
					return -1;
				length3--;
				orignal=start_dc_ac_length[tablenum][length3-1]+search-huff_ac_start_table[tablenum-4][length3-1];
				int length2=1;
				while (((orignal>max_dc_ac_length[tablenum][length2-1]) || ((length2<whyking)&&(orignal==0)))  &&(length2<17))
					length2++;
				if (length2>=17)
					return -1;
				writecode(length2,orignal,codedata,rotate_time,place);
				j+=16;
			}	

			else	
			{
			diffdc=*(before_entropy+i*64+qq);
			if (diffdc>0)
				absdc=diffdc;
			else
				absdc=-diffdc;
			int length=0;
			int big=1;
			while (absdc>=big){
				length++;
				big<<=1;
			}
			if (diffdc<0)
				diffdc=big-absdc-1;
			int serach;
			int length1;
			length1=length+((qq-j)<<4);
			for (jj=0;jj<huff_ac_table_num[tablenum-4];jj++)
				if (length1==huff_ac_table[tablenum-4][jj])
					break;
			if (jj==huff_ac_table_num[tablenum-4])//cant find it at ac table,divide it into two
			{
				if (length1<16)
					return -1;
				qq--;
				int step=qq-j;
				diffdc=0;
				absdc=0;
				length1=(qq-j)<<4;
				for (jj=0;jj<huff_ac_table_num[tablenum-4];jj++)
					if (length1==huff_ac_table[tablenum-4][jj])
						break;
				if (jj==huff_ac_table_num[tablenum-4])
					return -1;
			}
			search=jj;
			int length3=1;
			while ((search>=huff_ac_start_table[tablenum-4][length3-1])&&(length3<17))
				length3++;
			if (search>huff_ac_table_num[tablenum-4])
				return -1;
			length3--;
			orignal=start_dc_ac_length[tablenum][length3-1]+search-huff_ac_start_table[tablenum-4][length3-1];
			int length2=1;
			while (((orignal>max_dc_ac_length[tablenum][length2-1]) || ((length2<whyking)&&(orignal==0)))  &&(length2<17))
				length2++;
			if (length2>=17)
				return -1;
			writecode(length2,orignal,codedata,rotate_time,place);
			writecode(length,diffdc,codedata,rotate_time,place);	
			j+=(qq-j);
			j++;
			}

			}
		}
	}


	return 0;
}

int	KINGJPG::writecode(int length2,int orignal,BYTE *codedata,BYTE *rotate_time,int *place)
{
	int mask=1;
	int i;
	for (i=0;i<length2-1;i++)		
		mask<<=1;
	for (i=0;i<length2;i++)
	{
		(*(codedata+*place))<<=1;
		if ((mask&orignal)!=0)
			(*(codedata+*place))|=1;
		(*rotate_time)++;
		if (*rotate_time==8)
		{
			*rotate_time=0;
			(*place)++;
			if (*(codedata+*place-1)==0xff)
				*(codedata+(*place)++)=0;
		}
		mask>>=1;
	}
	return 0;
}

int	KINGJPG::init_ac_huff()
{
	ac_number=0;
	int total=0;
	int i;
	for (i=0;i<16;i++){
		huff_ac_start_table[ac_number][i]=total;
		total+=y_ac[i+3];
	}
	max_dc_ac_length[ac_number+4][0]=0;
	start_dc_ac_length[ac_number+4][0]=0;
	int number;
	int start=0;
	int scale=2;
	int last=0;
	if (huff_ac_start_table[ac_number][1]!=0)
		start=2;
	for (i=1;i<15;i++)
	{
		start_dc_ac_length[ac_number+4][i]=start;
		number=huff_ac_start_table[ac_number][i+1]-huff_ac_start_table[ac_number][i];
		max_dc_ac_length[ac_number+4][i]=start_dc_ac_length[ac_number+4][i]+number-1;
		if (number-1<0)
		{
			max_dc_ac_length[ac_number+4][i]=0;
			start_dc_ac_length[ac_number+4][i]=0;
		}
		if (number==0)
			scale*=2;
		else
		{
			scale=2;
			last=i;
		}
		if (last!=0)
			start=(max_dc_ac_length[ac_number+4][last]+1)*scale;
		else
			start=0;
	}
	i=0;
	while (huff_ac_start_table[ac_number][i]==0)
		i++;
	first_ac[ac_number]=i;
	start_dc_ac_length[ac_number+4][15]=start;
	number=total-huff_ac_start_table[ac_number][15];
	max_dc_ac_length[ac_number+4][15]=65535;
	//	max_dc_ac_length[ac_number+4][15]=start+number-1;
	delete huff_ac_table[ac_number];
	huff_ac_table[ac_number]=new BYTE[total];
	copy_byte(&huff_ac_table[ac_number][0],y_ac_table,total);
	huff_ac_table_num[ac_number]=total;
	ac_number++;



	
	total=0;
	for (i=0;i<16;i++){
		huff_ac_start_table[ac_number][i]=total;
		total+=cbcr_ac[i+3];
	}
	max_dc_ac_length[ac_number+4][0]=0;
	start_dc_ac_length[ac_number+4][0]=0;
	start=0;
	scale=2;
	last=0;
	if (huff_ac_start_table[ac_number][1]!=0)
		start=2;
	for (i=1;i<15;i++)
	{
		start_dc_ac_length[ac_number+4][i]=start;
		number=huff_ac_start_table[ac_number][i+1]-huff_ac_start_table[ac_number][i];
		max_dc_ac_length[ac_number+4][i]=start_dc_ac_length[ac_number+4][i]+number-1;
		if (number-1<0)
		{
			max_dc_ac_length[ac_number+4][i]=0;
			start_dc_ac_length[ac_number+4][i]=0;
		}
		if (number==0)
			scale*=2;
		else
		{
			scale=2;
			last=i;
		}
		if (last!=0)
			start=(max_dc_ac_length[ac_number+4][last]+1)*scale;
		else
			start=0;
	}
	i=0;
	while (huff_ac_start_table[ac_number][i]==0)
		i++;
	first_ac[ac_number]=i;
	start_dc_ac_length[ac_number+4][15]=start;
	number=total-huff_ac_start_table[ac_number][15];
	max_dc_ac_length[ac_number+4][15]=65535;
	delete huff_ac_table[ac_number];
	huff_ac_table[ac_number]=new BYTE[total];
	copy_byte(&huff_ac_table[ac_number][0],cbcr_ac_table,total);
	huff_ac_table_num[ac_number]=total;
	ac_number++;

	return 1;
}


int	KINGJPG::init_dc_huff()
{
	dc_number=0;
	int total=0;
	int i;
	for (i=0;i<16;i++){
		huff_dc_start_table[dc_number][i]=total;
		total+=y_dc[i+3];
	}
	max_dc_ac_length[dc_number][0]=0;
	start_dc_ac_length[dc_number][0]=0;
	int number;
	int start=0;
	int scale=2;
	int last=0;
	if (huff_dc_start_table[dc_number][1]!=0)
		start=2;
	for (i=1;i<15;i++)
	{
		start_dc_ac_length[dc_number][i]=start;
		number=huff_dc_start_table[dc_number][i+1]-huff_dc_start_table[dc_number][i];
		max_dc_ac_length[dc_number][i]=start_dc_ac_length[dc_number][i]+number-1;
		if (number-1<0)
		{
			max_dc_ac_length[dc_number][i]=0;
			start_dc_ac_length[dc_number][i]=0;
		}
		if (number==0)
			scale*=2;
		else
		{
			scale=2;
			last=i;
		}
		if (last!=0)
			start=(max_dc_ac_length[dc_number][last]+1)*scale;
		else
			start=0;
	}
	i=0;
	while (huff_dc_start_table[dc_number][i]==0)
		i++;
	first_dc[dc_number]=i;
	start_dc_ac_length[dc_number][15]=start;
	number=total-huff_dc_start_table[dc_number][15];
	max_dc_ac_length[dc_number][15]=start+number-1;
	//	max_dc_ac_length[dc_number+4][15]=start+number-1;
	delete huff_dc_table[dc_number];
	huff_dc_table[dc_number]=new BYTE[total];
	copy_byte(&huff_dc_table[dc_number][0],y_dc_table,total);
	huff_dc_table_num[dc_number]=total;
	dc_number++;



	
	total=0;
	for (i=0;i<16;i++){
		huff_dc_start_table[dc_number][i]=total;
		total+=cbcr_dc[i+3];
	}
	max_dc_ac_length[dc_number][0]=0;
	start_dc_ac_length[dc_number][0]=0;
	start=0;
	scale=2;
	last=0;
	if (huff_dc_start_table[dc_number][1]!=0)
		start=2;
	for (i=1;i<15;i++)
	{
		start_dc_ac_length[dc_number][i]=start;
		number=huff_dc_start_table[dc_number][i+1]-huff_dc_start_table[dc_number][i];
		max_dc_ac_length[dc_number][i]=start_dc_ac_length[dc_number][i]+number-1;
		if (number-1<0)
		{
			max_dc_ac_length[dc_number][i]=0;
			start_dc_ac_length[dc_number][i]=0;
		}
		if (number==0)
			scale*=2;
		else
		{
			scale=2;
			last=i;
		}
		if (last!=0)
			start=(max_dc_ac_length[dc_number][last]+1)*scale;
		else
			start=0;
	}
	i=0;
	while (huff_dc_start_table[dc_number][i]==0)
		i++;
	first_dc[dc_number]=i;
	start_dc_ac_length[dc_number][15]=start;
	number=total-huff_dc_start_table[dc_number][15];
	max_dc_ac_length[dc_number][15]=start+number-1;
	delete huff_dc_table[dc_number];
	huff_dc_table[dc_number]=new BYTE[total];
	copy_byte(&huff_dc_table[dc_number][0],cbcr_dc_table,total);
	huff_dc_table_num[dc_number]=total;
	dc_number++;

	return 1;
}