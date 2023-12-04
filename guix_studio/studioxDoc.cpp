// studioxDoc.cpp : implementation of the CstudioxDoc class
//

#include "stdafx.h"
#include "studiox.h"

#include "studioxDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CstudioxDoc

IMPLEMENT_DYNCREATE(CstudioxDoc, CDocument)

BEGIN_MESSAGE_MAP(CstudioxDoc, CDocument)
END_MESSAGE_MAP()


// CstudioxDoc construction/destruction

CstudioxDoc::CstudioxDoc()
{
	// TODO: add one-time construction code here

}

CstudioxDoc::~CstudioxDoc()
{
}

BOOL CstudioxDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CstudioxDoc serialization

void CstudioxDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CstudioxDoc diagnostics

#ifdef _DEBUG
void CstudioxDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CstudioxDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CstudioxDoc commands
