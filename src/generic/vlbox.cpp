///////////////////////////////////////////////////////////////////////////////
// Name:        generic/vlbox.cpp
// Purpose:     implementation of wxVListBox
// Author:      Vadim Zeitlin
// Modified by:
// Created:     31.05.03
// RCS-ID:      $Id$
// Copyright:   (c) 2003 Vadim Zeitlin <vadim@wxwindows.org>
// License:     wxWindows license
///////////////////////////////////////////////////////////////////////////////

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

#ifndef WX_PRECOMP
    #include "wx/settings.h"
    #include "wx/dcclient.h"
#endif //WX_PRECOMP

#include "wx/vlbox.h"
#include "wx/selstore.h"

// ----------------------------------------------------------------------------
// event tables
// ----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(wxVListBox, wxVScrolledWindow)
    EVT_PAINT(wxVListBox::OnPaint)

    EVT_KEY_DOWN(wxVListBox::OnKeyDown)
    EVT_LEFT_DOWN(wxVListBox::OnLeftDown)
    EVT_LEFT_DCLICK(wxVListBox::OnLeftDClick)
END_EVENT_TABLE()

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxVListBox creation
// ----------------------------------------------------------------------------

void wxVListBox::Init()
{
    m_current = wxNOT_FOUND;
    m_selStore = NULL;
}

bool wxVListBox::Create(wxWindow *parent,
                        wxWindowID id,
                        const wxPoint& pos,
                        const wxSize& size,
                        long style,
                        const wxString& name)
{
    if ( !wxVScrolledWindow::Create(parent, id, pos, size, style, name) )
        return false;

    SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_LISTBOX));

    if ( style & wxLB_MULTIPLE )
        m_selStore = new wxSelectionStore;

    return true;
}

wxVListBox::~wxVListBox()
{
    delete m_selStore;
}

void wxVListBox::SetItemCount(size_t count)
{
    if ( m_selStore )
    {
        // tell the selection store that our number of items has changed
        m_selStore->SetItemCount(count);
    }

    SetLineCount(count);
}

// ----------------------------------------------------------------------------
// selection handling
// ----------------------------------------------------------------------------

bool wxVListBox::IsSelected(size_t line) const
{
    return m_selStore ? m_selStore->IsSelected(line) : (int)line == m_current;
}

bool wxVListBox::Select(size_t item, bool select)
{
    wxCHECK_MSG( m_selStore, false,
                 _T("Select() may only be used with multiselection listbox") );

    wxCHECK_MSG( item < GetItemCount(), false,
                 _T("Select(): invalid item index") );

    bool changed = m_selStore->SelectItem(item, select);
    if ( changed )
    {
        // selection really changed
        RefreshLine(item);
    }

    DoSetCurrent(item);

    return changed;
}

bool wxVListBox::SelectRange(size_t from, size_t to)
{
    wxCHECK_MSG( m_selStore, false,
                 _T("SelectRange() may only be used with multiselection listbox") );

    // make sure items are in correct order
    if ( from > to )
    {
        size_t tmp = from;
        from = to;
        to = tmp;
    }

    wxCHECK_MSG( to < GetItemCount(), false,
                    _T("SelectRange(): invalid item index") );

    wxArrayInt changed;
    if ( !m_selStore->SelectRange(from, to, true, &changed) )
    {
        // too many items have changed, we didn't record them in changed array
        // so we have no choice but to refresh everything between from and to
        RefreshLines(from, to);
    }
    else // we've got the indices of the changed items
    {
        const size_t count = changed.GetCount();
        if ( !count )
        {
            // nothing changed
            return false;
        }

        // refresh just the lines which have really changed
        for ( size_t n = 0; n < count; n++ )
        {
            RefreshLine(changed[n]);
        }
    }

    // something changed
    return true;
}

bool wxVListBox::DoSelectAll(bool select)
{
    wxCHECK_MSG( m_selStore, false,
                 _T("SelectAll may only be used with multiselection listbox") );

    size_t count = GetItemCount();
    if ( count )
    {
        wxArrayInt changed;
        if ( !m_selStore->SelectRange(0, count - 1, select) ||
                !changed.IsEmpty() )
        {
            Refresh();

            // something changed
            return true;
        }
    }

    return false;
}

