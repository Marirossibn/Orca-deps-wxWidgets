/////////////////////////////////////////////////////////////////////////////
// Program:     wxWidgets Widgets Sample
// Name:        statbmp.cpp
// Purpose:     Part of the widgets sample showing wxStaticBitmap
// Author:      Marcin Wojdyr
// Created:     2008-06-19
// Id:          $Id$
// Copyright:   (c) 2008 Marcin Wojdyr
// License:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// for compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers
#ifndef WX_PRECOMP
    #include "wx/log.h"

    #include "wx/button.h"
    #include "wx/radiobox.h"
    #include "wx/statbmp.h"
    #include "wx/statbox.h"
    #include "wx/textctrl.h"
#endif

#include "wx/generic/statbmpg.h"
#include "wx/sizer.h"
#include "wx/filepicker.h"

#include "widgets.h"
#include "icons/statbmp.xpm"


class StatBmpWidgetsPage : public WidgetsPage
{
public:
    StatBmpWidgetsPage(WidgetsBookCtrl *book, wxImageList *imaglist)
        : WidgetsPage(book, imaglist, statbmp_xpm) {}

    virtual void CreateContent();
    virtual wxControl *GetWidget() const { return m_statbmp; }
    virtual void RecreateWidget();

private:
    void OnFileChange(wxFileDirPickerEvent &WXUNUSED(ev)) { RecreateWidget(); }
    void OnRadioChange(wxCommandEvent &WXUNUSED(ev)) { RecreateWidget(); }

    void OnMouseEvent(wxMouseEvent& WXUNUSED(event))
    {
        wxLogMessage(wxT("wxStaticBitmap clicked."));
    }

    wxStaticBitmapBase *m_statbmp;
    wxFilePickerCtrl *m_filepicker;
    wxRadioBox *m_radio;
    wxStaticBoxSizer *m_sbsizer;

    DECLARE_WIDGETS_PAGE(StatBmpWidgetsPage)
};

IMPLEMENT_WIDGETS_PAGE(StatBmpWidgetsPage, wxT("StaticBitmap"),
                       ALL_CTRLS);

void StatBmpWidgetsPage::CreateContent()
{

    static const wxString choices[] = { "native", "generic" };
    m_radio = new wxRadioBox(this, wxID_ANY, "implementation",
                             wxDefaultPosition, wxDefaultSize,
                             WXSIZEOF(choices), choices);

    m_filepicker = new wxFilePickerCtrl(this, wxID_ANY, "../image/toucan.png");

    m_sbsizer = new wxStaticBoxSizer(wxVERTICAL, this, "wxStaticBitmap inside");

    wxSizer *leftsizer = new wxBoxSizer(wxVERTICAL);
    leftsizer->Add(m_radio, wxSizerFlags().Expand().Border());
    leftsizer->Add(m_filepicker, wxSizerFlags().Expand().Border());
    wxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(leftsizer, wxSizerFlags().Border());
    sizer->Add(m_sbsizer, wxSizerFlags().Center());
    SetSizer(sizer);

    wxInitAllImageHandlers();

    Connect(wxEVT_COMMAND_FILEPICKER_CHANGED, 
            wxFileDirPickerEventHandler(StatBmpWidgetsPage::OnFileChange));
    Connect(wxEVT_COMMAND_RADIOBOX_SELECTED, 
            wxCommandEventHandler(StatBmpWidgetsPage::OnRadioChange));

    m_statbmp = NULL;
    RecreateWidget();
}

void StatBmpWidgetsPage::RecreateWidget()
{
    delete m_statbmp;
    wxString filepath = m_filepicker->GetPath();
    wxImage image(filepath);
    if (! image.Ok() )
    {
        wxLogMessage("Reading image from file '%s' failed.", filepath.c_str());
        m_statbmp = NULL;
        return;
    }
    if (m_radio->GetSelection() == 0)
        m_statbmp = new wxStaticBitmap(this, wxID_ANY, wxBitmap(image));
    else
        m_statbmp = new wxGenericStaticBitmap(this, wxID_ANY, wxBitmap(image));
    m_sbsizer->Add(m_statbmp, wxSizerFlags(1).Expand());
    GetSizer()->Layout();
    m_statbmp->Connect(wxEVT_LEFT_DOWN, 
                       wxMouseEventHandler(StatBmpWidgetsPage::OnMouseEvent),
                       NULL, this);
    // When switching from generic to native control on wxMSW under Wine, 
    // the explicit Refresh() is necessary
    m_statbmp->Refresh(); 
}

