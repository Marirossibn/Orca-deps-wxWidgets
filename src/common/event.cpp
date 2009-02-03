/////////////////////////////////////////////////////////////////////////////
// Name:        src/common/event.cpp
// Purpose:     Event classes
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/event.h"
#include "wx/evtloop.h"

#ifndef WX_PRECOMP
    #include "wx/list.h"
    #include "wx/app.h"
    #include "wx/utils.h"
    #include "wx/stopwatch.h"
    #include "wx/module.h"

    #if wxUSE_GUI
        #include "wx/window.h"
        #include "wx/control.h"
        #include "wx/dc.h"
        #include "wx/spinbutt.h"
        #include "wx/textctrl.h"
        #include "wx/validate.h"
    #endif // wxUSE_GUI
#endif

#include "wx/thread.h"

#if wxUSE_BASE
    #include "wx/scopedptr.h"

    wxDECLARE_SCOPED_PTR(wxEvent, wxEventPtr)
    wxDEFINE_SCOPED_PTR(wxEvent, wxEventPtr)
#endif // wxUSE_BASE

// ----------------------------------------------------------------------------
// wxWin macros
// ----------------------------------------------------------------------------

#if wxUSE_BASE
    IMPLEMENT_DYNAMIC_CLASS(wxEvtHandler, wxObject)
    IMPLEMENT_ABSTRACT_CLASS(wxEvent, wxObject)
    IMPLEMENT_DYNAMIC_CLASS(wxIdleEvent, wxEvent)
#endif // wxUSE_BASE

#if wxUSE_GUI
    IMPLEMENT_DYNAMIC_CLASS(wxCommandEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxNotifyEvent, wxCommandEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxScrollEvent, wxCommandEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxScrollWinEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxMouseEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxKeyEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxSizeEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxPaintEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxNcPaintEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxEraseEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxMoveEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxFocusEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxChildFocusEvent, wxCommandEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxCloseEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxShowEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxMaximizeEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxIconizeEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxMenuEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxJoystickEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxDropFilesEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxActivateEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxInitDialogEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxSetCursorEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxSysColourChangedEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxDisplayChangedEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxUpdateUIEvent, wxCommandEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxNavigationKeyEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxPaletteChangedEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxQueryNewPaletteEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxWindowCreateEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxWindowDestroyEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxHelpEvent, wxCommandEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxContextMenuEvent, wxCommandEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxMouseCaptureChangedEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxMouseCaptureLostEvent, wxEvent)
    IMPLEMENT_DYNAMIC_CLASS(wxClipboardTextEvent, wxCommandEvent)
#endif // wxUSE_GUI

#if wxUSE_BASE

const wxEventTable *wxEvtHandler::GetEventTable() const
    { return &wxEvtHandler::sm_eventTable; }

const wxEventTable wxEvtHandler::sm_eventTable =
    { (const wxEventTable *)NULL, &wxEvtHandler::sm_eventTableEntries[0] };

wxEventHashTable &wxEvtHandler::GetEventHashTable() const
    { return wxEvtHandler::sm_eventHashTable; }

wxEventHashTable wxEvtHandler::sm_eventHashTable(wxEvtHandler::sm_eventTable);

const wxEventTableEntry wxEvtHandler::sm_eventTableEntries[] =
    { DECLARE_EVENT_TABLE_TERMINATOR() };


// wxUSE_MEMORY_TRACING considers memory freed from the static objects dtors
// leaked, so we need to manually clean up all event tables before checking for
// the memory leaks when using it, however this breaks re-initializing the
// library (i.e. repeated calls to wxInitialize/wxUninitialize) because the
// event tables won't be rebuilt the next time, so disable this by default
#if defined(__WXDEBUG__) && wxUSE_MEMORY_TRACING

class wxEventTableEntryModule: public wxModule
{
public:
    wxEventTableEntryModule() { }
    virtual bool OnInit() { return true; }
    virtual void OnExit() { wxEventHashTable::ClearAll(); }

    DECLARE_DYNAMIC_CLASS(wxEventTableEntryModule)
};

IMPLEMENT_DYNAMIC_CLASS(wxEventTableEntryModule, wxModule)

#endif // __WXDEBUG__ && wxUSE_MEMORY_TRACING

// ----------------------------------------------------------------------------
// global variables
// ----------------------------------------------------------------------------

// List containing event handlers with pending events (each handler can occur
// at most once here)
wxList *wxHandlersWithPendingEvents = NULL;

#if wxUSE_THREADS
    // protects wxHandlersWithPendingEvents list
    wxCriticalSection *wxHandlersWithPendingEventsLocker = NULL;
#endif

// common event types are defined here, other event types are defined by the
// components which use them

const wxEventType wxEVT_FIRST = 10000;
const wxEventType wxEVT_USER_FIRST = wxEVT_FIRST + 2000;

DEFINE_EVENT_TYPE(wxEVT_NULL)
wxDEFINE_EVENT( wxEVT_IDLE, wxIdleEvent )

#endif // wxUSE_BASE

#if wxUSE_GUI

wxDEFINE_EVENT( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEvent )
wxDEFINE_EVENT( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEvent )
wxDEFINE_EVENT( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEvent )
wxDEFINE_EVENT( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEvent )
wxDEFINE_EVENT( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEvent )
wxDEFINE_EVENT( wxEVT_COMMAND_CHECKLISTBOX_TOGGLED, wxCommandEvent )
wxDEFINE_EVENT( wxEVT_COMMAND_MENU_SELECTED, wxCommandEvent )
wxDEFINE_EVENT( wxEVT_COMMAND_SLIDER_UPDATED, wxCommandEvent )
wxDEFINE_EVENT( wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEvent )
wxDEFINE_EVENT( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEvent )
wxDEFINE_EVENT( wxEVT_COMMAND_SCROLLBAR_UPDATED, wxCommandEvent )
wxDEFINE_EVENT( wxEVT_COMMAND_VLBOX_SELECTED, wxCommandEvent )
wxDEFINE_EVENT( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEvent )
wxDEFINE_EVENT( wxEVT_COMMAND_TOOL_RCLICKED, wxCommandEvent )
wxDEFINE_EVENT( wxEVT_COMMAND_TOOL_ENTER, wxCommandEvent )
wxDEFINE_EVENT( wxEVT_COMMAND_SPINCTRL_UPDATED, wxCommandEvent )
wxDEFINE_EVENT( wxEVT_COMMAND_SPINCTRLDOUBLE_UPDATED, wxCommandEvent )
wxDEFINE_EVENT( wxEVT_COMMAND_TOOL_DROPDOWN_CLICKED, wxCommandEvent )

