/////////////////////////////////////////////////////////////////////////////
// Name:        common/window.cpp
// Purpose:     common (to all ports) wxWindow functions
// Author:      Julian Smart, Vadim Zeitlin
// Modified by:
// Created:     13/07/98
// RCS-ID:      $Id$
// Copyright:   (c) wxWindows team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#ifdef __GNUG__
    #pragma implementation "windowbase.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/string.h"
    #include "wx/log.h"
    #include "wx/intl.h"
    #include "wx/frame.h"
    #include "wx/defs.h"
    #include "wx/window.h"
    #include "wx/control.h"
    #include "wx/checkbox.h"
    #include "wx/radiobut.h"
    #include "wx/statbox.h"
    #include "wx/textctrl.h"
    #include "wx/settings.h"
    #include "wx/dialog.h"
    #include "wx/msgdlg.h"
    #include "wx/statusbr.h"
    #include "wx/dcclient.h"
#endif //WX_PRECOMP

#if wxUSE_CONSTRAINTS
    #include "wx/layout.h"
#endif // wxUSE_CONSTRAINTS

#include "wx/sizer.h"

#if wxUSE_DRAG_AND_DROP
    #include "wx/dnd.h"
#endif // wxUSE_DRAG_AND_DROP

#if wxUSE_ACCESSIBILITY
    #include "wx/access.h"
#endif

#if wxUSE_HELP
    #include "wx/cshelp.h"
#endif // wxUSE_HELP

#if wxUSE_TOOLTIPS
    #include "wx/tooltip.h"
#endif // wxUSE_TOOLTIPS

#if wxUSE_CARET
    #include "wx/caret.h"
#endif // wxUSE_CARET

// ----------------------------------------------------------------------------
// static data
// ----------------------------------------------------------------------------

#if defined(__WXPM__)
int wxWindowBase::ms_lastControlId = 2000;
#else
int wxWindowBase::ms_lastControlId = -200;
#endif

IMPLEMENT_ABSTRACT_CLASS(wxWindowBase, wxEvtHandler)

// ----------------------------------------------------------------------------
// event table
// ----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(wxWindowBase, wxEvtHandler)
    EVT_SYS_COLOUR_CHANGED(wxWindowBase::OnSysColourChanged)
    EVT_INIT_DIALOG(wxWindowBase::OnInitDialog)
    EVT_MIDDLE_DOWN(wxWindowBase::OnMiddleClick)

#if wxUSE_HELP
    EVT_HELP(-1, wxWindowBase::OnHelp)
#endif // wxUSE_HELP

END_EVENT_TABLE()

// ============================================================================
// implementation of the common functionality of the wxWindow class
// ============================================================================

// ----------------------------------------------------------------------------
// initialization
// ----------------------------------------------------------------------------

// the default initialization
void wxWindowBase::InitBase()
{
    // no window yet, no parent nor children
    m_parent = (wxWindow *)NULL;
    m_windowId = -1;

    // no constraints on the minimal window size
    m_minWidth =
    m_minHeight =
    m_maxWidth =
    m_maxHeight = -1;

    // window is created enabled but it's not visible yet
    m_isShown = FALSE;
    m_isEnabled = TRUE;

    // the default event handler is just this window
    m_eventHandler = this;

#if wxUSE_VALIDATORS
    // no validator
    m_windowValidator = (wxValidator *) NULL;
#endif // wxUSE_VALIDATORS

    // use the system default colours
    m_backgroundColour = wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE);
    m_foregroundColour = wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT);

    // don't set the font here for wxMSW as we don't call WM_SETFONT here and
    // so the font is *not* really set - but calls to SetFont() later won't do
    // anything because m_font appears to be already set!
#ifndef __WXMSW__
    m_font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
#endif // __WXMSW__

    // the colours/fonts are default for now
    m_hasBgCol =
    m_hasFgCol =
    m_hasFont = FALSE;
    
    m_isBeingDeleted = FALSE;

    // no style bits
    m_exStyle =
    m_windowStyle = 0;

#if wxUSE_CONSTRAINTS
    // no constraints whatsoever
    m_constraints = (wxLayoutConstraints *) NULL;
    m_constraintsInvolvedIn = (wxWindowList *) NULL;
#endif // wxUSE_CONSTRAINTS

    m_windowSizer = (wxSizer *) NULL;
    m_containingSizer = (wxSizer *) NULL;
    m_autoLayout = FALSE;

#if wxUSE_DRAG_AND_DROP
    m_dropTarget = (wxDropTarget *)NULL;
#endif // wxUSE_DRAG_AND_DROP

#if wxUSE_TOOLTIPS
    m_tooltip = (wxToolTip *)NULL;
#endif // wxUSE_TOOLTIPS

#if wxUSE_CARET
    m_caret = (wxCaret *)NULL;
#endif // wxUSE_CARET

#if wxUSE_PALETTE
    m_hasCustomPalette = FALSE;
#endif // wxUSE_PALETTE

#if wxUSE_ACCESSIBILITY
    m_accessible = NULL;
#endif

    m_virtualSize = wxDefaultSize;

    m_minVirtualWidth =
    m_minVirtualHeight =
    m_maxVirtualWidth =
    m_maxVirtualHeight = -1;

    // Whether we're using the current theme for this window (wxGTK only for now)
    m_themeEnabled = FALSE;
}

// common part of window creation process
bool wxWindowBase::CreateBase(wxWindowBase *parent,
                              wxWindowID id,
                              const wxPoint& WXUNUSED(pos),
                              const wxSize& WXUNUSED(size),
                              long style,
                              const wxValidator& validator,
                              const wxString& name)
{
#if wxUSE_STATBOX
    // wxGTK doesn't allow to create controls with static box as the parent so
    // this will result in a crash when the program is ported to wxGTK so warn
    // the user about it

    // if you get this assert, the correct solution is to create the controls
    // as siblings of the static box
    wxASSERT_MSG( !parent || !wxDynamicCast(parent, wxStaticBox),
                  _T("wxStaticBox can't be used as a window parent!") );
#endif // wxUSE_STATBOX

    // ids are limited to 16 bits under MSW so if you care about portability,
    // it's not a good idea to use ids out of this range (and negative ids are
    // reserved for wxWindows own usage)
    wxASSERT_MSG( id == wxID_ANY || (id >= 0 && id < 32767),
                  _T("invalid id value") );

    // generate a new id if the user doesn't care about it
    m_windowId = id == wxID_ANY ? NewControlId() : id;

    SetName(name);
    SetWindowStyleFlag(style);
    SetParent(parent);

#if wxUSE_VALIDATORS
    SetValidator(validator);
#endif // wxUSE_VALIDATORS

    // if the parent window has wxWS_EX_VALIDATE_RECURSIVELY set, we want to
    // have it too - like this it's possible to set it only in the top level
    // dialog/frame and all children will inherit it by defult
    if ( parent && (parent->GetExtraStyle() & wxWS_EX_VALIDATE_RECURSIVELY) )
    {
        SetExtraStyle(GetExtraStyle() | wxWS_EX_VALIDATE_RECURSIVELY);
    }

    return TRUE;
}

// ----------------------------------------------------------------------------
// destruction
// ----------------------------------------------------------------------------

// common clean up
wxWindowBase::~wxWindowBase()
{
    wxASSERT_MSG( GetCapture() != this, wxT("attempt to destroy window with mouse capture") );

    // FIXME if these 2 cases result from programming errors in the user code
    //       we should probably assert here instead of silently fixing them

    // Just in case the window has been Closed, but we're then deleting
    // immediately: don't leave dangling pointers.
    wxPendingDelete.DeleteObject(this);

    // Just in case we've loaded a top-level window via LoadNativeDialog but
    // we weren't a dialog class
    wxTopLevelWindows.DeleteObject((wxWindow*)this);

    wxASSERT_MSG( GetChildren().GetCount() == 0, wxT("children not destroyed") );

#if wxUSE_CARET
    if ( m_caret )
        delete m_caret;
#endif // wxUSE_CARET

#if wxUSE_VALIDATORS
    if ( m_windowValidator )
        delete m_windowValidator;
#endif // wxUSE_VALIDATORS

#if wxUSE_CONSTRAINTS
    // Have to delete constraints/sizer FIRST otherwise sizers may try to look
    // at deleted windows as they delete themselves.
    DeleteRelatedConstraints();

    if ( m_constraints )
    {
        // This removes any dangling pointers to this window in other windows'
        // constraintsInvolvedIn lists.
        UnsetConstraints(m_constraints);
        delete m_constraints;
        m_constraints = NULL;
    }

#endif // wxUSE_CONSTRAINTS

    if ( m_containingSizer )
        m_containingSizer->Detach( (wxWindow*)this );

    if ( m_windowSizer )
        delete m_windowSizer;

#if wxUSE_DRAG_AND_DROP
    if ( m_dropTarget )
        delete m_dropTarget;
#endif // wxUSE_DRAG_AND_DROP

#if wxUSE_TOOLTIPS
    if ( m_tooltip )
        delete m_tooltip;
#endif // wxUSE_TOOLTIPS

#if wxUSE_ACCESSIBILITY
    if ( m_accessible )
        delete m_accessible;
#endif

    // reset the dangling pointer our parent window may keep to us
    if ( m_parent && m_parent->GetDefaultItem() == this )
    {
        m_parent->SetDefaultItem(NULL);
    }
}

