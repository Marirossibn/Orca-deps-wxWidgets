/////////////////////////////////////////////////////////////////////////////
// Name:        msw/dlmsw.cpp
// Purpose:     Win32-specific part of wxDynamicLibrary and related classes
// Author:      Vadim Zeitlin
// Modified by:
// Created:     2005-01-10 (partly extracted from common/dynlib.cpp)
// RCS-ID:      $Id$
// Copyright:   (c) 1998-2005 Vadim Zeitlin <vadim@wxwindows.org>
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include  "wx/wxprec.h"

#ifdef __BORLANDC__
  #pragma hdrstop
#endif

#if wxUSE_DYNLIB_CLASS

#include "wx/msw/private.h"
#include "wx/msw/debughlp.h"

const wxChar *wxDynamicLibrary::ms_dllext = _T(".dll");

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

// wrap some functions from version.dll: load them dynamically and provide a
// clean interface
class wxVersionDLL
{
public:
    // load version.dll and bind to its functions
    wxVersionDLL();

    // return the file version as string, e.g. "x.y.z.w"
    wxString GetFileVersion(const wxString& filename) const;

private:
    typedef DWORD (APIENTRY *GetFileVersionInfoSize_t)(PTSTR, PDWORD);
    typedef BOOL (APIENTRY *GetFileVersionInfo_t)(PTSTR, DWORD, DWORD, PVOID);
    typedef BOOL (APIENTRY *VerQueryValue_t)(const PVOID, PTSTR, PVOID *, PUINT);

    #define DO_FOR_ALL_VER_FUNCS(what)                                        \
        what(GetFileVersionInfoSize);                                         \
        what(GetFileVersionInfo);                                             \
        what(VerQueryValue)

    #define DECLARE_VER_FUNCTION(func) func ## _t m_pfn ## func

    DO_FOR_ALL_VER_FUNCS(DECLARE_VER_FUNCTION);

    #undef DECLARE_VER_FUNCTION


    wxDynamicLibrary m_dll;


    DECLARE_NO_COPY_CLASS(wxVersionDLL)
};

// class used to create wxDynamicLibraryDetails objects
class WXDLLIMPEXP_BASE wxDynamicLibraryDetailsCreator
{
public:
    // type of parameters being passed to EnumModulesProc
    struct EnumModulesProcParams
    {
        wxDynamicLibraryDetailsArray *dlls;
        wxVersionDLL *verDLL;
    };

    static BOOL CALLBACK
        EnumModulesProc(PSTR name, DWORD base, ULONG size, void *data);
};

// ----------------------------------------------------------------------------
// private functions
// ----------------------------------------------------------------------------

// return the module handle for the given base name
static
HMODULE wxGetModuleHandle(const char *name, void *addr)
{
    // we want to use GetModuleHandleEx() instead of usual GetModuleHandle()
    // because the former works correctly for comctl32.dll while the latter
    // returns NULL when comctl32.dll version 6 is used under XP (note that
    // GetModuleHandleEx() is only available under XP and later, coincidence?)

    // check if we can use GetModuleHandleEx
    typedef BOOL (WINAPI *GetModuleHandleEx_t)(DWORD, LPCTSTR, HMODULE *);

    static const GetModuleHandleEx_t INVALID_FUNC_PTR = (GetModuleHandleEx_t)-1;

    static GetModuleHandleEx_t s_pfnGetModuleHandleEx = INVALID_FUNC_PTR;
    if ( s_pfnGetModuleHandleEx == INVALID_FUNC_PTR )
    {
        wxDynamicLibrary dll(_T("kernel32.dll"), wxDL_VERBATIM);
        s_pfnGetModuleHandleEx =
            (GetModuleHandleEx_t)dll.RawGetSymbol(_T("GetModuleHandleExA"));

        // dll object can be destroyed, kernel32.dll won't be unloaded anyhow
    }

    // get module handle from its address
    if ( s_pfnGetModuleHandleEx )
    {
        // flags are GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT |
        //           GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS
        HMODULE hmod;
        if ( s_pfnGetModuleHandleEx(6, (LPCTSTR)addr, &hmod) && hmod )
            return hmod;
    }

    // if failed, try by name
    return ::GetModuleHandleA(name);
}

// ============================================================================
// wxVersionDLL implementation
// ============================================================================

// ----------------------------------------------------------------------------
// loading
// ----------------------------------------------------------------------------