// Mouse event types
wxDEFINE_EVENT( wxEVT_LEFT_DOWN, wxMouseEvent )
wxDEFINE_EVENT( wxEVT_LEFT_UP, wxMouseEvent )
wxDEFINE_EVENT( wxEVT_MIDDLE_DOWN, wxMouseEvent )
wxDEFINE_EVENT( wxEVT_MIDDLE_UP, wxMouseEvent )
wxDEFINE_EVENT( wxEVT_RIGHT_DOWN, wxMouseEvent )
wxDEFINE_EVENT( wxEVT_RIGHT_UP, wxMouseEvent )
wxDEFINE_EVENT( wxEVT_MOTION, wxMouseEvent )
wxDEFINE_EVENT( wxEVT_ENTER_WINDOW, wxMouseEvent )
wxDEFINE_EVENT( wxEVT_LEAVE_WINDOW, wxMouseEvent )
wxDEFINE_EVENT( wxEVT_LEFT_DCLICK, wxMouseEvent )
wxDEFINE_EVENT( wxEVT_MIDDLE_DCLICK, wxMouseEvent )
wxDEFINE_EVENT( wxEVT_RIGHT_DCLICK, wxMouseEvent )
wxDEFINE_EVENT( wxEVT_SET_FOCUS, wxFocusEvent )
wxDEFINE_EVENT( wxEVT_KILL_FOCUS, wxFocusEvent )
wxDEFINE_EVENT( wxEVT_CHILD_FOCUS, wxChildFocusEvent )
wxDEFINE_EVENT( wxEVT_MOUSEWHEEL, wxMouseEvent )
wxDEFINE_EVENT( wxEVT_AUX1_DOWN, wxMouseEvent )
wxDEFINE_EVENT( wxEVT_AUX1_UP, wxMouseEvent )
wxDEFINE_EVENT( wxEVT_AUX1_DCLICK, wxMouseEvent )
wxDEFINE_EVENT( wxEVT_AUX2_DOWN, wxMouseEvent )
wxDEFINE_EVENT( wxEVT_AUX2_UP, wxMouseEvent )
wxDEFINE_EVENT( wxEVT_AUX2_DCLICK, wxMouseEvent )

// Character input event type
wxDEFINE_EVENT( wxEVT_CHAR, wxKeyEvent )
wxDEFINE_EVENT( wxEVT_CHAR_HOOK, wxKeyEvent )
wxDEFINE_EVENT( wxEVT_NAVIGATION_KEY, wxNavigationKeyEvent )
wxDEFINE_EVENT( wxEVT_KEY_DOWN, wxKeyEvent )
wxDEFINE_EVENT( wxEVT_KEY_UP, wxKeyEvent )
#if wxUSE_HOTKEY
wxDEFINE_EVENT( wxEVT_HOTKEY, wxKeyEvent )
#endif

// Set cursor event
wxDEFINE_EVENT( wxEVT_SET_CURSOR, wxSetCursorEvent )

// wxScrollbar and wxSlider event identifiers
wxDEFINE_EVENT( wxEVT_SCROLL_TOP, wxScrollEvent )
wxDEFINE_EVENT( wxEVT_SCROLL_BOTTOM, wxScrollEvent )
wxDEFINE_EVENT( wxEVT_SCROLL_LINEUP, wxScrollEvent )
wxDEFINE_EVENT( wxEVT_SCROLL_LINEDOWN, wxScrollEvent )
wxDEFINE_EVENT( wxEVT_SCROLL_PAGEUP, wxScrollEvent )
wxDEFINE_EVENT( wxEVT_SCROLL_PAGEDOWN, wxScrollEvent )
wxDEFINE_EVENT( wxEVT_SCROLL_THUMBTRACK, wxScrollEvent )
wxDEFINE_EVENT( wxEVT_SCROLL_THUMBRELEASE, wxScrollEvent )
wxDEFINE_EVENT( wxEVT_SCROLL_CHANGED, wxScrollEvent )

// Due to a bug in older wx versions, wxSpinEvents were being sent with type of
// wxEVT_SCROLL_LINEUP, wxEVT_SCROLL_LINEDOWN and wxEVT_SCROLL_THUMBTRACK. But
// with the type-safe events in place, these event types are associated with
// wxScrollEvent. To allow handling of spin events, new event types have been
// defined in spinbutt.h/spinnbuttcmn.cpp. To maintain backward compatibility
// the spin event types are being initialized with the scroll event types.

#if wxUSE_SPINBTN

wxDEFINE_EVENT_ALIAS( wxEVT_SPIN_UP,   wxSpinEvent, wxEVT_SCROLL_LINEUP )
wxDEFINE_EVENT_ALIAS( wxEVT_SPIN_DOWN, wxSpinEvent, wxEVT_SCROLL_LINEDOWN )
wxDEFINE_EVENT_ALIAS( wxEVT_SPIN,      wxSpinEvent, wxEVT_SCROLL_THUMBTRACK )

#endif // wxUSE_SPINBTN

// Scroll events from wxWindow
wxDEFINE_EVENT( wxEVT_SCROLLWIN_TOP, wxScrollWinEvent )
wxDEFINE_EVENT( wxEVT_SCROLLWIN_BOTTOM, wxScrollWinEvent )
wxDEFINE_EVENT( wxEVT_SCROLLWIN_LINEUP, wxScrollWinEvent )
wxDEFINE_EVENT( wxEVT_SCROLLWIN_LINEDOWN, wxScrollWinEvent )
wxDEFINE_EVENT( wxEVT_SCROLLWIN_PAGEUP, wxScrollWinEvent )
wxDEFINE_EVENT( wxEVT_SCROLLWIN_PAGEDOWN, wxScrollWinEvent )
wxDEFINE_EVENT( wxEVT_SCROLLWIN_THUMBTRACK, wxScrollWinEvent )
wxDEFINE_EVENT( wxEVT_SCROLLWIN_THUMBRELEASE, wxScrollWinEvent )

// System events
wxDEFINE_EVENT( wxEVT_SIZE, wxSizeEvent )
wxDEFINE_EVENT( wxEVT_SIZING, wxSizeEvent )
wxDEFINE_EVENT( wxEVT_MOVE, wxMoveEvent )
wxDEFINE_EVENT( wxEVT_MOVING, wxMoveEvent )
wxDEFINE_EVENT( wxEVT_MOVE_START, wxMoveEvent )
wxDEFINE_EVENT( wxEVT_MOVE_END, wxMoveEvent )
wxDEFINE_EVENT( wxEVT_CLOSE_WINDOW, wxCloseEvent )
wxDEFINE_EVENT( wxEVT_END_SESSION, wxCloseEvent )
wxDEFINE_EVENT( wxEVT_QUERY_END_SESSION, wxCloseEvent )
wxDEFINE_EVENT( wxEVT_HIBERNATE, wxActivateEvent )
wxDEFINE_EVENT( wxEVT_ACTIVATE_APP, wxActivateEvent )
wxDEFINE_EVENT( wxEVT_ACTIVATE, wxActivateEvent )
wxDEFINE_EVENT( wxEVT_CREATE, wxWindowCreateEvent )
wxDEFINE_EVENT( wxEVT_DESTROY, wxWindowDestroyEvent )
wxDEFINE_EVENT( wxEVT_SHOW, wxShowEvent )
wxDEFINE_EVENT( wxEVT_ICONIZE, wxIconizeEvent )
wxDEFINE_EVENT( wxEVT_MAXIMIZE, wxMaximizeEvent )
wxDEFINE_EVENT( wxEVT_MOUSE_CAPTURE_CHANGED, wxMouseCaptureChangedEvent )
wxDEFINE_EVENT( wxEVT_MOUSE_CAPTURE_LOST, wxMouseCaptureLostEvent )
wxDEFINE_EVENT( wxEVT_PAINT, wxPaintEvent )
wxDEFINE_EVENT( wxEVT_ERASE_BACKGROUND, wxEraseEvent )
wxDEFINE_EVENT( wxEVT_NC_PAINT, wxNcPaintEvent )
wxDEFINE_EVENT( wxEVT_MENU_OPEN, wxMenuEvent )
wxDEFINE_EVENT( wxEVT_MENU_CLOSE, wxMenuEvent )
wxDEFINE_EVENT( wxEVT_MENU_HIGHLIGHT, wxMenuEvent )
wxDEFINE_EVENT( wxEVT_CONTEXT_MENU, wxContextMenuEvent )
wxDEFINE_EVENT( wxEVT_SYS_COLOUR_CHANGED, wxSysColourChangedEvent )
wxDEFINE_EVENT( wxEVT_DISPLAY_CHANGED, wxDisplayChangedEvent )
wxDEFINE_EVENT( wxEVT_QUERY_NEW_PALETTE, wxQueryNewPaletteEvent )
wxDEFINE_EVENT( wxEVT_PALETTE_CHANGED, wxPaletteChangedEvent )
wxDEFINE_EVENT( wxEVT_JOY_BUTTON_DOWN, wxJoystickEvent )
wxDEFINE_EVENT( wxEVT_JOY_BUTTON_UP, wxJoystickEvent )
wxDEFINE_EVENT( wxEVT_JOY_MOVE, wxJoystickEvent )
wxDEFINE_EVENT( wxEVT_JOY_ZMOVE, wxJoystickEvent )
wxDEFINE_EVENT( wxEVT_DROP_FILES, wxDropFilesEvent )
wxDEFINE_EVENT( wxEVT_INIT_DIALOG, wxInitDialogEvent )
wxDEFINE_EVENT( wxEVT_UPDATE_UI, wxUpdateUIEvent )

// Clipboard events
wxDEFINE_EVENT( wxEVT_COMMAND_TEXT_COPY, wxClipboardTextEvent )
wxDEFINE_EVENT( wxEVT_COMMAND_TEXT_CUT, wxClipboardTextEvent )
wxDEFINE_EVENT( wxEVT_COMMAND_TEXT_PASTE, wxClipboardTextEvent )

// Generic command events
// Note: a click is a higher-level event than button down/up
wxDEFINE_EVENT( wxEVT_COMMAND_LEFT_CLICK, wxCommandEvent )
wxDEFINE_EVENT( wxEVT_COMMAND_LEFT_DCLICK, wxCommandEvent )
wxDEFINE_EVENT( wxEVT_COMMAND_RIGHT_CLICK, wxCommandEvent )
wxDEFINE_EVENT( wxEVT_COMMAND_RIGHT_DCLICK, wxCommandEvent )
wxDEFINE_EVENT( wxEVT_COMMAND_SET_FOCUS, wxCommandEvent )
wxDEFINE_EVENT( wxEVT_COMMAND_KILL_FOCUS, wxCommandEvent )
wxDEFINE_EVENT( wxEVT_COMMAND_ENTER, wxCommandEvent )

// Help events
wxDEFINE_EVENT( wxEVT_HELP, wxHelpEvent )
wxDEFINE_EVENT( wxEVT_DETAILED_HELP, wxHelpEvent )

#endif // wxUSE_GUI

#if wxUSE_BASE

wxIdleMode wxIdleEvent::sm_idleMode = wxIDLE_PROCESS_ALL;

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// event initialization
// ----------------------------------------------------------------------------

int wxNewEventType()
{
    // MT-FIXME
    static int s_lastUsedEventType = wxEVT_FIRST;

    return s_lastUsedEventType++;
}
// ----------------------------------------------------------------------------
// wxEventFunctor
// ----------------------------------------------------------------------------

wxEventFunctor::~wxEventFunctor()
{
}

// ----------------------------------------------------------------------------
// wxEvent
// ----------------------------------------------------------------------------

/*
 * General wxWidgets events, covering
 * all interesting things that might happen (button clicking, resizing,
 * setting text in widgets, etc.).
 *
 * For each completely new event type, derive a new event class.
 *
 */

wxEvent::wxEvent(int theId, wxEventType commandType )
{
    m_eventType = commandType;
    m_eventObject = NULL;
    m_timeStamp = 0;
    m_id = theId;
    m_skipped = false;
    m_callbackUserData = NULL;
    m_isCommandEvent = false;
    m_propagationLevel = wxEVENT_PROPAGATE_NONE;
    m_wasProcessed = false;
}

wxEvent::wxEvent(const wxEvent& src)
    : wxObject(src)
    , m_eventObject(src.m_eventObject)
    , m_eventType(src.m_eventType)
    , m_timeStamp(src.m_timeStamp)
    , m_id(src.m_id)
    , m_callbackUserData(src.m_callbackUserData)
    , m_propagationLevel(src.m_propagationLevel)
    , m_skipped(src.m_skipped)
    , m_isCommandEvent(src.m_isCommandEvent)
    , m_wasProcessed(false)
{
}

wxEvent& wxEvent::operator=(const wxEvent& src)
{
    wxObject::operator=(src);

    m_eventObject = src.m_eventObject;
    m_eventType = src.m_eventType;
    m_timeStamp = src.m_timeStamp;
    m_id = src.m_id;
    m_callbackUserData = src.m_callbackUserData;
    m_propagationLevel = src.m_propagationLevel;
    m_skipped = src.m_skipped;
    m_isCommandEvent = src.m_isCommandEvent;

    // don't change m_wasProcessed

    return *this;
}

#endif // wxUSE_BASE

#if wxUSE_GUI

/*
 * Command events
 *
 */

#ifdef __VISUALC__
    // 'this' : used in base member initializer list (for m_commandString)
    #pragma warning(disable:4355)
#endif

wxCommandEvent::wxCommandEvent(wxEventType commandType, int theId)
              : wxEvent(theId, commandType)
{
    m_clientData = NULL;
    m_clientObject = NULL;
    m_extraLong = 0;
    m_commandInt = 0;
    m_isCommandEvent = true;

    // the command events are propagated upwards by default
    m_propagationLevel = wxEVENT_PROPAGATE_MAX;
}

#ifdef __VISUALC__
    #pragma warning(default:4355)
#endif

wxString wxCommandEvent::GetString() const
{
    if (m_eventType != wxEVT_COMMAND_TEXT_UPDATED || !m_eventObject)
    {
        return m_cmdString;
    }
    else
    {
#if wxUSE_TEXTCTRL
        wxTextCtrl *txt = wxDynamicCast(m_eventObject, wxTextCtrl);
        if ( txt )
            return txt->GetValue();
        else
#endif // wxUSE_TEXTCTRL
            return m_cmdString;
    }
}

/*
 * UI update events
 */

#if wxUSE_LONGLONG
wxLongLong wxUpdateUIEvent::sm_lastUpdate = 0;
#endif

long wxUpdateUIEvent::sm_updateInterval = 0;

wxUpdateUIMode wxUpdateUIEvent::sm_updateMode = wxUPDATE_UI_PROCESS_ALL;

// Can we update?
bool wxUpdateUIEvent::CanUpdate(wxWindowBase *win)
{
    // Don't update if we've switched global updating off
    // and this window doesn't support updates.
    if (win &&
       (GetMode() == wxUPDATE_UI_PROCESS_SPECIFIED &&
       ((win->GetExtraStyle() & wxWS_EX_PROCESS_UI_UPDATES) == 0)))
        return false;

    if (sm_updateInterval == -1)
        return false;

    if (sm_updateInterval == 0)
        return true;

#if wxUSE_STOPWATCH && wxUSE_LONGLONG
    wxLongLong now = wxGetLocalTimeMillis();
    if (now > (sm_lastUpdate + sm_updateInterval))
    {
        return true;
    }

    return false;
#else
    // If we don't have wxStopWatch or wxLongLong, we
    // should err on the safe side and update now anyway.
    return true;
#endif
}

// Reset the update time to provide a delay until the next
// time we should update
void wxUpdateUIEvent::ResetUpdateTime()
{
#if wxUSE_STOPWATCH && wxUSE_LONGLONG
    if (sm_updateInterval > 0)
    {
        wxLongLong now = wxGetLocalTimeMillis();
        if (now > (sm_lastUpdate + sm_updateInterval))
        {
            sm_lastUpdate = now;
        }
    }
#endif
}

/*
 * Scroll events
 */

wxScrollEvent::wxScrollEvent(wxEventType commandType,
                             int id,
                             int pos,
                             int orient)
    : wxCommandEvent(commandType, id)
{
    m_extraLong = orient;
    m_commandInt = pos;
}

/*
 * ScrollWin events
 */

wxScrollWinEvent::wxScrollWinEvent(wxEventType commandType,
                                   int pos,
                                   int orient)
{
    m_eventType = commandType;
    m_extraLong = orient;
    m_commandInt = pos;
}

/*
 * Mouse events
 *
 */

wxMouseEvent::wxMouseEvent(wxEventType commandType)
{
    m_eventType = commandType;

    m_x = 0;
    m_y = 0;

    m_leftDown = false;
    m_middleDown = false;
    m_rightDown = false;
    m_aux1Down = false;
    m_aux2Down = false;

    m_clickCount = -1;

    m_wheelRotation = 0;
    m_wheelDelta = 0;
    m_linesPerAction = 0;
    m_wheelAxis = 0;
}

void wxMouseEvent::Assign(const wxMouseEvent& event)
{
    wxEvent::operator=(event);

    // Borland C++ 5.82 doesn't compile an explicit call to an implicitly
    // defined operator=() so need to do it this way:
    *static_cast<wxMouseState *>(this) = event;

    m_x = event.m_x;
    m_y = event.m_y;

    m_leftDown = event.m_leftDown;
    m_middleDown = event.m_middleDown;
    m_rightDown = event.m_rightDown;
    m_aux1Down = event.m_aux1Down;
    m_aux2Down = event.m_aux2Down;

    m_wheelRotation = event.m_wheelRotation;
    m_wheelDelta = event.m_wheelDelta;
    m_linesPerAction = event.m_linesPerAction;
    m_wheelAxis = event.m_wheelAxis;
}

// return true if was a button dclick event
bool wxMouseEvent::ButtonDClick(int but) const
{
    switch (but)
    {
        default:
            wxFAIL_MSG(wxT("invalid parameter in wxMouseEvent::ButtonDClick"));
            // fall through

        case wxMOUSE_BTN_ANY:
            return (LeftDClick() || MiddleDClick() || RightDClick() ||
                    Aux1DClick() || Aux2DClick());

        case wxMOUSE_BTN_LEFT:
            return LeftDClick();

        case wxMOUSE_BTN_MIDDLE:
            return MiddleDClick();

        case wxMOUSE_BTN_RIGHT:
            return RightDClick();

        case wxMOUSE_BTN_AUX1:
            return Aux1DClick();

        case wxMOUSE_BTN_AUX2:
            return Aux2DClick();
    }
}

// return true if was a button down event
bool wxMouseEvent::ButtonDown(int but) const
{
    switch (but)
    {
        default:
            wxFAIL_MSG(wxT("invalid parameter in wxMouseEvent::ButtonDown"));
            // fall through

        case wxMOUSE_BTN_ANY:
            return (LeftDown() || MiddleDown() || RightDown() ||
                    Aux1Down() || Aux2Down());

        case wxMOUSE_BTN_LEFT:
            return LeftDown();

        case wxMOUSE_BTN_MIDDLE:
            return MiddleDown();

        case wxMOUSE_BTN_RIGHT:
            return RightDown();

        case wxMOUSE_BTN_AUX1:
            return Aux1Down();

        case wxMOUSE_BTN_AUX2:
            return Aux2Down();
    }
}

// return true if was a button up event
bool wxMouseEvent::ButtonUp(int but) const
{
    switch (but)
    {
        default:
            wxFAIL_MSG(wxT("invalid parameter in wxMouseEvent::ButtonUp"));
            // fall through

        case wxMOUSE_BTN_ANY:
            return (LeftUp() || MiddleUp() || RightUp() ||
                    Aux1Up() || Aux2Up());

        case wxMOUSE_BTN_LEFT:
            return LeftUp();

        case wxMOUSE_BTN_MIDDLE:
            return MiddleUp();

        case wxMOUSE_BTN_RIGHT:
            return RightUp();

        case wxMOUSE_BTN_AUX1:
            return Aux1Up();

        case wxMOUSE_BTN_AUX2:
            return Aux2Up();
    }
}

