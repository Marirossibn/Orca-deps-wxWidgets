/////////////////////////////////////////////////////////////////////////////
// Name:        src/common/dlgcmn.cpp
// Purpose:     common (to all ports) wxDialog functions
// Author:      Vadim Zeitlin
// Modified by:
// Created:     28.06.99
// RCS-ID:      $Id$
// Copyright:   (c) Vadim Zeitlin
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

#include "wx/dialog.h"

#ifndef WX_PRECOMP
    #include "wx/app.h"
    #include "wx/button.h"
    #include "wx/dcclient.h"
    #include "wx/intl.h"
    #include "wx/settings.h"
    #include "wx/stattext.h"
    #include "wx/sizer.h"
    #include "wx/containr.h"
#endif

#include "wx/statline.h"
#include "wx/sysopt.h"
#include "wx/module.h"
#include "wx/private/stattext.h"
#include "wx/bookctrl.h"
#include "wx/scrolwin.h"

#if wxUSE_DISPLAY
#include "wx/display.h"
#endif

// ----------------------------------------------------------------------------
// wxDialogBase
// ----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(wxDialogBase, wxTopLevelWindow)
    EVT_BUTTON(wxID_ANY, wxDialogBase::OnButton)

    EVT_CLOSE(wxDialogBase::OnCloseWindow)

    EVT_CHAR_HOOK(wxDialogBase::OnCharHook)
END_EVENT_TABLE()

wxDialogLayoutAdapter* wxDialogBase::sm_layoutAdapter = NULL;
bool wxDialogBase::sm_layoutAdaptation = false;

void wxDialogBase::Init()
{
    m_returnCode = 0;
    m_affirmativeId = wxID_OK;
    m_escapeId = wxID_ANY;
    m_layoutAdaptationLevel = 3;
    m_layoutAdaptationDone = FALSE;
    m_layoutAdaptationMode = wxDIALOG_ADAPTATION_MODE_DEFAULT;

    // the dialogs have this flag on by default to prevent the events from the
    // dialog controls from reaching the parent frame which is usually
    // undesirable and can lead to unexpected and hard to find bugs
    SetExtraStyle(GetExtraStyle() | wxWS_EX_BLOCK_EVENTS);
}

// helper of GetParentForModalDialog()
static bool CanBeUsedAsParent(wxWindow *parent)
{
    extern WXDLLIMPEXP_DATA_CORE(wxList) wxPendingDelete;

    return !parent->HasExtraStyle(wxWS_EX_TRANSIENT) &&
                parent->IsShownOnScreen() &&
                    !wxPendingDelete.Member(parent) &&
                        !parent->IsBeingDeleted();
}

wxWindow *wxDialogBase::GetParentForModalDialog(wxWindow *parent) const
{
    // creating a parent-less modal dialog will result (under e.g. wxGTK2)
    // in an unfocused dialog, so try to find a valid parent for it:
    if ( parent )
        parent = wxGetTopLevelParent(parent);

    if ( !parent || !CanBeUsedAsParent(parent) )
        parent = wxTheApp->GetTopWindow();

    if ( parent && !CanBeUsedAsParent(parent) )
    {
        // can't use this one, it's going to disappear
        parent = NULL;
    }

    return parent;
}

#if wxUSE_STATTEXT

class wxTextSizerWrapper : public wxTextWrapper
{
public:
    wxTextSizerWrapper(wxWindow *win)
    {
        m_win = win;
        m_hLine = 0;
    }

    wxSizer *CreateSizer(const wxString& text, int widthMax)
    {
        m_sizer = new wxBoxSizer(wxVERTICAL);
        Wrap(m_win, text, widthMax);
        return m_sizer;
    }

protected:
    virtual void OnOutputLine(const wxString& line)
    {
        if ( !line.empty() )
        {
            m_sizer->Add(new wxStaticText(m_win, wxID_ANY, line));
        }
        else // empty line, no need to create a control for it
        {
            if ( !m_hLine )
                m_hLine = m_win->GetCharHeight();

            m_sizer->Add(5, m_hLine);
        }
    }

private:
    wxWindow *m_win;
    wxSizer *m_sizer;
    int m_hLine;
};

