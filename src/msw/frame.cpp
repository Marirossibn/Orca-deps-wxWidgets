/////////////////////////////////////////////////////////////////////////////
// Name:        msw/frame.cpp
// Purpose:     wxFrameMSW
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart and Markus Holzem
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#ifdef __GNUG__
    #pragma implementation "frame.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/frame.h"
    #include "wx/app.h"
    #include "wx/menu.h"
    #include "wx/utils.h"
    #include "wx/dialog.h"
    #include "wx/settings.h"
    #include "wx/dcclient.h"
    #include "wx/mdi.h"
    #include "wx/panel.h"
#endif // WX_PRECOMP

#include "wx/msw/private.h"

#if wxUSE_STATUSBAR
    #include "wx/statusbr.h"
    #include "wx/generic/statusbr.h"
#endif // wxUSE_STATUSBAR

#if wxUSE_TOOLBAR
    #include "wx/toolbar.h"
#endif // wxUSE_TOOLBAR

#include "wx/menuitem.h"
#include "wx/log.h"

#ifdef __WXUNIVERSAL__
    #include "wx/univ/theme.h"
    #include "wx/univ/colschem.h"
#endif // __WXUNIVERSAL__

// ----------------------------------------------------------------------------
// globals
// ----------------------------------------------------------------------------

extern const wxChar *wxFrameClassName;

#if wxUSE_MENUS_NATIVE
extern wxMenu *wxCurrentPopupMenu;
#endif // wxUSE_MENUS_NATIVE

// ----------------------------------------------------------------------------
// event tables
// ----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(wxFrameMSW, wxFrameBase)
    EVT_ACTIVATE(wxFrameMSW::OnActivate)
    EVT_SYS_COLOUR_CHANGED(wxFrameMSW::OnSysColourChanged)
END_EVENT_TABLE()

#ifndef __WXUNIVERSAL__
    IMPLEMENT_DYNAMIC_CLASS(wxFrame, wxWindow)
#endif

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// static class members
// ----------------------------------------------------------------------------

#if wxUSE_STATUSBAR
    #if wxUSE_NATIVE_STATUSBAR
        bool wxFrameMSW::m_useNativeStatusBar = TRUE;
    #else
        bool wxFrameMSW::m_useNativeStatusBar = FALSE;
    #endif
#endif // wxUSE_NATIVE_STATUSBAR

// ----------------------------------------------------------------------------
// creation/destruction
// ----------------------------------------------------------------------------

void wxFrameMSW::Init()
{
#if wxUSE_TOOLTIPS
    m_hwndToolTip = 0;
#endif

    // Data to save/restore when calling ShowFullScreen
    m_fsStyle = 0;
    m_fsOldWindowStyle = 0;
    m_fsStatusBarFields = 0;
    m_fsStatusBarHeight = 0;
    m_fsToolBarHeight = 0;
//  m_fsMenu = 0;
    m_fsIsMaximized = FALSE;
    m_fsIsShowing = FALSE;

    m_winLastFocused = (wxWindow *)NULL;
}

bool wxFrameMSW::Create(wxWindow *parent,
                     wxWindowID id,
                     const wxString& title,
                     const wxPoint& pos,
                     const wxSize& size,
                     long style,
                     const wxString& name)
{
    if ( !wxTopLevelWindow::Create(parent, id, title, pos, size, style, name) )
        return FALSE;

    SetBackgroundColour(wxSystemSettings::GetSystemColour(wxSYS_COLOUR_APPWORKSPACE));

    wxModelessWindows.Append(this);

    return TRUE;
}

wxFrameMSW::~wxFrameMSW()
{
    m_isBeingDeleted = TRUE;

    DeleteAllBars();
}

// Get size *available for subwindows* i.e. excluding menu bar, toolbar etc.
void wxFrameMSW::DoGetClientSize(int *x, int *y) const
{
  RECT rect;
  ::GetClientRect(GetHwnd(), &rect);

#if wxUSE_STATUSBAR
  if ( GetStatusBar() && GetStatusBar()->IsShown() )
  {
    int statusX, statusY;
    GetStatusBar()->GetClientSize(&statusX, &statusY);
    rect.bottom -= statusY;
  }
#endif // wxUSE_STATUSBAR

  wxPoint pt(GetClientAreaOrigin());
  rect.bottom -= pt.y;
  rect.right -= pt.x;

  if ( x )
    *x = rect.right;
  if ( y )
    *y = rect.bottom;
}

void wxFrameMSW::DoSetClientSize(int width, int height)
{
    // leave enough space for the status bar if we have (and show) it
#if wxUSE_STATUSBAR
    wxStatusBar *statbar = GetStatusBar();
    if ( statbar && statbar->IsShown() )
    {
        height += statbar->GetSize().y;
    }
#endif // wxUSE_STATUSBAR

    wxTopLevelWindow::DoSetClientSize(width, height);
}

// ----------------------------------------------------------------------------
// wxFrameMSW: various geometry-related functions
// ----------------------------------------------------------------------------

void wxFrameMSW::Raise()
{
#ifdef __WIN16__
    // no SetForegroundWindow() in Win16
    wxFrameBase::Raise();
#else // Win32
    ::SetForegroundWindow(GetHwnd());
#endif // Win16/32
}

// generate an artificial resize event
void wxFrameMSW::SendSizeEvent()
{
    if ( !m_iconized )
    {
        RECT r = wxGetWindowRect(GetHwnd());

        (void)::PostMessage(GetHwnd(), WM_SIZE,
                            IsMaximized() ? SIZE_MAXIMIZED : SIZE_RESTORED,
                            MAKELPARAM(r.right - r.left, r.bottom - r.top));
    }
}

#if wxUSE_STATUSBAR
wxStatusBar *wxFrameMSW::OnCreateStatusBar(int number,
                                        long style,
                                        wxWindowID id,
                                        const wxString& name)
{
    wxStatusBar *statusBar = NULL;

#if wxUSE_NATIVE_STATUSBAR
    if ( !UsesNativeStatusBar() )
    {
        statusBar = (wxStatusBar *)new wxStatusBarGeneric(this, id, style);
    }
    else
#endif
    {
        statusBar = new wxStatusBar(this, id, style, name);
    }

    // Set the height according to the font and the border size
    wxClientDC dc(statusBar);
    dc.SetFont(statusBar->GetFont());

    wxCoord y;
    dc.GetTextExtent(_T("X"), NULL, &y );

    int height = (int)( (11*y)/10 + 2*statusBar->GetBorderY());

    statusBar->SetSize(-1, -1, -1, height);

    statusBar->SetFieldsCount(number);

    return statusBar;
}

void wxFrameMSW::PositionStatusBar()
{
    if ( !m_frameStatusBar )
        return;

    int w, h;
    GetClientSize(&w, &h);
    int sw, sh;
    m_frameStatusBar->GetSize(&sw, &sh);

    // Since we wish the status bar to be directly under the client area,
    // we use the adjusted sizes without using wxSIZE_NO_ADJUSTMENTS.
    m_frameStatusBar->SetSize(0, h, w, sh);
}
#endif // wxUSE_STATUSBAR

#if wxUSE_MENUS_NATIVE

void wxFrameMSW::AttachMenuBar(wxMenuBar *menubar)
{
    wxFrameBase::AttachMenuBar(menubar);

    if ( !menubar )
    {
        // actually remove the menu from the frame
        m_hMenu = (WXHMENU)0;
        InternalSetMenuBar();
    }
    else // set new non NULL menu bar
    {
        // Can set a menubar several times.
        if ( menubar->GetHMenu() )
        {
            m_hMenu = menubar->GetHMenu();
        }
        else // no HMENU yet
        {
            m_hMenu = menubar->Create();

            if ( !m_hMenu )
            {
                wxFAIL_MSG( _T("failed to create menu bar") );
                return;
            }
        }

        InternalSetMenuBar();
    }
}

void wxFrameMSW::InternalSetMenuBar()
{
#ifndef __WXMICROWIN__
    if ( !::SetMenu(GetHwnd(), (HMENU)m_hMenu) )
    {
        wxLogLastError(wxT("SetMenu"));
    }
#endif
}

#endif // wxUSE_MENUS_NATIVE