bool wxWindowBase::Destroy()
{
    delete this;

    return TRUE;
}

bool wxWindowBase::Close(bool force)
{
    wxCloseEvent event(wxEVT_CLOSE_WINDOW, m_windowId);
    event.SetEventObject(this);
#if WXWIN_COMPATIBILITY
    event.SetForce(force);
#endif // WXWIN_COMPATIBILITY
    event.SetCanVeto(!force);

    // return FALSE if window wasn't closed because the application vetoed the
    // close event
    return GetEventHandler()->ProcessEvent(event) && !event.GetVeto();
}

bool wxWindowBase::DestroyChildren()
{
    wxWindowList::compatibility_iterator node;
    for ( ;; )
    {
        // we iterate until the list becomes empty
        node = GetChildren().GetFirst();
        if ( !node )
            break;

        wxWindow *child = node->GetData();

        wxASSERT_MSG( child, wxT("children list contains empty nodes") );

        child->Show(FALSE);
        delete child;

        wxASSERT_MSG( !GetChildren().Find(child),
                      wxT("child didn't remove itself using RemoveChild()") );
    }

    return TRUE;
}

// ----------------------------------------------------------------------------
// size/position related methods
// ----------------------------------------------------------------------------

// centre the window with respect to its parent in either (or both) directions
void wxWindowBase::Centre(int direction)
{
    // the position/size of the parent window or of the entire screen
    wxPoint posParent;
    int widthParent, heightParent;

    wxWindow *parent = NULL;

    if ( !(direction & wxCENTRE_ON_SCREEN) )
    {
        // find the parent to centre this window on: it should be the
        // immediate parent for the controls but the top level parent for the
        // top level windows (like dialogs)
        parent = GetParent();
        if ( IsTopLevel() )
        {
            while ( parent && !parent->IsTopLevel() )
            {
                parent = parent->GetParent();
            }
        }

        // there is no wxTopLevelWindow under wxMotif yet
#ifndef __WXMOTIF__
        // we shouldn't center the dialog on the iconized window: under
        // Windows, for example, this places it completely off the screen
        if ( parent )
        {
            wxTopLevelWindow *winTop = wxDynamicCast(parent, wxTopLevelWindow);
            if ( winTop && winTop->IsIconized() )
            {
                parent = NULL;
            }
        }
#endif // __WXMOTIF__

        // did we find the parent?
        if ( !parent )
        {
            // no other choice
            direction |= wxCENTRE_ON_SCREEN;
        }
    }

    if ( direction & wxCENTRE_ON_SCREEN )
    {
        // centre with respect to the whole screen
        wxDisplaySize(&widthParent, &heightParent);
    }
    else
    {
        if ( IsTopLevel() )
        {
            // centre on the parent
            parent->GetSize(&widthParent, &heightParent);

            // adjust to the parents position
            posParent = parent->GetPosition();
        }
        else
        {
            // centre inside the parents client rectangle
            parent->GetClientSize(&widthParent, &heightParent);
        }
    }

    int width, height;
    GetSize(&width, &height);

    int xNew = -1,
        yNew = -1;

    if ( direction & wxHORIZONTAL )
        xNew = (widthParent - width)/2;

    if ( direction & wxVERTICAL )
        yNew = (heightParent - height)/2;

    xNew += posParent.x;
    yNew += posParent.y;

    // Base size of the visible dimensions of the display
    // to take into account the taskbar
    wxRect rect = wxGetClientDisplayRect();
    wxSize size (rect.width,rect.height);

    // NB: in wxMSW, negative position may not neccessary mean "out of screen",
    //     but it may mean that the window is placed on other than the main
    //     display. Therefore we only make sure centered window is on the main display
    //     if the parent is at least partially present here.
    if (posParent.x + widthParent >= 0)  // if parent is (partially) on the main display
    {
        if (xNew < 0)
            xNew = 0;
        else if (xNew+width > size.x)
            xNew = size.x-width-1;
    }
    if (posParent.y + heightParent >= 0)  // if parent is (partially) on the main display
    {
        if (yNew+height > size.y)
            yNew = size.y-height-1;

        // Make certain that the title bar is initially visible
        // always, even if this would push the bottom of the
        // dialog of the visible area of the display
        if (yNew < 0)
            yNew = 0;
    }

    // move the window to this position (keeping the old size but using
    // SetSize() and not Move() to allow xNew and/or yNew to be -1)
    SetSize(xNew, yNew, width, height, wxSIZE_ALLOW_MINUS_ONE);
}

// fits the window around the children
void wxWindowBase::Fit()
{
    if ( GetChildren().GetCount() > 0 )
    {
        SetClientSize(DoGetBestSize());
    }
    //else: do nothing if we have no children
}

// fits virtual size (ie. scrolled area etc.) around children
void wxWindowBase::FitInside()
{
    if ( GetChildren().GetCount() > 0 )
    {
        SetVirtualSize( GetBestVirtualSize() );
    }
}

// return the size best suited for the current window
wxSize wxWindowBase::DoGetBestSize() const
{
    if ( m_windowSizer )
    {
        return m_windowSizer->GetMinSize();
    }
#if wxUSE_CONSTRAINTS
    else if ( m_constraints )
    {
        wxConstCast(this, wxWindowBase)->SatisfyConstraints();

        // our minimal acceptable size is such that all our windows fit inside
        int maxX = 0,
            maxY = 0;

        for ( wxWindowList::compatibility_iterator node = GetChildren().GetFirst();
              node;
              node = node->GetNext() )
        {
            wxLayoutConstraints *c = node->GetData()->GetConstraints();
            if ( !c )
            {
                // it's not normal that we have an unconstrained child, but
                // what can we do about it?
                continue;
            }

            int x = c->right.GetValue(),
                y = c->bottom.GetValue();

            if ( x > maxX )
                maxX = x;

            if ( y > maxY )
                maxY = y;

            // TODO: we must calculate the overlaps somehow, otherwise we
            //       will never return a size bigger than the current one :-(
        }

        return wxSize(maxX, maxY);
    }
#endif // wxUSE_CONSTRAINTS
    else if ( GetChildren().GetCount() > 0 )
    {
        // our minimal acceptable size is such that all our windows fit inside
        int maxX = 0,
            maxY = 0;

        for ( wxWindowList::compatibility_iterator node = GetChildren().GetFirst();
              node;
              node = node->GetNext() )
        {
            wxWindow *win = node->GetData();
            if ( win->IsTopLevel()
#if wxUSE_STATUSBAR
                    || wxDynamicCast(win, wxStatusBar)
#endif // wxUSE_STATUSBAR
               )
            {
                // dialogs and frames lie in different top level windows -
                // don't deal with them here; as for the status bars, they
                // don't lie in the client area at all
                continue;
            }

            int wx, wy, ww, wh;
            win->GetPosition(&wx, &wy);

            // if the window hadn't been positioned yet, assume that it is in
            // the origin
            if ( wx == -1 )
                wx = 0;
            if ( wy == -1 )
                wy = 0;

            win->GetSize(&ww, &wh);
            if ( wx + ww > maxX )
                maxX = wx + ww;
            if ( wy + wh > maxY )
                maxY = wy + wh;
        }

        // for compatibility with the old versions and because it really looks
        // slightly more pretty like this, add a pad
        maxX += 7;
        maxY += 14;

        return wxSize(maxX, maxY);
    }
    else
    {
        // for a generic window there is no natural best size - just use the
        // current one
        return GetSize();
    }
}

// by default the origin is not shifted
wxPoint wxWindowBase::GetClientAreaOrigin() const
{
    return wxPoint(0, 0);
}

// set the min/max size of the window
void wxWindowBase::SetSizeHints(int minW, int minH,
                                int maxW, int maxH,
                                int WXUNUSED(incW), int WXUNUSED(incH))
{
    // setting min width greater than max width leads to infinite loops under
    // X11 and generally doesn't make any sense, so don't allow it
    wxCHECK_RET( (minW == -1 || maxW == -1 || minW <= maxW) &&
                    (minH == -1 || maxH == -1 || minH <= maxH),
                 _T("min width/height must be less than max width/height!") );

    m_minWidth = minW;
    m_maxWidth = maxW;
    m_minHeight = minH;
    m_maxHeight = maxH;
}

void wxWindowBase::SetVirtualSizeHints( int minW, int minH,
                                        int maxW, int maxH )
{
    m_minVirtualWidth = minW;
    m_maxVirtualWidth = maxW;
    m_minVirtualHeight = minH;
    m_maxVirtualHeight = maxH;
}

void wxWindowBase::DoSetVirtualSize( int x, int y )
{
    if ( m_minVirtualWidth != -1 && m_minVirtualWidth > x )
        x = m_minVirtualWidth;
    if ( m_maxVirtualWidth != -1 && m_maxVirtualWidth < x )
        x = m_maxVirtualWidth;
    if ( m_minVirtualHeight != -1 && m_minVirtualHeight > y )
        y = m_minVirtualHeight;
    if ( m_maxVirtualHeight != -1 && m_maxVirtualHeight < y )
        y = m_maxVirtualHeight;

    m_virtualSize = wxSize(x, y);
}