wxSizer *wxDialogBase::CreateTextSizer(const wxString& message)
{
    // I admit that this is complete bogus, but it makes
    // message boxes work for pda screens temporarily..
    int widthMax = -1;
    const bool is_pda = wxSystemSettings::GetScreenType() <= wxSYS_SCREEN_PDA;
    if (is_pda)
    {
        widthMax = wxSystemSettings::GetMetric( wxSYS_SCREEN_X ) - 25;
    }

    // '&' is used as accel mnemonic prefix in the wxWidgets controls but in
    // the static messages created by CreateTextSizer() (used by wxMessageBox,
    // for example), we don't want this special meaning, so we need to quote it
    wxString text(message);
    text.Replace(_T("&"), _T("&&"));

    wxTextSizerWrapper wrapper(this);

    return wrapper.CreateSizer(text, widthMax);
}

#endif // wxUSE_STATTEXT

wxSizer *wxDialogBase::CreateButtonSizer(long flags)
{
#ifdef __SMARTPHONE__
    wxDialog* dialog = (wxDialog*) this;
    if ( flags & wxOK )
        dialog->SetLeftMenu(wxID_OK);

    if ( flags & wxCANCEL )
        dialog->SetRightMenu(wxID_CANCEL);

    if ( flags & wxYES )
        dialog->SetLeftMenu(wxID_YES);

    if ( flags & wxNO )
        dialog->SetRightMenu(wxID_NO);

    return NULL;
#else // !__SMARTPHONE__

#if wxUSE_BUTTON

#ifdef __POCKETPC__
    // PocketPC guidelines recommend for Ok/Cancel dialogs to use OK button
    // located inside caption bar and implement Cancel functionality through
    // Undo outside dialog. As native behaviour this will be default here but
    // can be replaced with real wxButtons by setting the option below to 1
    if ( (flags & ~(wxCANCEL|wxNO_DEFAULT)) != wxOK ||
            wxSystemOptions::GetOptionInt(wxT("wince.dialog.real-ok-cancel")) )
#endif // __POCKETPC__
    {
        return CreateStdDialogButtonSizer(flags);
    }
#ifdef __POCKETPC__
    return NULL;
#endif // __POCKETPC__

#else // !wxUSE_BUTTON
    wxUnusedVar(flags);

    return NULL;
#endif // wxUSE_BUTTON/!wxUSE_BUTTON

#endif // __SMARTPHONE__/!__SMARTPHONE__
}

wxSizer *wxDialogBase::CreateSeparatedButtonSizer(long flags)
{
    wxSizer *sizer = CreateButtonSizer(flags);
    if ( !sizer )
        return NULL;

    // Mac Human Interface Guidelines recommend not to use static lines as
    // grouping elements
#if wxUSE_STATLINE && !defined(__WXMAC__)
    wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);
    topsizer->Add(new wxStaticLine(this),
                   wxSizerFlags().Expand().DoubleBorder(wxBOTTOM));
    topsizer->Add(sizer, wxSizerFlags().Expand());
    sizer = topsizer;
#endif // wxUSE_STATLINE

    return sizer;
}

#if wxUSE_BUTTON

wxStdDialogButtonSizer *wxDialogBase::CreateStdDialogButtonSizer( long flags )
{
    wxStdDialogButtonSizer *sizer = new wxStdDialogButtonSizer();

    wxButton *ok = NULL;
    wxButton *yes = NULL;
    wxButton *no = NULL;

    if (flags & wxOK)
    {
        ok = new wxButton(this, wxID_OK);
        sizer->AddButton(ok);
    }

    if (flags & wxCANCEL)
    {
        wxButton *cancel = new wxButton(this, wxID_CANCEL);
        sizer->AddButton(cancel);
    }

    if (flags & wxYES)
    {
        yes = new wxButton(this, wxID_YES);
        sizer->AddButton(yes);
    }

    if (flags & wxNO)
    {
        no = new wxButton(this, wxID_NO);
        sizer->AddButton(no);
    }

    if (flags & wxAPPLY)
    {
        wxButton *apply = new wxButton(this, wxID_APPLY);
        sizer->AddButton(apply);
    }

    if (flags & wxCLOSE)
    {
        wxButton *close = new wxButton(this, wxID_CLOSE);
        sizer->AddButton(close);
    }

    if (flags & wxHELP)
    {
        wxButton *help = new wxButton(this, wxID_HELP);
        sizer->AddButton(help);
    }

    if (flags & wxNO_DEFAULT)
    {
        if (no)
        {
            no->SetDefault();
            no->SetFocus();
        }
    }
    else
    {
        if (ok)
        {
            ok->SetDefault();
            ok->SetFocus();
        }
        else if (yes)
        {
            yes->SetDefault();
            yes->SetFocus();
        }
    }

    if (flags & wxOK)
        SetAffirmativeId(wxID_OK);
    else if (flags & wxYES)
        SetAffirmativeId(wxID_YES);

    sizer->Realize();

    return sizer;
}

