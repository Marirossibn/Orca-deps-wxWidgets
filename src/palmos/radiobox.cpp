/////////////////////////////////////////////////////////////////////////////
// Name:        src/palmos/radiobox.cpp
// Purpose:     wxRadioBox implementation
// Author:      William Osborne - minimal working wxPalmOS port
// Modified by: Wlodzimierz ABX Skiba - native wxRadioBox implementation
// Created:     10/13/04
// RCS-ID:      $Id$
// Copyright:   (c) William Osborne, Wlodzimierz Skiba
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ===========================================================================
// declarations
// ===========================================================================

// ---------------------------------------------------------------------------
// headers
// ---------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "radiobox.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_RADIOBOX

#ifndef WX_PRECOMP
    #include "wx/bitmap.h"
    #include "wx/brush.h"
    #include "wx/radiobox.h"
    #include "wx/settings.h"
    #include "wx/log.h"
#endif

#if wxUSE_TOOLTIPS
    #include "wx/tooltip.h"
#endif // wxUSE_TOOLTIPS

#include "wx/radiobut.h"

// TODO: wxCONSTRUCTOR
#if 0 // wxUSE_EXTENDED_RTTI
WX_DEFINE_FLAGS( wxRadioBoxStyle )

wxBEGIN_FLAGS( wxRadioBoxStyle )
    // new style border flags, we put them first to
    // use them for streaming out
    wxFLAGS_MEMBER(wxBORDER_SIMPLE)
    wxFLAGS_MEMBER(wxBORDER_SUNKEN)
    wxFLAGS_MEMBER(wxBORDER_DOUBLE)
    wxFLAGS_MEMBER(wxBORDER_RAISED)
    wxFLAGS_MEMBER(wxBORDER_STATIC)
    wxFLAGS_MEMBER(wxBORDER_NONE)

    // old style border flags
    wxFLAGS_MEMBER(wxSIMPLE_BORDER)
    wxFLAGS_MEMBER(wxSUNKEN_BORDER)
    wxFLAGS_MEMBER(wxDOUBLE_BORDER)
    wxFLAGS_MEMBER(wxRAISED_BORDER)
    wxFLAGS_MEMBER(wxSTATIC_BORDER)
    wxFLAGS_MEMBER(wxBORDER)

    // standard window styles
    wxFLAGS_MEMBER(wxTAB_TRAVERSAL)
    wxFLAGS_MEMBER(wxCLIP_CHILDREN)
    wxFLAGS_MEMBER(wxTRANSPARENT_WINDOW)
    wxFLAGS_MEMBER(wxWANTS_CHARS)
    wxFLAGS_MEMBER(wxFULL_REPAINT_ON_RESIZE)
    wxFLAGS_MEMBER(wxALWAYS_SHOW_SB )
    wxFLAGS_MEMBER(wxVSCROLL)
    wxFLAGS_MEMBER(wxHSCROLL)

    wxFLAGS_MEMBER(wxRA_SPECIFY_COLS)
    wxFLAGS_MEMBER(wxRA_HORIZONTAL)
    wxFLAGS_MEMBER(wxRA_SPECIFY_ROWS)
    wxFLAGS_MEMBER(wxRA_VERTICAL)

wxEND_FLAGS( wxRadioBoxStyle )

IMPLEMENT_DYNAMIC_CLASS_XTI(wxRadioBox, wxControl,"wx/radiobox.h")

wxBEGIN_PROPERTIES_TABLE(wxRadioBox)
    wxEVENT_PROPERTY( Select , wxEVT_COMMAND_RADIOBOX_SELECTED , wxCommandEvent )
    wxPROPERTY_FLAGS( WindowStyle , wxRadioBoxStyle , long , SetWindowStyleFlag , GetWindowStyleFlag , , 0 /*flags*/ , wxT("Helpstring") , wxT("group")) // style
wxEND_PROPERTIES_TABLE()

#else
IMPLEMENT_DYNAMIC_CLASS(wxRadioBox, wxControl)
#endif

/*
    selection
    content
        label
        dimension
        item
*/

// ===========================================================================
// implementation
// ===========================================================================

// ---------------------------------------------------------------------------
// wxRadioBox
// ---------------------------------------------------------------------------

void wxRadioBox::Init()
{
    m_pos = wxPoint(0,0);
    m_size = wxSize(0,0);
}

int wxRadioBox::GetCount() const
{
    return m_radios.GetCount();
}

int wxRadioBox::GetColumnCount() const
{
    return 0;
}

int wxRadioBox::GetRowCount() const
{
    return 0;
}

// returns the number of rows
int wxRadioBox::GetNumVer() const
{
    return 0;
}

// returns the number of columns
int wxRadioBox::GetNumHor() const
{
    return 0;
}

