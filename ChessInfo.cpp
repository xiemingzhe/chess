// ChessInfo.cpp : implementation file
//

#include "stdafx.h"
#include "Chess.h"
#include "ChessInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ChessInfo

IMPLEMENT_DYNCREATE(ChessInfo, CFormView)

ChessInfo::ChessInfo()
	: CFormView(ChessInfo::IDD)
{
	//{{AFX_DATA_INIT(ChessInfo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

ChessInfo::~ChessInfo()
{
}

void ChessInfo::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ChessInfo)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ChessInfo, CFormView)
	//{{AFX_MSG_MAP(ChessInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ChessInfo diagnostics

#ifdef _DEBUG
void ChessInfo::AssertValid() const
{
	CFormView::AssertValid();
}

void ChessInfo::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG
