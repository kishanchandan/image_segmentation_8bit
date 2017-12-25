// kingimageView.cpp : implementation of the CKingimageView class
//

#include "stdafx.h"
#include "kingimage.h"
#include "resource.h"
#include "kingimageDoc.h"
#include "kingimageView.h"
#include <array>
#include <fstream>
#include <algorithm>
#include <list> 
int findthreshold(int intensityfrequency[],int,int);
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKingimageView

IMPLEMENT_DYNCREATE(CKingimageView, CScrollView)

BEGIN_MESSAGE_MAP(CKingimageView, CScrollView)
	//{{AFX_MSG_MAP(CKingimageView)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
	ON_COMMAND(ID_ANSWER1_CREATENEGATIVEIMAGE, OnAnswer1Createnegativeimage)
	ON_COMMAND(ID_ANSWER1_APPLYHISTOGRAMEQUALIZATION, &CKingimageView::OnAnswer1Applyhistogramequalization)
	ON_COMMAND(ID_ANSWER2_LABELSHAPES, &CKingimageView::OnAnswer2Labelshapes)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKingimageView construction/destruction

CKingimageView::CKingimageView()
{
	// TODO: add construction code here

}

CKingimageView::~CKingimageView()
{
}

BOOL CKingimageView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CKingimageView drawing

void CKingimageView::OnDraw(CDC* pDC)
{
	CKingimageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
//	pDC->SetStretchBltMode(COLORONCOLOR);
//	int x,y,bytes;
	if (pDoc->imagetype==PCX)
		(pDoc->_pcx)->draw(pDC);
	if (pDoc->imagetype==BMP)
		(pDoc->_bmp)->draw(pDC);
	if (pDoc->imagetype==GIF)
		(pDoc->_gif)->draw(pDC);
	if (pDoc->imagetype==JPG)
		(pDoc->_jpg)->draw(pDC);

}

/////////////////////////////////////////////////////////////////////////////
// CKingimageView printing

BOOL CKingimageView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKingimageView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKingimageView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CKingimageView diagnostics