#endif // wxUSE_BUTTON

// ----------------------------------------------------------------------------
// standard buttons handling
// ----------------------------------------------------------------------------

void wxDialogBase::EndDialog(int rc)
{
    if ( IsModal() )
        EndModal(rc);
    else
        Hide();
}

void wxDialogBase::AcceptAndClose()
{
    if ( Validate() && TransferDataFromWindow() )
    {
        EndDialog(m_affirmativeId);
    }
}

void wxDialogBase::SetAffirmativeId(int affirmativeId)
{
    m_affirmativeId = affirmativeId;
}

void wxDialogBase::SetEscapeId(int escapeId)
{
    m_escapeId = escapeId;
}

bool wxDialogBase::EmulateButtonClickIfPresent(int id)
{
#if wxUSE_BUTTON
    wxButton *btn = wxDynamicCast(FindWindow(id), wxButton);

    if ( !btn || !btn->IsEnabled() || !btn->IsShown() )
        return false;

    wxCommandEvent event(wxEVT_COMMAND_BUTTON_CLICKED, id);
    event.SetEventObject(btn);
    btn->GetEventHandler()->ProcessEvent(event);

    return true;
#else // !wxUSE_BUTTON
    wxUnusedVar(id);
    return false;
#endif // wxUSE_BUTTON/!wxUSE_BUTTON
}

bool wxDialogBase::IsEscapeKey(const wxKeyEvent& event)
{
    // for most platforms, Esc key is used to close the dialogs
    return event.GetKeyCode() == WXK_ESCAPE &&
                event.GetModifiers() == wxMOD_NONE;
}

void wxDialogBase::OnCharHook(wxKeyEvent& event)
{
    if ( event.GetKeyCode() == WXK_ESCAPE )
    {
        int idCancel = GetEscapeId();
        switch ( idCancel )
        {
            case wxID_NONE:
                // don't handle Esc specially at all
                break;

            case wxID_ANY:
                // this value is special: it means translate Esc to wxID_CANCEL
                // but if there is no such button, then fall back to wxID_OK
                if ( EmulateButtonClickIfPresent(wxID_CANCEL) )
                    return;
                idCancel = GetAffirmativeId();
                // fall through

            default:
                // translate Esc to button press for the button with given id
                if ( EmulateButtonClickIfPresent(idCancel) )
                    return;
        }
    }

    event.Skip();
}

void wxDialogBase::OnButton(wxCommandEvent& event)
{
    const int id = event.GetId();
    if ( id == GetAffirmativeId() )
    {
        AcceptAndClose();
    }
    else if ( id == wxID_APPLY )
    {
        if ( Validate() )
            TransferDataFromWindow();

        // TODO: disable the Apply button until things change again
    }
    else if ( id == GetEscapeId() ||
                (id == wxID_CANCEL && GetEscapeId() == wxID_ANY) )
    {
        EndDialog(wxID_CANCEL);
    }
    else // not a standard button
    {
        event.Skip();
    }
}

// ----------------------------------------------------------------------------
// other event handlers
// ----------------------------------------------------------------------------

void wxDialogBase::OnCloseWindow(wxCloseEvent& WXUNUSED(event))
{
    // We'll send a Cancel message by default, which may close the dialog.
    // Check for looping if the Cancel event handler calls Close().

    // Note that if a cancel button and handler aren't present in the dialog,
    // nothing will happen when you close the dialog via the window manager, or
    // via Close(). We wouldn't want to destroy the dialog by default, since
    // the dialog may have been created on the stack. However, this does mean
    // that calling dialog->Close() won't delete the dialog unless the handler
    // for wxID_CANCEL does so. So use Destroy() if you want to be sure to
    // destroy the dialog. The default OnCancel (above) simply ends a modal
    // dialog, and hides a modeless dialog.

    // VZ: this is horrible and MT-unsafe. Can't we reuse some of these global
    //     lists here? don't dare to change it now, but should be done later!
    static wxList closing;

    if ( closing.Member(this) )
        return;

    closing.Append(this);

    wxCommandEvent cancelEvent(wxEVT_COMMAND_BUTTON_CLICKED, wxID_CANCEL);
    cancelEvent.SetEventObject( this );
    GetEventHandler()->ProcessEvent(cancelEvent); // This may close the dialog

    closing.DeleteObject(this);
}

