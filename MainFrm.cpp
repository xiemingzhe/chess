// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Chess.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//添加状态栏面板
	UINT  Indicates[4]; 
	for (int i = 0; i<4;i++)
		Indicates[i]= 40+i;
    m_wndStatusBar.SetIndicators(Indicates,4);
	CRect rect;
	GetClientRect(rect);
	UINT PaneWidth = rect.Width()/4;
	
	//设置面板宽度
	for(int n = 0;n<4;n++)
	{
		m_wndStatusBar.SetPaneInfo(n,45+n*10,SBPS_NORMAL,PaneWidth);
	}
	//设置状态栏面板文本
	
	m_wndStatusBar.GetStatusBarCtrl().SetMinHeight(30) ; //设置状态栏高度
    
	int Statusize = 700 ;
    m_wndStatusBar.SetPaneInfo(0,Statusize,SBPS_NORMAL,260) ;
	m_wndStatusBar.SetPaneInfo(1,Statusize,SBPS_NORMAL,140) ;
	m_wndStatusBar.SetPaneInfo(2,Statusize,SBPS_NORMAL,120) ;
	m_wndStatusBar.SetPaneInfo(3,Statusize,SBPS_NORMAL,180) ;
	

	m_wndStatusBar.SetPaneText(0,"电脑思考信息显示.");
	m_wndStatusBar.SetPaneText(1,"学号：201008202106");
	m_wndStatusBar.SetPaneText(2,"姓名：谢明哲");
	m_wndStatusBar.SetPaneText(3,"当前时间：获取系统时间");
	SetTimer(0,1000,NULL);

	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	/*
	return m_wndSplitter.Create(this,
		2, 2,               // TODO: adjust the number of rows, columns
		CSize(10, 10),      // TODO: adjust the minimum pane size
		pContext);*/

	VERIFY(m_wndSplitter.CreateStatic(this,1,2)); //创建一行两列
	m_wndSplitter.CreateView(0,0,RUNTIME_CLASS(CChessView),CSize(0,0),pContext);
	m_wndSplitter.CreateView(0,1,RUNTIME_CLASS(ChessInfo),CSize(0,0),pContext);
   
    //ChessInfo* pWnd=(ChessInfo*)m_wndSplitter.GetPane(0,0);
	//CSize size=pWnd->GetTotalSize();
	m_wndSplitter.SetColumnInfo(0,515,515); //第0列最大最小宽度
	m_wndSplitter.SetColumnInfo(1,0,0); //右视图隐藏（显示走法信息）

    return true;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	cs.cx=540;  
	cs.cy=690;
	//禁止窗口最大化
	//cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
    cs.style &= ~WS_MAXIMIZEBOX; //禁止窗口最大化
    //cs.style &= ~WS_MINIMIZEBOX; //禁止窗口最小化
	//cs.style &= ~WS_SYSMENU; //取消Title上的按钮
    cs.style &= ~WS_THICKFRAME;//使窗口不能用鼠标改变大小
    cs.lpszClass = AfxRegisterWndClass(0);
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
    m_strTitle = "谢明哲";
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CTime time;
	time = CTime::GetCurrentTime();
	CString s = time.Format("%H:%M:%S");
	m_wndStatusBar.SetPaneText(3,"当前时间："+s);

	CFrameWnd::OnTimer(nIDEvent);
}