bool wxVListBox::DoSetCurrent(int current)
{
    if ( current == m_current )
    {
        // nothing to do
        return false;
    }

    if ( m_current != wxNOT_FOUND )
        RefreshLine(m_current);

    m_current = current;

    if ( m_current != wxNOT_FOUND )
    {
        // if the line is not visible at all, we scroll it into view but we
        // don't need to refresh it -- it will be redrawn anyhow
        if ( !IsVisible(m_current) )
        {
            ScrollToLine(m_current);
        }
        else // line is at least partly visible
        {
            // it is, indeed, only partly visible, so scroll it into view to
            // make it entirely visible
            if ( (size_t)m_current == GetLastVisibleLine() )
            {
                ScrollToLine(m_current);
            }

            // but in any case refresh it as even if it was only partly visible
            // before we need to redraw it entirely as its background changed
            RefreshLine(m_current);
        }
    }

    return true;
}

void wxVListBox::SendSelectedEvent()
{
    wxASSERT_MSG( m_current != wxNOT_FOUND,
                    _T("SendSelectedEvent() shouldn't be called") );

    wxCommandEvent event(wxEVT_COMMAND_LISTBOX_SELECTED, GetId());
    event.SetEventObject(this);
    event.m_commandInt = m_current;

    (void)GetEventHandler()->ProcessEvent(event);
}

void wxVListBox::SetSelection(int selection)
{
    wxASSERT_MSG( !HasMultipleSelection(),
                  _T("SetSelection() is invalid with multiselection listbox") );

    DoSetCurrent(selection);
}

size_t wxVListBox::GetSelectedCount() const
{
    return m_selStore ? m_selStore->GetSelectedCount()
                      : m_current == wxNOT_FOUND ? 0 : 1;
}

int wxVListBox::GetFirstSelected(unsigned long& cookie) const
{
    cookie = 0;

    return GetNextSelected(cookie);
}

int wxVListBox::GetNextSelected(unsigned long& cookie) const
{
    wxCHECK_MSG( m_selStore, wxNOT_FOUND,
                  _T("GetFirst/NextSelected() may only be used with multiselection listboxes") );

    while ( cookie < GetItemCount() )
    {
        if ( IsSelected(cookie++) )
            return cookie - 1;
    }

    return wxNOT_FOUND;
}

// ----------------------------------------------------------------------------
// wxVListBox painting
// ----------------------------------------------------------------------------

void wxVListBox::SetMargins(const wxPoint& pt)
{
    if ( pt != m_ptMargins )
    {
        m_ptMargins = pt;

        Refresh();
    }
}

wxCoord wxVListBox::OnGetLineHeight(size_t line) const
{
    return OnMeasureItem(line) + 2*m_ptMargins.y;
}

void wxVListBox::OnDrawSeparator(wxDC& WXUNUSED(dc),
                                 wxRect& WXUNUSED(rect),
                                 size_t WXUNUSED(n)) const
{
}

void wxVListBox::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);

    // the update rectangle
    wxRect rectUpdate = GetUpdateClientRect();

    // the bounding rectangle of the current line
    wxRect rectLine;
    rectLine.width = GetClientSize().x;

    // iterate over all visible lines
    const size_t lineMax = GetLastVisibleLine();
    for ( size_t line = GetFirstVisibleLine(); line <= lineMax; line++ )
    {
        const wxCoord hLine = OnGetLineHeight(line);

        rectLine.height = hLine;

        // and draw the ones which intersect the update rect
        if ( rectLine.Intersects(rectUpdate) )
        {
            // don't allow drawing outside of the lines rectangle
            wxDCClipper clip(dc, rectLine);

            // we need to render selected and current items differently
            const bool isSelected = IsSelected(line);
            if ( isSelected || IsCurrent(line) )
            {
                if ( isSelected )
                {
                    wxBrush brush(wxSystemSettings::
                                    GetColour(wxSYS_COLOUR_HIGHLIGHT),
                                    wxSOLID);
                    dc.SetBrush(brush);
                }
                else // !selected
                {
                    dc.SetBrush(*wxTRANSPARENT_BRUSH);
                }

                dc.SetPen(*(IsCurrent(line) ? wxBLACK_PEN : wxTRANSPARENT_PEN));

                dc.DrawRectangle(rectLine);
            }

            wxRect rect = rectLine;
            OnDrawSeparator(dc, rect, line);

            rect.Deflate(m_ptMargins.x, m_ptMargins.y);
            OnDrawItem(dc, rect, line);
        }
        else // no intersection
        {
            if ( rectLine.GetTop() > rectUpdate.GetBottom() )
            {
                // we are already below the update rect, no need to continue
                // further
                break;
            }
            //else: the next line may intersect the update rect
        }

        rectLine.y += hLine;
    }
}

