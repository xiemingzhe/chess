#if !defined(AFX_SETLEVEL_H__60CC3A3C_7BAE_430B_BA41_029ED001B3F5__INCLUDED_)
#define AFX_SETLEVEL_H__60CC3A3C_7BAE_430B_BA41_029ED001B3F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetLevel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// SetLevel dialog

class SetLevel : public CDialog
{
// Construction
public:
	SetLevel(CWnd* pParent = NULL);   // standard constructor
    int getTime(){	return m_SeletedTime; };
// Dialog Data
	//{{AFX_DATA(SetLevel)
	enum { IDD = IDD_LEVEL };
	CSpinButtonCtrl	m_spinTime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SetLevel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    static int m_SeletedTime ;
	// Generated message map functions
	//{{AFX_MSG(SetLevel)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETLEVEL_H__60CC3A3C_7BAE_430B_BA41_029ED001B3F5__INCLUDED_)