void wxDialogBase::OnSysColourChanged(wxSysColourChangedEvent& event)
{
#ifndef __WXGTK__
    SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE));
    Refresh();
#endif

    event.Skip();
}

/// Do the adaptation
bool wxDialogBase::DoLayoutAdaptation()
{
    if (GetLayoutAdapter())
        return GetLayoutAdapter()->DoLayoutAdaptation((wxDialog*) this);
    else
        return false;
}

/// Can we do the adaptation?
bool wxDialogBase::CanDoLayoutAdaptation()
{
    // Check if local setting overrides the global setting
    bool layoutEnabled = (GetLayoutAdaptationMode() == wxDIALOG_ADAPTATION_MODE_ENABLED) || (IsLayoutAdaptationEnabled() && (GetLayoutAdaptationMode() != wxDIALOG_ADAPTATION_MODE_DISABLED));

    return (layoutEnabled && !m_layoutAdaptationDone && GetLayoutAdaptationLevel() != 0 && GetLayoutAdapter() != NULL && GetLayoutAdapter()->CanDoLayoutAdaptation((wxDialog*) this));
}

/// Set scrolling adapter class, returning old adapter
wxDialogLayoutAdapter* wxDialogBase::SetLayoutAdapter(wxDialogLayoutAdapter* adapter)
{
    wxDialogLayoutAdapter* oldLayoutAdapter = sm_layoutAdapter;
    sm_layoutAdapter = adapter;
    return oldLayoutAdapter;
}

/*!
 * Standard adapter
 */

IMPLEMENT_CLASS(wxDialogLayoutAdapter, wxObject)

IMPLEMENT_CLASS(wxStandardDialogLayoutAdapter, wxDialogLayoutAdapter)

// Allow for caption size on wxWidgets < 2.9
#if defined(__WXGTK__) && !wxCHECK_VERSION(2,9,0)
#define wxEXTRA_DIALOG_HEIGHT 30
#else
#define wxEXTRA_DIALOG_HEIGHT 0
#endif

/// Indicate that adaptation should be done
bool wxStandardDialogLayoutAdapter::CanDoLayoutAdaptation(wxDialog* dialog)
{
    if (dialog->GetSizer())
    {
        wxSize windowSize, displaySize;
        return MustScroll(dialog, windowSize, displaySize) != 0;
    }
    else
        return false;
}

