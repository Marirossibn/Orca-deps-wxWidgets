/////////////////////////////////////////////////////////////////////////////
// Name:        wx/gtk/spinbutt.h
// Purpose:     wxSpinButton class
// Author:      Robert Roebling
// Modified by:
// RCS-ID:      $Id$
// Copyright:   (c) Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_GTK_SPINBUTT_H_
#define _WX_GTK_SPINBUTT_H_

//-----------------------------------------------------------------------------
// wxSpinButton
//-----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxSpinButton : public wxSpinButtonBase
{
public:
    wxSpinButton();
    wxSpinButton(wxWindow *parent,
                 wxWindowID id = -1,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = wxSP_VERTICAL,
                 const wxString& name = wxSPIN_BUTTON_NAME)
    {
        Create(parent, id, pos, size, style, name);
    }

    bool Create(wxWindow *parent,
                wxWindowID id = -1,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxSP_VERTICAL,
                const wxString& name = wxSPIN_BUTTON_NAME);

    virtual int GetValue() const;
    virtual void SetValue( int value );
    virtual void SetRange( int minVal, int maxVal );
    virtual int GetMin() const;
    virtual int GetMax() const;

    static wxVisualAttributes
    GetClassDefaultAttributes(wxWindowVariant variant = wxWINDOW_VARIANT_NORMAL);
    
    // implementation
    void OnSize( wxSizeEvent &event );

    int m_pos;

protected:
    virtual wxSize DoGetBestSize() const;
    virtual GdkWindow *GTKGetWindow(wxArrayGdkWindows& windows) const;

private:
    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(wxSpinButton)
};

#endif
    // _WX_GTK_SPINBUTT_H_
