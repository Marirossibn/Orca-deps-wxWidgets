# =========================================================================
#     This makefile was generated by
#     Bakefile 0.1.1 (http://bakefile.sourceforge.net)
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
WXDEBUGFLAG =
!ifeq BUILD debug
!ifeq DEBUG_FLAG default
WXDEBUGFLAG = d
!endif
!endif
!ifeq DEBUG_FLAG 1
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
__DEBUGINFO =
!ifeq BUILD debug
!ifeq DEBUG_INFO default
__DEBUGINFO = debug all
!endif
!endif
!ifeq BUILD release
!ifeq DEBUG_INFO default
__DEBUGINFO = 
!endif
!endif
!ifeq DEBUG_INFO 0
__DEBUGINFO = 
!endif
!ifeq DEBUG_INFO 1
__DEBUGINFO = debug all
!endif
__DEBUGINFO_5 =
!ifeq BUILD debug
!ifeq DEBUG_INFO default
__DEBUGINFO_5 = -d2
!endif
!endif
!ifeq BUILD release
!ifeq DEBUG_INFO default
__DEBUGINFO_5 = -d0
!endif
!endif
!ifeq DEBUG_INFO 0
__DEBUGINFO_5 = -d0
!endif
!ifeq DEBUG_INFO 1
__DEBUGINFO_5 = -d2
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
__DLLFLAG_p =
!ifeq SHARED 1
__DLLFLAG_p = -dWXUSINGDLL
!endif
__LIB_JPEG_p =
!ifeq USE_GUI 1
__LIB_JPEG_p = wxjpeg$(WXDEBUGFLAG).lib
!endif
__LIB_PNG_p =
!ifeq USE_GUI 1
__LIB_PNG_p = wxpng$(WXDEBUGFLAG).lib
!endif
__LIB_TIFF_p =
!ifeq USE_GUI 1
__LIB_TIFF_p = wxtiff$(WXDEBUGFLAG).lib
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
__UNICODE_DEFINE_p =
!ifeq UNICODE 1
__UNICODE_DEFINE_p = -dwxUSE_UNICODE=1
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
__WXLIB_MONO_p =
!ifeq MONOLITHIC 1
__WXLIB_MONO_p = &
	wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG).lib
!endif
__WXUNIV_DEFINE_p =
!ifeq WXUNIV 1
__WXUNIV_DEFINE_p = -d__WXUNIVERSAL__
!endif

### Variables: ###

FL_DEMO1_CXXFLAGS = $(CPPFLAGS) $(__DEBUGINFO_5) $(__OPTIMIZEFLAG) -bm &
	$(__RUNTIME_LIBS) -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) &
	$(__UNICODE_DEFINE_p) -i=.\..\..\..\include -i=$(LIBDIRNAME) &
	-i=.\..\..\..\src\tiff -i=.\..\..\..\src\jpeg -i=.\..\..\..\src\png &
	-i=.\..\..\..\src\zlib -i=.\..\..\..\src\regex -i=.\..\..\..\src\expat\lib &
	-i=. $(__DLLFLAG_p) -i=.\..\..\include -dBMP_DIR="bitmaps/" $(CXXFLAGS)
FL_DEMO1_OBJECTS =  &
	$(OBJS)\fl_demo1_fl_demo1.obj
FL_DEMO2_CXXFLAGS = $(CPPFLAGS) $(__DEBUGINFO_5) $(__OPTIMIZEFLAG) -bm &
	$(__RUNTIME_LIBS) -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) &
	$(__UNICODE_DEFINE_p) -i=.\..\..\..\include -i=$(LIBDIRNAME) &
	-i=.\..\..\..\src\tiff -i=.\..\..\..\src\jpeg -i=.\..\..\..\src\png &
	-i=.\..\..\..\src\zlib -i=.\..\..\..\src\regex -i=.\..\..\..\src\expat\lib &
	-i=. $(__DLLFLAG_p) -i=.\..\..\include -dBMP_DIR="bitmaps/" $(CXXFLAGS)
FL_DEMO2_OBJECTS =  &
	$(OBJS)\fl_demo2_fl_demo2.obj
