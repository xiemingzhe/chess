# Microsoft Developer Studio Project File - Name="Chess" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Chess - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Chess.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Chess.mak" CFG="Chess - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Chess - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Chess - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Chess - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Chess - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Msimg32.lib Winmm.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Chess - Win32 Release"
# Name "Chess - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Chess.cpp
# End Source File
# Begin Source File

SOURCE=.\Chess.rc
# End Source File
# Begin Source File

SOURCE=.\ChessDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\ChessInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\ChessView.cpp
# End Source File
# Begin Source File

SOURCE=.\GeneratorMove.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\SetLevel.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Chess.h
# End Source File
# Begin Source File

SOURCE=.\ChessDoc.h
# End Source File
# Begin Source File

SOURCE=.\ChessInfo.h
# End Source File
# Begin Source File

SOURCE=.\ChessView.h
# End Source File
# Begin Source File

SOURCE=.\define1.h
# End Source File
# Begin Source File

SOURCE=.\define4.h
# End Source File
# Begin Source File

SOURCE=.\define5.h
# End Source File
# Begin Source File

SOURCE=.\extern.h
# End Source File
# Begin Source File

SOURCE=.\GeneratorMove.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SetLevel.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\B_BISHOP.BMP
# End Source File
# Begin Source File

SOURCE=.\res\B_CANNON.BMP
# End Source File
# Begin Source File

SOURCE=.\res\B_CAR.BMP
# End Source File
# Begin Source File

SOURCE=.\res\B_ELEPHANT.BMP
# End Source File
# Begin Source File

SOURCE=.\res\B_HORSE.BMP
# End Source File
# Begin Source File

SOURCE=.\res\B_KING.BMP
# End Source File
# Begin Source File

SOURCE=.\res\B_PAWN.BMP
# End Source File
# Begin Source File

SOURCE=.\res\BA.BMP
# End Source File
# Begin Source File

SOURCE=.\res\BB.BMP
# End Source File
# Begin Source File

SOURCE=.\res\BC.BMP
# End Source File
# Begin Source File

SOURCE=.\res\BK.BMP
# End Source File
# Begin Source File

SOURCE=.\res\BN.BMP
# End Source File
# Begin Source File

SOURCE=.\res\BOARD.BMP
# End Source File
# Begin Source File

SOURCE=.\res\BP.BMP
# End Source File
# Begin Source File

SOURCE=.\res\BR.BMP
# End Source File
# Begin Source File

SOURCE=.\res\Chess.ico
# End Source File
# Begin Source File

SOURCE=.\res\Chess.rc2
# End Source File
# Begin Source File

SOURCE=.\res\ChessDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\IDB_BA.bmp
# End Source File
# Begin Source File

SOURCE=.\res\IDB_BB.bmp
# End Source File
# Begin Source File

SOURCE=.\res\IDB_BC.bmp
# End Source File
# Begin Source File

SOURCE=.\res\IDB_BK.bmp
# End Source File
# Begin Source File

SOURCE=.\res\IDB_BN.bmp
# End Source File
# Begin Source File

SOURCE=.\res\IDB_BOARD.bmp
# End Source File
# Begin Source File

SOURCE=.\res\IDB_BP.bmp
# End Source File
# Begin Source File

SOURCE=.\res\IDB_BR.bmp
# End Source File
# Begin Source File

SOURCE=.\res\IDB_RA.bmp
# End Source File
# Begin Source File

SOURCE=.\res\IDB_RB.bmp
# End Source File
# Begin Source File

SOURCE=.\res\IDB_RC.bmp
# End Source File
# Begin Source File

SOURCE=.\res\IDB_RK.bmp
# End Source File
# Begin Source File

SOURCE=.\res\IDB_RN.bmp
# End Source File
# Begin Source File

SOURCE=.\res\IDB_RP.bmp
# End Source File
# Begin Source File

SOURCE=.\res\IDB_RR.bmp
# End Source File
# Begin Source File

SOURCE=.\res\IDB_SELECTED.bmp
# End Source File
# Begin Source File

SOURCE=.\res\R_BISHOP.BMP
# End Source File
# Begin Source File

SOURCE=.\res\R_CANNON.BMP
# End Source File
# Begin Source File

SOURCE=.\res\R_CAR.BMP
# End Source File
# Begin Source File

SOURCE=.\res\R_ELEPHANT.BMP
# End Source File
# Begin Source File

SOURCE=.\res\R_HORSE.BMP
# End Source File
# Begin Source File

SOURCE=.\res\R_KING.BMP
# End Source File
# Begin Source File

SOURCE=.\res\R_PAWN.BMP
# End Source File
# Begin Source File

SOURCE=.\res\RA.BMP
# End Source File
# Begin Source File

SOURCE=.\res\RB.BMP
# End Source File
# Begin Source File

SOURCE=.\res\RC.BMP
# End Source File
# Begin Source File

SOURCE=.\res\RK.BMP
# End Source File
# Begin Source File

SOURCE=.\res\RN.BMP
# End Source File
# Begin Source File

SOURCE=.\res\RP.BMP
# End Source File
# Begin Source File

SOURCE=.\res\RR.BMP
# End Source File
# Begin Source File

SOURCE=.\res\SELECTED.BMP
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\res\sounds\CAPTURE.WAV
# End Source File
# Begin Source File

SOURCE=.\res\sounds\CAPTURE2.WAV
# End Source File
# Begin Source File

SOURCE=.\res\sounds\CHECK.WAV
# End Source File
# Begin Source File

SOURCE=.\res\sounds\CHECK2.WAV
# End Source File
# Begin Source File

SOURCE=.\Chess.reg
# End Source File
# Begin Source File

SOURCE=.\res\sounds\CLICK.WAV
# End Source File
# Begin Source File

SOURCE=.\res\sounds\DRAW.WAV
# End Source File
# Begin Source File

SOURCE=.\res\sounds\FALL.WAV
# End Source File
# Begin Source File

SOURCE=.\res\sounds\ILLEGAL.WAV
# End Source File
# Begin Source File

SOURCE=.\res\sounds\LOSS.WAV
# End Source File
# Begin Source File

SOURCE=.\res\sounds\MOVE.WAV
# End Source File
# Begin Source File

SOURCE=.\res\sounds\MOVE2.WAV
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\res\sounds\WIN.WAV
# End Source File
# End Target
# End Project
