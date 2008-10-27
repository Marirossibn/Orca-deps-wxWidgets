# =========================================================================
#     This makefile was generated by
#     Bakefile 0.2.5 (http://www.bakefile.org)
#     Do not modify, all changes will be overwritten!
# =========================================================================

!include ../../build/msw/config.wat

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
LIBTYPE_SUFFIX =
!ifeq SHARED 0
LIBTYPE_SUFFIX = lib
!endif
!ifeq SHARED 1
LIBTYPE_SUFFIX = dll
!endif
EXTRALIBS_FOR_BASE =
!ifeq MONOLITHIC 0
EXTRALIBS_FOR_BASE = 
!endif
!ifeq MONOLITHIC 1
EXTRALIBS_FOR_BASE =  
!endif
__ipcclient___depname =
!ifeq USE_GUI 1
__ipcclient___depname = $(OBJS)\ipcclient.exe
!endif
__ipcserver___depname =
!ifeq USE_GUI 1
__ipcserver___depname = $(OBJS)\ipcserver.exe
!endif
__WXLIB_CORE_p =
!ifeq MONOLITHIC 0
__WXLIB_CORE_p = &
	wx$(PORTNAME)$(WXUNIVNAME)$(WX_RELEASE_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_core.lib
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
__DEBUGINFO_3 =
!ifeq BUILD debug
!ifeq DEBUG_INFO default
__DEBUGINFO_3 = debug all
!endif
!endif
!ifeq BUILD release
!ifeq DEBUG_INFO default
__DEBUGINFO_3 = 
!endif
!endif
!ifeq DEBUG_INFO 0
__DEBUGINFO_3 = 
!endif
!ifeq DEBUG_INFO 1
__DEBUGINFO_3 = debug all
!endif
__OPTIMIZEFLAG =
!ifeq BUILD debug
__OPTIMIZEFLAG = -od
!endif
!ifeq BUILD release
__OPTIMIZEFLAG = -ot -ox
!endif
__THREADSFLAG =
!ifeq USE_THREADS 0
__THREADSFLAG = 
!endif
!ifeq USE_THREADS 1
__THREADSFLAG = -bm
!endif
__RUNTIME_LIBS =
!ifeq RUNTIME_LIBS dynamic
__RUNTIME_LIBS = -br
!endif
!ifeq RUNTIME_LIBS static
__RUNTIME_LIBS = 
!endif
__RTTIFLAG =
!ifeq USE_RTTI 0
__RTTIFLAG = 
!endif
!ifeq USE_RTTI 1
__RTTIFLAG = -xr
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
__EXCEPTIONS_DEFINE_p =
!ifeq USE_EXCEPTIONS 0
__EXCEPTIONS_DEFINE_p = -dwxNO_EXCEPTIONS
!endif
__RTTI_DEFINE_p =
!ifeq USE_RTTI 0
__RTTI_DEFINE_p = -dwxNO_RTTI
!endif
__THREAD_DEFINE_p =
!ifeq USE_THREADS 0
__THREAD_DEFINE_p = -dwxNO_THREADS
!endif
__UNICODE_DEFINE_p =
!ifeq UNICODE 0
__UNICODE_DEFINE_p = -dwxUSE_UNICODE=0
!endif
!ifeq UNICODE 1
__UNICODE_DEFINE_p = -d_UNICODE
!endif
__GFXCTX_DEFINE_p =
!ifeq USE_GDIPLUS 1
__GFXCTX_DEFINE_p = -dwxUSE_GRAPHICS_CONTEXT=1
!endif
__DLLFLAG_p =
!ifeq SHARED 1
__DLLFLAG_p = -dWXUSINGDLL
!endif
__WXLIB_NET_p =
!ifeq MONOLITHIC 0
__WXLIB_NET_p = &
	wxbase$(WX_RELEASE_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR)_net.lib
!endif
__WXLIB_BASE_p =
!ifeq MONOLITHIC 0
__WXLIB_BASE_p = &
	wxbase$(WX_RELEASE_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR).lib
!endif
__WXLIB_MONO_p =
!ifeq MONOLITHIC 1
__WXLIB_MONO_p = &
	wx$(PORTNAME)$(WXUNIVNAME)$(WX_RELEASE_NODOT)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WX_LIB_FLAVOUR).lib
!endif
__GDIPLUS_LIB_p =
!ifeq USE_GDIPLUS 1
__GDIPLUS_LIB_p = gdiplus.lib
!endif

