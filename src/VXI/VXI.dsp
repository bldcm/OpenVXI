# Microsoft Developer Studio Project File - Name="VXI" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=VXI - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "VXI.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "VXI.mak" CFG="VXI - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VXI - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "VXI - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "VXI - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "build/i386-win32/Release"
# PROP Intermediate_Dir "build/i386-win32/Release/VXI"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "$(SWISBSDK)/include" /I "$(XERCESDIR)/src" /I "$(XERCESDIR)/src/xercesc" /I "$(XERCESDIR)/include" /I "$(XERCESDIR)/include/xercesc" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_UNICODE" /D COMPANY_DOMAIN=L\"com.yourcompany\" /D "HAVE_XERCES" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i "$(SWISBSDK)/include" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 SBtrd.lib VXIvalue.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib xerces-c_2.lib /nologo /dll /machine:I386 /libpath:"build/i386-win32/Release" /libpath:"$(XERCESDIR)/Build/Win32/VC6/Release" /libpath:"$(XERCESDIR)/lib" /libpath:"$(SWISBSDK)/lib"
# SUBTRACT LINK32 /map
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy build\i386-win32\Release\*.dll $(SWISBSDK)\bin	copy build\i386-win32\Release\*.lib $(SWISBSDK)\lib	copy *.xml $(SWISBSDK)\config
# End Special Build Tool

!ELSEIF  "$(CFG)" == "VXI - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "build/i386-win32/Debug"
# PROP Intermediate_Dir "build/i386-win32/Debug/VXI"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(SWISBSDK)/include" /I "$(XERCESDIR)/src" /I "$(XERCESDIR)/src/xercesc" /I "$(XERCESDIR)/include" /I "$(XERCESDIR)/include/xercesc" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_UNICODE" /D COMPANY_DOMAIN=L\"com.yourcompany\" /D "HAVE_XERCES" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /i "$(SWISBSDK)/include" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 SBtrdD.lib VXIvalueD.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib xerces-c_2D.lib /nologo /dll /map /debug /machine:I386 /out:"build/i386-win32/Debug/VXID.dll" /pdbtype:sept /libpath:"build/i386-win32/Debug" /libpath:"$(XERCESDIR)/Build/Win32/VC6/Debug" /libpath:"$(XERCESDIR)/lib" /libpath:"$(SWISBSDK)/lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy build\i386-win32\Debug\*.dll $(SWISBSDK)\bin	copy build\i386-win32\Debug\*.lib $(SWISBSDK)\lib	copy *.xml $(SWISBSDK)\config
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "VXI - Win32 Release"
# Name "VXI - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AnswerParser.cpp
# End Source File
# Begin Source File

SOURCE=.\Counters.cpp
# End Source File
# Begin Source File

SOURCE=.\DocumentConverter.cpp
# End Source File
# Begin Source File

SOURCE=.\DocumentModel.cpp
# End Source File
# Begin Source File

SOURCE=.\DocumentParser.cpp
# End Source File
# Begin Source File

SOURCE=.\DocumentStorage.cpp
# End Source File
# Begin Source File

SOURCE=.\GrammarManager.cpp
# End Source File
# Begin Source File

SOURCE=.\md5.c
# End Source File
# Begin Source File

SOURCE=.\PromptManager.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyList.cpp
# End Source File
# Begin Source File

SOURCE=.\Scripter.cpp
# End Source File
# Begin Source File

SOURCE=.\SimpleLogger.cpp
# End Source File
# Begin Source File

SOURCE=.\VXI.cpp
# End Source File
# Begin Source File

SOURCE=.\VXI_api.cpp
# End Source File
# Begin Source File

SOURCE=.\VXICacheStream.cpp
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\VXI.rc
# End Source File
# End Group
# End Target
# End Project
