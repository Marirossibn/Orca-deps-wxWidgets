/////////////////////////////////////////////////////////////////////////
// File:        src/msw/taskbar.cpp
// Purpose:     Implements wxTaskBarIcon class for manipulating icons on
//              the Windows task bar.
// Author:      Julian Smart
// Modified by: Vaclav Slavik
// Created:     24/3/98
// Copyright:   (c)
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_TASKBARICON

#ifndef WX_PRECOMP
    #include "wx/window.h"
    #include "wx/frame.h"
    #include "wx/utils.h"
    #include "wx/menu.h"
    #include "wx/app.h"
#endif

#include "wx/msw/wrapshl.h"

#include <string.h>
#include "wx/taskbar.h"
#include "wx/platinfo.h"
#include "wx/msw/private.h"

// initialized on demand
static UINT gs_msgTaskbar = 0;
static UINT gs_msgRestartTaskbar = 0;


wxIMPLEMENT_DYNAMIC_CLASS(wxTaskBarIcon, wxEvtHandler);

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxTaskBarIconWindow: helper window
// ----------------------------------------------------------------------------

// NB: this class serves two purposes:
//     1. win32 needs a HWND associated with taskbar icon, this provides it
//     2. we need wxTopLevelWindow so that the app doesn't exit when
//        last frame is closed but there still is a taskbar icon
class wxTaskBarIconWindow : public wxFrame
{
public:
    wxTaskBarIconWindow(wxTaskBarIcon *icon)
        : wxFrame(NULL, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0),
          m_icon(icon)
    {
    }

    WXLRESULT MSWWindowProc(WXUINT msg,
                            WXWPARAM wParam, WXLPARAM lParam) wxOVERRIDE
    {
        if (msg == gs_msgRestartTaskbar || msg == gs_msgTaskbar)
        {
            return m_icon->WindowProc(msg, wParam, lParam);
        }
        else
        {
            return wxFrame::MSWWindowProc(msg, wParam, lParam);
        }
    }

private:
    wxTaskBarIcon *m_icon;
};


// ----------------------------------------------------------------------------
// NotifyIconData: wrapper around NOTIFYICONDATA
// ----------------------------------------------------------------------------

struct NotifyIconData : public NOTIFYICONDATA
{
    NotifyIconData(WXHWND hwnd)
    {
        memset(this, 0, sizeof(NOTIFYICONDATA));

        // Since Vista there is a new member hBalloonIcon which will be used
        // if a user specified icon is specified in ShowBalloon(). For XP 
        // use the old size
        cbSize = wxPlatformInfo::Get().CheckOSVersion(6, 0)
                    ? sizeof(NOTIFYICONDATA)
                    : NOTIFYICONDATA_V2_SIZE;

        hWnd = (HWND) hwnd;
        uCallbackMessage = gs_msgTaskbar;
        uFlags = NIF_MESSAGE;

        // we use the same id for all taskbar icons as we don't need it to
        // distinguish between them
        uID = 99;
    }
};

// ----------------------------------------------------------------------------
// wxTaskBarIcon
// ----------------------------------------------------------------------------

wxTaskBarIcon::wxTaskBarIcon(wxTaskBarIconType WXUNUSED(iconType))
{
    m_win = NULL;
    m_iconAdded = false;
    RegisterWindowMessages();
}

wxTaskBarIcon::~wxTaskBarIcon()
{
    if ( m_iconAdded )
        RemoveIcon();

    if ( m_win )
    {
        // we must use delete and not Destroy() here because the latter will
        // only schedule the window to be deleted during the next idle event
        // processing but we may not get any idle events if there are no other
        // windows left in the program
        delete m_win;
    }
}

// Operations
bool wxTaskBarIcon::SetIcon(const wxIcon& icon, const wxString& tooltip)
{
    // NB: we have to create the window lazily because of backward compatibility,
    //     old applications may create a wxTaskBarIcon instance before wxApp
    //     is initialized (as samples/taskbar used to do)
    if (!m_win)
    {
        m_win = new wxTaskBarIconWindow(this);
    }

    m_icon = icon;
    m_strTooltip = tooltip;

    NotifyIconData notifyData(GetHwndOf(m_win));

    if (icon.IsOk())
    {
        notifyData.uFlags |= NIF_ICON;
        notifyData.hIcon = GetHiconOf(icon);
    }

    // set NIF_TIP even for an empty tooltip: otherwise it would be impossible
    // to remove an existing tooltip using this function
    notifyData.uFlags |= NIF_TIP;
    if ( !tooltip.empty() )
    {
        wxStrlcpy(notifyData.szTip, tooltip.t_str(), WXSIZEOF(notifyData.szTip));
    }

    bool ok = Shell_NotifyIcon(m_iconAdded ? NIM_MODIFY
                                            : NIM_ADD, &notifyData) != 0;

    if ( !ok )
    {
        wxLogLastError(wxT("Shell_NotifyIcon(NIM_MODIFY/ADD)"));
    }

    if ( !m_iconAdded && ok )
        m_iconAdded = true;

    return ok;
}

#if wxUSE_TASKBARICON_BALLOONS