bool wxRadioBox::Create(wxWindow *parent,
                        wxWindowID id,
                        const wxString& title,
                        const wxPoint& pos,
                        const wxSize& size,
                        int n,
                        const wxString choices[],
                        int majorDim,
                        long style,
                        const wxValidator& val,
                        const wxString& name)
{
    // initialize members
    m_majorDim = majorDim == 0 ? n : wxMin(majorDim, n);
    if(m_majorDim==0 || n==0) return false;

    // subtype of the native palmOS radio: checkbox or push button?
    const bool use_checkbox = style & wxRA_USE_CHECKBOX;
    const bool use_cols = style & wxRA_SPECIFY_COLS;

    // get default size and position for the initial placement
    m_size = size;
    m_pos = pos;
    int minor = n / m_majorDim;
    if(n % m_majorDim > 0) minor++;
    if(m_size.x==wxDefaultCoord)
        m_size.x=36*(use_cols?m_majorDim:minor);
    if(m_size.y==wxDefaultCoord)
        m_size.y=12*(use_cols?minor:m_majorDim);
    if(m_pos.x==wxDefaultCoord)
        m_pos.x=0;
    if(m_pos.y==wxDefaultCoord)
        m_pos.y=0;

    if(!wxControl::Create(parent, id, m_pos, m_size, style, val, name))
        return false;

    int i = 0;
    for ( int j = 0; j < minor; j++ )
    {
        for ( int k = 0; k < m_majorDim; k++ )
        {
            if(i<n)
            {
                wxPoint start, end;
                start.x = (use_cols ? (k*m_size.x)/m_majorDim : (j*m_size.x)/minor);
                start.y = (use_cols ? (j*m_size.y)/minor : (k*m_size.y)/m_majorDim);
                end.x = (use_cols ? ((k+1)*m_size.x)/m_majorDim : ((j+1)*m_size.x)/minor);
                end.y = (use_cols ? ((j+1)*m_size.y)/minor : ((k+1)*m_size.y)/m_majorDim);
                wxRadioButton* rb = new wxRadioButton();
                rb->SetGroup( id );
                rb->Create(
                    this,
                    wxID_ANY,
                    choices[i],
                    start,
                    wxSize(end.x-start.x-1,end.y-start.y-1),
                    ( n == 0 ? wxRB_GROUP : 0 ) |
                    use_checkbox ? wxRB_USE_CHECKBOX : 0
                );
                m_radios.Put(i,rb);
                i++;
            }
        }
    }
}

bool wxRadioBox::Create(wxWindow *parent,
                        wxWindowID id,
                        const wxString& title,
                        const wxPoint& pos,
                        const wxSize& size,
                        const wxArrayString& choices,
                        int majorDim,
                        long style,
                        const wxValidator& val,
                        const wxString& name)
{
    wxCArrayString chs(choices);

    return Create( parent, id, title, pos, size, chs.GetCount(),
                   chs.GetStrings(), majorDim, style, val, name );
}

wxRadioBox::~wxRadioBox()
{
}

wxRadioButton *wxRadioBox::GetRadioButton(int i)
{
    return (wxRadioButton *)m_radios.Get(i);
}

void wxRadioBox::DoGetPosition( int *x, int *y ) const
{
    *x = m_pos.x;
    *y = m_pos.y;
}

void wxRadioBox::DoGetSize( int *width, int *height ) const
{
    *width = m_size.x;
    *height = m_size.y;
}

void wxRadioBox::DoMoveWindow(int x, int y, int width, int height)
{
    m_size.x = width;
    m_size.y = height;

    const bool use_cols = HasFlag(wxRA_SPECIFY_COLS);

    const int n = GetCount();
    int minor = n / m_majorDim;
    if(n % m_majorDim > 0) minor++;

    int i = 0;
    for ( int j = 0; j < minor; j++ )
    {
        for ( int k = 0; k < m_majorDim; k++ )
        {
            if(i<n)
            {
                wxPoint start, end;
                start.x = (use_cols ? (k*m_size.x)/m_majorDim : (j*m_size.x)/minor);
                start.y = (use_cols ? (j*m_size.y)/minor : (k*m_size.y)/m_majorDim);
                end.x = (use_cols ? ((k+1)*m_size.x)/m_majorDim : ((j+1)*m_size.x)/minor);
                end.y = (use_cols ? ((j+1)*m_size.y)/minor : ((k+1)*m_size.y)/m_majorDim);
                wxRadioButton* rb = GetRadioButton(i);
                if(rb)
                    rb->SetSize(end.x-start.x-1,end.y-start.y-1);
                i++;
            }
        }
    }
}

// get the origin of the client area in the client coordinates
wxPoint wxRadioBox::GetClientAreaOrigin() const
{
    return GetParent()->GetClientAreaOrigin() + GetPosition();
}

void wxRadioBox::SetString(int item, const wxString& label)
{
}

void wxRadioBox::SetSelection(int N)
{
}

// Get single selection, for single choice list items
int wxRadioBox::GetSelection() const
{
    return 0;
}

// Find string for position
wxString wxRadioBox::GetString(int item) const
{
    wxString ret;

    return ret;
}

// ----------------------------------------------------------------------------
// size calculations
// ----------------------------------------------------------------------------

wxSize wxRadioBox::GetMaxButtonSize() const
{
    return wxSize(0,0);
}

wxSize wxRadioBox::GetTotalButtonSize(const wxSize& sizeBtn) const
{
    return wxSize(0,0);
}

wxSize wxRadioBox::DoGetBestSize() const
{
    return wxSize(0,0);
}

void wxRadioBox::SetFocus()
{
}

bool wxRadioBox::Show(bool show)
{
    return false;
}

// Enable a specific button
bool wxRadioBox::Enable(int item, bool enable)
{
    wxRadioButton *btn = GetRadioButton(item);
    if(btn)
        return btn->Enable(enable);
    return false;
}

// Enable all subcontrols
bool wxRadioBox::Enable(bool enable)
{
    for(int i=0; i<GetCount(); i++)
        Enable(i, enable);
    return true;
}

// Show a specific button
void wxRadioBox::Show(int item, bool show)
{
}

void wxRadioBox::Command(wxCommandEvent & event)
{
}

void wxRadioBox::SendNotificationEvent()
{
}

bool wxRadioBox::SetFont(const wxFont& font)
{
    return false;
}

#endif // wxUSE_RADIOBOX
