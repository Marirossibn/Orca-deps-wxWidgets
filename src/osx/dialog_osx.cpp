/////////////////////////////////////////////////////////////////////////////
// Name:        src/osx/dialog_osx.cpp
// Purpose:     wxDialog class
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// RCS-ID:      $Id: dialog.cpp 54820 2008-07-29 20:04:11Z SC $
// Copyright:   (c) Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#include "wx/dialog.h"

#ifndef WX_PRECOMP
    #include "wx/app.h"
    #include "wx/utils.h"
    #include "wx/frame.h"
    #include "wx/settings.h"
#endif // WX_PRECOMP

#include "wx/osx/private.h"


// Lists to keep track of windows, so we can disable/enable them
// for modal dialogs

wxList wxModalDialogs;

IMPLEMENT_DYNAMIC_CLASS(wxDialog, wxTopLevelWindow)

void wxDialog::Init()
{
    m_modality = wxDIALOG_MODALITY_NONE;
}

bool wxDialog::Create( wxWindow *parent,
    wxWindowID id,
    const wxString& title,
    const wxPoint& pos,
    const wxSize& size,
    long style,
    const wxString& name )
{
    SetExtraStyle( GetExtraStyle() | wxTOPLEVEL_EX_DIALOG );

    // All dialogs should really have this style...
    style |= wxTAB_TRAVERSAL;

    // ...but not these styles
    style &= ~(wxYES | wxOK | wxNO); // | wxCANCEL

    if ( !wxTopLevelWindow::Create( parent, id, title, pos, size, style, name ) )
        return false;

    return true;
}

wxDialog::~wxDialog()
{
    SendDestroyEvent();

    // if the dialog is modal, this will end its event loop
    Show(false);
}

// On mac command-stop does the same thing as Esc, let the base class know
// about it
bool wxDialog::IsEscapeKey(const wxKeyEvent& event)
{
    if ( event.GetKeyCode() == '.' && event.GetModifiers() == wxMOD_CMD )
        return true;

    return wxDialogBase::IsEscapeKey(event);
}

bool wxDialog::IsModal() const
{
    return m_modality != wxDIALOG_MODALITY_NONE;
}

bool wxDialog::Show(bool show)
{
    if ( m_modality == wxDIALOG_MODALITY_WINDOW_MODAL )
    {
        if ( !wxWindow::Show(show) )
            // nothing to do
            return false;
    }
    else 
    {
        if ( !wxDialogBase::Show(show) )
            // nothing to do
            return false;
    }

    if (show && CanDoLayoutAdaptation())
        DoLayoutAdaptation();

    if ( show )
        // usually will result in TransferDataToWindow() being called
        InitDialog();

    if ( !show )
    {
        switch( m_modality )
        {
            case wxDIALOG_MODALITY_WINDOW_MODAL:
                EndWindowModal(); // OS X implementation method for cleanup
                SendWindowModalDialogEvent ( wxEVT_WINDOW_MODAL_DIALOG_CLOSED  );        
                break;
            default:
                break;
        }
        m_modality = wxDIALOG_MODALITY_NONE;
    }
    
    return true;
}

// Replacement for Show(true) for modal dialogs - returns return code
int wxDialog::ShowModal()
{
    m_modality = wxDIALOG_MODALITY_APP_MODAL;
    
    Show();

    DoShowModal();

    return GetReturnCode();
}

void wxDialog::ShowWindowModal()
{
    m_modality = wxDIALOG_MODALITY_WINDOW_MODAL;
    
    Show();
    
    DoShowWindowModal();
}

wxDialogModality wxDialog::GetModality() const
{
    return m_modality;
}

// NB: this function (surprisingly) may be called for both modal and modeless
//     dialogs and should work for both of them
void wxDialog::EndModal(int retCode)
{
    SetReturnCode(retCode);
    Show(false);
}

