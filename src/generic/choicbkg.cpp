///////////////////////////////////////////////////////////////////////////////
// Name:        generic/choicbkg.cpp
// Purpose:     generic implementation of wxChoicebook
// Author:      Vadim Zeitlin
// Modified by: Wlodzimierz ABX Skiba from generic/listbkg.cpp
// Created:     15.09.04
// RCS-ID:      $Id$
// Copyright:   (c) Vadim Zeitlin, Wlodzimierz Skiba
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "choicebook.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_CHOICEBOOK

#include "wx/choice.h"
#include "wx/choicebk.h"
#include "wx/imaglist.h"
#include "wx/settings.h"

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// margin between the choice and the page
#if defined(__WXWINCE__)
const wxCoord MARGIN = 1;
#else
const wxCoord MARGIN = 5;
#endif

// ----------------------------------------------------------------------------
// various wxWidgets macros
// ----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxChoicebook, wxControl)
IMPLEMENT_DYNAMIC_CLASS(wxChoicebookEvent, wxNotifyEvent)

const wxEventType wxEVT_COMMAND_CHOICEBOOK_PAGE_CHANGING = wxNewEventType();
const wxEventType wxEVT_COMMAND_CHOICEBOOK_PAGE_CHANGED = wxNewEventType();
const int wxID_CHOICEBOOKCHOICE = wxNewId();

BEGIN_EVENT_TABLE(wxChoicebook, wxBookCtrl)
    EVT_SIZE(wxChoicebook::OnSize)
    EVT_CHOICE(wxID_CHOICEBOOKCHOICE, wxChoicebook::OnChoiceSelected)
END_EVENT_TABLE()

// ============================================================================
// wxChoicebook implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxChoicebook creation
// ----------------------------------------------------------------------------

void wxChoicebook::Init()
{
    m_choice = NULL;
    m_selection = wxNOT_FOUND;
}

bool
wxChoicebook::Create(wxWindow *parent,
                     wxWindowID id,
                     const wxPoint& pos,
                     const wxSize& size,
                     long style,
                     const wxString& name)
{
    if ( (style & wxCHB_ALIGN_MASK) == wxCHB_DEFAULT )
    {
        style |= wxCHB_TOP;
    }

    // no border for this control, it doesn't look nice together with
    // wxChoice border
    style &= ~wxBORDER_MASK;
    style |= wxBORDER_NONE;

    if ( !wxControl::Create(parent, id, pos, size, style,
                            wxDefaultValidator, name) )
        return false;

    m_choice = new wxChoice
                 (
                    this,
                    wxID_CHOICEBOOKCHOICE,
                    wxDefaultPosition,
                    wxDefaultSize
                 );

    return true;
}

// ----------------------------------------------------------------------------
// wxChoicebook geometry management
// ----------------------------------------------------------------------------

wxSize wxChoicebook::GetChoiceSize() const
{
    const wxSize sizeClient = GetClientSize(),
                 sizeChoice = m_choice->GetBestSize();

    wxSize size;
    if ( IsVertical() )
    {
        size.x = sizeClient.x;
        size.y = sizeChoice.y;
    }
    else // left/right aligned
    {
        size.x = sizeChoice.x;
        size.y = sizeClient.y;
    }

    return size;
}

wxRect wxChoicebook::GetPageRect() const
{
    const wxSize sizeChoice = m_choice->GetSize();

    wxRect rectPage(wxPoint(0, 0), GetClientSize());
    switch ( GetWindowStyle() & wxCHB_ALIGN_MASK )
    {
        default:
            wxFAIL_MSG( _T("unexpected wxChoicebook alignment") );
            // fall through

        case wxCHB_TOP:
            rectPage.y = sizeChoice.y + MARGIN;
            // fall through

        case wxCHB_BOTTOM:
            rectPage.height -= sizeChoice.y + MARGIN;
            break;

        case wxCHB_LEFT:
            rectPage.x = sizeChoice.x + MARGIN;
            // fall through

        case wxCHB_RIGHT:
            rectPage.width -= sizeChoice.x + MARGIN;
            break;
    }

    return rectPage;
}

void wxChoicebook::OnSize(wxSizeEvent& event)
{
    event.Skip();

    if ( !m_choice )
    {
        // we're not fully created yet
        return;
    }

    // resize the choice control and the page area to fit inside our new size
    const wxSize sizeClient = GetClientSize(),
                 sizeChoice = GetChoiceSize();

    wxPoint posChoice;
    switch ( GetWindowStyle() & wxCHB_ALIGN_MASK )
    {
        default:
            wxFAIL_MSG( _T("unexpected wxChoicebook alignment") );
            // fall through

        case wxCHB_TOP:
        case wxCHB_LEFT:
            // posChoice is already ok
            break;

        case wxCHB_BOTTOM:
            posChoice.y = sizeClient.y - sizeChoice.y;
            break;

        case wxCHB_RIGHT:
            posChoice.x = sizeClient.x - sizeChoice.x;
            break;
    }

    m_choice->Move(posChoice.x, posChoice.y);
    m_choice->SetSize(sizeChoice.x, sizeChoice.y);

    // we should always have some selection if possible
    if ( m_selection == wxNOT_FOUND && GetPageCount() )
    {
        SetSelection(0);
    }

    if ( m_selection != wxNOT_FOUND )
    {
        wxWindow *page = m_pages[m_selection];
        wxCHECK_RET( page, _T("NULL page in wxChoicebook?") );

        page->SetSize(GetPageRect());
        if ( !page->IsShown() )
        {
            page->Show();
        }
    }
}

