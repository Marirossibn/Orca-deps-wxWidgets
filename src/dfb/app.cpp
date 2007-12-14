/////////////////////////////////////////////////////////////////////////////
// Name:        src/dfb/app.cpp
// Purpose:     wxApp implementation
// Author:      Vaclav Slavik
//              based on MGL implementation
// Created:     2006-08-16
// RCS-ID:      $Id$
// Copyright:   (c) 2006 REA Elektronik GmbH
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/app.h"

#include "wx/evtloop.h"
#include "wx/thread.h"
#include "wx/dfb/private.h"
#include "wx/private/fontmgr.h"

//-----------------------------------------------------------------------------
// wxApp initialization
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxApp, wxEvtHandler)

wxApp::wxApp()
{
}

wxApp::~wxApp()
{
}

bool wxApp::Initialize(int& argc, wxChar **argv)
{
    if ( !wxAppBase::Initialize(argc, argv) )
        return false;

    // FIXME-UTF8: This code is taken from wxGTK and duplicated here. This
    //             is just a temporary fix to make wxDFB compile in Unicode
    //             build, the real fix is to change Initialize()'s signature
    //             to use char* on Unix.
#if wxUSE_UNICODE
    // DirectFBInit() wants UTF-8, not wchar_t, so convert
    int i;
    char **argvDFB = new char *[argc + 1];
    for ( i = 0; i < argc; i++ )
    {
        argvDFB[i] = strdup(wxConvUTF8.cWX2MB(argv[i]));
    }

    argvDFB[argc] = NULL;

    int argcDFB = argc;

    if ( !wxDfbCheckReturn(DirectFBInit(&argcDFB, &argvDFB)) )
        return false;

    if ( argcDFB != argc )
    {
        // we have to drop the parameters which were consumed by DFB+
        for ( i = 0; i < argcDFB; i++ )
        {
            while ( strcmp(wxConvUTF8.cWX2MB(argv[i]), argvDFB[i]) != 0 )
            {
                memmove(argv + i, argv + i + 1, (argc - i)*sizeof(*argv));
            }
        }

        argc = argcDFB;
    }
    //else: DirectFBInit() didn't modify our parameters

    // free our copy
    for ( i = 0; i < argcDFB; i++ )
    {
        free(argvDFB[i]);
    }

    delete [] argvDFB;

#else // ANSI

    if ( !wxDfbCheckReturn(DirectFBInit(&argc, &argv)) )
        return false;

#endif // Unicode/ANSI

    // update internal arg[cv] as DFB may have removed processed options:
    this->argc = argc;
    this->argv = argv;

    if ( !wxIDirectFB::Get() )
        return false;

    return true;
}

void wxApp::CleanUp()
{
    wxAppBase::CleanUp();

    wxFontsManager::CleanUp();

    wxEventLoop::CleanUp();
    wxIDirectFB::CleanUp();
}

//-----------------------------------------------------------------------------
// display mode
//-----------------------------------------------------------------------------

static wxVideoMode GetCurrentVideoMode()
{
    wxIDirectFBDisplayLayerPtr layer(wxIDirectFB::Get()->GetDisplayLayer());
    if ( !layer )
        return wxVideoMode(); // invalid

    return layer->GetVideoMode();
}

wxVideoMode wxApp::GetDisplayMode() const
{
    if ( !m_videoMode.IsOk() )
        wxConstCast(this, wxApp)->m_videoMode = GetCurrentVideoMode();

    return m_videoMode;
}

bool wxApp::SetDisplayMode(const wxVideoMode& mode)
{
    if ( !wxIDirectFB::Get()->SetVideoMode(mode.w, mode.h, mode.bpp) )
        return false;

    m_videoMode = mode;
    return true;
}

//-----------------------------------------------------------------------------
// events processing related
//-----------------------------------------------------------------------------

void wxApp::WakeUpIdle()
{
#if wxUSE_THREADS
    if (!wxThread::IsMain())
        wxMutexGuiEnter();
#endif

    wxEventLoopBase * const loop = wxEventLoop::GetActive();
    if ( loop )
        loop->WakeUp();

#if wxUSE_THREADS
    if (!wxThread::IsMain())
        wxMutexGuiLeave();
#endif
}


bool wxApp::Yield(bool onlyIfNeeded)
{
#if wxUSE_THREADS
    if ( !wxThread::IsMain() )
        return true; // can't process events from other threads
#endif // wxUSE_THREADS

    static bool s_inYield = false;

    if ( s_inYield )
    {
        if ( !onlyIfNeeded )
        {
            wxFAIL_MSG( wxT("wxYield called recursively" ) );
        }

        return false;
    }

    s_inYield = true;

#if wxUSE_LOG
    wxLog::Suspend();
#endif // wxUSE_LOG

    wxEventLoop * const
        loop = wx_static_cast(wxEventLoop *, wxEventLoop::GetActive());
    if ( loop )
        loop->Yield();

    // it's necessary to call ProcessIdle() to update the frames sizes which
    // might have been changed (it also will update other things set from
    // OnUpdateUI() which is a nice (and desired) side effect)
    while ( ProcessIdle() ) {}

#if wxUSE_LOG
    wxLog::Resume();
#endif // wxUSE_LOG

    s_inYield = false;

    return true;
}
