
# =========================================================================
#     This makefile was generated by
#     Bakefile 0.1.1 (http://bakefile.sourceforge.net)
#     Do not modify, all changes will be overwritten!
# =========================================================================


# -------------------------------------------------------------------------
# These are configurable options:
# -------------------------------------------------------------------------

# Type of compiled binaries [debug,release]
BUILD = release
# Name of your custom configuration. This affects directory
#            where object files are stored as well as the location of
#            compiled .lib files and setup.h under the lib/ toplevel directory. 
CFG = 
# Standard preprocessor flags (common for CC and CXX) 
CPPFLAGS = 
# C++ compiler 
CXX = wpp386
# Standard flags for C++ 
CXXFLAGS = 
# Standard linker flags 
LDFLAGS = 
# Multiple libraries or single huge monolithic one? [0,1]
MONOLITHIC = 1
# Is this official build by wxWindows developers? [0,1]
OFFICIAL_BUILD = 0
# What type of library to build? [0,1]
SHARED = 1
# Compile Unicode build of wxWindows? [0,1]
UNICODE = 0
# Build GUI libraries? [0,1]
USE_GUI = 1
# Build wxUniversal instead of native port? [0,1]
WXUNIV = 0

# -------------------------------------------------------------------------
# Do not modify the rest of this file!
# -------------------------------------------------------------------------

# Speed up compilation a bit:
!ifdef __LOADDLL__
!  loaddll wcc      wccd
!  loaddll wccaxp   wccdaxp
!  loaddll wcc386   wccd386
!  loaddll wpp      wppdi86
!  loaddll wppaxp   wppdaxp
!  loaddll wpp386   wppd386
!  loaddll wlink    wlink
!  loaddll wlib     wlibd
!endif

# We need these variables in some bakefile-made rules:
WATCOM_CWD = $+ $(%cdrive):$(%cwd) $-

### Conditionally set variables: ###

PORTNAME =
!ifeq USE_GUI 0
PORTNAME = base
!endif
!ifeq USE_GUI 1
PORTNAME = msw
!endif
WXDEBUGFLAG =
!ifeq BUILD debug
WXDEBUGFLAG = d
!endif
WXDLLFLAG =
!ifeq SHARED 1
WXDLLFLAG = dll
!endif
WXUNICODEFLAG =
!ifeq UNICODE 1
WXUNICODEFLAG = u
!endif
WXUNIVNAME =
!ifeq WXUNIV 1
WXUNIVNAME = univ
!endif
__DEBUGFLAG_0 =
!ifeq BUILD debug
__DEBUGFLAG_0 = -d2
!endif
!ifeq BUILD release
__DEBUGFLAG_0 = -d0
!endif
__DEBUGFLAG_1 =
!ifeq BUILD debug
__DEBUGFLAG_1 = debug all
!endif
!ifeq BUILD release
__DEBUGFLAG_1 = 
!endif
__DEBUG_DEFINE_p =
!ifeq BUILD debug
__DEBUG_DEFINE_p = -d__WXDEBUG__
!endif
__DLLFLAG_p =
!ifeq SHARED 1
__DLLFLAG_p = -dWXUSINGDLL
!endif
__MULTILIB_FLAG_p =
!ifeq MONOLITHIC 0
__MULTILIB_FLAG_p = -dwxUSE_GUI=0
!endif
__OPTIMIZEFLAG_2 =
!ifeq BUILD debug
__OPTIMIZEFLAG_2 = -od
!endif
!ifeq BUILD release
__OPTIMIZEFLAG_2 = -ot -ox
!endif
__UNICODE_DEFINE_p =
!ifeq UNICODE 1
__UNICODE_DEFINE_p = -dwxUSE_UNICODE=1
!endif
__WXLIB_BASE_p =
!ifeq MONOLITHIC 0
__WXLIB_BASE_p = wxbase25$(WXUNICODEFLAG)$(WXDEBUGFLAG).lib
!endif
__WXLIB_MONO_p =
!ifeq MONOLITHIC 1
__WXLIB_MONO_p = wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG).lib
!endif
__WXUNIV_DEFINE_p =
!ifeq WXUNIV 1
__WXUNIV_DEFINE_p = -d__WXUNIVERSAL__
!endif

### Variables: ###

OBJS = wat_$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WXDLLFLAG)$(CFG)
CONSOLE_CXXFLAGS = $(CPPFLAGS)  $(__DEBUGFLAG_0) $(__OPTIMIZEFLAG_2) -bm -br  -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) $(__UNICODE_DEFINE_p) -i=.\..\..\include -i=.\..\..\lib\wat_$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WXDLLFLAG)$(CFG) -i=.\..\..\src\tiff -i=.\..\..\src\jpeg -i=.\..\..\src\png -i=.\..\..\src\zlib  -i=.\..\..\src\regex -i=. $(__DLLFLAG_p) $(__MULTILIB_FLAG_p) $(CXXFLAGS)



all : $(OBJS)
$(OBJS) :
	-if not exist $(OBJS) mkdir $(OBJS)

### Targets: ###

all : .SYMBOLIC $(OBJS)\console.exe data

$(OBJS)\console_console.obj :  .AUTODEPEND .\console.cpp
	$(CXX) -zq -fo=$^@ $(CONSOLE_CXXFLAGS) $<

clean : .SYMBOLIC 
	-if exist $(OBJS)\*.obj del $(OBJS)\*.obj
	-if exist $(OBJS)\*.res del $(OBJS)\*.res
	-if exist $(OBJS)\*.lbc del $(OBJS)\*.lbc
	-if exist $(OBJS)\*.ilk del $(OBJS)\*.ilk
	-if exist $(OBJS)\console.exe del $(OBJS)\console.exe

$(OBJS)\console.exe :  $(OBJS)\console_console.obj
	@%create $(OBJS)\console.lbc
	@%append $(OBJS)\console.lbc option quiet
	@%append $(OBJS)\console.lbc name $^@
	@%append $(OBJS)\console.lbc option incremental
	@%append $(OBJS)\console.lbc $(LDFLAGS) $(__DEBUGFLAG_1)  libpath .\..\..\lib\wat_$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WXDLLFLAG)$(CFG)  system nt ref 'main_'
	@for %i in ( $(OBJS)\console_console.obj) do @%append $(OBJS)\console.lbc file %i
	@for %i in ( $(__WXLIB_MONO_p) $(__WXLIB_BASE_p) wxtiff$(WXDEBUGFLAG).lib wxjpeg$(WXDEBUGFLAG).lib wxpng$(WXDEBUGFLAG).lib wxzlib$(WXDEBUGFLAG).lib  wxregex$(WXDEBUGFLAG).lib  kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib) do @%append $(OBJS)\console.lbc library %i
	@%append $(OBJS)\console.lbc
	wlink @$(OBJS)\console.lbc

data :  
	if not exist $(OBJS) mkdir $(OBJS)
	for %f in (testdata.fc) do if not exist $(OBJS)\%f copy .\%f $(OBJS)

install : .SYMBOLIC all

uninstall : .SYMBOLIC 