bool wxStandardDialogLayoutAdapter::DoLayoutAdaptation(wxDialog* dialog)
{
    if (dialog->GetSizer())
    {
#if wxUSE_BOOKCTRL
        wxBookCtrlBase* bookContentWindow = wxDynamicCast(dialog->GetContentWindow(), wxBookCtrlBase);

        if (bookContentWindow)
        {
            // If we have a book control, make all the pages (that use sizers) scrollable
            wxWindowList windows;
            for (size_t i = 0; i < bookContentWindow->GetPageCount(); i++)
            {
                wxWindow* page = bookContentWindow->GetPage(i);

                wxScrolledWindow* scrolledWindow = wxDynamicCast(page, wxScrolledWindow);
                if (scrolledWindow)
                    windows.Append(scrolledWindow);
                else if (!scrolledWindow && page->GetSizer())
                {
                    // Create a scrolled window and reparent
                    scrolledWindow = CreateScrolledWindow(page);
                    wxSizer* oldSizer = page->GetSizer();

                    wxSizer* newSizer = new wxBoxSizer(wxVERTICAL);
                    newSizer->Add(scrolledWindow,1, wxEXPAND, 0);

                    page->SetSizer(newSizer, false /* don't delete the old sizer */);

                    scrolledWindow->SetSizer(oldSizer);

                    ReparentControls(page, scrolledWindow);

                    windows.Append(scrolledWindow);
                }
            }

            FitWithScrolling(dialog, windows);
        }
        else
#endif // wxUSE_BOOKCTRL
        {
            // If we have an arbitrary dialog, create a scrolling area for the main content, and a button sizer
            // for the main buttons.
            wxScrolledWindow* scrolledWindow = CreateScrolledWindow(dialog);

            int buttonSizerBorder = 0;

            // First try to find a wxStdDialogButtonSizer
            wxSizer* buttonSizer = FindButtonSizer(true /* find std button sizer */, dialog, dialog->GetSizer(), buttonSizerBorder);

            // Next try to find a wxBoxSizer containing the controls
            if (!buttonSizer && dialog->GetLayoutAdaptationLevel() > wxDIALOG_ADAPTATION_STANDARD_SIZER)
                buttonSizer = FindButtonSizer(false /* find ordinary sizer */, dialog, dialog->GetSizer(), buttonSizerBorder);

            // If we still don't have a button sizer, collect any 'loose' buttons in the layout
            if (!buttonSizer && dialog->GetLayoutAdaptationLevel() > wxDIALOG_ADAPTATION_ANY_SIZER)
            {
                int count = 0;
                wxStdDialogButtonSizer* stdButtonSizer = new wxStdDialogButtonSizer;
                buttonSizer = stdButtonSizer;

                FindLooseButtons(dialog, stdButtonSizer, dialog->GetSizer(), count);
                if (count > 0)
                    stdButtonSizer->Realize();
                else
                {
                    delete buttonSizer;
                    buttonSizer = NULL;
                }
            }

            if (buttonSizerBorder == 0)
                buttonSizerBorder = 5;

            ReparentControls(dialog, scrolledWindow, buttonSizer);

            wxBoxSizer* newTopSizer = new wxBoxSizer(wxVERTICAL);
            wxSizer* oldSizer = dialog->GetSizer();

            dialog->SetSizer(newTopSizer, false /* don't delete old sizer */);

            newTopSizer->Add(scrolledWindow, 1, wxEXPAND|wxALL, 0);
            if (buttonSizer)
                newTopSizer->Add(buttonSizer, 0, wxEXPAND|wxALL, buttonSizerBorder);

            scrolledWindow->SetSizer(oldSizer);

            FitWithScrolling(dialog, scrolledWindow);
        }
    }

    dialog->SetLayoutAdaptationDone(true);
    return true;
}

// Create the scrolled window
wxScrolledWindow* wxStandardDialogLayoutAdapter::CreateScrolledWindow(wxWindow* parent)
{
    wxScrolledWindow* scrolledWindow = new wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxVSCROLL|wxHSCROLL|wxBORDER_NONE);
    return scrolledWindow;
}

/// Find and remove the button sizer, if any
wxSizer* wxStandardDialogLayoutAdapter::FindButtonSizer(bool stdButtonSizer, wxDialog* dialog, wxSizer* sizer, int& retBorder, int accumlatedBorder)
{
    for ( wxSizerItemList::compatibility_iterator node = sizer->GetChildren().GetFirst();
          node; node = node->GetNext() )
    {
        wxSizerItem *item = node->GetData();
        wxSizer *childSizer = item->GetSizer();

        if ( childSizer )
        {
            int newBorder = accumlatedBorder;
            if (item->GetFlag() & wxALL)
                newBorder += item->GetBorder();

            if (stdButtonSizer) // find wxStdDialogButtonSizer
            {
                wxStdDialogButtonSizer* buttonSizer = wxDynamicCast(childSizer, wxStdDialogButtonSizer);
                if (buttonSizer)
                {
                    sizer->Detach(childSizer);
                    retBorder = newBorder;
                    return buttonSizer;
                }
            }
            else // find a horizontal box sizer containing standard buttons
            {
                wxBoxSizer* buttonSizer = wxDynamicCast(childSizer, wxBoxSizer);
                if (buttonSizer && IsOrdinaryButtonSizer(dialog, buttonSizer))
                {
                    sizer->Detach(childSizer);
                    retBorder = newBorder;
                    return buttonSizer;
                }
            }

            wxSizer* s = FindButtonSizer(stdButtonSizer, dialog, childSizer, retBorder, newBorder);
            if (s)
                return s;
        }
    }
    return NULL;
}

