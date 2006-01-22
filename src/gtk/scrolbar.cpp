/////////////////////////////////////////////////////////////////////////////
// Name:        src/gtk/scrolbar.cpp
// Purpose:
// Author:      Robert Roebling
// Id:          $Id$
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#if wxUSE_SCROLLBAR

#include "wx/scrolbar.h"

#include "wx/utils.h"
#include "wx/math.h"
#include "wx/gtk/private.h"

//-----------------------------------------------------------------------------
// idle system
//-----------------------------------------------------------------------------

extern void wxapp_install_idle_handler();
extern bool g_isIdle;

//-----------------------------------------------------------------------------
// data
//-----------------------------------------------------------------------------

extern bool   g_blockEventsOnDrag;
static wxEventType g_currentUpDownEvent = wxEVT_NULL;

static const float sensitivity = 0.02;

//-----------------------------------------------------------------------------
// "value_changed"
//-----------------------------------------------------------------------------

// FIXME: is GtkScrollType really passed to us as 2nd argument?

extern "C" {
static void gtk_scrollbar_callback( GtkAdjustment *adjust,
                                    SCROLLBAR_CBACK_ARG
                                    wxScrollBar *win )
{
    if (g_isIdle) wxapp_install_idle_handler();

    if (!win->m_hasVMT) return;
    if (g_blockEventsOnDrag) return;

    float diff = adjust->value - win->m_oldPos;
    if (fabs(diff) < sensitivity) return;

    win->m_oldPos = adjust->value;

    wxEventType command = GtkScrollTypeToWx(GET_SCROLL_TYPE(win->m_widget));

    double dvalue = adjust->value;
    int value = (int)(dvalue < 0 ? dvalue - 0.5 : dvalue + 0.5);

    int orient = win->HasFlag(wxSB_VERTICAL) ? wxVERTICAL : wxHORIZONTAL;

    // throw a LINEUP / LINEDOWN event if necessary
    if (g_currentUpDownEvent != wxEVT_NULL)
    {
        wxScrollEvent event( g_currentUpDownEvent, win->GetId(), value, orient );
        event.SetEventObject( win );
        win->GetEventHandler()->ProcessEvent( event );
    }

    // throw other event (wxEVT_SCROLL_THUMBTRACK)
    wxScrollEvent event( command, win->GetId(), value, orient );
    event.SetEventObject( win );
    win->GetEventHandler()->ProcessEvent( event );

/*
    wxCommandEvent cevent( wxEVT_COMMAND_SCROLLBAR_UPDATED, win->GetId() );
    cevent.SetEventObject( win );
    win->ProcessEvent( cevent );
*/
}
}

//-----------------------------------------------------------------------------
// "button_press_event" from slider
//-----------------------------------------------------------------------------
extern "C" {
static gint gtk_scrollbar_button_press_callback( GtkRange *widget,
                                                 GdkEventButton *gdk_event,
                                                 wxScrollBar *win )
{
    if (g_isIdle) wxapp_install_idle_handler();

    // check if a LINEUP/LINEDOWN event must be thrown
    // I suppose here the size of scrollbar top/bottom buttons is 16px height
    if (gdk_event->type == GDK_BUTTON_PRESS && gdk_event->button == 1)
    {
        int scroll_height, mouse_pos;

        // get the mouse position when the click is done
        if (win->HasFlag(wxSB_VERTICAL))
        {
            scroll_height = GTK_WIDGET(widget)->allocation.height - 16;
            mouse_pos = (int)gdk_event->y;
        }
        else
        {
            scroll_height = GTK_WIDGET(widget)->allocation.width - 16;
            mouse_pos = (int)gdk_event->x;
        }

        // compare mouse position to scrollbar height
        if  (mouse_pos > scroll_height)
            g_currentUpDownEvent = wxEVT_SCROLL_LINEDOWN;
        else if (mouse_pos < 16)
            g_currentUpDownEvent = wxEVT_SCROLL_LINEUP;
      }

    return FALSE;
}
}

//-----------------------------------------------------------------------------
// "button_release_event" from slider
//-----------------------------------------------------------------------------

extern "C" {
static gint
gtk_scrollbar_button_release_callback( GtkRange *WXUNUSED(widget),
                                       GdkEventButton *WXUNUSED(gdk_event),
                                       wxScrollBar *win )
{
    if (g_isIdle)
        wxapp_install_idle_handler();

    if (win->m_isScrolling)
    {
        wxEventType command = wxEVT_SCROLL_THUMBRELEASE;
        int value = (int)ceil(win->m_adjust->value);
        int orient = win->HasFlag(wxSB_VERTICAL) ? wxVERTICAL : wxHORIZONTAL;

        wxScrollEvent event( command, win->GetId(), value, orient );
        event.SetEventObject( win );
        win->GetEventHandler()->ProcessEvent( event );
    }

    win->m_isScrolling = false;

    // reset the LINEUP/LINEDOWN flag when the mouse button is released
    g_currentUpDownEvent = wxEVT_NULL;

    return FALSE;
}
}

//-----------------------------------------------------------------------------
// wxScrollBar
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxScrollBar,wxControl)

wxScrollBar::~wxScrollBar()
{
}

