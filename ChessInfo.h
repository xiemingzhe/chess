#if !defined(AFX_CHESSINFO_H__DFD95FDD_AFE1_4037_B2B4_CD757F805A14__INCLUDED_)
#define AFX_CHESSINFO_H__DFD95FDD_AFE1_4037_B2B4_CD757F805A14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChessInfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ChessInfo form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class ChessInfo : public CFormView
{
protected:
	ChessInfo();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(ChessInfo)

// Form Data
public:
	//{{AFX_DATA(ChessInfo)
	enum { IDD = IDD_CHESSINFO };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ChessInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~ChessInfo();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(ChessInfo)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHESSINFO_H__DFD95FDD_AFE1_4037_B2B4_CD757F805A14__INCLUDED_)
