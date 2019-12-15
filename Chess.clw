; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CChessView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "chess.h"
LastPage=0

ClassCount=7
Class1=CChessApp
Class2=CAboutDlg
Class3=CChessDoc
Class4=ChessInfo
Class5=CChessView
Class6=CMainFrame
Class7=SetLevel

ResourceCount=4
Resource1=IDR_MAINFRAME
Resource2=IDD_CHESSINFO
Resource3=IDD_ABOUTBOX
Resource4=IDD_LEVEL

[CLS:CChessApp]
Type=0
BaseClass=CWinApp
HeaderFile=Chess.h
ImplementationFile=Chess.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=Chess.cpp
ImplementationFile=Chess.cpp
LastObject=ID_ONEHORSE

[CLS:CChessDoc]
Type=0
BaseClass=CDocument
HeaderFile=ChessDoc.h
ImplementationFile=ChessDoc.cpp

[CLS:ChessInfo]
Type=0
BaseClass=CFormView
HeaderFile=ChessInfo.h
ImplementationFile=ChessInfo.cpp

[CLS:CChessView]
Type=0
BaseClass=CView
HeaderFile=ChessView.h
ImplementationFile=ChessView.cpp
Filter=C
VirtualFilter=VWC
LastObject=ID_TOWHORSE

[CLS:CMainFrame]
Type=0
BaseClass=CFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp

[CLS:SetLevel]
Type=0
BaseClass=CDialog
HeaderFile=SetLevel.h
ImplementationFile=SetLevel.cpp

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352

[DLG:IDD_CHESSINFO]
Type=1
Class=ChessInfo
ControlCount=0

[DLG:IDD_LEVEL]
Type=1
Class=SetLevel
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_TIME,edit,1350631552
Control5=IDC_SPIN_TIME,msctls_updown32,1342177330
Control6=IDC_STATIC,static,1342308352
Control7=IDC_TIME2,edit,1350631552
Control8=IDC_SPIN_DEPTH,msctls_updown32,1342177330

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_OPEN
Command2=ID_FILE_NEW
Command3=ID_COM_BLACK
Command4=ID_COM_RED
Command5=ID_FILE_SAVE
Command6=ID_FILE_SAVE_AS
Command7=ID_FILE_PRINT
Command8=ID_FILE_PRINT_PREVIEW
Command9=ID_FILE_PRINT_SETUP
Command10=ID_FILE_MRU_FILE1
Command11=ID_APP_EXIT
Command12=ID_LIGHT
Command13=ID_MODERATE
Command14=ID_STRONG
Command15=ID_SETLEVEL
Command16=ID_ONEHORSE
Command17=ID_TOWHORSE
Command18=ID_NINEPIECE
Command19=ID_VIEW_TOOLBAR
Command20=ID_VIEW_STATUS_BAR
Command21=ID_WINDOW_SPLIT
Command22=ID_APP_ABOUT
CommandCount=22

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
Command9=ID_COM_RED
Command10=ID_COM_BLACK
Command11=ID_FILP
Command12=ID_BACK
CommandCount=12

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