bool wxScrollBar::Create(wxWindow *parent, wxWindowID id,
           const wxPoint& pos, const wxSize& size,
           long style, const wxValidator& validator, const wxString& name )
{
    m_needParent = true;
    m_acceptsFocus = true;

    if (!PreCreation( parent, pos, size ) ||
        !CreateBase( parent, id, pos, size, style, validator, name ))
    {
        wxFAIL_MSG( wxT("wxScrollBar creation failed") );
        return false;
    }

    m_oldPos = 0.0;

    if ((style & wxSB_VERTICAL) == wxSB_VERTICAL)
        m_widget = gtk_vscrollbar_new( (GtkAdjustment *) NULL );
    else
        m_widget = gtk_hscrollbar_new( (GtkAdjustment *) NULL );

    m_adjust = gtk_range_get_adjustment( GTK_RANGE(m_widget) );

    g_signal_connect (m_adjust, "value_changed",
                      G_CALLBACK (gtk_scrollbar_callback), this);
    g_signal_connect (m_widget, "button_press_event",
                      G_CALLBACK (gtk_scrollbar_button_press_callback),
                      this);
    g_signal_connect (m_widget, "button_release_event",
                      G_CALLBACK (gtk_scrollbar_button_release_callback),
                      this);

    m_parent->DoAddChild( this );

    PostCreation(size);

    return true;
}

int wxScrollBar::GetThumbPosition() const
{
    double val = m_adjust->value;
    return (int)(val < 0 ? val - 0.5 : val + 0.5);
}

int wxScrollBar::GetThumbSize() const
{
    return (int)(m_adjust->page_size+0.5);
}

int wxScrollBar::GetPageSize() const
{
    return (int)(m_adjust->page_increment+0.5);
}

int wxScrollBar::GetRange() const
{
    return (int)(m_adjust->upper+0.5);
}

void wxScrollBar::SetThumbPosition( int viewStart )
{
    if (m_isScrolling) return;

    float fpos = (float)viewStart;
    m_oldPos = fpos;
    if (fabs(fpos-m_adjust->value) < 0.2) return;
    m_adjust->value = fpos;

    g_signal_handlers_disconnect_by_func (m_adjust,
                                          (gpointer) gtk_scrollbar_callback,
                                          this);

    g_signal_emit_by_name (m_adjust, "value_changed");

    g_signal_connect (m_adjust, "value_changed",
                      G_CALLBACK (gtk_scrollbar_callback), this);
}

void wxScrollBar::SetScrollbar( int position, int thumbSize, int range, int pageSize,
      bool WXUNUSED(refresh) )
{
    float fpos = (float)position;
    float frange = (float)range;
    float fthumb = (float)thumbSize;
    float fpage = (float)pageSize;

    if ((fabs(frange-m_adjust->upper) < 0.2) &&
        (fabs(fthumb-m_adjust->page_size) < 0.2) &&
        (fabs(fpage-m_adjust->page_increment) < 0.2))
    {
        SetThumbPosition( position );
        return;
    }

    m_oldPos = fpos;

    m_adjust->lower = 0.0;
    m_adjust->upper = frange;
    m_adjust->value = fpos;
    m_adjust->step_increment = 1.0;
    m_adjust->page_increment = (float)(wxMax(fpage,0));
    m_adjust->page_size = fthumb;

    g_signal_emit_by_name (m_adjust, "changed");
}

/* Backward compatibility */
int wxScrollBar::GetValue() const
{
    return GetThumbPosition();
}

void wxScrollBar::SetValue( int viewStart )
{
    SetThumbPosition( viewStart );
}

void wxScrollBar::GetValues( int *viewStart, int *viewLength, int *objectLength, int *pageLength ) const
{
    int pos = (int)(m_adjust->value+0.5);
    int thumb = (int)(m_adjust->page_size+0.5);
    int page = (int)(m_adjust->page_increment+0.5);
    int range = (int)(m_adjust->upper+0.5);

    *viewStart = pos;
    *viewLength = range;
    *objectLength = thumb;
    *pageLength = page;
}

int wxScrollBar::GetViewLength() const
{
    return (int)(m_adjust->upper+0.5);
}

int wxScrollBar::GetObjectLength() const
{
    return (int)(m_adjust->page_size+0.5);
}

void wxScrollBar::SetPageSize( int pageLength )
{
    int pos = (int)(m_adjust->value+0.5);
    int thumb = (int)(m_adjust->page_size+0.5);
    int range = (int)(m_adjust->upper+0.5);
    SetScrollbar( pos, thumb, range, pageLength );
}

void wxScrollBar::SetObjectLength( int objectLength )
{
    int pos = (int)(m_adjust->value+0.5);
    int page = (int)(m_adjust->page_increment+0.5);
    int range = (int)(m_adjust->upper+0.5);
    SetScrollbar( pos, objectLength, range, page );
}

void wxScrollBar::SetViewLength( int viewLength )
{
    int pos = (int)(m_adjust->value+0.5);
    int thumb = (int)(m_adjust->page_size+0.5);
    int page = (int)(m_adjust->page_increment+0.5);
    SetScrollbar( pos, thumb, viewLength, page );
}

bool wxScrollBar::IsOwnGtkWindow( GdkWindow *window )
{
    GtkRange *range = GTK_RANGE(m_widget);
    return ( (window == GTK_WIDGET(range)->window) );
}

wxSize wxScrollBar::DoGetBestSize() const
{
    return wxControl::DoGetBestSize();
}

// static
wxVisualAttributes
wxScrollBar::GetClassDefaultAttributes(wxWindowVariant WXUNUSED(variant))
{
    return GetDefaultAttributesFromGTKWidget(gtk_vscrollbar_new);
}

#endif
