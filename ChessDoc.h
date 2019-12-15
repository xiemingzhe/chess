// ChessDoc.h : interface of the CChessDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHESSDOC_H__F6FEB8BD_84D3_45BC_8EE1_191E94DC8EB2__INCLUDED_)
#define AFX_CHESSDOC_H__F6FEB8BD_84D3_45BC_8EE1_191E94DC8EB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CChessDoc : public CDocument
{
protected: // create from serialization only
	CChessDoc();
	DECLARE_DYNCREATE(CChessDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChessDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChessDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CChessDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHESSDOC_H__F6FEB8BD_84D3_45BC_8EE1_191E94DC8EB2__INCLUDED_)