// return true if the given button is currently changing state
bool wxMouseEvent::Button(int but) const
{
    switch (but)
    {
        default:
            wxFAIL_MSG(wxT("invalid parameter in wxMouseEvent::Button"));
            // fall through

        case wxMOUSE_BTN_ANY:
            return ButtonUp(wxMOUSE_BTN_ANY) ||
                    ButtonDown(wxMOUSE_BTN_ANY) ||
                        ButtonDClick(wxMOUSE_BTN_ANY);

        case wxMOUSE_BTN_LEFT:
            return LeftDown() || LeftUp() || LeftDClick();

        case wxMOUSE_BTN_MIDDLE:
            return MiddleDown() || MiddleUp() || MiddleDClick();

        case wxMOUSE_BTN_RIGHT:
            return RightDown() || RightUp() || RightDClick();

        case wxMOUSE_BTN_AUX1:
           return Aux1Down() || Aux1Up() || Aux1DClick();

        case wxMOUSE_BTN_AUX2:
           return Aux2Down() || Aux2Up() || Aux2DClick();
    }
}

bool wxMouseEvent::ButtonIsDown(int but) const
{
    switch (but)
    {
        default:
            wxFAIL_MSG(wxT("invalid parameter in wxMouseEvent::ButtonIsDown"));
            // fall through

        case wxMOUSE_BTN_ANY:
            return LeftIsDown() || MiddleIsDown() || RightIsDown() || Aux1Down() || Aux2Down();

        case wxMOUSE_BTN_LEFT:
            return LeftIsDown();

        case wxMOUSE_BTN_MIDDLE:
            return MiddleIsDown();

        case wxMOUSE_BTN_RIGHT:
            return RightIsDown();

        case wxMOUSE_BTN_AUX1:
            return Aux1IsDown();

        case wxMOUSE_BTN_AUX2:
            return Aux2IsDown();
    }
}

int wxMouseEvent::GetButton() const
{
    for ( int i = 1; i < wxMOUSE_BTN_MAX; i++ )
    {
        if ( Button(i) )
        {
            return i;
        }
    }

    return wxMOUSE_BTN_NONE;
}

// Find the logical position of the event given the DC
wxPoint wxMouseEvent::GetLogicalPosition(const wxDC& dc) const
{
    wxPoint pt(dc.DeviceToLogicalX(m_x), dc.DeviceToLogicalY(m_y));
    return pt;
}


/*
 * Keyboard event
 *
 */

wxKeyEvent::wxKeyEvent(wxEventType type)
{
    m_eventType = type;
    m_keyCode = 0;
    m_scanCode = 0;
#if wxUSE_UNICODE
    m_uniChar = 0;
#endif
}

wxKeyEvent::wxKeyEvent(const wxKeyEvent& evt)
          : wxEvent(evt),
            wxKeyboardState(evt)
{
    m_x = evt.m_x;
    m_y = evt.m_y;

    m_keyCode = evt.m_keyCode;

    m_scanCode = evt.m_scanCode;
    m_rawCode = evt.m_rawCode;
    m_rawFlags = evt.m_rawFlags;

#if wxUSE_UNICODE
    m_uniChar = evt.m_uniChar;
#endif
}

wxWindowCreateEvent::wxWindowCreateEvent(wxWindow *win)
{
    SetEventType(wxEVT_CREATE);
    SetEventObject(win);
}

wxWindowDestroyEvent::wxWindowDestroyEvent(wxWindow *win)
{
    SetEventType(wxEVT_DESTROY);
    SetEventObject(win);
}

wxChildFocusEvent::wxChildFocusEvent(wxWindow *win)
                 : wxCommandEvent(wxEVT_CHILD_FOCUS)
{
    SetEventObject(win);
}

// ----------------------------------------------------------------------------
// wxHelpEvent
// ----------------------------------------------------------------------------

/* static */
wxHelpEvent::Origin wxHelpEvent::GuessOrigin(Origin origin)
{
    if ( origin == Origin_Unknown )
    {
        // assume that the event comes from the help button if it's not from
        // keyboard and that pressing F1 always results in the help event
        origin = wxGetKeyState(WXK_F1) ? Origin_Keyboard : Origin_HelpButton;
    }

    return origin;
}

#endif // wxUSE_GUI


#if wxUSE_BASE

// ----------------------------------------------------------------------------
// wxEventHashTable
// ----------------------------------------------------------------------------

static const int EVENT_TYPE_TABLE_INIT_SIZE = 31; // Not too big not too small...

wxEventHashTable* wxEventHashTable::sm_first = NULL;

wxEventHashTable::wxEventHashTable(const wxEventTable &table)
                : m_table(table),
                  m_rebuildHash(true)
{
    AllocEventTypeTable(EVENT_TYPE_TABLE_INIT_SIZE);

    m_next = sm_first;
    if (m_next)
        m_next->m_previous = this;
    sm_first = this;
}

wxEventHashTable::~wxEventHashTable()
{
    if (m_next)
        m_next->m_previous = m_previous;
    if (m_previous)
        m_previous->m_next = m_next;
    if (sm_first == this)
        sm_first = m_next;

    Clear();
}

void wxEventHashTable::Clear()
{
    for ( size_t i = 0; i < m_size; i++ )
    {
        EventTypeTablePointer  eTTnode = m_eventTypeTable[i];
        delete eTTnode;
    }

    delete[] m_eventTypeTable;
    m_eventTypeTable = NULL;

    m_size = 0;
}

#if defined(__WXDEBUG__) && wxUSE_MEMORY_TRACING

// Clear all tables
void wxEventHashTable::ClearAll()
{
    wxEventHashTable* table = sm_first;
    while (table)
    {
        table->Clear();
        table = table->m_next;
    }
}

#endif // __WXDEBUG__ && wxUSE_MEMORY_TRACING

bool wxEventHashTable::HandleEvent(wxEvent &event, wxEvtHandler *self)
{
    if (m_rebuildHash)
    {
        InitHashTable();
        m_rebuildHash = false;
    }

    if (!m_eventTypeTable)
        return false;

    // Find all entries for the given event type.
    wxEventType eventType = event.GetEventType();
    const EventTypeTablePointer eTTnode = m_eventTypeTable[eventType % m_size];
    if (eTTnode && eTTnode->eventType == eventType)
    {
        // Now start the search for an event handler
        // that can handle an event with the given ID.
        const wxEventTableEntryPointerArray&
            eventEntryTable = eTTnode->eventEntryTable;

        const size_t count = eventEntryTable.GetCount();
        for (size_t n = 0; n < count; n++)
        {
            const wxEventTableEntry& entry = *eventEntryTable[n];
            if ( wxEvtHandler::ProcessEventIfMatchesId(entry, self, event) )
                return true;
        }
    }

    return false;
}

void wxEventHashTable::InitHashTable()
{
    // Loop over the event tables and all its base tables.
    const wxEventTable *table = &m_table;
    while (table)
    {
        // Retrieve all valid event handler entries
        const wxEventTableEntry *entry = table->entries;
        while (entry->m_fn != 0)
        {
            // Add the event entry in the Hash.
            AddEntry(*entry);

            entry++;
        }

        table = table->baseTable;
    }

    // Lets free some memory.
    size_t i;
    for(i = 0; i < m_size; i++)
    {
        EventTypeTablePointer  eTTnode = m_eventTypeTable[i];
        if (eTTnode)
        {
            eTTnode->eventEntryTable.Shrink();
        }
    }
}

