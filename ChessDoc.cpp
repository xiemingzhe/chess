// ChessDoc.cpp : implementation of the CChessDoc class
//

#include "stdafx.h"
#include "Chess.h"

#include "ChessDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChessDoc

IMPLEMENT_DYNCREATE(CChessDoc, CDocument)

BEGIN_MESSAGE_MAP(CChessDoc, CDocument)
	//{{AFX_MSG_MAP(CChessDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChessDoc construction/destruction

CChessDoc::CChessDoc()
{
	// TODO: add one-time construction code here

}

CChessDoc::~CChessDoc()
{
}

BOOL CChessDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	this->SetTitle("中国象棋人机博弈系统设计与实现(毕业设计)");
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChessDoc serialization

void CChessDoc::Serialize(CArchive& ar)
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

/////////////////////////////////////////////////////////////////////////////
// CChessDoc diagnostics

#ifdef _DEBUG
void CChessDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CChessDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChessDoc commands