// Responds to colour changes, and passes event on to children.
void wxFrameMSW::OnSysColourChanged(wxSysColourChangedEvent& event)
{
    SetBackgroundColour(wxSystemSettings::GetSystemColour(wxSYS_COLOUR_APPWORKSPACE));
    Refresh();

#if wxUSE_STATUSBAR
    if ( m_frameStatusBar )
    {
        wxSysColourChangedEvent event2;
        event2.SetEventObject( m_frameStatusBar );
        m_frameStatusBar->GetEventHandler()->ProcessEvent(event2);
    }
#endif // wxUSE_STATUSBAR

    // Propagate the event to the non-top-level children
    wxWindow::OnSysColourChanged(event);
}

// Pass TRUE to show full screen, FALSE to restore.
bool wxFrameMSW::ShowFullScreen(bool show, long style)
{
    if (show)
    {
        if (IsFullScreen())
            return FALSE;

        m_fsIsShowing = TRUE;
        m_fsStyle = style;

#if wxUSE_TOOLBAR
        wxToolBar *theToolBar = GetToolBar();
        if (theToolBar)
            theToolBar->GetSize(NULL, &m_fsToolBarHeight);

        // zap the toolbar, menubar, and statusbar

        if ((style & wxFULLSCREEN_NOTOOLBAR) && theToolBar)
        {
            theToolBar->SetSize(-1,0);
            theToolBar->Show(FALSE);
        }
#endif // wxUSE_TOOLBAR

#ifndef __WXMICROWIN__
        if (style & wxFULLSCREEN_NOMENUBAR)
            SetMenu((HWND)GetHWND(), (HMENU) NULL);
#endif

#if wxUSE_STATUSBAR
        wxStatusBar *theStatusBar = GetStatusBar();
        if (theStatusBar)
            theStatusBar->GetSize(NULL, &m_fsStatusBarHeight);

        // Save the number of fields in the statusbar
        if ((style & wxFULLSCREEN_NOSTATUSBAR) && theStatusBar)
        {
            //m_fsStatusBarFields = theStatusBar->GetFieldsCount();
            //SetStatusBar((wxStatusBar*) NULL);
            //delete theStatusBar;
            theStatusBar->Show(FALSE);
        }
        else
            m_fsStatusBarFields = 0;
#endif // wxUSE_STATUSBAR

        // zap the frame borders

        // save the 'normal' window style
        m_fsOldWindowStyle = GetWindowLong((HWND)GetHWND(), GWL_STYLE);

        // save the old position, width & height, maximize state
        m_fsOldSize = GetRect();
        m_fsIsMaximized = IsMaximized();

        // decide which window style flags to turn off
        LONG newStyle = m_fsOldWindowStyle;
        LONG offFlags = 0;

        if (style & wxFULLSCREEN_NOBORDER)
            offFlags |= WS_BORDER | WS_THICKFRAME;
        if (style & wxFULLSCREEN_NOCAPTION)
            offFlags |= (WS_CAPTION | WS_SYSMENU);

        newStyle &= (~offFlags);

        // change our window style to be compatible with full-screen mode
        ::SetWindowLong((HWND)GetHWND(), GWL_STYLE, newStyle);

        // resize to the size of the desktop
        int width, height;

        RECT rect = wxGetWindowRect(::GetDesktopWindow());
        width = rect.right - rect.left;
        height = rect.bottom - rect.top;

        SetSize(width, height);

        // now flush the window style cache and actually go full-screen
        SetWindowPos((HWND)GetHWND(), HWND_TOP, 0, 0, width, height, SWP_FRAMECHANGED);

        wxSizeEvent event(wxSize(width, height), GetId());
        GetEventHandler()->ProcessEvent(event);

        return TRUE;
    }
    else
    {
        if (!IsFullScreen())
            return FALSE;

        m_fsIsShowing = FALSE;

#if wxUSE_TOOLBAR
        wxToolBar *theToolBar = GetToolBar();

        // restore the toolbar, menubar, and statusbar
        if (theToolBar && (m_fsStyle & wxFULLSCREEN_NOTOOLBAR))
        {
            theToolBar->SetSize(-1, m_fsToolBarHeight);
            theToolBar->Show(TRUE);
        }
#endif // wxUSE_TOOLBAR

#if wxUSE_STATUSBAR
        if ( m_fsStyle & wxFULLSCREEN_NOSTATUSBAR )
        {
            //CreateStatusBar(m_fsStatusBarFields);
            if (GetStatusBar())
            {
                GetStatusBar()->Show(TRUE);
                PositionStatusBar();
            }
        }
#endif // wxUSE_STATUSBAR

#ifndef __WXMICROWIN__
        if ((m_fsStyle & wxFULLSCREEN_NOMENUBAR) && (m_hMenu != 0))
            SetMenu((HWND)GetHWND(), (HMENU)m_hMenu);
#endif

        Maximize(m_fsIsMaximized);
        SetWindowLong((HWND)GetHWND(),GWL_STYLE, m_fsOldWindowStyle);
        SetWindowPos((HWND)GetHWND(),HWND_TOP,m_fsOldSize.x, m_fsOldSize.y,
            m_fsOldSize.width, m_fsOldSize.height, SWP_FRAMECHANGED);

        return TRUE;
    }
}