FL_SAMPLE1_CXXFLAGS = $(CPPFLAGS) $(__DEBUGINFO_5) $(__OPTIMIZEFLAG) -bm &
	$(__RUNTIME_LIBS) -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) &
	$(__UNICODE_DEFINE_p) -i=.\..\..\..\include -i=$(LIBDIRNAME) &
	-i=.\..\..\..\src\tiff -i=.\..\..\..\src\jpeg -i=.\..\..\..\src\png &
	-i=.\..\..\..\src\zlib -i=.\..\..\..\src\regex -i=.\..\..\..\src\expat\lib &
	-i=. $(__DLLFLAG_p) -i=.\..\..\include -dBMP_DIR="bitmaps/" $(CXXFLAGS)
FL_SAMPLE1_OBJECTS =  &
	$(OBJS)\fl_sample1_fl_sample1.obj
FL_SAMPLE2_CXXFLAGS = $(CPPFLAGS) $(__DEBUGINFO_5) $(__OPTIMIZEFLAG) -bm &
	$(__RUNTIME_LIBS) -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) &
	$(__UNICODE_DEFINE_p) -i=.\..\..\..\include -i=$(LIBDIRNAME) &
	-i=.\..\..\..\src\tiff -i=.\..\..\..\src\jpeg -i=.\..\..\..\src\png &
	-i=.\..\..\..\src\zlib -i=.\..\..\..\src\regex -i=.\..\..\..\src\expat\lib &
	-i=. $(__DLLFLAG_p) -i=.\..\..\include -dBMP_DIR="bitmaps/" $(CXXFLAGS)
FL_SAMPLE2_OBJECTS =  &
	$(OBJS)\fl_sample2_fl_sample2.obj
FL_SAMPLE3_CXXFLAGS = $(CPPFLAGS) $(__DEBUGINFO_5) $(__OPTIMIZEFLAG) -bm &
	$(__RUNTIME_LIBS) -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) &
	$(__UNICODE_DEFINE_p) -i=.\..\..\..\include -i=$(LIBDIRNAME) &
	-i=.\..\..\..\src\tiff -i=.\..\..\..\src\jpeg -i=.\..\..\..\src\png &
	-i=.\..\..\..\src\zlib -i=.\..\..\..\src\regex -i=.\..\..\..\src\expat\lib &
	-i=. $(__DLLFLAG_p) -i=.\..\..\include -dBMP_DIR="bitmaps/" $(CXXFLAGS)
FL_SAMPLE3_OBJECTS =  &
	$(OBJS)\fl_sample3_fl_sample3.obj
LIBDIRNAME = &
	.\..\..\..\lib\wat_$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WXDLLFLAG)$(CFG)
OBJS = &
	wat_$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WXDLLFLAG)$(CFG)

MAKEARGS = BUILD=$(BUILD) CFG=$(CFG) CPPFLAGS=$(CPPFLAGS) CXX=$(CXX) CXXFLAGS=$(CXXFLAGS) DEBUG_FLAG=$(DEBUG_FLAG) DEBUG_INFO=$(DEBUG_INFO) LDFLAGS=$(LDFLAGS) MONOLITHIC=$(MONOLITHIC) OFFICIAL_BUILD=$(OFFICIAL_BUILD) RUNTIME_LIBS=$(RUNTIME_LIBS) SHARED=$(SHARED) UNICODE=$(UNICODE) USE_GUI=$(USE_GUI) WXUNIV=$(WXUNIV)


all : $(OBJS)
$(OBJS) :
	-if not exist $(OBJS) mkdir $(OBJS)

### Targets: ###

all : .SYMBOLIC $(OBJS)\fl_demo1.exe $(OBJS)\fl_demo2.exe $(OBJS)\fl_sample1.exe $(OBJS)\fl_sample2.exe $(OBJS)\fl_sample3.exe

$(OBJS)\fl_demo1_fl_demo1.obj :  .AUTODEPEND .\fl_demo1.cpp
	$(CXX) -zq -fo=$^@ $(FL_DEMO1_CXXFLAGS) $<

