///////////////////////////////////////////////////////////////////////////////
// Name:        msw/evtloop.cpp
// Purpose:     implements wxEventLoop for MSW
// Author:      Vadim Zeitlin
// Modified by:
// Created:     01.06.01
// RCS-ID:      $Id$
// Copyright:   (c) 2001 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// License:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "evtloop.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/window.h"
    #include "wx/app.h"
#endif //WX_PRECOMP

#include "wx/evtloop.h"

#include "wx/tooltip.h"
#include "wx/except.h"
#include "wx/ptr_scpd.h"
#include "wx/scopeguard.h"

#include "wx/msw/private.h"

#if wxUSE_THREADS
    #include "wx/thread.h"

    // define the array of MSG strutures
    WX_DECLARE_OBJARRAY(MSG, wxMsgArray);

    #include "wx/arrimpl.cpp"

    WX_DEFINE_OBJARRAY(wxMsgArray);
#endif // wxUSE_THREADS

// ----------------------------------------------------------------------------
// wxEventLoopImpl
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxEventLoopImpl
{
public:
    // ctor
    wxEventLoopImpl() { SetExitCode(0); }

    // process a message
    void ProcessMessage(MSG *msg);

    // generate an idle message, return TRUE if more idle time requested
    bool SendIdleMessage();

    // set/get the exit code
    void SetExitCode(int exitcode) { m_exitcode = exitcode; }
    int GetExitCode() const { return m_exitcode; }

private:
    // preprocess a message, return TRUE if processed (i.e. no further
    // dispatching required)
    bool PreProcessMessage(MSG *msg);

    // the exit code of the event loop
    int m_exitcode;
};

// ----------------------------------------------------------------------------
// helper class
// ----------------------------------------------------------------------------

wxDEFINE_TIED_SCOPED_PTR_TYPE(wxEventLoopImpl);

// this object sets the wxEventLoop given to the ctor as the currently active
// one and unsets it in its dtor
class wxEventLoopActivator
{
public:
    wxEventLoopActivator(wxEventLoop **pActive,
                         wxEventLoop *evtLoop)
    {
        m_pActive = pActive;
        m_evtLoopOld = *pActive;
        *pActive = evtLoop;
    }

    ~wxEventLoopActivator()
    {
        // restore the previously active event loop
        *m_pActive = m_evtLoopOld;
    }

private:
    wxEventLoop *m_evtLoopOld;
    wxEventLoop **m_pActive;
};

// ============================================================================
// wxEventLoopImpl implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxEventLoopImpl message processing
// ----------------------------------------------------------------------------

void wxEventLoopImpl::ProcessMessage(MSG *msg)
{
    // give us the chance to preprocess the message first
    if ( !PreProcessMessage(msg) )
    {
        // if it wasn't done, dispatch it to the corresponding window
        ::TranslateMessage(msg);
        ::DispatchMessage(msg);
    }
}

bool wxEventLoopImpl::PreProcessMessage(MSG *msg)
{
    HWND hwnd = msg->hwnd;
    wxWindow *wndThis = wxGetWindowFromHWND((WXHWND)hwnd);

    // this may happen if the event occured in a standard modeless dialog (the
    // only example of which I know of is the find/replace dialog) - then call
    // IsDialogMessage() to make TAB navigation in it work
    if ( !wndThis )
    {
        // we need to find the dialog containing this control as
        // IsDialogMessage() just eats all the messages (i.e. returns TRUE for
        // them) if we call it for the control itself
        while ( hwnd && ::GetWindowLong(hwnd, GWL_STYLE) & WS_CHILD )
        {
            hwnd = ::GetParent(hwnd);
        }

        return hwnd && ::IsDialogMessage(hwnd, msg) != 0;
    }

#if wxUSE_TOOLTIPS
    // we must relay WM_MOUSEMOVE events to the tooltip ctrl if we want it to
    // popup the tooltip bubbles
    if ( msg->message == WM_MOUSEMOVE )
    {
        wxToolTip *tt = wndThis->GetToolTip();
        if ( tt )
        {
            tt->RelayEvent((WXMSG *)msg);
        }
    }
#endif // wxUSE_TOOLTIPS

    // allow the window to prevent certain messages from being
    // translated/processed (this is currently used by wxTextCtrl to always
    // grab Ctrl-C/V/X, even if they are also accelerators in some parent)
    if ( !wndThis->MSWShouldPreProcessMessage((WXMSG *)msg) )
    {
        return FALSE;
    }

    // try translations first: the accelerators override everything
    wxWindow *wnd;

    for ( wnd = wndThis; wnd; wnd = wnd->GetParent() )
    {
        if ( wnd->MSWTranslateMessage((WXMSG *)msg))
            return TRUE;

        // stop at first top level window, i.e. don't try to process the key
        // strokes originating in a dialog using the accelerators of the parent
        // frame - this doesn't make much sense
        if ( wnd->IsTopLevel() )
            break;
    }

    // now try the other hooks (kbd navigation is handled here): we start from
    // wndThis->GetParent() because wndThis->MSWProcessMessage() was already
    // called above
    for ( wnd = wndThis->GetParent(); wnd; wnd = wnd->GetParent() )
    {
        if ( wnd->MSWProcessMessage((WXMSG *)msg) )
            return TRUE;
    }

    // no special preprocessing for this message, dispatch it normally
    return FALSE;
}