wxVersionDLL::wxVersionDLL()
{
    // don't give errors if DLL can't be loaded or used, we're prepared to
    // handle it
    wxLogNull noLog;

    if ( m_dll.Load(_T("version.dll"), wxDL_VERBATIM) )
    {
        // the functions we load have either 'A' or 'W' suffix depending on
        // whether we're in ANSI or Unicode build
        #ifdef UNICODE
            #define SUFFIX L"W"
        #else // ANSI
            #define SUFFIX "A"
        #endif // UNICODE/ANSI

        #define LOAD_VER_FUNCTION(name)                                       \
            m_pfn ## name = (name ## _t)m_dll.GetSymbol(_T(#name SUFFIX));    \
        if ( !m_pfn ## name )                                                 \
        {                                                                     \
            m_dll.Unload();                                                   \
            return;                                                           \
        }

        DO_FOR_ALL_VER_FUNCS(LOAD_VER_FUNCTION);

        #undef LOAD_VER_FUNCTION
    }
}

// ----------------------------------------------------------------------------
// wxVersionDLL operations
// ----------------------------------------------------------------------------

wxString wxVersionDLL::GetFileVersion(const wxString& filename) const
{
    wxString ver;
    if ( m_dll.IsLoaded() )
    {
        wxChar *pc = wx_const_cast(wxChar *, filename.c_str());

        DWORD dummy;
        DWORD sizeVerInfo = m_pfnGetFileVersionInfoSize(pc, &dummy);
        if ( sizeVerInfo )
        {
            wxCharBuffer buf(sizeVerInfo);
            if ( m_pfnGetFileVersionInfo(pc, 0, sizeVerInfo, buf.data()) )
            {
                void *pVer;
                UINT sizeInfo;
                if ( m_pfnVerQueryValue(buf.data(), _T("\\"), &pVer, &sizeInfo) )
                {
                    VS_FIXEDFILEINFO *info = (VS_FIXEDFILEINFO *)pVer;
                    ver.Printf(_T("%d.%d.%d.%d"),
                               HIWORD(info->dwFileVersionMS),
                               LOWORD(info->dwFileVersionMS),
                               HIWORD(info->dwFileVersionLS),
                               LOWORD(info->dwFileVersionLS));
                }
            }
        }
    }
    //else: we failed to load DLL, can't retrieve version info

    return ver;
}

// ============================================================================
// wxDynamicLibraryDetailsCreator implementation
// ============================================================================

/* static */
BOOL CALLBACK
wxDynamicLibraryDetailsCreator::EnumModulesProc(PSTR name,
                                                DWORD base,
                                                ULONG size,
                                                void *data)
{
    EnumModulesProcParams *params = (EnumModulesProcParams *)data;

    wxDynamicLibraryDetails *details = new wxDynamicLibraryDetails;

    // fill in simple properties
    details->m_name = wxString::FromAscii(name);
    details->m_address = wx_reinterpret_cast(void *, base);
    details->m_length = size;

    // to get the version, we first need the full path
    HMODULE hmod = wxGetModuleHandle(name, (void *)base);
    if ( hmod )
    {
        wxString fullname = wxGetFullModuleName(hmod);
        if ( !fullname.empty() )
        {
            details->m_path = fullname;
            details->m_version = params->verDLL->GetFileVersion(fullname);
        }
    }

    params->dlls->Add(details);

    // continue enumeration (returning FALSE would have stopped it)
    return TRUE;
}

// ============================================================================
// wxDynamicLibrary implementation
// ============================================================================

// ----------------------------------------------------------------------------
// loading/unloading DLLs
// ----------------------------------------------------------------------------

/* static */
wxDllType
wxDynamicLibrary::RawLoad(const wxString& libname, int WXUNUSED(flags))
{
    return ::LoadLibrary(libname);
}

/* static */
void wxDynamicLibrary::Unload(wxDllType handle)
{
    ::FreeLibrary(handle);
}

/* static */
void *wxDynamicLibrary::RawGetSymbol(wxDllType handle, const wxString& name)
{
    return ::GetProcAddress(handle, name.ToAscii());
}

// ----------------------------------------------------------------------------
// enumerating loaded DLLs
// ----------------------------------------------------------------------------

/* static */
wxDynamicLibraryDetailsArray wxDynamicLibrary::ListLoaded()
{
    wxDynamicLibraryDetailsArray dlls;

#if wxUSE_DBGHELP
    if ( wxDbgHelpDLL::Init() )
    {
        // prepare to use functions for version info extraction
        wxVersionDLL verDLL;

        wxDynamicLibraryDetailsCreator::EnumModulesProcParams params;
        params.dlls = &dlls;
        params.verDLL = &verDLL;

        if ( !wxDbgHelpDLL::EnumerateLoadedModules
                            (
                                ::GetCurrentProcess(),
                                wxDynamicLibraryDetailsCreator::EnumModulesProc,
                                &params
                            ) )
        {
            wxLogLastError(_T("EnumerateLoadedModules"));
        }
    }
#endif // wxUSE_DBGHELP

    return dlls;
}

#endif // wxUSE_DYNLIB_CLASS