$(OBJS)\fl_demo1_fl_demo1.res :  .AUTODEPEND .\fl_demo1.rc
	wrc -q -ad -bt=nt -r -fo=$^@ -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) $(__UNICODE_DEFINE_p) -i=.\..\..\..\include -i=$(LIBDIRNAME) -i=.\..\..\..\src\tiff -i=.\..\..\..\src\jpeg -i=.\..\..\..\src\png -i=.\..\..\..\src\zlib  -i=.\..\..\..\src\regex -i=.\..\..\..\src\expat\lib -i=. $(__DLLFLAG_p) -i=.\..\..\include -dBMP_DIR="bitmaps/" $<

$(OBJS)\fl_demo2_fl_demo2.obj :  .AUTODEPEND .\fl_demo2.cpp
	$(CXX) -zq -fo=$^@ $(FL_DEMO2_CXXFLAGS) $<

$(OBJS)\fl_demo2_fl_demo2.res :  .AUTODEPEND .\fl_demo2.rc
	wrc -q -ad -bt=nt -r -fo=$^@ -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) $(__UNICODE_DEFINE_p) -i=.\..\..\..\include -i=$(LIBDIRNAME) -i=.\..\..\..\src\tiff -i=.\..\..\..\src\jpeg -i=.\..\..\..\src\png -i=.\..\..\..\src\zlib  -i=.\..\..\..\src\regex -i=.\..\..\..\src\expat\lib -i=. $(__DLLFLAG_p) -i=.\..\..\include -dBMP_DIR="bitmaps/" $<

$(OBJS)\fl_sample1_fl_sample1.obj :  .AUTODEPEND .\fl_sample1.cpp
	$(CXX) -zq -fo=$^@ $(FL_SAMPLE1_CXXFLAGS) $<

$(OBJS)\fl_sample1_fl_sample1.res :  .AUTODEPEND .\fl_sample1.rc
	wrc -q -ad -bt=nt -r -fo=$^@ -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) $(__UNICODE_DEFINE_p) -i=.\..\..\..\include -i=$(LIBDIRNAME) -i=.\..\..\..\src\tiff -i=.\..\..\..\src\jpeg -i=.\..\..\..\src\png -i=.\..\..\..\src\zlib  -i=.\..\..\..\src\regex -i=.\..\..\..\src\expat\lib -i=. $(__DLLFLAG_p) -i=.\..\..\include -dBMP_DIR="bitmaps/" $<

$(OBJS)\fl_sample2_fl_sample2.obj :  .AUTODEPEND .\fl_sample2.cpp
	$(CXX) -zq -fo=$^@ $(FL_SAMPLE2_CXXFLAGS) $<

$(OBJS)\fl_sample2_fl_sample2.res :  .AUTODEPEND .\fl_sample2.rc
	wrc -q -ad -bt=nt -r -fo=$^@ -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) $(__UNICODE_DEFINE_p) -i=.\..\..\..\include -i=$(LIBDIRNAME) -i=.\..\..\..\src\tiff -i=.\..\..\..\src\jpeg -i=.\..\..\..\src\png -i=.\..\..\..\src\zlib  -i=.\..\..\..\src\regex -i=.\..\..\..\src\expat\lib -i=. $(__DLLFLAG_p) -i=.\..\..\include -dBMP_DIR="bitmaps/" $<

$(OBJS)\fl_sample3_fl_sample3.obj :  .AUTODEPEND .\fl_sample3.cpp
	$(CXX) -zq -fo=$^@ $(FL_SAMPLE3_CXXFLAGS) $<

$(OBJS)\fl_sample3_fl_sample3.res :  .AUTODEPEND .\fl_sample3.rc
	wrc -q -ad -bt=nt -r -fo=$^@ -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) $(__UNICODE_DEFINE_p) -i=.\..\..\..\include -i=$(LIBDIRNAME) -i=.\..\..\..\src\tiff -i=.\..\..\..\src\jpeg -i=.\..\..\..\src\png -i=.\..\..\..\src\zlib  -i=.\..\..\..\src\regex -i=.\..\..\..\src\expat\lib -i=. $(__DLLFLAG_p) -i=.\..\..\include -dBMP_DIR="bitmaps/" $<

