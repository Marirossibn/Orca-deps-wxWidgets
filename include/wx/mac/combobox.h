/////////////////////////////////////////////////////////////////////////////
// Name:        combobox.h
// Purpose:     wxComboBox class
// Author:      Stefan Csomor
// Modified by:
// Created:     1998-01-01
// RCS-ID:      $Id$
// Copyright:   (c) Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_COMBOBOX_H_
#define _WX_COMBOBOX_H_

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "combobox.h"
#endif

#include "wx/textctrl.h"
#include "wx/choice.h"

WXDLLEXPORT_DATA(extern const wxChar*) wxComboBoxNameStr;

// Combobox item
class WXDLLEXPORT wxComboBox : public wxControl, public wxComboBoxBase
{
  DECLARE_DYNAMIC_CLASS(wxComboBox)

 public:
    inline wxComboBox() {}
    virtual ~wxComboBox();
    // override the base class virtuals involved in geometry calculations
    virtual wxSize DoGetBestSize() const;
    virtual void DoMoveWindow(int x, int y, int width, int height);

    // forward these functions to all subcontrols
    virtual bool Enable(bool enable = TRUE);
    virtual bool Show(bool show = TRUE);
    virtual void SetFocus();

    // callback functions
    virtual void DelegateTextChanged( const wxString& value );
    virtual void DelegateChoice( const wxString& value );

    inline wxComboBox(wxWindow *parent, wxWindowID id,
           const wxString& value = wxEmptyString,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize,
           int n = 0, const wxString choices[] = NULL,
           long style = 0,
           const wxValidator& validator = wxDefaultValidator,
           const wxString& name = wxComboBoxNameStr)
    {
    Create(parent, id, value, pos, size, n, choices, style, validator, name);
    }

    bool Create(wxWindow *parent, wxWindowID id,
           const wxString& value = wxEmptyString,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize,
           int n = 0, const wxString choices[] = NULL,
           long style = 0,
           const wxValidator& validator = wxDefaultValidator,
           const wxString& name = wxComboBoxNameStr);

    // List functions
    virtual void Delete(int n);
    virtual void Clear();

    virtual int GetSelection() const ;
    virtual void SetSelection(int n);
    virtual void Select(int n) { SetSelection(n) ; }
    virtual int FindString(const wxString& s) const;
    virtual wxString GetString(int n) const ;
    virtual wxString GetStringSelection() const ;
    virtual void SetString(int n, const wxString& s) ;
    virtual bool SetStringSelection(const wxString& sel);

    // Text field functions
    virtual wxString GetValue() const ;
    virtual void SetValue(const wxString& value);

    // Clipboard operations
    virtual void Copy();
    virtual void Cut();
    virtual void Paste();
    virtual void SetInsertionPoint(long pos);
    virtual void SetInsertionPointEnd();
    virtual long GetInsertionPoint() const ;
    virtual long GetLastPosition() const ;
    virtual void Replace(long from, long to, const wxString& value);
    virtual void Remove(long from, long to);
    virtual void SetSelection(long from, long to);
    virtual void SetEditable(bool editable);
    virtual int GetCount() const { return m_choice->GetCount() ; }
    void MacHandleControlClick( WXWidget control , wxInt16 controlpart , bool mouseStillDown ) ;

    wxCONTROL_ITEMCONTAINER_CLIENTDATAOBJECT_RECAST

protected:
    virtual int DoAppend(const wxString& item) ;
    virtual int DoInsert(const wxString& item, int pos) ;

    virtual void DoSetItemClientData(int n, void* clientData) ;
    virtual void* DoGetItemClientData(int n) const ;
    virtual void DoSetItemClientObject(int n, wxClientData* clientData) ;
    virtual wxClientData* DoGetItemClientObject(int n) const ;

    void FreeData();

    // the subcontrols
    wxTextCtrl*     m_text;
    wxChoice*       m_choice;
};

#endif
    // _WX_COMBOBOX_H_