#ifdef _DEBUG
void CKingimageView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CKingimageView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CKingimageDoc* CKingimageView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKingimageDoc)));
	return (CKingimageDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CKingimageView message handlers



void CKingimageView::OnInitialUpdate() 
{
	CScrollView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	CSize totalSize=CSize(::GetSystemMetrics(SM_CXSCREEN),
		                  ::GetSystemMetrics(SM_CYSCREEN));
	CSize pageSize=CSize(totalSize.cx/2,totalSize.cy/2);
	CSize lineSize=CSize(totalSize.cx/10,totalSize.cy/10);

	SetScrollSizes(MM_TEXT,totalSize,pageSize,lineSize);
}

void CKingimageView::OnMouseMove(UINT nFlags, CPoint point) 
{
	CScrollView::OnMouseMove(nFlags, point);
}


int findthreshold(int intensityfrequency[], int threshold, int newthreshold)
{
	if (newthreshold == threshold)
	{
		return threshold;
	}
	else 
	{
		
		float darkcluster = 0;
		float lightcluster = 0;
		float darkclustermean = 0, lightclustermean = 0;
		for (int p = 0; p < threshold; p++)
		{
			darkcluster = darkcluster + intensityfrequency[p];
		}
		
		for (int q = int(threshold + 1); q <= 255; q++)
		{
			lightcluster = lightcluster + intensityfrequency[q];

		}

		float darkclustersum = 0,lightclustersum=0;
		for (int p = 0; p < threshold; p++)
		{
			darkclustersum += p*intensityfrequency[p];
		}
		darkclustermean =  darkclustersum/ darkcluster;
		
		for (int q = int(threshold + 1); q <= 255; q++)
		{
			lightclustersum += q * intensityfrequency[q];

		}
		lightclustermean = lightclustersum / lightcluster;

		newthreshold = (darkclustermean + lightclustermean) / 2;

		
		if (newthreshold == threshold)
		{
			return threshold;
		}

		else
		{
			threshold = newthreshold;
			newthreshold = 0;
		}

		return findthreshold(intensityfrequency, threshold, newthreshold);

	}
		

}





void CKingimageView::OnAnswer2Labelshapes()
{
	CKingimageDoc* pDoc = GetDocument();


	int iBitPerPixel = pDoc->_bmp->bitsperpixel;
	int iWidth = pDoc->_bmp->width;
	int iHeight = pDoc->_bmp->height;
	BYTE *pImg = pDoc->_bmp->point;
	BYTE  *poriginal = pDoc->_bmp->point;

	int plabel[200000] = { 0 };
	int label = 1;
	int merge_len = 0;
	int unique_label[200] = { 0 };
	int min = 0;
	int max = 0;

	int Wp = iWidth;
	if (iBitPerPixel == 8)  ////Grey scale 8 bits image
	{
		int r = iWidth % 4;
		int p = (4 - r) % 4;
		Wp = iWidth + p;
	}
	else if (iBitPerPixel == 24)	// RGB image
	{
		int r = (3 * iWidth) % 4;
		int p = (4 - r) % 4;
		Wp = 3 * iWidth + p;
	}

	if (iBitPerPixel == 8)  ////Grey scale 8 bits image
	{

		int intensityfrequency[256] = { 0 };
		for (int i = 0; i<iHeight; i++)
			for (int j = 0; j<iWidth; j++)
			{
				intensityfrequency[int(pImg[i*Wp + j])] = intensityfrequency[int(pImg[i*Wp + j])] + 1;// increments frequency of that intensity
			}
		
		int threshold = 50;


		int finalthreshold = 0;
		int newthreshold = 0;
		finalthreshold = findthreshold(intensityfrequency, threshold, newthreshold);


		for (int i = 0; i<iHeight; i++)
			for (int j = 0; j<iWidth; j++)
			{
				if (int(pImg[i*Wp + j]) < finalthreshold) {
					pImg[i*Wp + j] = 0;
				}
				else
				{
					pImg[i*Wp + j] = 1;
				}
			}
		for (int i = 0; i < iHeight; i++)
			for (int j = 0; j < iWidth; j++)
			{
				if (int(pImg[i*Wp + j]) == 1)
				{
					if (i == 0 && j == 0)
					{
						plabel[i*Wp + j] = label;
						label = label + 1;
					}

					else if (i == 0 && j != 0)
					{
						if (pImg[i*Wp + j - 1] == pImg[i*Wp + j])
							plabel[i*Wp + j] = plabel[(i*Wp + j - 1)];
						else
						{
							plabel[i*Wp + j] = label;
							label = label + 1;
						}

					}


					else if (i != 0 && j != 0 && pImg[i*Wp + j - 1] != pImg[i*Wp + j] && pImg[(i - 1)*Wp + j] != pImg[i*Wp + j]) // left, upper and current are all different
					{
						plabel[i*Wp + j] = label;
						label = label + 1;
						//ofs << label << "Label assigned by new label" << "\n";
					}

					else if (j != 0 && pImg[i*Wp + j - 1] == pImg[i*Wp + j]) //left same as current
					{
						if (pImg[(i - 1)*Wp + j] == pImg[i*Wp + j])// current same as upper
						{

							if (plabel[(i*Wp + j - 1)] < plabel[(i - 1)*Wp + j])
							{
								plabel[i*Wp + j] = plabel[(i*Wp + j - 1)];

							}
							else
							{
								plabel[i*Wp + j] = plabel[(i - 1)*Wp + j];

							}

							//merge
							if (pImg[i*Wp + j - 1] == pImg[(i - 1)*Wp + j])
							{
								if (plabel[(i*Wp + j - 1)] < plabel[(i - 1)*Wp + j])//min left
								{
									min = plabel[(i*Wp + j - 1)];
									max = plabel[(i - 1)*Wp + j];
									for (int k = 0; k < i*Wp + j; k++)
									{
										if (plabel[k] == max)
											plabel[k] = min;
									}
								}
								else
								{
									min = plabel[(i - 1)*Wp + j];
									max = plabel[(i*Wp + j - 1)];
									for (int k = 0; k < i*Wp + j; k++)
									{
										if (plabel[k] == max)
											plabel[k] = min;
									}
								}
							}//merge complete
						}

						else
						{
							plabel[i*Wp + j] = plabel[(i*Wp + j - 1)];
							//	ofs << label << "assigned by left" << "\n";
						}
					}


					else if (i != 0 && pImg[(i - 1)*Wp + j] == pImg[i*Wp + j])//upper same as current
					{
						plabel[i*Wp + j] = plabel[(i - 1)*Wp + j];
						//	ofs << label << "Label assigned by upper" << "\n";
					}

				}

			}

		int sizeofplabel = sizeof(plabel);
		int uniq_trick[200000] = { 0 };
		int uniq_freq[100000] = { 0 };
		for (int i = 0; i < iHeight; i++)
			for (int j = 0; j < iWidth; j++)
			{
				uniq_freq[plabel[i*Wp + j]] = uniq_freq[plabel[i*Wp + j]] + 1;
				uniq_trick[plabel[i*Wp + j]] = plabel[i*Wp + j];
			}
		int k = 0, m = 0;
		int map[50] = { 0 };
		int unique_value[20] = { 0 };
		int unique_freq[20] = { 0 };
		int totaluniquevalues = 0;
		for (int i = 0; i < iHeight; i++)
			for (int j = 0; j < iWidth; j++)
			{

				if (uniq_freq[i*Wp + j] != 0 && uniq_trick[i*Wp + j] != 0)
				{
					unique_freq[m] = uniq_freq[i*Wp + j];
					m++;
				}
				if (uniq_trick[i*Wp + j] != 0)
				{
					unique_value[k] = uniq_trick[i*Wp + j];
					k++;
					totaluniquevalues++;
				}
			}


		int temp1 = 0;
		int temp2 = 0;
		for (int i = 0; i<totaluniquevalues; i++)
		{
			for (int j = i + 1; j<totaluniquevalues; j++)
			{
				if (unique_freq[i]<unique_freq[j])
				{
					temp1 = unique_freq[i];
					temp2 = unique_value[i];
					unique_freq[i] = unique_freq[j];
					unique_value[i] = unique_value[j];
					unique_freq[j] = temp1;
					unique_value[j] = temp2;
				}
			}
		}
		

		for (int i = 0; i < iHeight; i++)
			for (int j = 0; j < iWidth; j++)
			{

				if (plabel[i*Wp + j] == 0)
					pImg[i*Wp + j] = 0;
				else if (plabel[i*Wp + j] == unique_value[0])
					pImg[i*Wp + j] = 200;
				else if (plabel[i*Wp + j] == unique_value[totaluniquevalues - 1])
					pImg[i*Wp + j] = 60;
				else if (plabel[i*Wp + j] == unique_value[(totaluniquevalues) / 2])
					pImg[i*Wp + j] = 120;
				else
					pImg[i*Wp + j] = 255;


			}


	
}
	else if (iBitPerPixel == 24)  ////True color 24bits image
	{
		AfxMessageBox(_T("Image is not a 8 Bit Image"));
	}



	////redraw the screen
	OnDraw(GetDC());


}