bool
wxTaskBarIcon::ShowBalloon(const wxString& title,
                           const wxString& text,
                           unsigned msec,
                           int flags,
                           const wxIcon& icon)
{
    wxCHECK_MSG( m_iconAdded, false,
                    wxT("can't be used before the icon is created") );

    const HWND hwnd = GetHwndOf(m_win);

    // we need to enable version 5.0 behaviour to receive notifications about
    // the balloon disappearance
    NotifyIconData notifyData(hwnd);
    notifyData.uFlags = 0;
    notifyData.uVersion = 3 /* NOTIFYICON_VERSION for Windows 2000/XP */;

    if ( !Shell_NotifyIcon(NIM_SETVERSION, &notifyData) )
    {
        wxLogLastError(wxT("Shell_NotifyIcon(NIM_SETVERSION)"));
    }

    // do show the balloon now
    notifyData = NotifyIconData(hwnd);
    notifyData.uFlags |= NIF_INFO;
    notifyData.uTimeout = msec;
    wxStrlcpy(notifyData.szInfo, text.t_str(), WXSIZEOF(notifyData.szInfo));
    wxStrlcpy(notifyData.szInfoTitle, title.t_str(),
                WXSIZEOF(notifyData.szInfoTitle));

    wxUnusedVar(icon); // It's only unused if not supported actually.

#ifdef NIIF_LARGE_ICON
    // User specified icon is only supported since Vista
    if ( icon.IsOk() && wxPlatformInfo::Get().CheckOSVersion(6, 0) )
    {
        notifyData.hBalloonIcon = GetHiconOf(icon);
        notifyData.dwInfoFlags |= NIIF_USER | NIIF_LARGE_ICON;
    }
    else
#endif
    if ( flags & wxICON_INFORMATION )
        notifyData.dwInfoFlags |= NIIF_INFO;
    else if ( flags & wxICON_WARNING )
        notifyData.dwInfoFlags |= NIIF_WARNING;
    else if ( flags & wxICON_ERROR )
        notifyData.dwInfoFlags |= NIIF_ERROR;

    bool ok = Shell_NotifyIcon(NIM_MODIFY, &notifyData) != 0;
    if ( !ok )
    {
        wxLogLastError(wxT("Shell_NotifyIcon(NIM_MODIFY)"));
    }

    return ok;
}

#endif // wxUSE_TASKBARICON_BALLOONS

bool wxTaskBarIcon::RemoveIcon()
{
    if (!m_iconAdded)
        return false;

    m_iconAdded = false;

    NotifyIconData notifyData(GetHwndOf(m_win));

    bool ok = Shell_NotifyIcon(NIM_DELETE, &notifyData) != 0;
    if ( !ok )
    {
        wxLogLastError(wxT("Shell_NotifyIcon(NIM_DELETE)"));
    }

    return ok;
}

#if wxUSE_MENUS
bool wxTaskBarIcon::PopupMenu(wxMenu *menu)
{
    wxASSERT_MSG( m_win != NULL, wxT("taskbar icon not initialized") );

    static bool s_inPopup = false;

    if (s_inPopup)
        return false;

    s_inPopup = true;

    int         x, y;
    wxGetMousePosition(&x, &y);

    m_win->Move(x, y);

    m_win->PushEventHandler(this);

    menu->UpdateUI();

    // the SetForegroundWindow() and PostMessage() calls are needed to work
    // around Win32 bug with the popup menus shown for the notifications as
    // documented at http://support.microsoft.com/kb/q135788/
    ::SetForegroundWindow(GetHwndOf(m_win));

    bool rval = m_win->PopupMenu(menu, 0, 0);

    ::PostMessage(GetHwndOf(m_win), WM_NULL, 0, 0L);

    m_win->PopEventHandler(false);

    s_inPopup = false;

    return rval;
}
#endif // wxUSE_MENUS

void wxTaskBarIcon::RegisterWindowMessages()
{
    static bool s_registered = false;

    if ( !s_registered )
    {
        // Taskbar restart msg will be sent to us if the icon needs to be redrawn
        gs_msgRestartTaskbar = RegisterWindowMessage(wxT("TaskbarCreated"));

        // Also register the taskbar message here
        gs_msgTaskbar = ::RegisterWindowMessage(wxT("wxTaskBarIconMessage"));

        s_registered = true;
    }
}

// ----------------------------------------------------------------------------
// wxTaskBarIcon window proc
// ----------------------------------------------------------------------------

long wxTaskBarIcon::WindowProc(unsigned int msg,
                               unsigned int WXUNUSED(wParam),
                               long lParam)
{
    if ( msg == gs_msgRestartTaskbar )   // does the icon need to be redrawn?
    {
        m_iconAdded = false;
        SetIcon(m_icon, m_strTooltip);
        return 0;
    }

    // this function should only be called for gs_msg(Restart)Taskbar messages
    wxASSERT( msg == gs_msgTaskbar );

    wxEventType eventType = 0;
    switch ( lParam )
    {
        case WM_LBUTTONDOWN:
            eventType = wxEVT_TASKBAR_LEFT_DOWN;
            break;

        case WM_LBUTTONUP:
            eventType = wxEVT_TASKBAR_LEFT_UP;
            break;

        case WM_RBUTTONDOWN:
            eventType = wxEVT_TASKBAR_RIGHT_DOWN;
            break;

        case WM_RBUTTONUP:
            eventType = wxEVT_TASKBAR_RIGHT_UP;
            break;

        case WM_LBUTTONDBLCLK:
            eventType = wxEVT_TASKBAR_LEFT_DCLICK;
            break;

        case WM_RBUTTONDBLCLK:
            eventType = wxEVT_TASKBAR_RIGHT_DCLICK;
            break;

        case WM_MOUSEMOVE:
            eventType = wxEVT_TASKBAR_MOVE;
            break;

        case NIN_BALLOONTIMEOUT:
            eventType = wxEVT_TASKBAR_BALLOON_TIMEOUT;
            break;

        case NIN_BALLOONUSERCLICK:
            eventType = wxEVT_TASKBAR_BALLOON_CLICK;
            break;
    }

    if ( eventType )
    {
        wxTaskBarIconEvent event(eventType, this);

        ProcessEvent(event);
    }

    return 0;
}

#endif // wxUSE_TASKBARICON

