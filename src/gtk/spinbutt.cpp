/////////////////////////////////////////////////////////////////////////////
// Name:        spinbutt.cpp
// Purpose:     wxSpinButton
// Author:      Robert
// Modified by:
// RCS-ID:      $Id$
// Copyright:   (c) Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/spinbutt.h"

#if wxUSE_SPINBTN

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

static const float sensitivity = 0.02;

//-----------------------------------------------------------------------------
// "value_changed"
//-----------------------------------------------------------------------------

extern "C" {
static void gtk_spinbutt_callback( GtkWidget *WXUNUSED(widget), wxSpinButton *win )
{
    if (g_isIdle) wxapp_install_idle_handler();

    if (!win->m_hasVMT) return;
    if (g_blockEventsOnDrag) return;

    float diff = win->m_adjust->value - win->m_oldPos;
    if (fabs(diff) < sensitivity) return;

    wxEventType command = wxEVT_NULL;

    float line_step = win->m_adjust->step_increment;

    if (fabs(diff-line_step) < sensitivity) command = wxEVT_SCROLL_LINEUP;
    else if (fabs(diff+line_step) < sensitivity) command = wxEVT_SCROLL_LINEDOWN;
    else command = wxEVT_SCROLL_THUMBTRACK;

    int value = (int)ceil(win->m_adjust->value);

    wxSpinEvent event( command, win->GetId());
    event.SetPosition( value );
    event.SetEventObject( win );

    if ((win->GetEventHandler()->ProcessEvent( event )) &&
        !event.IsAllowed() )
    {
        /* program has vetoed */
        win->m_adjust->value = win->m_oldPos;

        g_signal_handlers_disconnect_by_func (win->m_adjust,
                                              (gpointer) gtk_spinbutt_callback,
                                              win);

        g_signal_emit_by_name (win->m_adjust, "value_changed");

        g_signal_connect (win->m_adjust, "value_changed",
                          G_CALLBACK (gtk_spinbutt_callback), win);
        return;
    }

    win->m_oldPos = win->m_adjust->value;

    /* always send a thumbtrack event */
    if (command != wxEVT_SCROLL_THUMBTRACK)
    {
        command = wxEVT_SCROLL_THUMBTRACK;
        wxSpinEvent event2( command, win->GetId());
        event2.SetPosition( value );
        event2.SetEventObject( win );
        win->GetEventHandler()->ProcessEvent( event2 );
    }
}
}

//-----------------------------------------------------------------------------
// wxSpinButton
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxSpinButton,wxControl)
IMPLEMENT_DYNAMIC_CLASS(wxSpinEvent, wxNotifyEvent)

BEGIN_EVENT_TABLE(wxSpinButton, wxControl)
    EVT_SIZE(wxSpinButton::OnSize)
END_EVENT_TABLE()

bool wxSpinButton::Create(wxWindow *parent,
                          wxWindowID id,
                          const wxPoint& pos,
                          const wxSize& size,
                          long style,
                          const wxString& name)
{
    m_needParent = TRUE;

    wxSize new_size = size,
           sizeBest = DoGetBestSize();
    new_size.x = sizeBest.x;            // override width always
    if (new_size.y == -1)
        new_size.y = sizeBest.y;

    if (!PreCreation( parent, pos, new_size ) ||
        !CreateBase( parent, id, pos, new_size, style, wxDefaultValidator, name ))
    {
        wxFAIL_MSG( wxT("wxXX creation failed") );
        return FALSE;
    }

    m_oldPos = 0.0;

    m_adjust = (GtkAdjustment*) gtk_adjustment_new( 0.0, 0.0, 100.0, 1.0, 5.0, 0.0);

    m_widget = gtk_spin_button_new( m_adjust, 0, 0 );

    gtk_spin_button_set_wrap( GTK_SPIN_BUTTON(m_widget),
                              (int)(m_windowStyle & wxSP_WRAP) );

    g_signal_connect (m_adjust, "value_changed",
                      G_CALLBACK (gtk_spinbutt_callback), this);

    m_parent->DoAddChild( this );

    PostCreation(new_size);

    return TRUE;
}

int wxSpinButton::GetMin() const
{
    wxCHECK_MSG( (m_widget != NULL), 0, wxT("invalid spin button") );

    return (int)ceil(m_adjust->lower);
}

int wxSpinButton::GetMax() const
{
    wxCHECK_MSG( (m_widget != NULL), 0, wxT("invalid spin button") );

    return (int)ceil(m_adjust->upper);
}

int wxSpinButton::GetValue() const
{
    wxCHECK_MSG( (m_widget != NULL), 0, wxT("invalid spin button") );

    return (int)ceil(m_adjust->value);
}

void wxSpinButton::SetValue( int value )
{
    wxCHECK_RET( (m_widget != NULL), wxT("invalid spin button") );

    float fpos = (float)value;
    m_oldPos = fpos;
    if (fabs(fpos-m_adjust->value) < sensitivity) return;

    m_adjust->value = fpos;

    g_signal_emit_by_name (m_adjust, "value_changed");
}

void wxSpinButton::SetRange(int minVal, int maxVal)
{
    wxCHECK_RET( (m_widget != NULL), wxT("invalid spin button") );

    float fmin = (float)minVal;
    float fmax = (float)maxVal;

    if ((fabs(fmin-m_adjust->lower) < sensitivity) &&
        (fabs(fmax-m_adjust->upper) < sensitivity))
    {
        return;
    }

    m_adjust->lower = fmin;
    m_adjust->upper = fmax;

    g_signal_emit_by_name (m_adjust, "changed");

    // these two calls are required due to some bug in GTK
    Refresh();
    SetFocus();
}

void wxSpinButton::OnSize( wxSizeEvent &WXUNUSED(event) )
{
    wxCHECK_RET( (m_widget != NULL), wxT("invalid spin button") );

    m_width = DoGetBestSize().x;
    gtk_widget_set_size_request( m_widget, m_width, m_height );
}

bool wxSpinButton::IsOwnGtkWindow( GdkWindow *window )
{
    return GTK_SPIN_BUTTON(m_widget)->panel == window;
}

wxSize wxSpinButton::DoGetBestSize() const
{
    wxSize best(15, 26); // FIXME
    CacheBestSize(best);
    return best;
}

// static
wxVisualAttributes
wxSpinButton::GetClassDefaultAttributes(wxWindowVariant WXUNUSED(variant))
{
    // TODO: overload to accept functions like gtk_spin_button_new?
    // Until then use a similar type
    return GetDefaultAttributesFromGTKWidget(gtk_button_new);
}

#endif
