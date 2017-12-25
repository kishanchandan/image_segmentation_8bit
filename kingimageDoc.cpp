// kingimageDoc.cpp : implementation of the CKingimageDoc class
//

#include "stdafx.h"
#include "kingimage.h"
#include "kingimageDoc.h"
#include "picture.h"
#include "kingbmp.h"
#include "kinggif.h"
#include "kingpcx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKingimageDoc

IMPLEMENT_DYNCREATE(CKingimageDoc, CDocument)

BEGIN_MESSAGE_MAP(CKingimageDoc, CDocument)
	//{{AFX_MSG_MAP(CKingimageDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKingimageDoc construction/destruction

CKingimageDoc::CKingimageDoc()
{
	// TODO: add one-time construction code here
	imagetype=0;
	_picture=0;
}

CKingimageDoc::~CKingimageDoc()
{
	delete _picture;
//	delete _bmp;
}

BOOL CKingimageDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CKingimageDoc serialization

void CKingimageDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		//write
		if (imagetype!=0){
		CFile* fp = ar.GetFile();
		CString extname;
		extname=(ar.m_strFileName).Right(3);
		extname.MakeUpper();
		if (extname.Compare("BMP")==0){
			if (imagetype==BMP){
			returnkey=_bmp->savepicture(fp);
			if (returnkey!=0)
				AfxMessageBox("SAVE FILE FAILURE");
			else
				AfxMessageBox("SAVE FILE VICTURE");
			}
		}

		if (extname.Compare("KIN")==0){
			if (imagetype==KIN){
			returnkey=_picture->savepicture(fp);
			if (returnkey!=0)
				AfxMessageBox("SAVE FILE FAILURE");
			else 
				AfxMessageBox("SAVE FILE VICTURE");
			}
		}

		if (extname.Compare("PCX")==0){
			if (imagetype==PCX){
			returnkey=_pcx->savepicture(fp);
			if (returnkey!=0)
				AfxMessageBox("SAVE FILE FAILURE");
			else
				AfxMessageBox("SAVE FILE VICTURE");
			}
		}

		if (extname.Compare("GIF")==0){
			if (imagetype==GIF){
			returnkey=_gif->savepicture(fp);
			if (returnkey!=0)
				AfxMessageBox("SAVE FILE FAILURE");
			else
				AfxMessageBox("SAVE FILE VICTURE");
			}
		}

		if (extname.Compare("JPG")==0){
			if (imagetype==JPG){
			returnkey=_jpg->savepicture(fp);
			if (returnkey!=0)
				AfxMessageBox("SAVE FILE FAILURE");
			else 
				AfxMessageBox("SAVE FILE VICTURE");
			}
		}

		}
	}
	else	//read
	{
		CFile* fp = ar.GetFile();
		CString extname;
		extname=(ar.m_strFileName).Right(3);
		extname.MakeUpper();
		if (extname.Compare("BMP")==0){
			_bmp=new KINGBMP;
			returnkey=_bmp->getpicture(fp);
			if (returnkey!=0)
				AfxMessageBox("ERROR OPEN FILE");
			else
				imagetype=BMP;
		}
		if (extname.Compare("KIN")==0){
			_picture=new PICTURE;
			returnkey=_picture->getpicture(fp);
			if (returnkey!=0)
				AfxMessageBox("ERROR OPEN FILE");
			else 
				imagetype=KIN;
		}
		if (extname.Compare("GIF")==0){
			_gif=new KINGGIF;
			returnkey=_gif->getpicture(fp);
			if (returnkey!=0)
				AfxMessageBox("ERROR OPEN FILE");
			else
				imagetype=GIF;
		}
		if (extname.Compare("PCX")==0){
			_pcx=new KINGPCX;
			returnkey=_pcx->getpicture(fp);
			if (returnkey!=0)
				AfxMessageBox("ERROR OPEN FILE");
			else
				imagetype=PCX;
		}
		if (extname.Compare("JPG")==0){
			_jpg=new KINGJPG;
			returnkey=_jpg->getpicture(fp);
			if (returnkey<0)
				AfxMessageBox("ERROR OPEN FILE");
			else
				imagetype=JPG;
		}
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CKingimageDoc diagnostics

#ifdef _DEBUG
void CKingimageDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CKingimageDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CKingimageDoc commands
