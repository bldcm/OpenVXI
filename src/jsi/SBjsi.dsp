# Microsoft Developer Studio Project File - Name="SBjsi" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=SBjsi - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SBjsi.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SBjsi.mak" CFG="SBjsi - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SBjsi - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "SBjsi - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SBjsi - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "build/i386-win32/Release"
# PROP Intermediate_Dir "build/i386-win32/Release/SBjsi"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "$(SWISBSDK)/include" /I "$(SPIDERMONKEYDIR)/src" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_UNICODE" /D COMPANY_DOMAIN=L\"com.yourcompany\" /D "_win32_" /D "__THREADED" /D "XP_WIN" /D "HAVE_SPIDERMONKEY" /D "VXICHAR_SIZE_16_BITS" /D "JSI_MUST_DECLARE_VARS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i "$(SWISBSDK)/include" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 VXIvalue.lib SBtrd.lib js32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /libpath:"$(SPIDERMONKEYDIR)/src/Release" /libpath:"build/i386-win32/Release" /libpath:"$(SWISBSDK)/lib"
# SUBTRACT LINK32 /map
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy *.h $(SWISBSDK)\include	copy build\i386-win32\Release\*.dll $(SWISBSDK)\bin	copy build\i386-win32\Release\*.lib $(SWISBSDK)\lib	copy *.xml $(SWISBSDK)\config
# End Special Build Tool

!ELSEIF  "$(CFG)" == "SBjsi - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "build/i386-win32/Debug"
# PROP Intermediate_Dir "build/i386-win32/Debug/SBjsi"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "$(SWISBSDK)/include" /I "$(SPIDERMONKEYDIR)/src" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_UNICODE" /D COMPANY_DOMAIN=L\"com.yourcompany\" /D "_win32_" /D "__THREADED" /D "XP_WIN" /D "HAVE_SPIDERMONKEY" /D "VXICHAR_SIZE_16_BITS" /D "JSI_MUST_DECLARE_VARS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /i "$(SWISBSDK)/include" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 VXIvalueD.lib SBtrdD.lib js32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /map /debug /machine:I386 /out:"build/i386-win32/Debug/SBjsiD.dll" /pdbtype:sept /libpath:"$(SPIDERMONKEYDIR)/src/Debug" /libpath:"build/i386-win32/Debug" /libpath:"$(SWISBSDK)/lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy *.h $(SWISBSDK)\include	copy build\i386-win32\Debug\*.dll $(SWISBSDK)\bin	copy build\i386-win32\Debug\*.lib $(SWISBSDK)\lib	copy *.xml $(SWISBSDK)\config
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "SBjsi - Win32 Release"
# Name "SBjsi - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\JsiContext.cpp
# End Source File
# Begin Source File

SOURCE=.\JsiRuntime.cpp
# End Source File
# Begin Source File

SOURCE=.\SBjsi.cpp
# End Source File
# Begin Source File

SOURCE=.\SBjsiFuncs.cpp
# End Source File
# Begin Source File

SOURCE=.\SBjsiLogger.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\SBjsi.rc
# End Source File
# End Group
# End Target
# End Project
