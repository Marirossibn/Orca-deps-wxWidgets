/////////////////////////////////////////////////////////////////////////////
// Name:        textdlgg.h
// Purpose:     wxTextEntryDialog class
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __TEXTDLGH_G__
#define __TEXTDLGH_G__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "textdlgg.h"
#endif

#if wxUSE_TEXTDLG
#include "wx/defs.h"

#include "wx/dialog.h"

#if wxUSE_VALIDATORS
#include "wx/valtext.h"
#endif

class WXDLLEXPORT wxTextCtrl;

WXDLLEXPORT_DATA(extern const wxChar*) wxGetTextFromUserPromptStr;

#define wxTextEntryDialogStyle (wxOK | wxCANCEL | wxCENTRE | wxWS_EX_VALIDATE_RECURSIVELY)

// ----------------------------------------------------------------------------
// wxTextEntryDialog: a dialog with text control, [ok] and [cancel] buttons
// ----------------------------------------------------------------------------

class WXDLLEXPORT wxTextEntryDialog : public wxDialog
{
public:
    wxTextEntryDialog(wxWindow *parent,
                      const wxString& message,
                      const wxString& caption = wxGetTextFromUserPromptStr,
                      const wxString& value = wxEmptyString,
                      long style = wxTextEntryDialogStyle,
                      const wxPoint& pos = wxDefaultPosition);

    void SetValue(const wxString& val);
    wxString GetValue() const { return m_value; }

#if wxUSE_VALIDATORS
    void SetTextValidator( wxTextValidator& validator );
    void SetTextValidator( long style = wxFILTER_NONE );
    wxTextValidator* GetTextValidator() { return (wxTextValidator*)m_textctrl->GetValidator(); }
#endif
  // wxUSE_VALIDATORS

    // implementation only
    void OnOK(wxCommandEvent& event);

protected:
    wxTextCtrl *m_textctrl;
    wxString    m_value;
    long        m_dialogStyle;

private:
    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(wxTextEntryDialog)
    DECLARE_NO_COPY_CLASS(wxTextEntryDialog)
};

// ----------------------------------------------------------------------------
// function to get a string from user
// ----------------------------------------------------------------------------

wxString WXDLLEXPORT
wxGetTextFromUser(const wxString& message,
                  const wxString& caption = wxGetTextFromUserPromptStr,
                  const wxString& default_value = wxEmptyString,
                  wxWindow *parent = (wxWindow *) NULL,
                  wxCoord x = wxDefaultCoord,
                  wxCoord y = wxDefaultCoord,
                  bool centre = true);

wxString WXDLLEXPORT
wxGetPasswordFromUser(const wxString& message,
                      const wxString& caption = wxGetTextFromUserPromptStr,
                      const wxString& default_value = wxEmptyString,
                      wxWindow *parent = (wxWindow *) NULL);

#endif
    // wxUSE_TEXTDLG
#endif
    // __TEXTDLGH_G__