/// Check if this sizer contains standard buttons, and so can be repositioned in the dialog
bool wxStandardDialogLayoutAdapter::IsOrdinaryButtonSizer(wxDialog* dialog, wxBoxSizer* sizer)
{
    if (sizer->GetOrientation() != wxHORIZONTAL)
        return false;

    for ( wxSizerItemList::compatibility_iterator node = sizer->GetChildren().GetFirst();
          node; node = node->GetNext() )
    {
        wxSizerItem *item = node->GetData();
        wxButton *childButton = wxDynamicCast(item->GetWindow(), wxButton);

        if (childButton && IsStandardButton(dialog, childButton))
            return true;
    }
    return false;
}

/// Check if this is a standard button
bool wxStandardDialogLayoutAdapter::IsStandardButton(wxDialog* dialog, wxButton* button)
{
    wxWindowID id = button->GetId();

    return (id == wxID_OK || id == wxID_CANCEL || id == wxID_YES || id == wxID_NO || id == wxID_SAVE ||
            id == wxID_APPLY || id == wxID_HELP || id == wxID_CONTEXT_HELP || dialog->IsMainButtonId(id));
}

/// Find 'loose' main buttons in the existing layout and add them to the standard dialog sizer
bool wxStandardDialogLayoutAdapter::FindLooseButtons(wxDialog* dialog, wxStdDialogButtonSizer* buttonSizer, wxSizer* sizer, int& count)
{
    wxSizerItemList::compatibility_iterator node = sizer->GetChildren().GetFirst();
    while (node)
    {
        wxSizerItemList::compatibility_iterator next = node->GetNext();
        wxSizerItem *item = node->GetData();
        wxSizer *childSizer = item->GetSizer();
        wxButton *childButton = wxDynamicCast(item->GetWindow(), wxButton);

        if (childButton && IsStandardButton(dialog, childButton))
        {
            sizer->Detach(childButton);
            buttonSizer->AddButton(childButton);
            count ++;
        }

        if (childSizer)
            FindLooseButtons(dialog, buttonSizer, childSizer, count);

        node = next;
    }
    return true;
}

/// Reparent the controls to the scrolled window
void wxStandardDialogLayoutAdapter::ReparentControls(wxWindow* parent, wxWindow* reparentTo, wxSizer* buttonSizer)
{
    DoReparentControls(parent, reparentTo, buttonSizer);
}

void wxStandardDialogLayoutAdapter::DoReparentControls(wxWindow* parent, wxWindow* reparentTo, wxSizer* buttonSizer)
{
    wxWindowList::compatibility_iterator node = parent->GetChildren().GetFirst();
    while (node)
    {
        wxWindowList::compatibility_iterator next = node->GetNext();

        wxWindow *win = node->GetData();

        // Don't reparent the scrolled window or buttons in the button sizer
        if (win != reparentTo && (!buttonSizer || !buttonSizer->GetItem(win)))
        {
            win->Reparent(reparentTo);
#ifdef __WXMSW__
            // Restore correct tab order
            ::SetWindowPos((HWND) win->GetHWND(), HWND_BOTTOM, -1, -1, -1, -1, SWP_NOMOVE|SWP_NOSIZE);
#endif
        }

        node = next;
    }
}

/// Find whether scrolling will be necessary for the dialog, returning wxVERTICAL, wxHORIZONTAL or both
int wxStandardDialogLayoutAdapter::MustScroll(wxDialog* dialog, wxSize& windowSize, wxSize& displaySize)
{
    return DoMustScroll(dialog, windowSize, displaySize);
}

/// Find whether scrolling will be necessary for the dialog, returning wxVERTICAL, wxHORIZONTAL or both
int wxStandardDialogLayoutAdapter::DoMustScroll(wxDialog* dialog, wxSize& windowSize, wxSize& displaySize)
{
    wxSize minWindowSize = dialog->GetSizer()->GetMinSize();
    windowSize = dialog->GetSize();
    windowSize = wxSize(wxMax(windowSize.x, minWindowSize.x), wxMax(windowSize.y, minWindowSize.y));
#if wxUSE_DISPLAY
    displaySize = wxDisplay(wxDisplay::GetFromWindow(dialog)).GetClientArea().GetSize();
#else
    displaySize = wxGetClientDisplayRect().GetSize();
#endif

    int flags = 0;

    if (windowSize.y >= (displaySize.y - wxEXTRA_DIALOG_HEIGHT))
        flags |= wxVERTICAL;
    if (windowSize.x >= displaySize.x)
        flags |= wxHORIZONTAL;

    return flags;
}

