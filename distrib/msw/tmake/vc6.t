#!#############################################################################
#! File:    vc6.t
#! Purpose: tmake template file from which wxWindows.dsp is generated by running
#!          tmake -t vc6 wxwin.pro -o wxWindows.dsp
#! Author:  Vadim Zeitlin
#! Created: 14.07.99
#! Version: $Id$
#!#############################################################################
#${
    #! include the code which parses filelist.txt file and initializes
    #! %wxCommon, %wxGeneric and %wxMSW hashes.
    IncludeTemplate("filelist.t");

    #! now transform these hashes into $project tags
    foreach $file (sort keys %wxGeneric) {
        next if $wxGeneric{$file} =~ /\b(PS|G|16|U)\b/;
        $project{"WXGENERICSRCS"} .= $file . " "
    }

    foreach $file (sort keys %wxCommon) {
        next if $wxCommon{$file} =~ /\b16\b/;

        my $tag = $file =~ /\.c$/ ? "WXCSRCS" : "WXCOMMONSRCS";
        $project{$tag} .= $file . " "
    }

    foreach $file (sort keys %wxMSW) {
        next if $wxMSW{$file} =~ /\b16\b/;

        my $tag;
        if ( $wxMSW{$file} =~ /\bO\b/ ) { $tag = "WXOLESRCS" }
        else { $tag = $file =~ /\.c$/ ? "WXMSWCSRCS" : "WXMSWSRCS" }
        $project{$tag} .= $file . " "
    }

    foreach $file (sort keys %wxHTML) {
        next if $wxHTML{$file} =~ /\b16\b/;
        $project{"WXHTMLSRCS"} .= $file . " "
    }

	if ( Config("wxnodir") ) {
	    $WXDIR = ".";
	}
	else {
	    if ( $ENV{'wx'} ) { $WXDIR = "\$(wx)"; }
	    else { $WXDIR = "\$(WXWIN)"; }
	}
#$}
# Microsoft Developer Studio Project File - Name="wxWindows" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=wxWindows - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "wxWindows.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "wxWindows.mak" CFG="wxWindows - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "wxWindows - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "wxWindows - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "wxWindows - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MT" /YX /FD /c
# ADD CPP /nologo /MD /W4 /Zi /O2 /I #$ $text = "\"$WXDIR\\include\" /I \"$WXDIR\\src\\regex\" /I \"$WXDIR\\src\\zlib\" /I \"$WXDIR\\src\\jpeg\" /I \"$WXDIR\\src\\png\" /I \"$WXDIR\\src\\tiff\" /D \"NDEBUG\" /D wxUSE_GUI=1 /D WIN95=1 /D \"__WIN95__\" /D \"WIN32\" /D \"_WIN32\" /D WINVER=0x400 /D \"__WINDOWS__\" /D \"__WXMSW__\" /D \"__WIN32__\" /D \"_MT\" /Yu\"wx/wxprec.h\" /FD /c"
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "wxWindows - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" -D "_MT" /YX /FD /c
# ADD CPP /nologo /MDd /W4 /Zi /Od /I #$ $text = "\"$WXDIR\\include\" /I \"$WXDIR\\src\\regex\" /I \"$WXDIR\\src\\zlib\" /I \"$WXDIR\\src\\jpeg\" /I \"$WXDIR\\src\\png\" /I \"$WXDIR\\src\\tiff\" /D \"_DEBUG\" /D DEBUG=1 /D WXDEBUG=1 /D \"__WXDEBUG__\" /D wxUSE_GUI=1 /D \"__WIN95__\" /D \"WIN32\" /D \"_WIN32\" /D WINVER=0x400 /D \"__WINDOWS__\" /D \"__WIN32__\" /D \"__WXMSW__\" /Fr /D \"_MT\" /Yu\"wx/wxprec.h\" /FD /c"
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"lib/wxWindows.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "wxWindows - Win32 Release"
# Name "wxWindows - Win32 Debug"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\msw\dummy.cpp
# ADD CPP /Yc"wx/wxprec.h"
# End Source File
#$ ExpandGlue("WXMSWSRCS", "# Begin Source File\n\nSOURCE=.\\src\\msw\\", "\n# End Source File\n# Begin Source File\n\nSOURCE=.\\src\\msw\\", "\n# End Source File\n");
#$ ExpandGlue("WXOLESRCS", "# Begin Source File\n\nSOURCE=.\\src\\msw\\ole\\", "\n# End Source File\n# Begin Source File\n\nSOURCE=.\\src\\msw\\ole\\", "\n# End Source File\n");
#$ ExpandGlue("WXGENERICSRCS", "# Begin Source File\n\nSOURCE=.\\src\\generic\\", "\n# End Source File\n# Begin Source File\n\nSOURCE=.\\src\\generic\\", "\n# End Source File\n");
#$ ExpandGlue("WXCOMMONSRCS", "# Begin Source File\n\nSOURCE=.\\src\\common\\", "\n# End Source File\n# Begin Source File\n\nSOURCE=.\\src\\common\\", "\n# End Source File\n");
#$ ExpandGlue("WXCSRCS", "# Begin Source File\n\nSOURCE=.\\src\\common\\", "\n# SUBTRACT CPP /YX /Yc /Yu\n# End Source File\n# Begin Source File\n\nSOURCE=.\\src\\common\\", "\n# SUBTRACT CPP /YX /Yc /Yu\n# End Source File\n");
#$ ExpandGlue("WXMSWCSRCS", "# Begin Source File\n\nSOURCE=.\\src\\msw\\", "\n# SUBTRACT CPP /YX /Yc /Yu\n# End Source File\n# Begin Source File\n\nSOURCE=.\\src\\msw\\", "\n# SUBTRACT CPP /YX /Yc /Yu\n# End Source File\n");
#$ ExpandGlue("WXHTMLSRCS", "# Begin Source File\n\nSOURCE=.\\src\\html\\", "\n# End Source File\n# Begin Source File\n\nSOURCE=.\\src\\html\\", "\n# End Source File\n");

# Begin Source File

SOURCE=.\src\common\y_tab.c

!IF  "$(CFG)" == "wxWindows - Win32 Release"

# ADD CPP /W1 /D "USE_DEFINE"
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "wxWindows - Win32 Debug"

# ADD CPP /W1 /D "USE_DEFINE"
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# End Target
# End Project
