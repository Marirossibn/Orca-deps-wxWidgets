///////////////////////////////////////////////////////////////////////////////
// Name:        src/generic/filepickerg.cpp
// Purpose:     wxGenericFileDirButton class implementation
// Author:      Francesco Montorsi
// Modified by:
// Created:     15/04/2006
// RCS-ID:      $Id$
// Copyright:   (c) Francesco Montorsi
// Licence:     wxWindows licence
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
    #include "wx/window.h"
#endif //WX_PRECOMP

#include "wx/filepicker.h"
#include "wx/filedlg.h"


// ============================================================================
// implementation
// ============================================================================

#if wxUSE_FILEPICKERCTRL || wxUSE_DIRPICKERCTRL

IMPLEMENT_DYNAMIC_CLASS(wxGenericFileButton, wxButton)
IMPLEMENT_DYNAMIC_CLASS(wxGenericDirButton, wxButton)

// ----------------------------------------------------------------------------
// wxGenericFileButton
// ----------------------------------------------------------------------------

bool wxGenericFileDirButton::Create( wxWindow *parent, wxWindowID id,
                        const wxString &label, const wxString &path,
                        const wxString &message, const wxString &wildcard,
                        const wxPoint &pos, const wxSize &size, long style,
                        const wxValidator& validator, const wxString &name)
{
    // create this button
    if (!wxButton::Create(parent, id, label, pos, size, style,
                          validator, name))
    {
        wxFAIL_MSG( wxT("wxGenericFileButton creation failed") );
        return false;
    }

    // and handle user clicks on it
    Connect(wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(wxGenericFileDirButton::OnButtonClick),
            NULL, this);

    // create the dialog associated with this button
    m_path = path;
    return CreateDialog(message, wildcard);
}

void wxGenericFileDirButton::OnButtonClick(wxCommandEvent& WXUNUSED(ev))
{
    if (m_dialog->ShowModal() == wxID_OK)
    {
        // save the path
        UpdatePathFromDialog();

        // fire an event
        wxFileDirPickerEvent event(GetEventType(), this, GetId(), m_path);
        GetEventHandler()->ProcessEvent(event);
    }
}

#endif      // wxUSE_FILEPICKERCTRL || wxUSE_DIRPICKERCTRL