// A function to fit the dialog around its contents, and then adjust for screen size.
// If scrolled windows are passed, scrolling is enabled in the required orientation(s).
bool wxStandardDialogLayoutAdapter::FitWithScrolling(wxDialog* dialog, wxWindowList& windows)
{
    return DoFitWithScrolling(dialog, windows);
}

// A function to fit the dialog around its contents, and then adjust for screen size.
// If a scrolled window is passed, scrolling is enabled in the required orientation(s).
bool wxStandardDialogLayoutAdapter::FitWithScrolling(wxDialog* dialog, wxScrolledWindow* scrolledWindow)
{
    return DoFitWithScrolling(dialog, scrolledWindow);
}

// A function to fit the dialog around its contents, and then adjust for screen size.
// If a scrolled window is passed, scrolling is enabled in the required orientation(s).
bool wxStandardDialogLayoutAdapter::DoFitWithScrolling(wxDialog* dialog, wxScrolledWindow* scrolledWindow)
{
    wxWindowList windows;
    windows.Append(scrolledWindow);
    return DoFitWithScrolling(dialog, windows);
}

bool wxStandardDialogLayoutAdapter::DoFitWithScrolling(wxDialog* dialog, wxWindowList& windows)
{
    wxSizer* sizer = dialog->GetSizer();
    if (!sizer)
        return false;

    sizer->SetSizeHints(dialog);

    wxSize windowSize, displaySize;
    int scrollFlags = DoMustScroll(dialog, windowSize, displaySize);
    int scrollBarSize = 20;

    if (scrollFlags)
    {
        int scrollBarExtraX = 0, scrollBarExtraY = 0;
        bool resizeHorizontally = (scrollFlags & wxHORIZONTAL) != 0;
        bool resizeVertically = (scrollFlags & wxVERTICAL) != 0;

        if (windows.GetCount() != 0)
        {
            // Allow extra for a scrollbar, assuming we resizing in one direction only.
            if ((resizeVertically && !resizeHorizontally) && (windowSize.x < (displaySize.x - scrollBarSize)))
                scrollBarExtraX = scrollBarSize;
            if ((resizeHorizontally && !resizeVertically) && (windowSize.y < (displaySize.y - scrollBarSize)))
                scrollBarExtraY = scrollBarSize;
        }

        wxWindowList::compatibility_iterator node = windows.GetFirst();
        while (node)
        {
            wxWindow *win = node->GetData();
            wxScrolledWindow* scrolledWindow = wxDynamicCast(win, wxScrolledWindow);
            if (scrolledWindow)
            {
                scrolledWindow->SetScrollRate(resizeHorizontally ? 10 : 0, resizeVertically ? 10 : 0);

                if (scrolledWindow->GetSizer())
                    scrolledWindow->GetSizer()->Fit(scrolledWindow);
            }

            node = node->GetNext();
        }

        wxSize limitTo = windowSize + wxSize(scrollBarExtraX, scrollBarExtraY);
        if (resizeVertically)
            limitTo.y = displaySize.y - wxEXTRA_DIALOG_HEIGHT;
        if (resizeHorizontally)
            limitTo.x = displaySize.x;

        dialog->SetMinSize(limitTo);
        dialog->SetSize(limitTo);

        dialog->SetSizeHints( limitTo.x, limitTo.y, dialog->GetMaxWidth(), dialog->GetMaxHeight() );
    }

    return true;
}

/*!
 * Module to initialise standard adapter
 */

class wxDialogLayoutAdapterModule: public wxModule
{
    DECLARE_DYNAMIC_CLASS(wxDialogLayoutAdapterModule)
public:
    wxDialogLayoutAdapterModule() {}
    virtual void OnExit() { delete wxDialogBase::SetLayoutAdapter(NULL); }
    virtual bool OnInit() { wxDialogBase::SetLayoutAdapter(new wxStandardDialogLayoutAdapter); return true; }
};

IMPLEMENT_DYNAMIC_CLASS(wxDialogLayoutAdapterModule, wxModule)
