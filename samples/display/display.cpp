/////////////////////////////////////////////////////////////////////////////
// Name:        display.cpp
// Purpose:     wxWidgets sample showing the features of wxDisplay class
// Author:      Vadim Zeitlin
// Modified by: Ryan Norton & Brian Victor
// Created:     23.02.03
// RCS-ID:      $Id$
// Copyright:   (c) Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// for compilers that support precompilation, includes "wx/wx.h"
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers explicitly
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#if !wxUSE_DISPLAY
    #error "To compile this sample you must build the library with wxUSE_DISPLAY set to 1"
#endif

#include "wx/notebook.h"

#include "wx/display.h"


// the application icon (under Windows and OS/2 it is in resources)
#if defined(__WXGTK__) || defined(__WXMOTIF__) || defined(__WXMAC__) || defined(__WXMGL__) || defined(__WXX11__)
    #include "../sample.xpm"
#endif

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

// Define a new application type, each program should derive a class from wxApp
class MyApp : public wxApp
{
public:
    // override base class virtuals
    // ----------------------------

    // this one is called on application startup and is a good place for the app
    // initialization (doing it here and not in the ctor allows to have an error
    // return: if OnInit() returns false, the application terminates)
    virtual bool OnInit();
};

// Define a new frame type: this is going to be our main frame
class MyFrame : public wxFrame
{
public:
    // ctor(s)
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size,
            long style = wxDEFAULT_FRAME_STYLE);

    // event handlers (these functions should _not_ be virtual)
    void OnQuit(wxCommandEvent& event);
    void OnFromPoint(wxCommandEvent& event);
    void OnFullScreen(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    void OnChangeMode(wxCommandEvent& event);
    void OnResetMode(wxCommandEvent& event);

    void OnLeftClick(wxMouseEvent& event);

    void OnDisplayChanged(wxDisplayChangedEvent& event);

private:
    // convert video mode to textual description
    wxString VideoModeToText(const wxVideoMode& mode);

    // GUI controls
    wxNotebook *m_notebook;

    // any class wishing to process wxWidgets events must use this macro
    DECLARE_EVENT_TABLE()
};

// Client data class for the choice control containing the video modes
class MyVideoModeClientData : public wxClientData
{
public:
    MyVideoModeClientData(const wxVideoMode& m) : mode(m) { }

    const wxVideoMode mode;
};

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
    // menu items
    Display_Quit = 1,

    Display_FromPoint,
    Display_FullScreen,

    // controls
    Display_ChangeMode = 1000,
    Display_ResetMode,
    Display_CurrentMode,


    // it is important for the id corresponding to the "About" command to have
    // this standard value as otherwise it won't be handled properly under Mac
    // (where it is special and put into the "Apple" menu)
    Display_About = wxID_ABOUT
};

// ----------------------------------------------------------------------------
// event tables and other macros for wxWidgets
// ----------------------------------------------------------------------------

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(Display_Quit,  MyFrame::OnQuit)
    EVT_MENU(Display_FromPoint,  MyFrame::OnFromPoint)
    EVT_MENU(Display_FullScreen, MyFrame::OnFullScreen)
    EVT_MENU(Display_About, MyFrame::OnAbout)

    EVT_CHOICE(Display_ChangeMode, MyFrame::OnChangeMode)
    EVT_BUTTON(Display_ResetMode, MyFrame::OnResetMode)

    EVT_LEFT_UP(MyFrame::OnLeftClick)


    EVT_DISPLAY_CHANGED(MyFrame::OnDisplayChanged)
END_EVENT_TABLE()

// Create a new application object: this macro will allow wxWidgets to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also declares the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)
IMPLEMENT_APP(MyApp)

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
#ifdef __WXMSW__
    if ( argc == 2 && !wxStricmp(argv[1],  _T("/dx")) )
    {
        wxDisplay::UseDirectX(true);
    }
#endif // __WXMSW__

    // create the main application window
    MyFrame *frame = new MyFrame(_("Display wxWidgets Sample"),
                                 wxDefaultPosition, wxDefaultSize);

    // and show it (the frames, unlike simple controls, are not shown when
    // created initially)
    frame->Show();

    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned false here, the
    // application would exit immediately.
    return true;
}

// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style)
       : wxFrame(NULL, wxID_ANY, title, pos, size, style)
{
    // set the frame icon
    SetIcon(wxICON(sample));

#if wxUSE_MENUS
    // create a menu bar
    wxMenu *menuDisplay = new wxMenu;
    menuDisplay->Append(Display_FromPoint, _("Find from &point..."));
    menuDisplay->AppendSeparator();
    menuDisplay->AppendCheckItem(Display_FullScreen, _("Full &screen\tF12"));
    menuDisplay->AppendSeparator();
    menuDisplay->Append(Display_Quit, _("E&xit\tAlt-X"), _("Quit this program"));

    // the "About" item should be in the help menu
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(Display_About, _("&About...\tF1"), _("Show about dialog"));

    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(menuDisplay, _("&Display"));
    menuBar->Append(helpMenu, _("&Help"));

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);
#endif // wxUSE_MENUS

    // create status bar
    CreateStatusBar();

    // create child controls

    wxPanel *panel = new wxPanel(this, wxID_ANY);

    m_notebook = new wxNotebook(panel, wxID_ANY);
    const size_t count = wxDisplay::GetCount();
    for ( size_t nDpy = 0; nDpy < count; nDpy++ )
    {
        wxDisplay display(nDpy);

        wxWindow *page = new wxPanel(m_notebook, wxID_ANY);

        // create 2 column flex grid sizer with growable 2nd column
        wxFlexGridSizer *sizer = new wxFlexGridSizer(2, 10, 20);
        sizer->AddGrowableCol(1);

        const wxRect r(display.GetGeometry());
        sizer->Add(new wxStaticText(page, wxID_ANY, _T("Origin: ")));
        sizer->Add(new wxStaticText
                       (
                        page,
                        wxID_ANY,
                        wxString::Format(_T("(%d, %d)"),
                                         r.x, r.y)
                       ));

        sizer->Add(new wxStaticText(page, wxID_ANY, _T("Size: ")));
        sizer->Add(new wxStaticText
                       (
                        page,
                        wxID_ANY,
                        wxString::Format(_T("(%d, %d)"),
                                         r.width, r.height)
                       ));


        sizer->Add(new wxStaticText(page, wxID_ANY, _T("Name: ")));
        sizer->Add(new wxStaticText(page, wxID_ANY, display.GetName()));

        wxChoice *choiceModes = new wxChoice(page, Display_ChangeMode);
        const wxArrayVideoModes modes = display.GetModes();
        const size_t count = modes.GetCount();
        for ( size_t nMode = 0; nMode < count; nMode++ )
        {
            const wxVideoMode& mode = modes[nMode];

            choiceModes->Append(VideoModeToText(mode),
                                new MyVideoModeClientData(mode));
        }

        sizer->Add(new wxStaticText(page, wxID_ANY, _T("&Modes: ")));
        sizer->Add(choiceModes, 0, wxEXPAND);

        sizer->Add(new wxStaticText(page, wxID_ANY, _T("Current: ")));
        sizer->Add(new wxStaticText(page, Display_CurrentMode,
                                    VideoModeToText(display.GetCurrentMode())));

        // add it to another sizer to have borders around it and button below
        wxSizer *sizerTop = new wxBoxSizer(wxVERTICAL);
        sizerTop->Add(sizer, 1, wxALL | wxEXPAND, 10);

        sizerTop->Add(new wxButton(page, Display_ResetMode, _T("&Reset mode")),
                      0, wxALL | wxCENTRE, 5);
        page->SetSizer(sizerTop);

        m_notebook->AddPage(page,
                            wxString::Format(_T("Display %lu"),
                                             (unsigned long)nDpy));
    }

    wxNotebookSizer *notebookSizer = new wxNotebookSizer(m_notebook);
    panel->SetSizer(notebookSizer);
    notebookSizer->Fit(this);
    notebookSizer->SetSizeHints(this);
}

wxString MyFrame::VideoModeToText(const wxVideoMode& mode)
{
    wxString s;
    s.Printf(_T("%dx%d"), mode.w, mode.h);

    if ( mode.bpp )
    {
        s += wxString::Format(_T(", %dbpp"), mode.bpp);
    }

    if ( mode.refresh )
    {
        s += wxString::Format(_T(", %dHz"), mode.refresh);
    }

    return s;
}

// event handlers

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // true is to force the frame to close
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(_T("Demo program for wxDisplay class.\n\n(c) 2003 Vadim Zeitlin"),
                 _T("About Display Sample"),
                 wxOK | wxICON_INFORMATION,
                 this);
}

void MyFrame::OnFromPoint(wxCommandEvent& WXUNUSED(event))
{
    SetStatusText(_T("Press the mouse anywhere..."));

    CaptureMouse();
}

void MyFrame::OnFullScreen(wxCommandEvent& event)
{
    ShowFullScreen(event.IsChecked());
}

void MyFrame::OnChangeMode(wxCommandEvent& event)
{
    wxDisplay dpy(m_notebook->GetSelection());

    // you wouldn't write this in real code, would you?
    if ( !dpy.ChangeMode(((MyVideoModeClientData *)
                wxDynamicCast(event.GetEventObject(), wxChoice)->
                    GetClientObject(event.GetInt()))->mode) )
    {
        wxLogError(_T("Changing video mode failed!"));
    }
}

void MyFrame::OnResetMode(wxCommandEvent& WXUNUSED(event))
{
    wxDisplay dpy(m_notebook->GetSelection());

    dpy.ResetMode();
}

void MyFrame::OnLeftClick(wxMouseEvent& event)
{
    if ( HasCapture() )
    {
        // mouse events are in client coords, wxDisplay works in screen ones
        const wxPoint ptScreen = ClientToScreen(event.GetPosition());
        int dpy = wxDisplay::GetFromPoint(ptScreen);
        if ( dpy == wxNOT_FOUND )
        {
            wxLogError(_T("Mouse clicked outside of display!?"));
        }

        wxLogStatus(this, _T("Mouse clicked in display %d (at (%d, %d))"),
                    dpy, ptScreen.x, ptScreen.y);

        ReleaseMouse();
    }
}

void MyFrame::OnDisplayChanged(wxDisplayChangedEvent& event)
{
    // update the current mode text
    for ( size_t n = 0; n < m_notebook->GetPageCount(); n++ )
    {
        wxStaticText *label = wxDynamicCast(m_notebook->GetPage(n)->
                                                FindWindow(Display_CurrentMode),
                                            wxStaticText);
        if ( label )
            label->SetLabel(VideoModeToText(wxDisplay(n).GetCurrentMode()));
    }


    wxLogStatus(this, _T("Display resolution was changed."));

    event.Skip();
}