### Variables: ###

WX_RELEASE_NODOT = 29
COMPILER_PREFIX = wat
OBJS = &
	$(COMPILER_PREFIX)_$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)$(WXDLLFLAG)$(CFG)
LIBDIRNAME = .\..\..\lib\$(COMPILER_PREFIX)_$(LIBTYPE_SUFFIX)$(CFG)
SETUPHDIR = &
	$(LIBDIRNAME)\$(PORTNAME)$(WXUNIVNAME)$(WXUNICODEFLAG)$(WXDEBUGFLAG)
IPCCLIENT_CXXFLAGS = $(__DEBUGINFO) $(__OPTIMIZEFLAG) $(__THREADSFLAG) &
	$(__RUNTIME_LIBS) -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) &
	$(__EXCEPTIONS_DEFINE_p) $(__RTTI_DEFINE_p) $(__THREAD_DEFINE_p) &
	$(__UNICODE_DEFINE_p) $(__GFXCTX_DEFINE_p) -i=$(SETUPHDIR) &
	-i=.\..\..\include -wx -wcd=549 -wcd=656 -wcd=657 -wcd=667 -i=. $(__DLLFLAG_p) &
	-i=.\..\..\samples -dNOPCH $(__RTTIFLAG) $(__EXCEPTIONSFLAG) $(CPPFLAGS) &
	$(CXXFLAGS)
IPCCLIENT_OBJECTS =  &
	$(OBJS)\ipcclient_client.obj
IPCSERVER_CXXFLAGS = $(__DEBUGINFO) $(__OPTIMIZEFLAG) $(__THREADSFLAG) &
	$(__RUNTIME_LIBS) -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) &
	$(__EXCEPTIONS_DEFINE_p) $(__RTTI_DEFINE_p) $(__THREAD_DEFINE_p) &
	$(__UNICODE_DEFINE_p) $(__GFXCTX_DEFINE_p) -i=$(SETUPHDIR) &
	-i=.\..\..\include -wx -wcd=549 -wcd=656 -wcd=657 -wcd=667 -i=. $(__DLLFLAG_p) &
	-i=.\..\..\samples -dNOPCH $(__RTTIFLAG) $(__EXCEPTIONSFLAG) $(CPPFLAGS) &
	$(CXXFLAGS)
IPCSERVER_OBJECTS =  &
	$(OBJS)\ipcserver_server.obj
BASEIPCCLIENT_CXXFLAGS = $(__DEBUGINFO) $(__OPTIMIZEFLAG) $(__THREADSFLAG) &
	$(__RUNTIME_LIBS) -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) &
	$(__EXCEPTIONS_DEFINE_p) $(__RTTI_DEFINE_p) $(__THREAD_DEFINE_p) &
	$(__UNICODE_DEFINE_p) $(__GFXCTX_DEFINE_p) -i=$(SETUPHDIR) &
	-i=.\..\..\include -wx -wcd=549 -wcd=656 -wcd=657 -wcd=667 -i=. $(__DLLFLAG_p) &
	-dwxUSE_GUI=0 $(__RTTIFLAG) $(__EXCEPTIONSFLAG) $(CPPFLAGS) $(CXXFLAGS)
BASEIPCCLIENT_OBJECTS =  &
	$(OBJS)\baseipcclient_baseclient.obj
BASEIPCSERVER_CXXFLAGS = $(__DEBUGINFO) $(__OPTIMIZEFLAG) $(__THREADSFLAG) &
	$(__RUNTIME_LIBS) -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) &
	$(__EXCEPTIONS_DEFINE_p) $(__RTTI_DEFINE_p) $(__THREAD_DEFINE_p) &
	$(__UNICODE_DEFINE_p) $(__GFXCTX_DEFINE_p) -i=$(SETUPHDIR) &
	-i=.\..\..\include -wx -wcd=549 -wcd=656 -wcd=657 -wcd=667 -i=. $(__DLLFLAG_p) &
	-dwxUSE_GUI=0 $(__RTTIFLAG) $(__EXCEPTIONSFLAG) $(CPPFLAGS) $(CXXFLAGS)
BASEIPCSERVER_OBJECTS =  &
	$(OBJS)\baseipcserver_baseserver.obj


all : $(OBJS)
$(OBJS) :
	-if not exist $(OBJS) mkdir $(OBJS)

### Targets: ###

