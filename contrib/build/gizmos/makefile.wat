# =========================================================================
#     This makefile was generated by
#     Bakefile 0.1.3 (http://bakefile.sourceforge.net)
#     Do not modify, all changes will be overwritten!
# =========================================================================

!include ../../../build/msw/config.wat

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
VENDORTAG =
!ifeq OFFICIAL_BUILD 0
VENDORTAG = _$(VENDOR)
!endif
!ifeq OFFICIAL_BUILD 1
VENDORTAG = 
!endif
WXDEBUGFLAG =
!ifeq BUILD debug
!ifeq DEBUG_FLAG default
WXDEBUGFLAG = d
!endif
!endif
!ifeq DEBUG_FLAG 1
WXDEBUGFLAG = d
!endif
WXUNICODEFLAG =
!ifeq UNICODE 1
WXUNICODEFLAG = u
!endif
WXUNIVNAME =
!ifeq WXUNIV 1
WXUNIVNAME = univ
!endif
WXDLLFLAG =
!ifeq SHARED 1
WXDLLFLAG = dll
!endif
__gizmosdll___depname =
!ifeq SHARED 1
__gizmosdll___depname = &
	$(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)251$(WXUNICODEFLAG)$(WXDEBUGFLAG)_gizmos_wat$(VENDORTAG).dll
!endif
__DEBUGINFO_1 =
!ifeq BUILD debug
!ifeq DEBUG_INFO default
__DEBUGINFO_1 = debug all
!endif
!endif
!ifeq BUILD release
!ifeq DEBUG_INFO default
__DEBUGINFO_1 = 
!endif
!endif
!ifeq DEBUG_INFO 0
__DEBUGINFO_1 = 
!endif
!ifeq DEBUG_INFO 1
__DEBUGINFO_1 = debug all
!endif
__WXLIB_MONO_p =
!ifeq MONOLITHIC 1
__WXLIB_MONO_p = &
	wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG).lib
!endif
__LIB_TIFF_p =
!ifeq USE_GUI 1
__LIB_TIFF_p = wxtiff$(WXDEBUGFLAG).lib
!endif
__LIB_JPEG_p =
!ifeq USE_GUI 1
__LIB_JPEG_p = wxjpeg$(WXDEBUGFLAG).lib
!endif
__LIB_PNG_p =
!ifeq USE_GUI 1
__LIB_PNG_p = wxpng$(WXDEBUGFLAG).lib
!endif
__WXLIB_BASE_p =
!ifeq MONOLITHIC 0
__WXLIB_BASE_p = wxbase25$(WXUNICODEFLAG)$(WXDEBUGFLAG).lib
!endif
__WXLIB_CORE_p =
!ifeq MONOLITHIC 0
__WXLIB_CORE_p = &
	wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_core.lib
!endif
__gizmoslib___depname =
!ifeq SHARED 0
__gizmoslib___depname = &
	$(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_gizmos.lib
!endif
__DEBUGINFO =
!ifeq BUILD debug
!ifeq DEBUG_INFO default
__DEBUGINFO = -d2
!endif
!endif
!ifeq BUILD release
!ifeq DEBUG_INFO default
__DEBUGINFO = -d0
!endif
!endif
!ifeq DEBUG_INFO 0
__DEBUGINFO = -d0
!endif
!ifeq DEBUG_INFO 1
__DEBUGINFO = -d2
!endif
__OPTIMIZEFLAG =
!ifeq BUILD debug
__OPTIMIZEFLAG = -od
!endif
!ifeq BUILD release
__OPTIMIZEFLAG = -ot -ox
!endif
__RUNTIME_LIBS =
!ifeq RUNTIME_LIBS dynamic
__RUNTIME_LIBS = -br
!endif
!ifeq RUNTIME_LIBS static
__RUNTIME_LIBS = 
!endif
__EXCEPTIONSFLAG =
!ifeq USE_EXCEPTIONS 0
__EXCEPTIONSFLAG = 
!endif
!ifeq USE_EXCEPTIONS 1
__EXCEPTIONSFLAG = -xs
!endif
__WXUNIV_DEFINE_p =
!ifeq WXUNIV 1
__WXUNIV_DEFINE_p = -d__WXUNIVERSAL__
!endif
__DEBUG_DEFINE_p =
!ifeq BUILD debug
!ifeq DEBUG_FLAG default
__DEBUG_DEFINE_p = -d__WXDEBUG__
!endif
!endif
!ifeq DEBUG_FLAG 1
__DEBUG_DEFINE_p = -d__WXDEBUG__
!endif
__UNICODE_DEFINE_p =
!ifeq UNICODE 1
__UNICODE_DEFINE_p = -d_UNICODE
!endif
LIBDIRNAME =
!ifeq SHARED 0
LIBDIRNAME = ..\..\src\gizmos\..\..\..\lib\wat_lib$(CFG)
!endif
!ifeq SHARED 1
LIBDIRNAME = ..\..\src\gizmos\..\..\..\lib\wat_dll$(CFG)
!endif

### Variables: ###

OBJS = &
	wat_$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WXDLLFLAG)$(CFG)