void wxEventHashTable::AddEntry(const wxEventTableEntry &entry)
{
    // This might happen 'accidentally' as the app is exiting
    if (!m_eventTypeTable)
        return;

    EventTypeTablePointer *peTTnode = &m_eventTypeTable[entry.m_eventType % m_size];
    EventTypeTablePointer  eTTnode = *peTTnode;

    if (eTTnode)
    {
        if (eTTnode->eventType != entry.m_eventType)
        {
            // Resize the table!
            GrowEventTypeTable();
            // Try again to add it.
            AddEntry(entry);
            return;
        }
    }
    else
    {
        eTTnode = new EventTypeTable;
        eTTnode->eventType = entry.m_eventType;
        *peTTnode = eTTnode;
    }

    // Fill all hash entries between entry.m_id and entry.m_lastId...
    eTTnode->eventEntryTable.Add(&entry);
}

void wxEventHashTable::AllocEventTypeTable(size_t size)
{
    m_eventTypeTable = new EventTypeTablePointer[size];
    memset((void *)m_eventTypeTable, 0, sizeof(EventTypeTablePointer)*size);
    m_size = size;
}

void wxEventHashTable::GrowEventTypeTable()
{
    size_t oldSize = m_size;
    EventTypeTablePointer *oldEventTypeTable = m_eventTypeTable;

    // TODO: Search the most optimal grow sequence
    AllocEventTypeTable(/* GetNextPrime(oldSize) */oldSize*2+1);

    for ( size_t i = 0; i < oldSize; /* */ )
    {
        EventTypeTablePointer  eTToldNode = oldEventTypeTable[i];
        if (eTToldNode)
        {
            EventTypeTablePointer *peTTnode = &m_eventTypeTable[eTToldNode->eventType % m_size];
            EventTypeTablePointer  eTTnode = *peTTnode;

            // Check for collision, we don't want any.
            if (eTTnode)
            {
                GrowEventTypeTable();
                continue; // Don't increment the counter,
                          // as we still need to add this element.
            }
            else
            {
                // Get the old value and put it in the new table.
                *peTTnode = oldEventTypeTable[i];
            }
        }

        i++;
    }

    delete[] oldEventTypeTable;
}

// ----------------------------------------------------------------------------
// wxEvtHandler
// ----------------------------------------------------------------------------

/*
 * Event handler
 */

wxEvtHandler::wxEvtHandler()
{
    m_nextHandler = NULL;
    m_previousHandler = NULL;
    m_enabled = true;
    m_dynamicEvents = NULL;
    m_pendingEvents = NULL;

    // no client data (yet)
    m_clientData = NULL;
    m_clientDataType = wxClientData_None;
}

wxEvtHandler::~wxEvtHandler()
{
    Unlink();

    if (m_dynamicEvents)
    {
        for ( wxList::iterator it = m_dynamicEvents->begin(),
                               end = m_dynamicEvents->end();
              it != end;
              ++it )
        {
            wxDynamicEventTableEntry *entry = (wxDynamicEventTableEntry*)*it;

            // Remove ourselves from sink destructor notifications
            // (this has usually been done, in wxTrackable destructor)
            wxEvtHandler *eventSink = entry->m_fn->GetEvtHandler();
            if ( eventSink )
            {
                wxEventConnectionRef * const
                    evtConnRef = FindRefInTrackerList(eventSink);
                if ( evtConnRef )
                {
                    eventSink->RemoveNode(evtConnRef);
                    delete evtConnRef;
                }
            }

            delete entry->m_callbackUserData;
            delete entry;
        }
        delete m_dynamicEvents;
    };

    if (m_pendingEvents)
        m_pendingEvents->DeleteContents(true);
    delete m_pendingEvents;

    // Remove us from wxHandlersWithPendingEvents if necessary.
    if ( wxHandlersWithPendingEvents )
    {
#if wxUSE_THREADS
        if (wxHandlersWithPendingEventsLocker)
            wxENTER_CRIT_SECT(*wxHandlersWithPendingEventsLocker);
#endif

        if ( wxHandlersWithPendingEvents->DeleteObject(this) )
        {
            // check that we were present only once in the list
            wxASSERT_MSG( !wxHandlersWithPendingEvents->Find(this),
                          "Handler occurs twice in wxHandlersWithPendingEvents list" );
        }
        //else: we weren't in this list at all, it's ok

#if wxUSE_THREADS
        if (wxHandlersWithPendingEventsLocker)
            wxLEAVE_CRIT_SECT(*wxHandlersWithPendingEventsLocker);
#endif
    }

    // we only delete object data, not untyped
    if ( m_clientDataType == wxClientData_Object )
        delete m_clientObject;
}

void wxEvtHandler::Unlink()
{
    // this event handler must take itself out of the chain of handlers:

    if (m_previousHandler)
        m_previousHandler->SetNextHandler(m_nextHandler);

    if (m_nextHandler)
        m_nextHandler->SetPreviousHandler(m_previousHandler);

    m_nextHandler = NULL;
    m_previousHandler = NULL;
}

bool wxEvtHandler::IsUnlinked() const
{
    return m_previousHandler == NULL &&
           m_nextHandler == NULL;
}

#if wxUSE_THREADS

bool wxEvtHandler::ProcessThreadEvent(const wxEvent& event)
{
    // check that we are really in a child thread
    wxASSERT_MSG( !wxThread::IsMain(),
                  wxT("use ProcessEvent() in main thread") );

    AddPendingEvent(event);

    return true;
}

#endif // wxUSE_THREADS

void wxEvtHandler::QueueEvent(wxEvent *event)
{
    wxCHECK_RET( event, "NULL event can't be posted" );

    // 1) Add this event to our list of pending events
    wxENTER_CRIT_SECT( m_pendingEventsLock );

    if ( !m_pendingEvents )
      m_pendingEvents = new wxList;

    m_pendingEvents->Append(event);

    // 2) Add this event handler to list of event handlers that
    //    have pending events.

    wxENTER_CRIT_SECT(*wxHandlersWithPendingEventsLocker);

    if ( !wxHandlersWithPendingEvents )
        wxHandlersWithPendingEvents = new wxList;
    if ( !wxHandlersWithPendingEvents->Find(this) )
        wxHandlersWithPendingEvents->Append(this);

    wxLEAVE_CRIT_SECT(*wxHandlersWithPendingEventsLocker);

    // only release m_pendingEventsLock now because otherwise there is a race
    // condition as described in the ticket #9093: we could process the event
    // just added to m_pendingEvents in our ProcessPendingEvents() below before
    // we had time to append this pointer to wxHandlersWithPendingEvents list; thus
    // breaking the invariant that a handler should be in the list iff it has
    // any pending events to process
    wxLEAVE_CRIT_SECT( m_pendingEventsLock );

    // 3) Inform the system that new pending events are somewhere,
    //    and that these should be processed in idle time.
    wxWakeUpIdle();
}