wxSize wxWindowBase::DoGetVirtualSize() const
{
    wxSize  s( GetClientSize() );

    return wxSize( wxMax( m_virtualSize.GetWidth(), s.GetWidth() ),
                   wxMax( m_virtualSize.GetHeight(), s.GetHeight() ) );
}

// ----------------------------------------------------------------------------
// show/hide/enable/disable the window
// ----------------------------------------------------------------------------

bool wxWindowBase::Show(bool show)
{
    if ( show != m_isShown )
    {
        m_isShown = show;

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

bool wxWindowBase::Enable(bool enable)
{
    if ( enable != m_isEnabled )
    {
        m_isEnabled = enable;

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
// ----------------------------------------------------------------------------
// RTTI
// ----------------------------------------------------------------------------

bool wxWindowBase::IsTopLevel() const
{
    return FALSE;
}

// ----------------------------------------------------------------------------
// reparenting the window
// ----------------------------------------------------------------------------

void wxWindowBase::AddChild(wxWindowBase *child)
{
    wxCHECK_RET( child, wxT("can't add a NULL child") );

    // this should never happen and it will lead to a crash later if it does
    // because RemoveChild() will remove only one node from the children list
    // and the other(s) one(s) will be left with dangling pointers in them
    wxASSERT_MSG( !GetChildren().Find((wxWindow*)child), _T("AddChild() called twice") );

    GetChildren().Append((wxWindow*)child);
    child->SetParent(this);
}

void wxWindowBase::RemoveChild(wxWindowBase *child)
{
    wxCHECK_RET( child, wxT("can't remove a NULL child") );

    GetChildren().DeleteObject((wxWindow *)child);
    child->SetParent(NULL);
}

bool wxWindowBase::Reparent(wxWindowBase *newParent)
{
    wxWindow *oldParent = GetParent();
    if ( newParent == oldParent )
    {
        // nothing done
        return FALSE;
    }

    // unlink this window from the existing parent.
    if ( oldParent )
    {
        oldParent->RemoveChild(this);
    }
    else
    {
        wxTopLevelWindows.DeleteObject((wxWindow *)this);
    }

    // add it to the new one
    if ( newParent )
    {
        newParent->AddChild(this);
    }
    else
    {
        wxTopLevelWindows.Append((wxWindow *)this);
    }

    return TRUE;
}

// ----------------------------------------------------------------------------
// event handler stuff
// ----------------------------------------------------------------------------

void wxWindowBase::PushEventHandler(wxEvtHandler *handler)
{
    wxEvtHandler *handlerOld = GetEventHandler();

    handler->SetNextHandler(handlerOld);

    if ( handlerOld )
        GetEventHandler()->SetPreviousHandler(handler);

    SetEventHandler(handler);
}

wxEvtHandler *wxWindowBase::PopEventHandler(bool deleteHandler)
{
    wxEvtHandler *handlerA = GetEventHandler();
    if ( handlerA )
    {
        wxEvtHandler *handlerB = handlerA->GetNextHandler();
        handlerA->SetNextHandler((wxEvtHandler *)NULL);

        if ( handlerB )
            handlerB->SetPreviousHandler((wxEvtHandler *)NULL);
        SetEventHandler(handlerB);

        if ( deleteHandler )
        {
            delete handlerA;
            handlerA = (wxEvtHandler *)NULL;
        }
    }

    return handlerA;
}

bool wxWindowBase::RemoveEventHandler(wxEvtHandler *handler)
{
    wxCHECK_MSG( handler, FALSE, _T("RemoveEventHandler(NULL) called") );

    wxEvtHandler *handlerPrev = NULL,
                 *handlerCur = GetEventHandler();
    while ( handlerCur )
    {
        wxEvtHandler *handlerNext = handlerCur->GetNextHandler();

        if ( handlerCur == handler )
        {
            if ( handlerPrev )
            {
                handlerPrev->SetNextHandler(handlerNext);
            }
            else
            {
                SetEventHandler(handlerNext);
            }

            if ( handlerNext )
            {
                handlerNext->SetPreviousHandler ( handlerPrev );
            }
            handler->SetNextHandler(NULL);

            return TRUE;
        }

        handlerPrev = handlerCur;
        handlerCur = handlerNext;
    }

    wxFAIL_MSG( _T("where has the event handler gone?") );

    return FALSE;
}

// ----------------------------------------------------------------------------
// cursors, fonts &c
// ----------------------------------------------------------------------------

bool wxWindowBase::SetBackgroundColour( const wxColour &colour )
{
    if ( !colour.Ok() || (colour == m_backgroundColour) )
        return FALSE;

    m_backgroundColour = colour;

    m_hasBgCol = TRUE;

    return TRUE;
}

bool wxWindowBase::SetForegroundColour( const wxColour &colour )
{
    if ( !colour.Ok() || (colour == m_foregroundColour) )
        return FALSE;

    m_foregroundColour = colour;

    m_hasFgCol = TRUE;

    return TRUE;
}

bool wxWindowBase::SetCursor(const wxCursor& cursor)
{
    // setting an invalid cursor is ok, it means that we don't have any special
    // cursor
    if ( m_cursor == cursor )
    {
        // no change
        return FALSE;
    }

    m_cursor = cursor;

    return TRUE;
}

bool wxWindowBase::SetFont(const wxFont& font)
{
    // don't try to set invalid font, always fall back to the default
    const wxFont& fontOk = font.Ok() ? font : *wxSWISS_FONT;

    if ( fontOk == m_font )
    {
        // no change
        return FALSE;
    }

    m_font = fontOk;

    m_hasFont = TRUE;

    return TRUE;
}

#if wxUSE_PALETTE

void wxWindowBase::SetPalette(const wxPalette& pal)
{
    m_hasCustomPalette = TRUE;
    m_palette = pal;

    // VZ: can anyone explain me what do we do here?
    wxWindowDC d((wxWindow *) this);
    d.SetPalette(pal);
}

wxWindow *wxWindowBase::GetAncestorWithCustomPalette() const
{
    wxWindow *win = (wxWindow *)this;
    while ( win && !win->HasCustomPalette() )
    {
        win = win->GetParent();
    }

    return win;
}

#endif // wxUSE_PALETTE

#if wxUSE_CARET
void wxWindowBase::SetCaret(wxCaret *caret)
{
    if ( m_caret )
    {
        delete m_caret;
    }

    m_caret = caret;

    if ( m_caret )
    {
        wxASSERT_MSG( m_caret->GetWindow() == this,
                      wxT("caret should be created associated to this window") );
    }
}
#endif // wxUSE_CARET

#if wxUSE_VALIDATORS
// ----------------------------------------------------------------------------
// validators
// ----------------------------------------------------------------------------

void wxWindowBase::SetValidator(const wxValidator& validator)
{
    if ( m_windowValidator )
        delete m_windowValidator;

    m_windowValidator = (wxValidator *)validator.Clone();

    if ( m_windowValidator )
        m_windowValidator->SetWindow(this) ;
}
#endif // wxUSE_VALIDATORS

// ----------------------------------------------------------------------------
// update region stuff
// ----------------------------------------------------------------------------

wxRect wxWindowBase::GetUpdateClientRect() const
{
    wxRegion rgnUpdate = GetUpdateRegion();
    rgnUpdate.Intersect(GetClientRect());
    wxRect rectUpdate = rgnUpdate.GetBox();
    wxPoint ptOrigin = GetClientAreaOrigin();
    rectUpdate.x -= ptOrigin.x;
    rectUpdate.y -= ptOrigin.y;

    return rectUpdate;
}

bool wxWindowBase::IsExposed(int x, int y) const
{
    return m_updateRegion.Contains(x, y) != wxOutRegion;
}

bool wxWindowBase::IsExposed(int x, int y, int w, int h) const
{
    return m_updateRegion.Contains(x, y, w, h) != wxOutRegion;
}

// ----------------------------------------------------------------------------
// find child window by id or name
// ----------------------------------------------------------------------------

wxWindow *wxWindowBase::FindWindow( long id )
{
    if ( id == m_windowId )
        return (wxWindow *)this;

    wxWindowBase *res = (wxWindow *)NULL;
    wxWindowList::compatibility_iterator node;
    for ( node = m_children.GetFirst(); node && !res; node = node->GetNext() )
    {
        wxWindowBase *child = node->GetData();
        res = child->FindWindow( id );
    }

    return (wxWindow *)res;
}

wxWindow *wxWindowBase::FindWindow( const wxString& name )
{
    if ( name == m_windowName )
        return (wxWindow *)this;

    wxWindowBase *res = (wxWindow *)NULL;
    wxWindowList::compatibility_iterator node;
    for ( node = m_children.GetFirst(); node && !res; node = node->GetNext() )
    {
        wxWindow *child = node->GetData();
        res = child->FindWindow(name);
    }

    return (wxWindow *)res;
}


// find any window by id or name or label: If parent is non-NULL, look through
// children for a label or title matching the specified string. If NULL, look
// through all top-level windows.
//
// to avoid duplicating code we reuse the same helper function but with
// different comparators

typedef bool (*wxFindWindowCmp)(const wxWindow *win,
                                const wxString& label, long id);

static
bool wxFindWindowCmpLabels(const wxWindow *win, const wxString& label,
                           long WXUNUSED(id))
{
    return win->GetLabel() == label;
}

static
bool wxFindWindowCmpNames(const wxWindow *win, const wxString& label,
                          long WXUNUSED(id))
{
    return win->GetName() == label;
}

static
bool wxFindWindowCmpIds(const wxWindow *win, const wxString& WXUNUSED(label),
                        long id)
{
    return win->GetId() == id;
}

// recursive helper for the FindWindowByXXX() functions
static
wxWindow *wxFindWindowRecursively(const wxWindow *parent,
                                  const wxString& label,
                                  long id,
                                  wxFindWindowCmp cmp)
{
    if ( parent )
    {
        // see if this is the one we're looking for
        if ( (*cmp)(parent, label, id) )
            return (wxWindow *)parent;

        // It wasn't, so check all its children
        for ( wxWindowList::compatibility_iterator node = parent->GetChildren().GetFirst();
              node;
              node = node->GetNext() )
        {
            // recursively check each child
            wxWindow *win = (wxWindow *)node->GetData();
            wxWindow *retwin = wxFindWindowRecursively(win, label, id, cmp);
            if (retwin)
                return retwin;
        }
    }

    // Not found
    return NULL;
}

// helper for FindWindowByXXX()
static
wxWindow *wxFindWindowHelper(const wxWindow *parent,
                             const wxString& label,
                             long id,
                             wxFindWindowCmp cmp)
{
    if ( parent )
    {
        // just check parent and all its children
        return wxFindWindowRecursively(parent, label, id, cmp);
    }

    // start at very top of wx's windows
    for ( wxWindowList::compatibility_iterator node = wxTopLevelWindows.GetFirst();
          node;
          node = node->GetNext() )
    {
        // recursively check each window & its children
        wxWindow *win = node->GetData();
        wxWindow *retwin = wxFindWindowRecursively(win, label, id, cmp);
        if (retwin)
            return retwin;
    }

    return NULL;
}

/* static */
wxWindow *
wxWindowBase::FindWindowByLabel(const wxString& title, const wxWindow *parent)
{
    return wxFindWindowHelper(parent, title, 0, wxFindWindowCmpLabels);
}

/* static */
wxWindow *
wxWindowBase::FindWindowByName(const wxString& title, const wxWindow *parent)
{
    wxWindow *win = wxFindWindowHelper(parent, title, 0, wxFindWindowCmpNames);

    if ( !win )
    {
        // fall back to the label
        win = FindWindowByLabel(title, parent);
    }

    return win;
}

/* static */
wxWindow *
wxWindowBase::FindWindowById( long id, const wxWindow* parent )
{
    return wxFindWindowHelper(parent, _T(""), id, wxFindWindowCmpIds);
}

// ----------------------------------------------------------------------------
// dialog oriented functions
// ----------------------------------------------------------------------------

void wxWindowBase::MakeModal(bool modal)
{
    // Disable all other windows
    if ( IsTopLevel() )
    {
        wxWindowList::compatibility_iterator node = wxTopLevelWindows.GetFirst();
        while (node)
        {
            wxWindow *win = node->GetData();
            if (win != this)
                win->Enable(!modal);

            node = node->GetNext();
        }
    }
}

bool wxWindowBase::Validate()
{
#if wxUSE_VALIDATORS
    bool recurse = (GetExtraStyle() & wxWS_EX_VALIDATE_RECURSIVELY) != 0;

    wxWindowList::compatibility_iterator node;
    for ( node = m_children.GetFirst(); node; node = node->GetNext() )
    {
        wxWindowBase *child = node->GetData();
        wxValidator *validator = child->GetValidator();
        if ( validator && !validator->Validate((wxWindow *)this) )
        {
            return FALSE;
        }

        if ( recurse && !child->Validate() )
        {
            return FALSE;
        }
    }
#endif // wxUSE_VALIDATORS

    return TRUE;
}

bool wxWindowBase::TransferDataToWindow()
{
#if wxUSE_VALIDATORS
    bool recurse = (GetExtraStyle() & wxWS_EX_VALIDATE_RECURSIVELY) != 0;

    wxWindowList::compatibility_iterator node;
    for ( node = m_children.GetFirst(); node; node = node->GetNext() )
    {
        wxWindowBase *child = node->GetData();
        wxValidator *validator = child->GetValidator();
        if ( validator && !validator->TransferToWindow() )
        {
            wxLogWarning(_("Could not transfer data to window"));
#if wxUSE_LOG
            wxLog::FlushActive();
#endif // wxUSE_LOG

            return FALSE;
        }

        if ( recurse )
        {
            if ( !child->TransferDataToWindow() )
            {
                // warning already given
                return FALSE;
            }
        }
    }
#endif // wxUSE_VALIDATORS

    return TRUE;
}

bool wxWindowBase::TransferDataFromWindow()
{
#if wxUSE_VALIDATORS
    bool recurse = (GetExtraStyle() & wxWS_EX_VALIDATE_RECURSIVELY) != 0;

    wxWindowList::compatibility_iterator node;
    for ( node = m_children.GetFirst(); node; node = node->GetNext() )
    {
        wxWindow *child = node->GetData();
        wxValidator *validator = child->GetValidator();
        if ( validator && !validator->TransferFromWindow() )
        {
            // nop warning here because the application is supposed to give
            // one itself - we don't know here what might have gone wrongly

            return FALSE;
        }

        if ( recurse )
        {
            if ( !child->TransferDataFromWindow() )
            {
                // warning already given
                return FALSE;
            }
        }
    }
#endif // wxUSE_VALIDATORS

    return TRUE;
}

void wxWindowBase::InitDialog()
{
    wxInitDialogEvent event(GetId());
    event.SetEventObject( this );
    GetEventHandler()->ProcessEvent(event);
}

// ----------------------------------------------------------------------------
// context-sensitive help support
// ----------------------------------------------------------------------------

#if wxUSE_HELP

// associate this help text with this window
void wxWindowBase::SetHelpText(const wxString& text)
{
    wxHelpProvider *helpProvider = wxHelpProvider::Get();
    if ( helpProvider )
    {
        helpProvider->AddHelp(this, text);
    }
}

// associate this help text with all windows with the same id as this
// one
void wxWindowBase::SetHelpTextForId(const wxString& text)
{
    wxHelpProvider *helpProvider = wxHelpProvider::Get();
    if ( helpProvider )
    {
        helpProvider->AddHelp(GetId(), text);
    }
}

// get the help string associated with this window (may be empty)
wxString wxWindowBase::GetHelpText() const
{
    wxString text;
    wxHelpProvider *helpProvider = wxHelpProvider::Get();
    if ( helpProvider )
    {
        text = helpProvider->GetHelp(this);
    }

    return text;
}

// show help for this window
void wxWindowBase::OnHelp(wxHelpEvent& event)
{
    wxHelpProvider *helpProvider = wxHelpProvider::Get();
    if ( helpProvider )
    {
        if ( helpProvider->ShowHelp(this) )
        {
            // skip the event.Skip() below
            return;
        }
    }

    event.Skip();
}

#endif // wxUSE_HELP

// ----------------------------------------------------------------------------
// tooltipsroot.Replace("\\", "/");
// ----------------------------------------------------------------------------

#if wxUSE_TOOLTIPS

void wxWindowBase::SetToolTip( const wxString &tip )
{
    // don't create the new tooltip if we already have one
    if ( m_tooltip )
    {
        m_tooltip->SetTip( tip );
    }
    else
    {
        SetToolTip( new wxToolTip( tip ) );
    }

    // setting empty tooltip text does not remove the tooltip any more - use
    // SetToolTip((wxToolTip *)NULL) for this
}

void wxWindowBase::DoSetToolTip(wxToolTip *tooltip)
{
    if ( m_tooltip )
        delete m_tooltip;

    m_tooltip = tooltip;
}

#endif // wxUSE_TOOLTIPS

// ----------------------------------------------------------------------------
// constraints and sizers
// ----------------------------------------------------------------------------

#if wxUSE_CONSTRAINTS

void wxWindowBase::SetConstraints( wxLayoutConstraints *constraints )
{
    if ( m_constraints )
    {
        UnsetConstraints(m_constraints);
        delete m_constraints;
    }
    m_constraints = constraints;
    if ( m_constraints )
    {
        // Make sure other windows know they're part of a 'meaningful relationship'
        if ( m_constraints->left.GetOtherWindow() && (m_constraints->left.GetOtherWindow() != this) )
            m_constraints->left.GetOtherWindow()->AddConstraintReference(this);
        if ( m_constraints->top.GetOtherWindow() && (m_constraints->top.GetOtherWindow() != this) )
            m_constraints->top.GetOtherWindow()->AddConstraintReference(this);
        if ( m_constraints->right.GetOtherWindow() && (m_constraints->right.GetOtherWindow() != this) )
            m_constraints->right.GetOtherWindow()->AddConstraintReference(this);
        if ( m_constraints->bottom.GetOtherWindow() && (m_constraints->bottom.GetOtherWindow() != this) )
            m_constraints->bottom.GetOtherWindow()->AddConstraintReference(this);
        if ( m_constraints->width.GetOtherWindow() && (m_constraints->width.GetOtherWindow() != this) )
            m_constraints->width.GetOtherWindow()->AddConstraintReference(this);
        if ( m_constraints->height.GetOtherWindow() && (m_constraints->height.GetOtherWindow() != this) )
            m_constraints->height.GetOtherWindow()->AddConstraintReference(this);
        if ( m_constraints->centreX.GetOtherWindow() && (m_constraints->centreX.GetOtherWindow() != this) )
            m_constraints->centreX.GetOtherWindow()->AddConstraintReference(this);
        if ( m_constraints->centreY.GetOtherWindow() && (m_constraints->centreY.GetOtherWindow() != this) )
            m_constraints->centreY.GetOtherWindow()->AddConstraintReference(this);
    }
}

// This removes any dangling pointers to this window in other windows'
// constraintsInvolvedIn lists.
void wxWindowBase::UnsetConstraints(wxLayoutConstraints *c)
{
    if ( c )
    {
        if ( c->left.GetOtherWindow() && (c->top.GetOtherWindow() != this) )
            c->left.GetOtherWindow()->RemoveConstraintReference(this);
        if ( c->top.GetOtherWindow() && (c->top.GetOtherWindow() != this) )
            c->top.GetOtherWindow()->RemoveConstraintReference(this);
        if ( c->right.GetOtherWindow() && (c->right.GetOtherWindow() != this) )
            c->right.GetOtherWindow()->RemoveConstraintReference(this);
        if ( c->bottom.GetOtherWindow() && (c->bottom.GetOtherWindow() != this) )
            c->bottom.GetOtherWindow()->RemoveConstraintReference(this);
        if ( c->width.GetOtherWindow() && (c->width.GetOtherWindow() != this) )
            c->width.GetOtherWindow()->RemoveConstraintReference(this);
        if ( c->height.GetOtherWindow() && (c->height.GetOtherWindow() != this) )
            c->height.GetOtherWindow()->RemoveConstraintReference(this);
        if ( c->centreX.GetOtherWindow() && (c->centreX.GetOtherWindow() != this) )
            c->centreX.GetOtherWindow()->RemoveConstraintReference(this);
        if ( c->centreY.GetOtherWindow() && (c->centreY.GetOtherWindow() != this) )
            c->centreY.GetOtherWindow()->RemoveConstraintReference(this);
    }
}

// Back-pointer to other windows we're involved with, so if we delete this
// window, we must delete any constraints we're involved with.
void wxWindowBase::AddConstraintReference(wxWindowBase *otherWin)
{
    if ( !m_constraintsInvolvedIn )
        m_constraintsInvolvedIn = new wxWindowList;
    if ( !m_constraintsInvolvedIn->Find((wxWindow *)otherWin) )
        m_constraintsInvolvedIn->Append((wxWindow *)otherWin);
}

// REMOVE back-pointer to other windows we're involved with.
void wxWindowBase::RemoveConstraintReference(wxWindowBase *otherWin)
{
    if ( m_constraintsInvolvedIn )
        m_constraintsInvolvedIn->DeleteObject((wxWindow *)otherWin);
}

// Reset any constraints that mention this window
void wxWindowBase::DeleteRelatedConstraints()
{
    if ( m_constraintsInvolvedIn )
    {
        wxWindowList::compatibility_iterator node = m_constraintsInvolvedIn->GetFirst();
        while (node)
        {
            wxWindow *win = node->GetData();
            wxLayoutConstraints *constr = win->GetConstraints();

            // Reset any constraints involving this window
            if ( constr )
            {
                constr->left.ResetIfWin(this);
                constr->top.ResetIfWin(this);
                constr->right.ResetIfWin(this);
                constr->bottom.ResetIfWin(this);
                constr->width.ResetIfWin(this);
                constr->height.ResetIfWin(this);
                constr->centreX.ResetIfWin(this);
                constr->centreY.ResetIfWin(this);
            }

            wxWindowList::compatibility_iterator next = node->GetNext();
            m_constraintsInvolvedIn->Erase(node);
            node = next;
        }

        delete m_constraintsInvolvedIn;
        m_constraintsInvolvedIn = (wxWindowList *) NULL;
    }
}

#endif // wxUSE_CONSTRAINTS

void wxWindowBase::SetSizer(wxSizer *sizer, bool deleteOld)
{
    if ( deleteOld )
        delete m_windowSizer;

    m_windowSizer = sizer;

    SetAutoLayout( sizer != NULL );
}

void wxWindowBase::SetSizerAndFit(wxSizer *sizer, bool deleteOld)
{
    SetSizer( sizer, deleteOld );
    sizer->SetSizeHints( (wxWindow*) this );
}

#if wxUSE_CONSTRAINTS

void wxWindowBase::SatisfyConstraints()
{
    wxLayoutConstraints *constr = GetConstraints();
    bool wasOk = constr && constr->AreSatisfied();

    ResetConstraints();   // Mark all constraints as unevaluated

    int noChanges = 1;

    // if we're a top level panel (i.e. our parent is frame/dialog), our
    // own constraints will never be satisfied any more unless we do it
    // here
    if ( wasOk )
    {
        while ( noChanges > 0 )
        {
            LayoutPhase1(&noChanges);
        }
    }

    LayoutPhase2(&noChanges);
}

#endif // wxUSE_CONSTRAINTS

bool wxWindowBase::Layout()
{
    // If there is a sizer, use it instead of the constraints
    if ( GetSizer() )
    {
        int w, h;
        GetVirtualSize(&w, &h);
        GetSizer()->SetDimension( 0, 0, w, h );
    }
#if wxUSE_CONSTRAINTS
    else
    {
        SatisfyConstraints(); // Find the right constraints values
        SetConstraintSizes(); // Recursively set the real window sizes
    }
#endif

    return TRUE;
}

#if wxUSE_CONSTRAINTS

// first phase of the constraints evaluation: set our own constraints
bool wxWindowBase::LayoutPhase1(int *noChanges)
{
    wxLayoutConstraints *constr = GetConstraints();

    return !constr || constr->SatisfyConstraints(this, noChanges);
}

// second phase: set the constraints for our children
bool wxWindowBase::LayoutPhase2(int *noChanges)
{
    *noChanges = 0;

    // Layout children
    DoPhase(1);

    // Layout grand children
    DoPhase(2);

    return TRUE;
}

// Do a phase of evaluating child constraints
bool wxWindowBase::DoPhase(int phase)
{
    // the list containing the children for which the constraints are already
    // set correctly
    wxWindowList succeeded;

    // the max number of iterations we loop before concluding that we can't set
    // the constraints
    static const int maxIterations = 500;

    for ( int noIterations = 0; noIterations < maxIterations; noIterations++ )
    {
        int noChanges = 0;

        // loop over all children setting their constraints
        for ( wxWindowList::compatibility_iterator node = GetChildren().GetFirst();
              node;
              node = node->GetNext() )
        {
            wxWindow *child = node->GetData();
            if ( child->IsTopLevel() )
            {
                // top level children are not inside our client area
                continue;
            }

            if ( !child->GetConstraints() || succeeded.Find(child) )
            {
                // this one is either already ok or nothing we can do about it
                continue;
            }

            int tempNoChanges = 0;
            bool success = phase == 1 ? child->LayoutPhase1(&tempNoChanges)
                                      : child->LayoutPhase2(&tempNoChanges);
            noChanges += tempNoChanges;

            if ( success )
            {
                succeeded.Append(child);
            }
        }

        if ( !noChanges )
        {
            // constraints are set
            break;
        }
    }

    return TRUE;
}

void wxWindowBase::ResetConstraints()
{
    wxLayoutConstraints *constr = GetConstraints();
    if ( constr )
    {
        constr->left.SetDone(FALSE);
        constr->top.SetDone(FALSE);
        constr->right.SetDone(FALSE);
        constr->bottom.SetDone(FALSE);
        constr->width.SetDone(FALSE);
        constr->height.SetDone(FALSE);
        constr->centreX.SetDone(FALSE);
        constr->centreY.SetDone(FALSE);
    }

    wxWindowList::compatibility_iterator node = GetChildren().GetFirst();
    while (node)
    {
        wxWindow *win = node->GetData();
        if ( !win->IsTopLevel() )
            win->ResetConstraints();
        node = node->GetNext();
    }
}

// Need to distinguish between setting the 'fake' size for windows and sizers,
// and setting the real values.
void wxWindowBase::SetConstraintSizes(bool recurse)
{
    wxLayoutConstraints *constr = GetConstraints();
    if ( constr && constr->AreSatisfied() )
    {
        int x = constr->left.GetValue();
        int y = constr->top.GetValue();
        int w = constr->width.GetValue();
        int h = constr->height.GetValue();

        if ( (constr->width.GetRelationship() != wxAsIs ) ||
             (constr->height.GetRelationship() != wxAsIs) )
        {
            SetSize(x, y, w, h);
        }
        else
        {
            // If we don't want to resize this window, just move it...
            Move(x, y);
        }
    }
    else if ( constr )
    {
        wxLogDebug(wxT("Constraints not satisfied for %s named '%s'."),
                   GetClassInfo()->GetClassName(),
                   GetName().c_str());
    }

    if ( recurse )
    {
        wxWindowList::compatibility_iterator node = GetChildren().GetFirst();
        while (node)
        {
            wxWindow *win = node->GetData();
            if ( !win->IsTopLevel() && win->GetConstraints() )
                win->SetConstraintSizes();
            node = node->GetNext();
        }
    }
}

// Only set the size/position of the constraint (if any)
void wxWindowBase::SetSizeConstraint(int x, int y, int w, int h)
{
    wxLayoutConstraints *constr = GetConstraints();
    if ( constr )
    {
        if ( x != -1 )
        {
            constr->left.SetValue(x);
            constr->left.SetDone(TRUE);
        }
        if ( y != -1 )
        {
            constr->top.SetValue(y);
            constr->top.SetDone(TRUE);
        }
        if ( w != -1 )
        {
            constr->width.SetValue(w);
            constr->width.SetDone(TRUE);
        }
        if ( h != -1 )
        {
            constr->height.SetValue(h);
            constr->height.SetDone(TRUE);
        }
    }
}

void wxWindowBase::MoveConstraint(int x, int y)
{
    wxLayoutConstraints *constr = GetConstraints();
    if ( constr )
    {
        if ( x != -1 )
        {
            constr->left.SetValue(x);
            constr->left.SetDone(TRUE);
        }
        if ( y != -1 )
        {
            constr->top.SetValue(y);
            constr->top.SetDone(TRUE);
        }
    }
}

void wxWindowBase::GetSizeConstraint(int *w, int *h) const
{
    wxLayoutConstraints *constr = GetConstraints();
    if ( constr )
    {
        *w = constr->width.GetValue();
        *h = constr->height.GetValue();
    }
    else
        GetSize(w, h);
}

void wxWindowBase::GetClientSizeConstraint(int *w, int *h) const
{
    wxLayoutConstraints *constr = GetConstraints();
    if ( constr )
    {
        *w = constr->width.GetValue();
        *h = constr->height.GetValue();
    }
    else
        GetClientSize(w, h);
}

void wxWindowBase::GetPositionConstraint(int *x, int *y) const
{
    wxLayoutConstraints *constr = GetConstraints();
    if ( constr )
    {
        *x = constr->left.GetValue();
        *y = constr->top.GetValue();
    }
    else
        GetPosition(x, y);
}

#endif // wxUSE_CONSTRAINTS

void wxWindowBase::AdjustForParentClientOrigin(int& x, int& y, int sizeFlags) const
{
    // don't do it for the dialogs/frames - they float independently of their
    // parent
    if ( !IsTopLevel() )
    {
        wxWindow *parent = GetParent();
        if ( !(sizeFlags & wxSIZE_NO_ADJUSTMENTS) && parent )
        {
            wxPoint pt(parent->GetClientAreaOrigin());
            x += pt.x;
            y += pt.y;
        }
    }
}

// ----------------------------------------------------------------------------
// do Update UI processing for child controls
// ----------------------------------------------------------------------------

void wxWindowBase::UpdateWindowUI(long flags)
{
    wxUpdateUIEvent event(GetId());
    event.m_eventObject = this;

    if ( GetEventHandler()->ProcessEvent(event) )
    {
        DoUpdateWindowUI(event);
    }

    if (flags & wxUPDATE_UI_RECURSE)
    {
        wxWindowList::compatibility_iterator node = GetChildren().GetFirst();
        while (node)
        {
            wxWindow* child = (wxWindow*) node->GetData();
            child->UpdateWindowUI(flags);
            node = node->GetNext();
        }
    }
}

// do the window-specific processing after processing the update event
// TODO: take specific knowledge out of this function and
// put in each control's base class. Unfortunately we don't
// yet have base implementation files for wxCheckBox and wxRadioButton.
void wxWindowBase::DoUpdateWindowUI(wxUpdateUIEvent& event)
{
    if ( event.GetSetEnabled() )
        Enable(event.GetEnabled());
    
#if wxUSE_CONTROLS
    if ( event.GetSetText() )
    {
        wxControl *control = wxDynamicCastThis(wxControl);
        if ( control )
        {
            if ( event.GetText() != control->GetLabel() )
                control->SetLabel(event.GetText());
        }
#if wxUSE_CHECKBOX
        wxCheckBox *checkbox = wxDynamicCastThis(wxCheckBox);
        if ( checkbox )
        {
            if ( event.GetSetChecked() )
                checkbox->SetValue(event.GetChecked());
        }
#endif // wxUSE_CHECKBOX

#if wxUSE_RADIOBTN
        wxRadioButton *radiobtn = wxDynamicCastThis(wxRadioButton);
        if ( radiobtn )
        {
            if ( event.GetSetChecked() )
                radiobtn->SetValue(event.GetChecked());
        }
#endif // wxUSE_RADIOBTN
    }    
#endif
}

#if 0
// call internal idle recursively
// may be obsolete (wait until OnIdle scheme stabilises)
void wxWindowBase::ProcessInternalIdle()
{
    OnInternalIdle();

    wxWindowList::compatibility_iterator node = GetChildren().GetFirst();
    while (node)
    {
        wxWindow *child = node->GetData();
        child->ProcessInternalIdle();
        node = node->GetNext();
    }
}
#endif

// ----------------------------------------------------------------------------
// dialog units translations
// ----------------------------------------------------------------------------

wxPoint wxWindowBase::ConvertPixelsToDialog(const wxPoint& pt)
{
    int charWidth = GetCharWidth();
    int charHeight = GetCharHeight();
    wxPoint pt2(-1, -1);
    if (pt.x != -1)
        pt2.x = (int) ((pt.x * 4) / charWidth) ;
    if (pt.y != -1)
        pt2.y = (int) ((pt.y * 8) / charHeight) ;

    return pt2;
}

wxPoint wxWindowBase::ConvertDialogToPixels(const wxPoint& pt)
{
    int charWidth = GetCharWidth();
    int charHeight = GetCharHeight();
    wxPoint pt2(-1, -1);
    if (pt.x != -1)
        pt2.x = (int) ((pt.x * charWidth) / 4) ;
    if (pt.y != -1)
        pt2.y = (int) ((pt.y * charHeight) / 8) ;

    return pt2;
}

// ----------------------------------------------------------------------------
// event handlers
// ----------------------------------------------------------------------------

// propagate the colour change event to the subwindows
void wxWindowBase::OnSysColourChanged(wxSysColourChangedEvent& event)
{
    wxWindowList::compatibility_iterator node = GetChildren().GetFirst();
    while ( node )
    {
        // Only propagate to non-top-level windows
        wxWindow *win = node->GetData();
        if ( !win->IsTopLevel() )
        {
            wxSysColourChangedEvent event2;
            event.m_eventObject = win;
            win->GetEventHandler()->ProcessEvent(event2);
        }

        node = node->GetNext();
    }
}

// the default action is to populate dialog with data when it's created,
// and nudge the UI into displaying itself correctly in case
// we've turned the wxUpdateUIEvents frequency down low.
void wxWindowBase::OnInitDialog( wxInitDialogEvent &WXUNUSED(event) )
{
    TransferDataToWindow();
    
    // Update the UI at this point
    UpdateWindowUI(wxUPDATE_UI_RECURSE);
}

// process Ctrl-Alt-mclick
void wxWindowBase::OnMiddleClick( wxMouseEvent& event )
{
#if wxUSE_MSGDLG
    if ( event.ControlDown() && event.AltDown() )
    {
        // don't translate these strings
        wxString port;

#ifdef __WXUNIVERSAL__
        port = _T("Univ/");
#endif // __WXUNIVERSAL__

        switch ( wxGetOsVersion() )
        {
            case wxMOTIF_X:     port += _T("Motif"); break;
            case wxMAC:
            case wxMAC_DARWIN:  port += _T("Mac"); break;
            case wxBEOS:        port += _T("BeOS"); break;
            case wxGTK:
            case wxGTK_WIN32:
            case wxGTK_OS2:
            case wxGTK_BEOS:    port += _T("GTK"); break;
            case wxWINDOWS:
            case wxPENWINDOWS:
            case wxWINDOWS_NT:
            case wxWIN32S:
            case wxWIN95:
            case wxWIN386:      port += _T("MS Windows"); break;
            case wxMGL_UNIX:
            case wxMGL_X:
            case wxMGL_WIN32:
            case wxMGL_OS2:     port += _T("MGL"); break;
            case wxWINDOWS_OS2:
            case wxOS2_PM:      port += _T("OS/2"); break;
            default:            port += _T("unknown"); break;
        }

        wxMessageBox(wxString::Format(
                                      _T(
                                        "       wxWindows Library (%s port)\nVersion %u.%u.%u%s, compiled at %s %s\n   Copyright (c) 1995-2002 wxWindows team"
                                        ),
                                      port.c_str(),
                                      wxMAJOR_VERSION,
                                      wxMINOR_VERSION,
                                      wxRELEASE_NUMBER,
#if wxUSE_UNICODE
                                      L" (Unicode)",
#else
                                      "",
#endif
                                      __TDATE__,
                                      __TTIME__
                                     ),
                     _T("wxWindows information"),
                     wxICON_INFORMATION | wxOK,
                     (wxWindow *)this);
    }
    else
#endif // wxUSE_MSGDLG
    {
        event.Skip();
    }
}

// ----------------------------------------------------------------------------
// accessibility
// ----------------------------------------------------------------------------

#if wxUSE_ACCESSIBILITY
void wxWindowBase::SetAccessible(wxAccessible* accessible)
{
    if (m_accessible && (accessible != m_accessible))
        delete m_accessible;
    m_accessible = accessible;
    if (m_accessible)
        m_accessible->SetWindow((wxWindow*) this);
}

// Returns the accessible object, creating if necessary.
wxAccessible* wxWindowBase::GetOrCreateAccessible()
{
    if (!m_accessible)
        m_accessible = CreateAccessible();
    return m_accessible;
}

// Override to create a specific accessible object.
wxAccessible* wxWindowBase::CreateAccessible()
{
    return new wxWindowAccessible((wxWindow*) this);
}

#endif

#if !wxUSE_STL
// ----------------------------------------------------------------------------
// list classes implementation
// ----------------------------------------------------------------------------

void wxWindowListNode::DeleteData()
{
    delete (wxWindow *)GetData();
}
#endif

// ----------------------------------------------------------------------------
// borders
// ----------------------------------------------------------------------------

wxBorder wxWindowBase::GetBorder(long flags) const
{
    wxBorder border = (wxBorder)(flags & wxBORDER_MASK);
    if ( border == wxBORDER_DEFAULT )
    {
        border = GetDefaultBorder();
    }

    return border;
}

wxBorder wxWindowBase::GetDefaultBorder() const
{
    return wxBORDER_NONE;
}

// ----------------------------------------------------------------------------
// hit testing
// ----------------------------------------------------------------------------

wxHitTest wxWindowBase::DoHitTest(wxCoord x, wxCoord y) const
{
    // here we just check if the point is inside the window or not

    // check the top and left border first
    bool outside = x < 0 || y < 0;
    if ( !outside )
    {
        // check the right and bottom borders too
        wxSize size = GetSize();
        outside = x >= size.x || y >= size.y;
    }

    return outside ? wxHT_WINDOW_OUTSIDE : wxHT_WINDOW_INSIDE;
}

// ----------------------------------------------------------------------------
// mouse capture
// ----------------------------------------------------------------------------

struct WXDLLEXPORT wxWindowNext
{
    wxWindow *win;
    wxWindowNext *next;
} *wxWindowBase::ms_winCaptureNext = NULL;

void wxWindowBase::CaptureMouse()
{
    wxLogTrace(_T("mousecapture"), _T("CaptureMouse(%p)"), this);

    wxWindow *winOld = GetCapture();
    if ( winOld )
    {
        ((wxWindowBase*) winOld)->DoReleaseMouse();

        // save it on stack
        wxWindowNext *item = new wxWindowNext;
        item->win = winOld;
        item->next = ms_winCaptureNext;
        ms_winCaptureNext = item;
    }
    //else: no mouse capture to save

    DoCaptureMouse();
}

void wxWindowBase::ReleaseMouse()
{
    wxLogTrace(_T("mousecapture"), _T("ReleaseMouse(%p)"), this);

    wxASSERT_MSG( GetCapture() == this, wxT("attempt to release mouse, but this window hasn't captured it") );

    DoReleaseMouse();

    if ( ms_winCaptureNext )
    {
        ((wxWindowBase*)ms_winCaptureNext->win)->DoCaptureMouse();

        wxWindowNext *item = ms_winCaptureNext;
        ms_winCaptureNext = item->next;
        delete item;
    }
    //else: stack is empty, no previous capture

    wxLogTrace(_T("mousecapture"),
               _T("After ReleaseMouse() mouse is captured by %p"),
               GetCapture());
}

#if wxUSE_HOTKEY

bool
wxWindowBase::RegisterHotKey(int WXUNUSED(hotkeyId),
                             int WXUNUSED(modifiers),
                             int WXUNUSED(keycode))
{
    // not implemented
    return false;
}

bool wxWindowBase::UnregisterHotKey(int WXUNUSED(hotkeyId))
{
    // not implemented
    return false;
}

#endif // wxUSE_HOTKEY

void wxWindowBase::SendDestroyEvent()
{
    wxWindowDestroyEvent event;
    event.SetEventObject(this);
    event.SetId(GetId());
    GetEventHandler()->ProcessEvent(event);
}

// ----------------------------------------------------------------------------
// event processing
// ----------------------------------------------------------------------------

#if wxUSE_VALIDATORS

bool wxWindowBase::TryValidator(wxEvent& event)
{
    // Can only use the validator of the window which
    // is receiving the event
    if ( event.GetEventObject() == this )
    {
        wxValidator *validator = GetValidator();
        if ( validator && validator->ProcessEvent(event) )
        {
            return TRUE;
        }
    }

    return FALSE;
}

#endif // wxUSE_VALIDATORS

bool wxWindowBase::TryParent(wxEvent& event)
{
    // carry on up the parent-child hierarchy if the propgation count hasn't
    // reached zero yet
    if ( event.ShouldPropagate() )
    {
        // honour the requests to stop propagation at this window: this is
        // used by the dialogs, for example, to prevent processing the events
        // from the dialog controls in the parent frame which rarely, if ever,
        // makes sense
        if ( !(GetExtraStyle() & wxWS_EX_BLOCK_EVENTS) )
        {
            wxWindow *parent = GetParent();
            if ( parent && !parent->IsBeingDeleted() )
            {
                wxPropagateOnce propagateOnce(event);

                return parent->GetEventHandler()->ProcessEvent(event);
            }
        }
    }

    return wxEvtHandler::TryParent(event);
}

// ----------------------------------------------------------------------------
// global functions
// ----------------------------------------------------------------------------

wxWindow* wxGetTopLevelParent(wxWindow *win)
{
    while ( win && !win->IsTopLevel() )
         win = win->GetParent();

    return win;
}

#if wxUSE_ACCESSIBILITY
// ----------------------------------------------------------------------------
// accessible object for windows
// ----------------------------------------------------------------------------

// Can return either a child object, or an integer
// representing the child element, starting from 1.
wxAccStatus wxWindowAccessible::HitTest(const wxPoint& pt, int* childId, wxAccessible** childObject)
{
    wxASSERT( GetWindow() != NULL );
    if (!GetWindow())
        return wxACC_FAIL;

    return wxACC_NOT_IMPLEMENTED;
}

// Returns the rectangle for this object (id = 0) or a child element (id > 0).
wxAccStatus wxWindowAccessible::GetLocation(wxRect& rect, int elementId)
{
    wxASSERT( GetWindow() != NULL );
    if (!GetWindow())
        return wxACC_FAIL;

    wxWindow* win = NULL;
    if (elementId == 0)
    {
        win = GetWindow();
    }
    else
    {
        if (elementId <= (int) GetWindow()->GetChildren().GetCount())
        {
            win = (wxWindow*) GetWindow()->GetChildren().Nth(elementId-1)->GetData();
        }
        else
            return wxACC_FAIL;
    }
    if (win)
    {
        rect = win->GetRect();
        if (win->GetParent() && !win->IsKindOf(CLASSINFO(wxTopLevelWindow)))
            rect.SetPosition(win->GetParent()->ClientToScreen(rect.GetPosition()));
        return wxACC_OK;
    }

    return wxACC_NOT_IMPLEMENTED;
}

// Navigates from fromId to toId/toObject.
wxAccStatus wxWindowAccessible::Navigate(wxNavDir navDir, int fromId,
                             int* toId, wxAccessible** toObject)
{
    wxASSERT( GetWindow() != NULL );
    if (!GetWindow())
        return wxACC_FAIL;

    switch (navDir)
    {
    case wxNAVDIR_FIRSTCHILD:
        {
            if (GetWindow()->GetChildren().GetCount() == 0)
                return wxACC_FALSE;
            wxWindow* childWindow = (wxWindow*) GetWindow()->GetChildren().GetFirst()->GetData();
            *toObject = childWindow->GetOrCreateAccessible();

            return wxACC_OK;
        }
    case wxNAVDIR_LASTCHILD:
        {
            if (GetWindow()->GetChildren().GetCount() == 0)
                return wxACC_FALSE;
            wxWindow* childWindow = (wxWindow*) GetWindow()->GetChildren().GetLast()->GetData();
            *toObject = childWindow->GetOrCreateAccessible();

            return wxACC_OK;
        }
    case wxNAVDIR_RIGHT:
    case wxNAVDIR_DOWN:
    case wxNAVDIR_NEXT:
        {
            wxWindowList::compatibility_iterator node = NULL;
            if (fromId == 0)
            {
                // Can't navigate to sibling of this window
                // if we're a top-level window.
                if (!GetWindow()->GetParent())
                    return wxACC_NOT_IMPLEMENTED;

                node = GetWindow()->GetParent()->GetChildren().Find(GetWindow());
            }
            else
            {
                if (fromId <= (int) GetWindow()->GetChildren().GetCount())
                    node = GetWindow()->GetChildren().Nth(fromId-1);
            }

            if (node && node->GetNext())
            {
                wxWindow* nextWindow = (wxWindow*) node->GetNext()->Data();
                *toObject = nextWindow->GetOrCreateAccessible();
                return wxACC_OK;
            }
            else
                return wxACC_FALSE;
        }
    case wxNAVDIR_LEFT:
    case wxNAVDIR_UP:
    case wxNAVDIR_PREVIOUS:
        {
            wxWindowList::compatibility_iterator node = NULL;
            if (fromId == 0)
            {
                // Can't navigate to sibling of this window
                // if we're a top-level window.
                if (!GetWindow()->GetParent())
                    return wxACC_NOT_IMPLEMENTED;

                node = GetWindow()->GetParent()->GetChildren().Find(GetWindow());
            }
            else
            {
                if (fromId <= (int) GetWindow()->GetChildren().GetCount())
                    node = GetWindow()->GetChildren().Nth(fromId-1);
            }

            if (node && node->GetPrevious())
            {
                wxWindow* previousWindow = (wxWindow*) node->GetPrevious()->Data();
                *toObject = previousWindow->GetOrCreateAccessible();
                return wxACC_OK;
            }
            else
                return wxACC_FALSE;
        }
    }

    return wxACC_NOT_IMPLEMENTED;
}

// Gets the name of the specified object.
wxAccStatus wxWindowAccessible::GetName(int childId, wxString* name)
{
    wxASSERT( GetWindow() != NULL );
    if (!GetWindow())
        return wxACC_FAIL;

    wxString title;

    // If a child, leave wxWindows to call the function on the actual
    // child object.
    if (childId > 0)
        return wxACC_NOT_IMPLEMENTED;

    // This will eventually be replaced by specialised
    // accessible classes, one for each kind of wxWindows
    // control or window.
    if (GetWindow()->IsKindOf(CLASSINFO(wxButton)))
        title = ((wxButton*) GetWindow())->GetLabel();
    else
        title = GetWindow()->GetName();
    
    if (!title.IsEmpty())
    {
        *name = title;
        return wxACC_OK;
    }
    else
        return wxACC_NOT_IMPLEMENTED;
}

// Gets the number of children.
wxAccStatus wxWindowAccessible::GetChildCount(int* childId)
{
    wxASSERT( GetWindow() != NULL );
    if (!GetWindow())
        return wxACC_FAIL;

    *childId = (int) GetWindow()->GetChildren().GetCount();
    return wxACC_OK;
}

// Gets the specified child (starting from 1).
// If *child is NULL and return value is wxACC_OK,
// this means that the child is a simple element and
// not an accessible object.
wxAccStatus wxWindowAccessible::GetChild(int childId, wxAccessible** child)
{
    wxASSERT( GetWindow() != NULL );
    if (!GetWindow())
        return wxACC_FAIL;

    if (childId == 0)
    {
        *child = this;
        return wxACC_OK;
    }

    if (childId > (int) GetWindow()->GetChildren().GetCount())
        return wxACC_FAIL;

    wxWindow* childWindow = (wxWindow*) GetWindow()->GetChildren().Nth(childId-1)->GetData();
    *child = childWindow->GetOrCreateAccessible();
    if (*child)
        return wxACC_OK;
    else
        return wxACC_FAIL;
}

// Gets the parent, or NULL.
wxAccStatus wxWindowAccessible::GetParent(wxAccessible** parent)
{
    wxASSERT( GetWindow() != NULL );
    if (!GetWindow())
        return wxACC_FAIL;

    wxWindow* parentWindow = GetWindow()->GetParent();
    if (!parentWindow)
    {
        *parent = NULL;
        return wxACC_OK;
    }
    else
    {
        *parent = parentWindow->GetOrCreateAccessible();
        if (*parent)
            return wxACC_OK;
        else
            return wxACC_FAIL;
    }
}

// Performs the default action. childId is 0 (the action for this object)
// or > 0 (the action for a child).
// Return wxACC_NOT_SUPPORTED if there is no default action for this
// window (e.g. an edit control).
wxAccStatus wxWindowAccessible::DoDefaultAction(int childId)
{
    wxASSERT( GetWindow() != NULL );
    if (!GetWindow())
        return wxACC_FAIL;

    return wxACC_NOT_IMPLEMENTED;
}

// Gets the default action for this object (0) or > 0 (the action for a child).
// Return wxACC_OK even if there is no action. actionName is the action, or the empty
// string if there is no action.
// The retrieved string describes the action that is performed on an object,
// not what the object does as a result. For example, a toolbar button that prints
// a document has a default action of "Press" rather than "Prints the current document."
wxAccStatus wxWindowAccessible::GetDefaultAction(int childId, wxString* actionName)
{
    wxASSERT( GetWindow() != NULL );
    if (!GetWindow())
        return wxACC_FAIL;

    return wxACC_NOT_IMPLEMENTED;
}

// Returns the description for this object or a child.
wxAccStatus wxWindowAccessible::GetDescription(int childId, wxString* description)
{
    wxASSERT( GetWindow() != NULL );
    if (!GetWindow())
        return wxACC_FAIL;

    wxString ht(GetWindow()->GetHelpText());
    if (!ht.IsEmpty())
    {
        *description = ht;
        return wxACC_OK;
    }
    return wxACC_NOT_IMPLEMENTED;
}

// Returns help text for this object or a child, similar to tooltip text.
wxAccStatus wxWindowAccessible::GetHelpText(int childId, wxString* helpText)
{
    wxASSERT( GetWindow() != NULL );
    if (!GetWindow())
        return wxACC_FAIL;

    wxString ht(GetWindow()->GetHelpText());
    if (!ht.IsEmpty())
    {
        *helpText = ht;
        return wxACC_OK;
    }
    return wxACC_NOT_IMPLEMENTED;
}

// Returns the keyboard shortcut for this object or child.
// Return e.g. ALT+K
wxAccStatus wxWindowAccessible::GetKeyboardShortcut(int childId, wxString* shortcut)
{
    wxASSERT( GetWindow() != NULL );
    if (!GetWindow())
        return wxACC_FAIL;

    return wxACC_NOT_IMPLEMENTED;
}

// Returns a role constant.
wxAccStatus wxWindowAccessible::GetRole(int childId, wxAccRole* role)
{
    wxASSERT( GetWindow() != NULL );
    if (!GetWindow())
        return wxACC_FAIL;

    // If a child, leave wxWindows to call the function on the actual
    // child object.
    if (childId > 0)
        return wxACC_NOT_IMPLEMENTED;

    if (GetWindow()->IsKindOf(CLASSINFO(wxControl)))
        return wxACC_NOT_IMPLEMENTED;
#if wxUSE_STATUSBAR
    if (GetWindow()->IsKindOf(CLASSINFO(wxStatusBar)))
        return wxACC_NOT_IMPLEMENTED;
#endif
#if wxUSE_TOOLBAR
    if (GetWindow()->IsKindOf(CLASSINFO(wxToolBar)))
        return wxACC_NOT_IMPLEMENTED;
#endif

    //*role = wxROLE_SYSTEM_CLIENT;
    *role = wxROLE_SYSTEM_CLIENT;
    return wxACC_OK;

    return wxACC_NOT_IMPLEMENTED;
}

// Returns a state constant.
wxAccStatus wxWindowAccessible::GetState(int childId, long* state)
{
    wxASSERT( GetWindow() != NULL );
    if (!GetWindow())
        return wxACC_FAIL;

    // If a child, leave wxWindows to call the function on the actual
    // child object.
    if (childId > 0)
        return wxACC_NOT_IMPLEMENTED;

    if (GetWindow()->IsKindOf(CLASSINFO(wxControl)))
        return wxACC_NOT_IMPLEMENTED;

#if wxUSE_STATUSBAR
    if (GetWindow()->IsKindOf(CLASSINFO(wxStatusBar)))
        return wxACC_NOT_IMPLEMENTED;
#endif
#if wxUSE_TOOLBAR
    if (GetWindow()->IsKindOf(CLASSINFO(wxToolBar)))
        return wxACC_NOT_IMPLEMENTED;
#endif

    *state = 0;
    return wxACC_OK;

    return wxACC_NOT_IMPLEMENTED;
}

// Returns a localized string representing the value for the object
// or child.
wxAccStatus wxWindowAccessible::GetValue(int childId, wxString* strValue)
{
    wxASSERT( GetWindow() != NULL );
    if (!GetWindow())
        return wxACC_FAIL;

    return wxACC_NOT_IMPLEMENTED;
}

// Selects the object or child.
wxAccStatus wxWindowAccessible::Select(int childId, wxAccSelectionFlags selectFlags)
{
    wxASSERT( GetWindow() != NULL );
    if (!GetWindow())
        return wxACC_FAIL;

    return wxACC_NOT_IMPLEMENTED;
}

// Gets the window with the keyboard focus.
// If childId is 0 and child is NULL, no object in
// this subhierarchy has the focus.
// If this object has the focus, child should be 'this'.
wxAccStatus wxWindowAccessible::GetFocus(int* childId, wxAccessible** child)
{
    wxASSERT( GetWindow() != NULL );
    if (!GetWindow())
        return wxACC_FAIL;

    return wxACC_NOT_IMPLEMENTED;
}

// Gets a variant representing the selected children
// of this object.
// Acceptable values:
// - a null variant (IsNull() returns TRUE)
// - a list variant (GetType() == wxT("list")
// - an integer representing the selected child element,
//   or 0 if this object is selected (GetType() == wxT("long")
// - a "void*" pointer to a wxAccessible child object
wxAccStatus wxWindowAccessible::GetSelections(wxVariant* selections)
{
    wxASSERT( GetWindow() != NULL );
    if (!GetWindow())
        return wxACC_FAIL;

    return wxACC_NOT_IMPLEMENTED;
}

#endif // wxUSE_ACCESSIBILITY