SETUPHDIR = &
	$(LIBDIRNAME)\$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)
GIZMOSDLL_CXXFLAGS = -bd $(__DEBUGINFO) $(__OPTIMIZEFLAG) -bm $(__RUNTIME_LIBS) &
	-d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) $(__UNICODE_DEFINE_p) &
	-i=..\..\src\gizmos\..\..\..\include -i=$(SETUPHDIR) &
	-i=..\..\src\gizmos\..\..\include -dWXUSINGDLL -dWXMAKINGDLL_GIZMOS &
	/fh=$(OBJS)\wxprec_gizmosdll.pch $(__EXCEPTIONSFLAG) $(CPPFLAGS) &
	$(CXXFLAGS)
GIZMOSDLL_OBJECTS =  &
	$(OBJS)\gizmosdll_dummy.obj &
	$(OBJS)\gizmosdll_dynamicsash.obj &
	$(OBJS)\gizmosdll_editlbox.obj &
	$(OBJS)\gizmosdll_ledctrl.obj &
	$(OBJS)\gizmosdll_multicell.obj &
	$(OBJS)\gizmosdll_splittree.obj &
	$(OBJS)\gizmosdll_statpict.obj
GIZMOSLIB_CXXFLAGS = $(__DEBUGINFO) $(__OPTIMIZEFLAG) -bm $(__RUNTIME_LIBS) &
	-d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) $(__UNICODE_DEFINE_p) &
	-i=..\..\src\gizmos\..\..\..\include -i=$(SETUPHDIR) &
	-i=..\..\src\gizmos\..\..\include /fh=$(OBJS)\wxprec_gizmoslib.pch &
	$(__EXCEPTIONSFLAG) $(CPPFLAGS) $(CXXFLAGS)
GIZMOSLIB_OBJECTS =  &
	$(OBJS)\gizmoslib_dummy.obj &
	$(OBJS)\gizmoslib_dynamicsash.obj &
	$(OBJS)\gizmoslib_editlbox.obj &
	$(OBJS)\gizmoslib_ledctrl.obj &
	$(OBJS)\gizmoslib_multicell.obj &
	$(OBJS)\gizmoslib_splittree.obj &
	$(OBJS)\gizmoslib_statpict.obj


all : $(OBJS)
$(OBJS) :
	-if not exist $(OBJS) mkdir $(OBJS)

### Targets: ###

all : .SYMBOLIC $(__gizmosdll___depname) $(__gizmoslib___depname)

clean : .SYMBOLIC 
	-if exist $(OBJS)\*.obj del $(OBJS)\*.obj
	-if exist $(OBJS)\*.res del $(OBJS)\*.res
	-if exist $(OBJS)\*.lbc del $(OBJS)\*.lbc
	-if exist $(OBJS)\*.ilk del $(OBJS)\*.ilk
	-if exist $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)251$(WXUNICODEFLAG)$(WXDEBUGFLAG)_gizmos_wat$(VENDORTAG).dll del $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)251$(WXUNICODEFLAG)$(WXDEBUGFLAG)_gizmos_wat$(VENDORTAG).dll
	-if exist $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_gizmos.lib del $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_gizmos.lib
	-if exist $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_gizmos.lib del $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_gizmos.lib