void wxEvtHandler::ProcessPendingEvents()
{
    wxENTER_CRIT_SECT( m_pendingEventsLock );

    // this method is only called by wxApp if this handler does have
    // pending events
    wxCHECK_RET( m_pendingEvents && !m_pendingEvents->IsEmpty(),
                 "should have pending events if called" );

    wxList::compatibility_iterator node = m_pendingEvents->GetFirst();
    wxEventPtr event(static_cast<wxEvent *>(node->GetData()));

    // it's important we remove event from list before processing it, else a
    // nested event loop, for example from a modal dialog, might process the
    // same event again.
    m_pendingEvents->Erase(node);

    // if there are no more pending events left, we don't need to stay in this
    // list
    if ( m_pendingEvents->IsEmpty() )
    {
#if wxUSE_THREADS
        if (wxHandlersWithPendingEventsLocker)
            wxENTER_CRIT_SECT(*wxHandlersWithPendingEventsLocker);
#endif
        wxHandlersWithPendingEvents->DeleteObject(this);
#if wxUSE_THREADS
        if (wxHandlersWithPendingEventsLocker)
            wxLEAVE_CRIT_SECT(*wxHandlersWithPendingEventsLocker);
#endif
    }

    wxLEAVE_CRIT_SECT( m_pendingEventsLock );

    ProcessEvent(*event);

    // careful: this object could have been deleted by the event handler
    // executed by the above ProcessEvent() call, so we can't access any fields
    // of this object any more
}

/*
 * Event table stuff
 */
/* static */ bool
wxEvtHandler::ProcessEventIfMatchesId(const wxEventTableEntryBase& entry,
                                      wxEvtHandler *handler,
                                      wxEvent& event)
{
    int tableId1 = entry.m_id,
        tableId2 = entry.m_lastId;

    // match only if the event type is the same and the id is either -1 in
    // the event table (meaning "any") or the event id matches the id
    // specified in the event table either exactly or by falling into
    // range between first and last
    if ((tableId1 == wxID_ANY) ||
        (tableId2 == wxID_ANY && tableId1 == event.GetId()) ||
        (tableId2 != wxID_ANY &&
         (event.GetId() >= tableId1 && event.GetId() <= tableId2)))
    {
        event.Skip(false);
        event.m_callbackUserData = entry.m_callbackUserData;

#if wxUSE_EXCEPTIONS
        if ( wxTheApp )
        {
            // call the handler via wxApp method which allows the user to catch
            // any exceptions which may be thrown by any handler in the program
            // in one place
            wxTheApp->CallEventHandler(handler, *entry.m_fn, event);
        }
        else
#endif // wxUSE_EXCEPTIONS
        {
            (*entry.m_fn)(handler, event);
        }

        if (!event.GetSkipped())
            return true;
    }

    return false;
}

bool wxEvtHandler::TryParent(wxEvent& event)
{
    if ( GetNextHandler() )
    {
        // the next handler will pass it to wxTheApp if it doesn't process it,
        // so return from here to avoid doing it again
        return GetNextHandler()->TryParent(event);
    }

    if ( wxTheApp && (this != wxTheApp) )
    {
        // Special case: don't pass wxEVT_IDLE to wxApp, since it'll always
        // swallow it. wxEVT_IDLE is sent explicitly to wxApp so it will be
        // processed appropriately via SearchEventTable.
        if ( event.GetEventType() != wxEVT_IDLE )
        {
            if ( wxTheApp->ProcessEvent(event) )
                return true;
        }
    }

    return false;
}

bool wxEvtHandler::ProcessEvent(wxEvent& event)
{
    // allow the application to hook into event processing
    //
    // note that we should only do it if we're the first event handler called
    // to avoid calling FilterEvent() multiple times as the event goes through
    // the event handler chain and possibly upwards the window hierarchy
    if ( !event.WasProcessed() )
    {
        if ( wxTheApp )
        {
            int rc = wxTheApp->FilterEvent(event);
            if ( rc != -1 )
            {
                wxASSERT_MSG( rc == 1 || rc == 0,
                              "unexpected wxApp::FilterEvent return value" );

                return rc != 0;
            }
            //else: proceed normally
        }
    }

    if ( ProcessEventHere(event) )
        return true;

    // pass the event to the next handler, notice that we shouldn't call
    // TryParent() even if it doesn't handle the event as the last handler in
    // the chain will do it
    if ( GetNextHandler() )
        return GetNextHandler()->ProcessEvent(event);

    // propagate the event upwards the window chain and/or to the application
    // object if it wasn't processed at this level
    return TryParent(event);
}

bool wxEvtHandler::ProcessEventHere(wxEvent& event)
{
    // If the event handler is disabled it doesn't process any events
    if ( !GetEvtHandlerEnabled() )
        return false;

    // If we have a validator, it has higher priority than our own event
    // handlers
    if ( TryValidator(event) )
        return true;

    // Handle per-instance dynamic event tables first
    if ( m_dynamicEvents && SearchDynamicEventTable(event) )
        return true;

    // Then static per-class event tables
    if ( GetEventHashTable().HandleEvent(event, this) )
        return true;

    // We don't have a handler for this event.
    return false;
}

bool wxEvtHandler::SafelyProcessEvent(wxEvent& event)
{
#if wxUSE_EXCEPTIONS
    try
    {
#endif
        return ProcessEvent(event);
#if wxUSE_EXCEPTIONS
    }
    catch ( ... )
    {
        wxEventLoopBase *loop = wxEventLoopBase::GetActive();
        try
        {
            if ( !wxTheApp || !wxTheApp->OnExceptionInMainLoop() )
            {
                if ( loop )
                    loop->Exit();
            }
            //else: continue running current event loop

            return false;
        }
        catch ( ... )
        {
            // OnExceptionInMainLoop() threw, possibly rethrowing the same
            // exception again: very good, but we still need Exit() to
            // be called
            if ( loop )
                loop->Exit();
            throw;
        }
    }
#endif // wxUSE_EXCEPTIONS
}


bool wxEvtHandler::SearchEventTable(wxEventTable& table, wxEvent& event)
{
    const wxEventType eventType = event.GetEventType();
    for ( int i = 0; table.entries[i].m_fn != 0; i++ )
    {
        const wxEventTableEntry& entry = table.entries[i];
        if ( eventType == entry.m_eventType )
        {
            if ( ProcessEventIfMatchesId(entry, this, event) )
                return true;
        }
    }

    return false;
}