all : .SYMBOLIC $(__ipcclient___depname) $(__ipcserver___depname) $(OBJS)\baseipcclient.exe $(OBJS)\baseipcserver.exe

clean : .SYMBOLIC 
	-if exist $(OBJS)\*.obj del $(OBJS)\*.obj
	-if exist $(OBJS)\*.res del $(OBJS)\*.res
	-if exist $(OBJS)\*.lbc del $(OBJS)\*.lbc
	-if exist $(OBJS)\*.ilk del $(OBJS)\*.ilk
	-if exist $(OBJS)\*.pch del $(OBJS)\*.pch
	-if exist $(OBJS)\ipcclient.exe del $(OBJS)\ipcclient.exe
	-if exist $(OBJS)\ipcserver.exe del $(OBJS)\ipcserver.exe
	-if exist $(OBJS)\baseipcclient.exe del $(OBJS)\baseipcclient.exe
	-if exist $(OBJS)\baseipcserver.exe del $(OBJS)\baseipcserver.exe

!ifeq USE_GUI 1
$(OBJS)\ipcclient.exe :  $(IPCCLIENT_OBJECTS) $(OBJS)\ipcclient_sample.res
	@%create $(OBJS)\ipcclient.lbc
	@%append $(OBJS)\ipcclient.lbc option quiet
	@%append $(OBJS)\ipcclient.lbc name $^@
	@%append $(OBJS)\ipcclient.lbc option caseexact
	@%append $(OBJS)\ipcclient.lbc  $(__DEBUGINFO_3)  libpath $(LIBDIRNAME) system nt_win ref '_WinMain@16' $(LDFLAGS)
	@for %i in ($(IPCCLIENT_OBJECTS)) do @%append $(OBJS)\ipcclient.lbc file %i
	@for %i in ( $(__WXLIB_CORE_p)  $(__WXLIB_NET_p)  $(__WXLIB_BASE_p)  $(__WXLIB_MONO_p) $(__LIB_TIFF_p) $(__LIB_JPEG_p) $(__LIB_PNG_p)  wxzlib$(WXDEBUGFLAG).lib wxregex$(WXUNICODEFLAG)$(WXDEBUGFLAG).lib wxexpat$(WXDEBUGFLAG).lib $(EXTRALIBS_FOR_BASE)  $(__GDIPLUS_LIB_p) kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib) do @%append $(OBJS)\ipcclient.lbc library %i
	@%append $(OBJS)\ipcclient.lbc option resource=$(OBJS)\ipcclient_sample.res
	@for %i in () do @%append $(OBJS)\ipcclient.lbc option stack=%i
	wlink @$(OBJS)\ipcclient.lbc
!endif

!ifeq USE_GUI 1
$(OBJS)\ipcserver.exe :  $(IPCSERVER_OBJECTS) $(OBJS)\ipcserver_sample.res
	@%create $(OBJS)\ipcserver.lbc
	@%append $(OBJS)\ipcserver.lbc option quiet
	@%append $(OBJS)\ipcserver.lbc name $^@
	@%append $(OBJS)\ipcserver.lbc option caseexact
	@%append $(OBJS)\ipcserver.lbc  $(__DEBUGINFO_3)  libpath $(LIBDIRNAME) system nt_win ref '_WinMain@16' $(LDFLAGS)
	@for %i in ($(IPCSERVER_OBJECTS)) do @%append $(OBJS)\ipcserver.lbc file %i
	@for %i in ( $(__WXLIB_CORE_p)  $(__WXLIB_NET_p)  $(__WXLIB_BASE_p)  $(__WXLIB_MONO_p) $(__LIB_TIFF_p) $(__LIB_JPEG_p) $(__LIB_PNG_p)  wxzlib$(WXDEBUGFLAG).lib wxregex$(WXUNICODEFLAG)$(WXDEBUGFLAG).lib wxexpat$(WXDEBUGFLAG).lib $(EXTRALIBS_FOR_BASE)  $(__GDIPLUS_LIB_p) kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib) do @%append $(OBJS)\ipcserver.lbc library %i
	@%append $(OBJS)\ipcserver.lbc option resource=$(OBJS)\ipcserver_sample.res
	@for %i in () do @%append $(OBJS)\ipcserver.lbc option stack=%i
	wlink @$(OBJS)\ipcserver.lbc
!endif