!ifeq SHARED 1
$(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)251$(WXUNICODEFLAG)$(WXDEBUGFLAG)_gizmos_wat$(VENDORTAG).dll :  $(GIZMOSDLL_OBJECTS)
	@%create $(OBJS)\gizmosdll.lbc
	@%append $(OBJS)\gizmosdll.lbc option quiet
	@%append $(OBJS)\gizmosdll.lbc name $^@
	@%append $(OBJS)\gizmosdll.lbc option caseexact
	@%append $(OBJS)\gizmosdll.lbc $(LDFLAGS) $(__DEBUGINFO_1)  libpath $(LIBDIRNAME)
	@for %i in ($(GIZMOSDLL_OBJECTS)) do @%append $(OBJS)\gizmosdll.lbc file %i
	@for %i in ( $(__WXLIB_MONO_p) $(__LIB_TIFF_p) $(__LIB_JPEG_p) $(__LIB_PNG_p) wxzlib$(WXDEBUGFLAG).lib  wxregex$(WXUNICODEFLAG)$(WXDEBUGFLAG).lib wxexpat$(WXDEBUGFLAG).lib   kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib  $(__WXLIB_BASE_p)  $(__WXLIB_CORE_p) ) do @%append $(OBJS)\gizmosdll.lbc library %i
	@%append $(OBJS)\gizmosdll.lbc
	@%append $(OBJS)\gizmosdll.lbc system nt_dll
	wlink @$(OBJS)\gizmosdll.lbc
	wlib -q -n -b $(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_gizmos.lib +$^@
!endif

!ifeq SHARED 0
$(LIBDIRNAME)\wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_gizmos.lib :  $(GIZMOSLIB_OBJECTS)
	@%create $(OBJS)\gizmoslib.lbc
	@for %i in ($(GIZMOSLIB_OBJECTS)) do @%append $(OBJS)\gizmoslib.lbc +%i
	wlib -q -p4096 -n -b $^@ @$(OBJS)\gizmoslib.lbc
!endif

$(OBJS)\gizmosdll_dummy.obj :  .AUTODEPEND ../../src/gizmos\..\..\..\src\msw\dummy.cpp
	$(CXX) -zq -fo=$^@ $(GIZMOSDLL_CXXFLAGS) $<

$(OBJS)\gizmosdll_dynamicsash.obj :  .AUTODEPEND ../../src/gizmos\dynamicsash.cpp
	$(CXX) -zq -fo=$^@ $(GIZMOSDLL_CXXFLAGS) $<

$(OBJS)\gizmosdll_editlbox.obj :  .AUTODEPEND ../../src/gizmos\editlbox.cpp
	$(CXX) -zq -fo=$^@ $(GIZMOSDLL_CXXFLAGS) $<

$(OBJS)\gizmosdll_ledctrl.obj :  .AUTODEPEND ../../src/gizmos\ledctrl.cpp
	$(CXX) -zq -fo=$^@ $(GIZMOSDLL_CXXFLAGS) $<

$(OBJS)\gizmosdll_multicell.obj :  .AUTODEPEND ../../src/gizmos\multicell.cpp
	$(CXX) -zq -fo=$^@ $(GIZMOSDLL_CXXFLAGS) $<

$(OBJS)\gizmosdll_splittree.obj :  .AUTODEPEND ../../src/gizmos\splittree.cpp
	$(CXX) -zq -fo=$^@ $(GIZMOSDLL_CXXFLAGS) $<

$(OBJS)\gizmosdll_statpict.obj :  .AUTODEPEND ../../src/gizmos\statpict.cpp
	$(CXX) -zq -fo=$^@ $(GIZMOSDLL_CXXFLAGS) $<

$(OBJS)\gizmoslib_dummy.obj :  .AUTODEPEND ../../src/gizmos\..\..\..\src\msw\dummy.cpp
	$(CXX) -zq -fo=$^@ $(GIZMOSLIB_CXXFLAGS) $<

$(OBJS)\gizmoslib_dynamicsash.obj :  .AUTODEPEND ../../src/gizmos\dynamicsash.cpp
	$(CXX) -zq -fo=$^@ $(GIZMOSLIB_CXXFLAGS) $<

$(OBJS)\gizmoslib_editlbox.obj :  .AUTODEPEND ../../src/gizmos\editlbox.cpp
	$(CXX) -zq -fo=$^@ $(GIZMOSLIB_CXXFLAGS) $<

$(OBJS)\gizmoslib_ledctrl.obj :  .AUTODEPEND ../../src/gizmos\ledctrl.cpp
	$(CXX) -zq -fo=$^@ $(GIZMOSLIB_CXXFLAGS) $<

$(OBJS)\gizmoslib_multicell.obj :  .AUTODEPEND ../../src/gizmos\multicell.cpp
	$(CXX) -zq -fo=$^@ $(GIZMOSLIB_CXXFLAGS) $<

$(OBJS)\gizmoslib_splittree.obj :  .AUTODEPEND ../../src/gizmos\splittree.cpp
	$(CXX) -zq -fo=$^@ $(GIZMOSLIB_CXXFLAGS) $<

$(OBJS)\gizmoslib_statpict.obj :  .AUTODEPEND ../../src/gizmos\statpict.cpp
	$(CXX) -zq -fo=$^@ $(GIZMOSLIB_CXXFLAGS) $<