// ============================================================================
// wxVListBox keyboard/mouse handling
// ============================================================================

void wxVListBox::DoHandleItemClick(int item, bool shiftDown, bool ctrlDown)
{
    // has anything worth telling the client code about happened?
    bool notify = false;

    if ( HasMultipleSelection() )
    {
        // select the iteem clicked?
        bool select = true;

        // NB: the keyboard interface we implement here corresponds to
        //     wxLB_EXTENDED rather than wxLB_MULTIPLE but this one makes more
        //     sense IMHO
        if ( shiftDown )
        {
            if ( m_current != wxNOT_FOUND )
            {
                select = false;

                // only the range from old m_current to new m_current must be
                // selected
                if ( DeselectAll() )
                    notify = true;

                if ( SelectRange(m_current, item) )
                    notify = true;
            }
            //else: treat it as ordinary click/keypress
        }
        else if ( ctrlDown )
        {
            select = false;

            Toggle(item);

            // the status of the item has definitely changed
            notify = true;
        }
        //else: behave as in single selection case

        if ( select )
        {
            // make the clicked item the only selection
            if ( DeselectAll() )
                notify = true;

            if ( Select(item) )
                notify = true;
        }
    }

    // in any case the item should become the current one
    if ( DoSetCurrent(item) )
    {
        if ( !HasMultipleSelection() )
        {
            // this has also changed the selection for single selection case
            notify = true;
        }
    }

    if ( notify )
    {
        // notify the user about the selection change
        SendSelectedEvent();
    }
    //else: nothing changed at all
}

// ----------------------------------------------------------------------------
// keyboard handling
// ----------------------------------------------------------------------------

void wxVListBox::OnKeyDown(wxKeyEvent& event)
{
    int current = 0; // just to silent the stupid compiler warnings
    switch ( event.GetKeyCode() )
    {
        case WXK_HOME:
            current = 0;
            break;

        case WXK_END:
            current = GetLineCount() - 1;
            break;

        case WXK_DOWN:
            if ( m_current == (int)GetLineCount() - 1 )
                return;

            current = m_current + 1;
            break;

        case WXK_UP:
            if ( m_current == wxNOT_FOUND )
                current = GetLineCount() - 1;
            else if ( m_current != 0 )
                current = m_current - 1;
            else // m_current == 0
                return;
            break;

        case WXK_PAGEDOWN:
        case WXK_NEXT:
            PageDown();
            current = GetFirstVisibleLine();
            break;

        case WXK_PAGEUP:
        case WXK_PRIOR:
            if ( m_current == (int)GetFirstVisibleLine() )
            {
                PageUp();
            }

            current = GetFirstVisibleLine();
            break;

        default:
            event.Skip();
            return;
    }

    DoHandleItemClick(current, event.ShiftDown(), event.ControlDown());
}

// ----------------------------------------------------------------------------
// wxVListBox mouse handling
// ----------------------------------------------------------------------------

void wxVListBox::OnLeftDown(wxMouseEvent& event)
{
    int item = HitTest(event.GetPosition());

    DoHandleItemClick(item, event.ShiftDown(),
#ifdef __WXMAC__
                            event.MetaDown()
#else
                            event.ControlDown()
#endif
                     );
}

void wxVListBox::OnLeftDClick(wxMouseEvent& event)
{
    int item = HitTest(event.GetPosition());
    if ( item != wxNOT_FOUND )
    {
        wxCommandEvent event(wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, GetId());
        event.SetEventObject(this);
        event.m_commandInt = item;

        (void)GetEventHandler()->ProcessEvent(event);
    }
}