clean : .SYMBOLIC 
	-if exist $(OBJS)\*.obj del $(OBJS)\*.obj
	-if exist $(OBJS)\*.res del $(OBJS)\*.res
	-if exist $(OBJS)\*.lbc del $(OBJS)\*.lbc
	-if exist $(OBJS)\*.ilk del $(OBJS)\*.ilk
	-if exist $(OBJS)\fl_demo1.exe del $(OBJS)\fl_demo1.exe
	-if exist $(OBJS)\fl_demo2.exe del $(OBJS)\fl_demo2.exe
	-if exist $(OBJS)\fl_sample1.exe del $(OBJS)\fl_sample1.exe
	-if exist $(OBJS)\fl_sample2.exe del $(OBJS)\fl_sample2.exe
	-if exist $(OBJS)\fl_sample3.exe del $(OBJS)\fl_sample3.exe

$(OBJS)\fl_demo1.exe :  $(FL_DEMO1_OBJECTS) $(OBJS)\fl_demo1_fl_demo1.res
	@%create $(OBJS)\fl_demo1.lbc
	@%append $(OBJS)\fl_demo1.lbc option quiet
	@%append $(OBJS)\fl_demo1.lbc name $^@
	@%append $(OBJS)\fl_demo1.lbc option incremental
	@%append $(OBJS)\fl_demo1.lbc $(LDFLAGS) $(__DEBUGINFO)  libpath $(LIBDIRNAME) system nt_win ref '_WinMain@16'
	@for %i in ($(FL_DEMO1_OBJECTS)) do @%append $(OBJS)\fl_demo1.lbc file %i
	@for %i in ( wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_fl.lib $(__WXLIB_CORE_p) $(__WXLIB_BASE_p) $(__WXLIB_MONO_p) $(__LIB_TIFF_p) $(__LIB_JPEG_p) $(__LIB_PNG_p) wxzlib$(WXDEBUGFLAG).lib  wxregex$(WXDEBUGFLAG).lib wxexpat$(WXDEBUGFLAG).lib  kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib ) do @%append $(OBJS)\fl_demo1.lbc library %i
	@%append $(OBJS)\fl_demo1.lbc option resource=$(OBJS)\fl_demo1_fl_demo1.res
	wlink @$(OBJS)\fl_demo1.lbc

$(OBJS)\fl_demo2.exe :  $(FL_DEMO2_OBJECTS) $(OBJS)\fl_demo2_fl_demo2.res
	@%create $(OBJS)\fl_demo2.lbc
	@%append $(OBJS)\fl_demo2.lbc option quiet
	@%append $(OBJS)\fl_demo2.lbc name $^@
	@%append $(OBJS)\fl_demo2.lbc option incremental
	@%append $(OBJS)\fl_demo2.lbc $(LDFLAGS) $(__DEBUGINFO)  libpath $(LIBDIRNAME) system nt_win ref '_WinMain@16'
	@for %i in ($(FL_DEMO2_OBJECTS)) do @%append $(OBJS)\fl_demo2.lbc file %i
	@for %i in ( wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_fl.lib $(__WXLIB_CORE_p) $(__WXLIB_BASE_p) $(__WXLIB_MONO_p) $(__LIB_TIFF_p) $(__LIB_JPEG_p) $(__LIB_PNG_p) wxzlib$(WXDEBUGFLAG).lib  wxregex$(WXDEBUGFLAG).lib wxexpat$(WXDEBUGFLAG).lib  kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib ) do @%append $(OBJS)\fl_demo2.lbc library %i
	@%append $(OBJS)\fl_demo2.lbc option resource=$(OBJS)\fl_demo2_fl_demo2.res
	wlink @$(OBJS)\fl_demo2.lbc