// Default activation behaviour - set the focus for the first child
// subwindow found.
void wxFrameMSW::OnActivate(wxActivateEvent& event)
{
    if ( event.GetActive() )
    {
        // restore focus to the child which was last focused
        wxLogTrace(_T("focus"), _T("wxFrameMSW %08x activated."), m_hWnd);

        wxWindow *parent = m_winLastFocused ? m_winLastFocused->GetParent()
                                            : NULL;
        if ( !parent )
        {
            parent = this;
        }

        wxSetFocusToChild(parent, &m_winLastFocused);
    }
    else // deactivating
    {
        // remember the last focused child if it is our child
        m_winLastFocused = FindFocus();

        // so we NULL it out if it's a child from some other frame
        wxWindow *win = m_winLastFocused;
        while ( win )
        {
            if ( win->IsTopLevel() )
            {
                if ( win != this )
                {
                    m_winLastFocused = NULL;
                }

                break;
            }

            win = win->GetParent();
        }

        wxLogTrace(_T("focus"),
                   _T("wxFrameMSW %08x deactivated, last focused: %08x."),
                   m_hWnd,
                   m_winLastFocused ? GetHwndOf(m_winLastFocused)
                                    : NULL);

        event.Skip();
    }
}

// ----------------------------------------------------------------------------
// tool/status bar stuff
// ----------------------------------------------------------------------------

#if wxUSE_TOOLBAR

wxToolBar* wxFrameMSW::CreateToolBar(long style, wxWindowID id, const wxString& name)
{
    if ( wxFrameBase::CreateToolBar(style, id, name) )
    {
        PositionToolBar();
    }

    return m_frameToolBar;
}

void wxFrameMSW::PositionToolBar()
{
    RECT rect;
    ::GetClientRect(GetHwnd(), &rect);

#if wxUSE_STATUSBAR
    if ( GetStatusBar() )
    {
        int statusX, statusY;
        GetStatusBar()->GetClientSize(&statusX, &statusY);
        rect.bottom -= statusY;
    }
#endif // wxUSE_STATUSBAR

    if ( GetToolBar() && GetToolBar()->IsShown() )
    {
        int tw, th;
        GetToolBar()->GetSize(&tw, &th);

        if ( GetToolBar()->GetWindowStyleFlag() & wxTB_VERTICAL )
        {
            th = rect.bottom;
        }
        else
        {
            tw = rect.right;
        }

        // Use the 'real' MSW position here
        GetToolBar()->SetSize(0, 0, tw, th, wxSIZE_NO_ADJUSTMENTS);
    }
}
#endif // wxUSE_TOOLBAR

// ----------------------------------------------------------------------------
// frame state (iconized/maximized/...)
// ----------------------------------------------------------------------------