void wxEvtHandler::DoConnect(int id,
                             int lastId,
                             wxEventType eventType,
                             wxEventFunctor *func,
                             wxObject *userData)
{
    wxDynamicEventTableEntry *entry =
        new wxDynamicEventTableEntry(eventType, id, lastId, func, userData);

    if (!m_dynamicEvents)
        m_dynamicEvents = new wxList;

    // Insert at the front of the list so most recent additions are found first
    m_dynamicEvents->Insert( (wxObject*) entry );

    // Make sure we get to know when a sink is destroyed
    wxEvtHandler *eventSink = func->GetEvtHandler();
    if ( eventSink && eventSink != this )
    {
        wxEventConnectionRef *evtConnRef = FindRefInTrackerList(eventSink);
        if ( evtConnRef )
            evtConnRef->IncRef( );
        else
            new wxEventConnectionRef(this, eventSink);
    }
}

bool
wxEvtHandler::DoDisconnect(int id,
                           int lastId,
                           wxEventType eventType,
                           const wxEventFunctor& func,
                           wxObject *userData)
{
    if (!m_dynamicEvents)
        return false;

    // Remove connection from tracker node (wxEventConnectionRef)
    wxEvtHandler *eventSink = func.GetEvtHandler();
    if ( eventSink && eventSink != this )
    {
        wxEventConnectionRef *evtConnRef = FindRefInTrackerList(eventSink);
        if ( evtConnRef )
            evtConnRef->DecRef();
    }

    wxList::compatibility_iterator node = m_dynamicEvents->GetFirst();
    while (node)
    {
        wxDynamicEventTableEntry *entry = (wxDynamicEventTableEntry*)node->GetData();

        if ((entry->m_id == id) &&
            ((entry->m_lastId == lastId) || (lastId == wxID_ANY)) &&
            ((entry->m_eventType == eventType) || (eventType == wxEVT_NULL)) &&
            entry->m_fn->Matches(func) &&
            ((entry->m_callbackUserData == userData) || !userData))
        {
            delete entry->m_callbackUserData;
            m_dynamicEvents->Erase( node );
            delete entry;
            return true;
        }
        node = node->GetNext();
    }
    return false;
}

bool wxEvtHandler::SearchDynamicEventTable( wxEvent& event )
{
    wxCHECK_MSG( m_dynamicEvents, false,
                 wxT("caller should check that we have dynamic events") );

    wxList::compatibility_iterator node = m_dynamicEvents->GetFirst();
    while (node)
    {
        wxDynamicEventTableEntry *entry = (wxDynamicEventTableEntry*)node->GetData();

        // get next node before (maybe) calling the event handler as it could
        // call Disconnect() invalidating the current node
        node = node->GetNext();

        if ( event.GetEventType() == entry->m_eventType )
        {
            wxEvtHandler *handler = entry->m_fn->GetEvtHandler();
            if ( !handler )
               handler = this;
            if ( ProcessEventIfMatchesId(*entry, handler, event) )
                return true;
        }
    }

    return false;
}

void wxEvtHandler::DoSetClientObject( wxClientData *data )
{
    wxASSERT_MSG( m_clientDataType != wxClientData_Void,
                  wxT("can't have both object and void client data") );

    if ( m_clientObject )
        delete m_clientObject;

    m_clientObject = data;
    m_clientDataType = wxClientData_Object;
}

wxClientData *wxEvtHandler::DoGetClientObject() const
{
    // it's not an error to call GetClientObject() on a window which doesn't
    // have client data at all - NULL will be returned
    wxASSERT_MSG( m_clientDataType != wxClientData_Void,
                  wxT("this window doesn't have object client data") );

    return m_clientObject;
}

void wxEvtHandler::DoSetClientData( void *data )
{
    wxASSERT_MSG( m_clientDataType != wxClientData_Object,
                  wxT("can't have both object and void client data") );

    m_clientData = data;
    m_clientDataType = wxClientData_Void;
}

void *wxEvtHandler::DoGetClientData() const
{
    // it's not an error to call GetClientData() on a window which doesn't have
    // client data at all - NULL will be returned
    wxASSERT_MSG( m_clientDataType != wxClientData_Object,
                  wxT("this window doesn't have void client data") );

    return m_clientData;
}

// A helper to find an wxEventConnectionRef object
wxEventConnectionRef *
wxEvtHandler::FindRefInTrackerList(wxEvtHandler *eventSink)
{
    for ( wxTrackerNode *node = eventSink->GetFirst(); node; node = node->m_nxt )
    {
        // we only want wxEventConnectionRef nodes here
        wxEventConnectionRef *evtConnRef = node->ToEventConnection();
        if ( evtConnRef && evtConnRef->m_src == this )
        {
            wxASSERT( evtConnRef->m_sink==eventSink );
            return evtConnRef;
        }
    }

    return NULL;
}

void wxEvtHandler::OnSinkDestroyed( wxEvtHandler *sink )
{
    wxASSERT(m_dynamicEvents);

    // remove all connections with this sink
    wxList::compatibility_iterator node = m_dynamicEvents->GetFirst(), node_nxt;
    while (node)
    {
        wxDynamicEventTableEntry *entry = (wxDynamicEventTableEntry*)node->GetData();
        node_nxt = node->GetNext();

        if ( entry->m_fn->GetEvtHandler() == sink )
        {
            delete entry->m_callbackUserData;
            m_dynamicEvents->Erase( node );
            delete entry;
        }
        node = node_nxt;
    }
}

#endif // wxUSE_BASE

#if wxUSE_GUI

// Find a window with the focus, that is also a descendant of the given window.
// This is used to determine the window to initially send commands to.
wxWindow* wxFindFocusDescendant(wxWindow* ancestor)
{
    // Process events starting with the window with the focus, if any.
    wxWindow* focusWin = wxWindow::FindFocus();
    wxWindow* win = focusWin;

    // Check if this is a descendant of this frame.
    // If not, win will be set to NULL.
    while (win)
    {
        if (win == ancestor)
            break;
        else
            win = win->GetParent();
    }
    if (win == NULL)
        focusWin = NULL;

    return focusWin;
}

// ----------------------------------------------------------------------------
// wxEventBlocker
// ----------------------------------------------------------------------------

wxEventBlocker::wxEventBlocker(wxWindow *win, wxEventType type)
{
    wxCHECK_RET(win, wxT("Null window given to wxEventBlocker"));

    m_window = win;

    Block(type);
    m_window->PushEventHandler(this);
}

wxEventBlocker::~wxEventBlocker()
{
    wxEvtHandler *popped = m_window->PopEventHandler(false);
    wxCHECK_RET(popped == this,
        wxT("Don't push other event handlers into a window managed by wxEventBlocker!"));
}

bool wxEventBlocker::ProcessEvent(wxEvent& event)
{
    // should this event be blocked?
    for ( size_t i = 0; i < m_eventsToBlock.size(); i++ )
    {
        wxEventType t = (wxEventType)m_eventsToBlock[i];
        if ( t == wxEVT_ANY || t == event.GetEventType() )
            return true;   // yes, it should: mark this event as processed
    }

    return false;
}

#endif // wxUSE_GUI

