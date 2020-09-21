///////////////////////////////////////////////////////////////////////////////
// Name:        wx/radiobut.h
// Purpose:     wxRadioButton declaration
// Author:      Vadim Zeitlin
// Modified by:
// Created:     07.09.00
// Copyright:   (c) Vadim Zeitlin
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_RADIOBUT_H_BASE_
#define _WX_RADIOBUT_H_BASE_

#include "wx/defs.h"

#if wxUSE_RADIOBTN

#include "wx/control.h"

class WXDLLIMPEXP_FWD_CORE wxRadioButton;

namespace wxPrivate
{
    WXDLLIMPEXP_CORE wxRadioButton* wxGetNextButtonInGroup(const wxRadioButton *btn);
    WXDLLIMPEXP_CORE wxRadioButton* wxGetPreviousButtonInGroup(const wxRadioButton *btn);
    WXDLLIMPEXP_CORE wxRadioButton* wxGetFirstButtonInGroup(const wxRadioButton *btn);
    WXDLLIMPEXP_CORE wxRadioButton* wxGetLastButtonInGroup(const wxRadioButton *btn);
} // namespace wxPrivate

// Unlike most of the other wxXXXBase classes, this one needs to be a template
// as wxRadioButton derives from different classes in different ports.
template <class W>
class wxRadioButtonBase : public W
{
public:
    typedef W BaseWindowClass;

    wxRadioButtonBase() { }

    // Methods to be implemented by the derived classes:
    virtual void SetValue(bool value) = 0;
    virtual bool GetValue() const = 0;


    // Methods implemented by this class itself.
    wxRadioButton* GetFirstInGroup() const
    {
        return wxPrivate::wxGetFirstButtonInGroup(static_cast<const wxRadioButton*>(this));
    }

    wxRadioButton* GetLastInGroup() const
    {
        return wxPrivate::wxGetLastButtonInGroup(static_cast<const wxRadioButton*>(this));
    }

    wxRadioButton* GetPreviousInGroup() const
    {
        return wxPrivate::wxGetPreviousButtonInGroup(static_cast<const wxRadioButton*>(this));
    }

    wxRadioButton* GetNextInGroup() const
    {
        return wxPrivate::wxGetNextButtonInGroup(static_cast<const wxRadioButton*>(this));
    }

private:
    wxDECLARE_NO_COPY_TEMPLATE_CLASS(wxRadioButtonBase, W);
};

extern WXDLLIMPEXP_DATA_CORE(const char) wxRadioButtonNameStr[];

#if defined(__WXUNIVERSAL__)
    #include "wx/univ/radiobut.h"
#elif defined(__WXMSW__)
    #include "wx/msw/radiobut.h"
#elif defined(__WXMOTIF__)
    #include "wx/motif/radiobut.h"
#elif defined(__WXGTK20__)
    #include "wx/gtk/radiobut.h"
#elif defined(__WXGTK__)
    #include "wx/gtk1/radiobut.h"
#elif defined(__WXMAC__)
    #include "wx/osx/radiobut.h"
#elif defined(__WXQT__)
    #include "wx/qt/radiobut.h"
#endif

#endif // wxUSE_RADIOBTN

#endif
    // _WX_RADIOBUT_H_BASE_