$(OBJS)\fl_sample1.exe :  $(FL_SAMPLE1_OBJECTS) $(OBJS)\fl_sample1_fl_sample1.res
	@%create $(OBJS)\fl_sample1.lbc
	@%append $(OBJS)\fl_sample1.lbc option quiet
	@%append $(OBJS)\fl_sample1.lbc name $^@
	@%append $(OBJS)\fl_sample1.lbc option incremental
	@%append $(OBJS)\fl_sample1.lbc $(LDFLAGS) $(__DEBUGINFO)  libpath $(LIBDIRNAME) system nt_win ref '_WinMain@16'
	@for %i in ($(FL_SAMPLE1_OBJECTS)) do @%append $(OBJS)\fl_sample1.lbc file %i
	@for %i in ( wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_fl.lib $(__WXLIB_CORE_p) $(__WXLIB_BASE_p) $(__WXLIB_MONO_p) $(__LIB_TIFF_p) $(__LIB_JPEG_p) $(__LIB_PNG_p) wxzlib$(WXDEBUGFLAG).lib  wxregex$(WXDEBUGFLAG).lib wxexpat$(WXDEBUGFLAG).lib  kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib ) do @%append $(OBJS)\fl_sample1.lbc library %i
	@%append $(OBJS)\fl_sample1.lbc option resource=$(OBJS)\fl_sample1_fl_sample1.res
	wlink @$(OBJS)\fl_sample1.lbc

$(OBJS)\fl_sample2.exe :  $(FL_SAMPLE2_OBJECTS) $(OBJS)\fl_sample2_fl_sample2.res
	@%create $(OBJS)\fl_sample2.lbc
	@%append $(OBJS)\fl_sample2.lbc option quiet
	@%append $(OBJS)\fl_sample2.lbc name $^@
	@%append $(OBJS)\fl_sample2.lbc option incremental
	@%append $(OBJS)\fl_sample2.lbc $(LDFLAGS) $(__DEBUGINFO)  libpath $(LIBDIRNAME) system nt_win ref '_WinMain@16'
	@for %i in ($(FL_SAMPLE2_OBJECTS)) do @%append $(OBJS)\fl_sample2.lbc file %i
	@for %i in ( wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_fl.lib $(__WXLIB_CORE_p) $(__WXLIB_BASE_p) $(__WXLIB_MONO_p) $(__LIB_TIFF_p) $(__LIB_JPEG_p) $(__LIB_PNG_p) wxzlib$(WXDEBUGFLAG).lib  wxregex$(WXDEBUGFLAG).lib wxexpat$(WXDEBUGFLAG).lib  kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib ) do @%append $(OBJS)\fl_sample2.lbc library %i
	@%append $(OBJS)\fl_sample2.lbc option resource=$(OBJS)\fl_sample2_fl_sample2.res
	wlink @$(OBJS)\fl_sample2.lbc

$(OBJS)\fl_sample3.exe :  $(FL_SAMPLE3_OBJECTS) $(OBJS)\fl_sample3_fl_sample3.res
	@%create $(OBJS)\fl_sample3.lbc
	@%append $(OBJS)\fl_sample3.lbc option quiet
	@%append $(OBJS)\fl_sample3.lbc name $^@
	@%append $(OBJS)\fl_sample3.lbc option incremental
	@%append $(OBJS)\fl_sample3.lbc $(LDFLAGS) $(__DEBUGINFO)  libpath $(LIBDIRNAME) system nt_win ref '_WinMain@16'
	@for %i in ($(FL_SAMPLE3_OBJECTS)) do @%append $(OBJS)\fl_sample3.lbc file %i
	@for %i in ( wx$(PORTNAME)$(WXUNIVNAME)25$(WXUNICODEFLAG)$(WXDEBUGFLAG)_fl.lib $(__WXLIB_CORE_p) $(__WXLIB_BASE_p) $(__WXLIB_MONO_p) $(__LIB_TIFF_p) $(__LIB_JPEG_p) $(__LIB_PNG_p) wxzlib$(WXDEBUGFLAG).lib  wxregex$(WXDEBUGFLAG).lib wxexpat$(WXDEBUGFLAG).lib  kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib odbc32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib ) do @%append $(OBJS)\fl_sample3.lbc library %i
	@%append $(OBJS)\fl_sample3.lbc option resource=$(OBJS)\fl_sample3_fl_sample3.res
	wlink @$(OBJS)\fl_sample3.lbc
