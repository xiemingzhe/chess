// Chess.h : main header file for the CHESS application
//

#if !defined(AFX_CHESS_H__74BE5A19_C15F_4EAB_AE96_83487A1A0E37__INCLUDED_)
#define AFX_CHESS_H__74BE5A19_C15F_4EAB_AE96_83487A1A0E37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


/////////////////////////////////////////////////////////////////////////////
// CChessApp:
// See Chess.cpp for the implementation of this class
//
class CChessApp : public CWinApp
{
public:
	CChessApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChessApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CChessApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHESS_H__74BE5A19_C15F_4EAB_AE96_83487A1A0E37__INCLUDED_)
