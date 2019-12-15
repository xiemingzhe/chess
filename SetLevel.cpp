// SetLevel.cpp : implementation file
//

#include "stdafx.h"
#include "Chess.h"
#include "SetLevel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SetLevel dialog

int SetLevel::m_SeletedTime  = 1 ;
SetLevel::SetLevel(CWnd* pParent /*=NULL*/)
	: CDialog(SetLevel::IDD, pParent)
{
	//{{AFX_DATA_INIT(SetLevel)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void SetLevel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SetLevel)
	DDX_Control(pDX, IDC_SPIN_TIME, m_spinTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(SetLevel, CDialog)
	//{{AFX_MSG_MAP(SetLevel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SetLevel message handlers

BOOL SetLevel::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
	m_spinTime.SetRange(1,5);
	m_spinTime.SetPos(m_SeletedTime);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void SetLevel::OnOK() 
{
	// TODO: Add extra validation here
	m_SeletedTime = m_spinTime.GetPos();

	CDialog::OnOK();
}