// ----------------------------------------------------------------------------
// wxEventLoopImpl idle event processing
// ----------------------------------------------------------------------------

bool wxEventLoopImpl::SendIdleMessage()
{
    return wxTheApp->ProcessIdle();
}

// ============================================================================
// wxEventLoop implementation
// ============================================================================

wxEventLoop *wxEventLoop::ms_activeLoop = NULL;

// ----------------------------------------------------------------------------
// wxEventLoop running and exiting
// ----------------------------------------------------------------------------

wxEventLoop::~wxEventLoop()
{
    wxASSERT_MSG( !m_impl, _T("should have been deleted in Run()") );
}

bool wxEventLoop::IsRunning() const
{
    return m_impl != NULL;
}

int wxEventLoop::Run()
{
    // event loops are not recursive, you need to create another loop!
    wxCHECK_MSG( !IsRunning(), -1, _T("can't reenter a message loop") );

    // SendIdleMessage() and Dispatch() below may throw so the code here should
    // be exception-safe, hence we must use local objects for all actions we
    // should undo
    wxEventLoopActivator activate(&ms_activeLoop, this);
    wxEventLoopImplTiedPtr impl(&m_impl, new wxEventLoopImpl);

    wxON_BLOCK_EXIT_OBJ0(*this, &wxEventLoop::OnExit);

    for ( ;; )
    {
#if wxUSE_THREADS
        wxMutexGuiLeaveOrEnter();
#endif // wxUSE_THREADS

        // generate and process idle events for as long as we don't have
        // anything else to do
        while ( !Pending() && m_impl->SendIdleMessage() )
            ;

        // a message came or no more idle processing to do, sit in
        // Dispatch() waiting for the next message
        if ( !Dispatch() )
        {
            // we got WM_QUIT
            break;
        }
    }

    return m_impl->GetExitCode();
}

void wxEventLoop::Exit(int rc)
{
    wxCHECK_RET( IsRunning(), _T("can't call Exit() if not running") );

    m_impl->SetExitCode(rc);

    ::PostQuitMessage(rc);
}

// ----------------------------------------------------------------------------
// wxEventLoop message processing dispatching
// ----------------------------------------------------------------------------

bool wxEventLoop::Pending() const
{
    MSG msg;
    return ::PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE) != 0;
}

bool wxEventLoop::Dispatch()
{
    wxCHECK_MSG( IsRunning(), FALSE, _T("can't call Dispatch() if not running") );

    MSG msg;
    BOOL rc = ::GetMessage(&msg, (HWND) NULL, 0, 0);

    if ( rc == 0 )
    {
        // got WM_QUIT
        return FALSE;
    }

    if ( rc == -1 )
    {
        // should never happen, but let's test for it nevertheless
        wxLogLastError(wxT("GetMessage"));

        // still break from the loop
        return FALSE;
    }

#if wxUSE_THREADS
    wxASSERT_MSG( wxThread::IsMain(),
                  wxT("only the main thread can process Windows messages") );

    static bool s_hadGuiLock = TRUE;
    static wxMsgArray s_aSavedMessages;

    // if a secondary thread owning the mutex is doing GUI calls, save all
    // messages for later processing - we can't process them right now because
    // it will lead to recursive library calls (and we're not reentrant)
    if ( !wxGuiOwnedByMainThread() )
    {
        s_hadGuiLock = FALSE;

        // leave out WM_COMMAND messages: too dangerous, sometimes
        // the message will be processed twice
        if ( !wxIsWaitingForThread() || msg.message != WM_COMMAND )
        {
            s_aSavedMessages.Add(msg);
        }

        return TRUE;
    }
    else
    {
        // have we just regained the GUI lock? if so, post all of the saved
        // messages
        //
        // FIXME of course, it's not _exactly_ the same as processing the
        //       messages normally - expect some things to break...
        if ( !s_hadGuiLock )
        {
            s_hadGuiLock = TRUE;

            size_t count = s_aSavedMessages.Count();
            for ( size_t n = 0; n < count; n++ )
            {
                MSG& msg = s_aSavedMessages[n];
                m_impl->ProcessMessage(&msg);
            }

            s_aSavedMessages.Empty();
        }
    }
#endif // wxUSE_THREADS

    m_impl->ProcessMessage(&msg);

    return TRUE;
}