wxSize wxChoicebook::CalcSizeFromPage(const wxSize& sizePage) const
{
    // we need to add the size of the choice control and the margin
    const wxSize sizeChoice = GetChoiceSize();

    wxSize size = sizePage;
    if ( IsVertical() )
    {
        size.y += sizeChoice.y + MARGIN;
    }
    else // left/right aligned
    {
        size.x += sizeChoice.x + MARGIN;
    }

    return size;
}


// ----------------------------------------------------------------------------
// accessing the pages
// ----------------------------------------------------------------------------

bool wxChoicebook::SetPageText(size_t n, const wxString& strText)
{
    m_choice->SetString(n, strText);

    return true;
}

wxString wxChoicebook::GetPageText(size_t n) const
{
    return m_choice->GetString(n);
}

int wxChoicebook::GetPageImage(size_t WXUNUSED(n)) const
{
    wxFAIL_MSG( _T("wxChoicebook::GetPageImage() not implemented") );

    return -1;
}

bool wxChoicebook::SetPageImage(size_t WXUNUSED(n), int WXUNUSED(imageId))
{
    wxFAIL_MSG( _T("wxChoicebook::SetPageImage() not implemented") );

    return false;
}

// ----------------------------------------------------------------------------
// image list stuff
// ----------------------------------------------------------------------------

void wxChoicebook::SetImageList(wxImageList *imageList)
{
    // TODO: can be implemented in form of static bitmap near choice control

    wxBookCtrl::SetImageList(imageList);
}

// ----------------------------------------------------------------------------
// selection
// ----------------------------------------------------------------------------

int wxChoicebook::GetSelection() const
{
    return m_selection;
}

int wxChoicebook::SetSelection(size_t n)
{
    wxCHECK_MSG( n < GetPageCount(), wxNOT_FOUND,
                 _T("invalid page index in wxChoicebook::SetSelection()") );

    int selOld = m_selection;

    if ( (int)n != m_selection )
    {
        m_choice->Select(n);

        // change m_selection only now, otherwise OnChoiceSelected() would ignore
        // the selection change event

        if ( m_selection != wxNOT_FOUND )
            m_pages[m_selection]->Hide();
        wxWindow *page = m_pages[m_selection = n];
        page->SetSize(GetPageRect());
        page->Show();
    }

    return selOld;
}


// ----------------------------------------------------------------------------
// adding/removing the pages
// ----------------------------------------------------------------------------

bool
wxChoicebook::InsertPage(size_t n,
                         wxWindow *page,
                         const wxString& text,
                         bool bSelect,
                         int imageId)
{
    if ( !wxBookCtrl::InsertPage(n, page, text, bSelect, imageId) )
        return false;

    m_choice->Insert(text, n);

    if ( bSelect )
    {
        m_choice->Select(n);
    }
    else // don't select this page
    {
        // it will be shown only when selected
        page->Hide();
    }

    InvalidateBestSize();
    return true;
}

wxWindow *wxChoicebook::DoRemovePage(size_t page)
{
    wxWindow *win = wxBookCtrl::DoRemovePage(page);
    if ( win )
    {
        m_choice->Delete(page);
    }

    return win;
}


bool wxChoicebook::DeleteAllPages()
{
    m_choice->Clear();
    return wxBookCtrl::DeleteAllPages();
}

// ----------------------------------------------------------------------------
// wxChoicebook events
// ----------------------------------------------------------------------------

void wxChoicebook::OnChoiceSelected(wxCommandEvent& eventChoice)
{
    const int selNew = eventChoice.GetSelection();

    if ( selNew == m_selection )
    {
        // this event can only come from our own Select(m_selection) below
        // which we call when the page change is vetoed, so we should simply
        // ignore it
        return;
    }

    // first send "change in progress" event which may be vetoed by user
    wxChoicebookEvent eventIng(wxEVT_COMMAND_CHOICEBOOK_PAGE_CHANGING, GetId());

    eventIng.SetEventObject(this);
    eventIng.SetSelection(selNew);
    eventIng.SetOldSelection(m_selection);
    if ( GetEventHandler()->ProcessEvent(eventIng) && !eventIng.IsAllowed() )
    {
        m_choice->Select(m_selection);
        return;
    }

    // change allowed: do change the page and notify the user about it
    if ( m_selection != wxNOT_FOUND )
        m_pages[m_selection]->Hide();
    wxWindow *page = m_pages[m_selection = selNew];
    page->SetSize(GetPageRect());
    page->Show();

    wxChoicebookEvent eventEd(wxEVT_COMMAND_CHOICEBOOK_PAGE_CHANGED, GetId());

    eventEd.SetEventObject(this);
    eventEd.SetSelection(selNew);
    eventEd.SetOldSelection(m_selection);

    (void)GetEventHandler()->ProcessEvent(eventEd);
}

#endif // wxUSE_CHOICEBOOK