// propagate our state change to all child frames: this allows us to emulate X
// Windows behaviour where child frames float independently of the parent one
// on the desktop, but are iconized/restored with it
void wxFrameMSW::IconizeChildFrames(bool bIconize)
{
    for ( wxWindowList::Node *node = GetChildren().GetFirst();
          node;
          node = node->GetNext() )
    {
        wxWindow *win = node->GetData();

        // iconizing the frames with this style under Win95 shell puts them at
        // the bottom of the screen (as the MDI children) instead of making
        // them appear in the taskbar because they are, by virtue of this
        // style, not managed by the taskbar - instead leave Windows take care
        // of them
#ifdef __WIN95__
        if ( win->GetWindowStyle() & wxFRAME_TOOL_WINDOW )
            continue;
#endif // Win95

        // the child MDI frames are a special case and should not be touched by
        // the parent frame - instead, they are managed by the user
        wxFrame *frame = wxDynamicCast(win, wxFrame);
        if ( frame
#if wxUSE_MDI_ARCHITECTURE
                && !wxDynamicCast(frame, wxMDIChildFrame)
#endif // wxUSE_MDI_ARCHITECTURE
           )
        {
            frame->Iconize(bIconize);
        }
    }
}

WXHICON wxFrameMSW::GetDefaultIcon() const
{
    return (WXHICON)(wxSTD_FRAME_ICON ? wxSTD_FRAME_ICON
                                      : wxDEFAULT_FRAME_ICON);
}

// ===========================================================================
// message processing
// ===========================================================================

// ---------------------------------------------------------------------------
// preprocessing
// ---------------------------------------------------------------------------

bool wxFrameMSW::MSWTranslateMessage(WXMSG* pMsg)
{
    if ( wxWindow::MSWTranslateMessage(pMsg) )
        return TRUE;

#if wxUSE_MENUS && wxUSE_ACCEL && !defined(__WXUNIVERSAL__)
    // try the menu bar accels
    wxMenuBar *menuBar = GetMenuBar();
    if ( !menuBar )
        return FALSE;

    const wxAcceleratorTable& acceleratorTable = menuBar->GetAccelTable();
    return acceleratorTable.Translate(this, pMsg);
#else
    return FALSE;
#endif // wxUSE_MENUS && wxUSE_ACCEL
}

// ---------------------------------------------------------------------------
// our private (non virtual) message handlers
// ---------------------------------------------------------------------------

bool wxFrameMSW::HandlePaint()
{
    RECT rect;
    if ( GetUpdateRect(GetHwnd(), &rect, FALSE) )
    {
#ifndef __WXMICROWIN__
        if ( m_iconized )
        {
            HICON hIcon = m_icon.Ok() ? GetHiconOf(m_icon)
                                      : (HICON)GetDefaultIcon();

            // Hold a pointer to the dc so long as the OnPaint() message
            // is being processed
            PAINTSTRUCT ps;
            HDC hdc = ::BeginPaint(GetHwnd(), &ps);

            // Erase background before painting or we get white background
            MSWDefWindowProc(WM_ICONERASEBKGND, (WORD)(LONG)ps.hdc, 0L);

            if ( hIcon )
            {
                RECT rect;
                ::GetClientRect(GetHwnd(), &rect);

                // FIXME: why hardcoded?
                static const int icon_width = 32;
                static const int icon_height = 32;

                int icon_x = (int)((rect.right - icon_width)/2);
                int icon_y = (int)((rect.bottom - icon_height)/2);

                ::DrawIcon(hdc, icon_x, icon_y, hIcon);
            }

            ::EndPaint(GetHwnd(), &ps);

            return TRUE;
        }
        else
 #endif
        {
            return wxWindow::HandlePaint();
        }
    }
    else
    {
        // nothing to paint - processed
        return TRUE;
    }
}

bool wxFrameMSW::HandleSize(int x, int y, WXUINT id)
{
    bool processed = FALSE;
#ifndef __WXMICROWIN__

    switch ( id )
    {
        case SIZENORMAL:
            // only do it it if we were iconized before, otherwise resizing the
            // parent frame has a curious side effect of bringing it under it's
            // children
            if ( !m_iconized )
                break;

            // restore all child frames too
            IconizeChildFrames(FALSE);

            (void)SendIconizeEvent(FALSE);

            // fall through

        case SIZEFULLSCREEN:
            m_iconized = FALSE;
            break;

        case SIZEICONIC:
            // iconize all child frames too
            IconizeChildFrames(TRUE);

            (void)SendIconizeEvent();

            m_iconized = TRUE;
            break;
    }
#endif

    if ( !m_iconized )
    {
#if wxUSE_STATUSBAR
        PositionStatusBar();
#endif // wxUSE_STATUSBAR

#if wxUSE_TOOLBAR
        PositionToolBar();
#endif // wxUSE_TOOLBAR

        wxSizeEvent event(wxSize(x, y), m_windowId);
        event.SetEventObject( this );
        processed = GetEventHandler()->ProcessEvent(event);
    }

    return processed;
}