$(OBJS)\baseipcclient.exe :  $(BASEIPCCLIENT_OBJECTS)
	@%create $(OBJS)\baseipcclient.lbc
	@%append $(OBJS)\baseipcclient.lbc option quiet
	@%append $(OBJS)\baseipcclient.lbc name $^@
	@%append $(OBJS)\baseipcclient.lbc option caseexact
	@%append $(OBJS)\baseipcclient.lbc  $(__DEBUGINFO_3)  libpath $(LIBDIRNAME) system nt ref 'main_' $(LDFLAGS)
	@for %i in ($(BASEIPCCLIENT_OBJECTS)) do @%append $(OBJS)\baseipcclient.lbc file %i
	@for %i in ( $(__WXLIB_NET_p)  $(__WXLIB_BASE_p)  $(__WXLIB_MONO_p) wxzlib$(WXDEBUGFLAG).lib wxregex$(WXUNICODEFLAG)$(WXDEBUGFLAG).lib wxexpat$(WXDEBUGFLAG).lib $(EXTRALIBS_FOR_BASE)  $(__GDIPLUS_LIB_p) kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib) do @%append $(OBJS)\baseipcclient.lbc library %i
	@%append $(OBJS)\baseipcclient.lbc
	@for %i in () do @%append $(OBJS)\baseipcclient.lbc option stack=%i
	wlink @$(OBJS)\baseipcclient.lbc

$(OBJS)\baseipcserver.exe :  $(BASEIPCSERVER_OBJECTS)
	@%create $(OBJS)\baseipcserver.lbc
	@%append $(OBJS)\baseipcserver.lbc option quiet
	@%append $(OBJS)\baseipcserver.lbc name $^@
	@%append $(OBJS)\baseipcserver.lbc option caseexact
	@%append $(OBJS)\baseipcserver.lbc  $(__DEBUGINFO_3)  libpath $(LIBDIRNAME) system nt ref 'main_' $(LDFLAGS)
	@for %i in ($(BASEIPCSERVER_OBJECTS)) do @%append $(OBJS)\baseipcserver.lbc file %i
	@for %i in ( $(__WXLIB_NET_p)  $(__WXLIB_BASE_p)  $(__WXLIB_MONO_p) wxzlib$(WXDEBUGFLAG).lib wxregex$(WXUNICODEFLAG)$(WXDEBUGFLAG).lib wxexpat$(WXDEBUGFLAG).lib $(EXTRALIBS_FOR_BASE)  $(__GDIPLUS_LIB_p) kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib) do @%append $(OBJS)\baseipcserver.lbc library %i
	@%append $(OBJS)\baseipcserver.lbc
	@for %i in () do @%append $(OBJS)\baseipcserver.lbc option stack=%i
	wlink @$(OBJS)\baseipcserver.lbc

$(OBJS)\ipcclient_sample.res :  .AUTODEPEND .\..\..\samples\sample.rc
	wrc -q -ad -bt=nt -r -fo=$^@    -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) $(__EXCEPTIONS_DEFINE_p) $(__RTTI_DEFINE_p) $(__THREAD_DEFINE_p) $(__UNICODE_DEFINE_p)  $(__GFXCTX_DEFINE_p) -i=$(SETUPHDIR) -i=.\..\..\include -i=. $(__DLLFLAG_p) -i=.\..\..\samples -dNOPCH $<

$(OBJS)\ipcclient_client.obj :  .AUTODEPEND .\client.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(IPCCLIENT_CXXFLAGS) $<

$(OBJS)\ipcserver_sample.res :  .AUTODEPEND .\..\..\samples\sample.rc
	wrc -q -ad -bt=nt -r -fo=$^@    -d__WXMSW__ $(__WXUNIV_DEFINE_p) $(__DEBUG_DEFINE_p) $(__EXCEPTIONS_DEFINE_p) $(__RTTI_DEFINE_p) $(__THREAD_DEFINE_p) $(__UNICODE_DEFINE_p)  $(__GFXCTX_DEFINE_p) -i=$(SETUPHDIR) -i=.\..\..\include -i=. $(__DLLFLAG_p) -i=.\..\..\samples -dNOPCH $<

$(OBJS)\ipcserver_server.obj :  .AUTODEPEND .\server.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(IPCSERVER_CXXFLAGS) $<

$(OBJS)\baseipcclient_baseclient.obj :  .AUTODEPEND .\baseclient.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(BASEIPCCLIENT_CXXFLAGS) $<

$(OBJS)\baseipcserver_baseserver.obj :  .AUTODEPEND .\baseserver.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(BASEIPCSERVER_CXXFLAGS) $<