bool wxFrameMSW::HandleCommand(WXWORD id, WXWORD cmd, WXHWND control)
{
    if ( control )
    {
        // In case it's e.g. a toolbar.
        wxWindow *win = wxFindWinFromHandle(control);
        if ( win )
            return win->MSWCommand(cmd, id);
    }

    // handle here commands from menus and accelerators
    if ( cmd == 0 || cmd == 1 )
    {
#if wxUSE_MENUS_NATIVE
        if ( wxCurrentPopupMenu )
        {
            wxMenu *popupMenu = wxCurrentPopupMenu;
            wxCurrentPopupMenu = NULL;

            return popupMenu->MSWCommand(cmd, id);
        }
#endif // wxUSE_MENUS_NATIVE

        if ( ProcessCommand(id) )
        {
            return TRUE;
        }
    }

    return FALSE;
}

bool wxFrameMSW::HandleMenuSelect(WXWORD nItem, WXWORD flags, WXHMENU hMenu)
{
    int item;
    if ( flags == 0xFFFF && hMenu == 0 )
    {
        // menu was removed from screen
        item = -1;
    }
#ifndef __WXMICROWIN__
    else if ( !(flags & MF_POPUP) && !(flags & MF_SEPARATOR) )
    {
        item = nItem;
    }
#endif
    else
    {
#if wxUSE_STATUSBAR
        // don't give hints for separators (doesn't make sense) nor for the
        // items opening popup menus (they don't have them anyhow) but do clear
        // the status line - otherwise, we would be left with the help message
        // for the previous item which doesn't apply any more
        wxStatusBar *statbar = GetStatusBar();
        if ( statbar )
        {
            statbar->SetStatusText(wxEmptyString);
        }
#endif // wxUSE_STATUSBAR

        return FALSE;
    }

    wxMenuEvent event(wxEVT_MENU_HIGHLIGHT, item);
    event.SetEventObject( this );

    return GetEventHandler()->ProcessEvent(event);
}

// ---------------------------------------------------------------------------
// the window proc for wxFrameMSW
// ---------------------------------------------------------------------------

long wxFrameMSW::MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam)
{
    long rc = 0;
    bool processed = FALSE;

    switch ( message )
    {
        case WM_CLOSE:
            // if we can't close, tell the system that we processed the
            // message - otherwise it would close us
            processed = !Close();
            break;

        case WM_COMMAND:
            {
                WORD id, cmd;
                WXHWND hwnd;
                UnpackCommand((WXWPARAM)wParam, (WXLPARAM)lParam,
                              &id, &hwnd, &cmd);

                processed = HandleCommand(id, cmd, (WXHWND)hwnd);
            }
            break;

#ifndef __WXMICROWIN__
        case WM_MENUSELECT:
            {
                WXWORD item, flags;
                WXHMENU hmenu;
                UnpackMenuSelect(wParam, lParam, &item, &flags, &hmenu);

                processed = HandleMenuSelect(item, flags, hmenu);
            }
            break;
#endif

        case WM_PAINT:
            processed = HandlePaint();
            break;

#ifndef __WXMICROWIN__
        case WM_QUERYDRAGICON:
            {
                HICON hIcon = m_icon.Ok() ? GetHiconOf(m_icon)
                                          : (HICON)GetDefaultIcon();
                rc = (long)hIcon;
                processed = rc != 0;
            }
            break;
#endif

        case WM_SIZE:
            processed = HandleSize(LOWORD(lParam), HIWORD(lParam), wParam);
            break;
    }

    if ( !processed )
        rc = wxWindow::MSWWindowProc(message, wParam, lParam);

    return rc;
}

