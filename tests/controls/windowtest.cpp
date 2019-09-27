///////////////////////////////////////////////////////////////////////////////
// Name:        tests/controls/windowtest.cpp
// Purpose:     wxWindow unit test
// Author:      Steven Lamerton
// Created:     2010-07-10
// Copyright:   (c) 2010 Steven Lamerton
///////////////////////////////////////////////////////////////////////////////

#include "testprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/app.h"
    #include "wx/window.h"
    #include "wx/button.h"
    #include "wx/sizer.h"
#endif // WX_PRECOMP

#include "asserthelper.h"
#include "testableframe.h"
#include "testwindow.h"

#include "wx/uiaction.h"
#include "wx/caret.h"
#include "wx/cshelp.h"
#include "wx/tooltip.h"

class WindowTestCase
{
public:
    WindowTestCase()
        : m_window(new wxWindow(wxTheApp->GetTopWindow(), wxID_ANY))
    {
    }

    ~WindowTestCase()
    {
        wxTheApp->GetTopWindow()->DestroyChildren();
    }

protected:
    wxWindow* const m_window;

    wxDECLARE_NO_COPY_CLASS(WindowTestCase);
};

TEST_CASE_METHOD(WindowTestCase, "Window::ShowHideEvent", "[window]")
{
#if defined(__WXMSW__)
    EventCounter show(m_window, wxEVT_SHOW);

    CPPUNIT_ASSERT(m_window->IsShown());

    m_window->Show(false);

    CPPUNIT_ASSERT(!m_window->IsShown());

    m_window->Show();

    CPPUNIT_ASSERT(m_window->IsShown());

    CPPUNIT_ASSERT_EQUAL(2, show.GetCount());
#endif // __WXMSW__
}

TEST_CASE_METHOD(WindowTestCase, "Window::KeyEvent", "[window]")
{
#if wxUSE_UIACTIONSIMULATOR
    if ( !EnableUITests() )
        return;

    EventCounter keydown(m_window, wxEVT_KEY_DOWN);
    EventCounter keyup(m_window, wxEVT_KEY_UP);
    EventCounter keychar(m_window, wxEVT_CHAR);

    wxUIActionSimulator sim;

    m_window->SetFocus();
    wxYield();

    sim.Text("text");
    sim.Char(WXK_SHIFT);
    wxYield();

    CPPUNIT_ASSERT_EQUAL(5, keydown.GetCount());
    CPPUNIT_ASSERT_EQUAL(5, keyup.GetCount());
    CPPUNIT_ASSERT_EQUAL(4, keychar.GetCount());
#endif
}

TEST_CASE_METHOD(WindowTestCase, "Window::FocusEvent", "[window]")
{
#ifndef __WXOSX__
    if ( IsAutomaticTest() )
    {
        // Skip this test when running under buildbot, it fails there for
        // unknown reason and this failure can't be reproduced locally.
        return;
    }

    EventCounter setfocus(m_window, wxEVT_SET_FOCUS);
    EventCounter killfocus(m_window, wxEVT_KILL_FOCUS);

    m_window->SetFocus();

    CPPUNIT_ASSERT(setfocus.WaitEvent(500));
    CHECK_FOCUS_IS( m_window );

    wxButton* button = new wxButton(wxTheApp->GetTopWindow(), wxID_ANY);

    wxYield();
    button->SetFocus();

    CPPUNIT_ASSERT_EQUAL(1, killfocus.GetCount());
    CPPUNIT_ASSERT(!m_window->HasFocus());
#endif
}

TEST_CASE_METHOD(WindowTestCase, "Window::Mouse", "[window]")
{
    wxCursor cursor(wxCURSOR_CHAR);
    m_window->SetCursor(cursor);

    CPPUNIT_ASSERT(m_window->GetCursor().IsOk());

    //A plain window doesn't have a caret
    CPPUNIT_ASSERT(!m_window->GetCaret());

    wxCaret* caret = new wxCaret(m_window, 16, 16);
    m_window->SetCaret(caret);

    CPPUNIT_ASSERT(m_window->GetCaret()->IsOk());

    m_window->CaptureMouse();

    CPPUNIT_ASSERT(m_window->HasCapture());

    m_window->ReleaseMouse();

    CPPUNIT_ASSERT(!m_window->HasCapture());
}

TEST_CASE_METHOD(WindowTestCase, "Window::Properties", "[window]")
{
    m_window->SetLabel("label");

    CPPUNIT_ASSERT_EQUAL("label", m_window->GetLabel());

    m_window->SetName("name");

    CPPUNIT_ASSERT_EQUAL("name", m_window->GetName());

    //As we used wxID_ANY we should have a negative id
    CPPUNIT_ASSERT(m_window->GetId() < 0);

    m_window->SetId(wxID_HIGHEST + 10);

    CPPUNIT_ASSERT_EQUAL(wxID_HIGHEST + 10, m_window->GetId());
}

#if wxUSE_TOOLTIPS
TEST_CASE_METHOD(WindowTestCase, "Window::ToolTip", "[window]")
{
    CPPUNIT_ASSERT(!m_window->GetToolTip());
    CPPUNIT_ASSERT_EQUAL("", m_window->GetToolTipText());

    m_window->SetToolTip("text tip");

    CPPUNIT_ASSERT_EQUAL("text tip", m_window->GetToolTipText());

    m_window->UnsetToolTip();

    CPPUNIT_ASSERT(!m_window->GetToolTip());
    CPPUNIT_ASSERT_EQUAL("", m_window->GetToolTipText());

    wxToolTip* tip = new wxToolTip("other tip");

    m_window->SetToolTip(tip);

    CPPUNIT_ASSERT_EQUAL(tip, m_window->GetToolTip());
    CPPUNIT_ASSERT_EQUAL("other tip", m_window->GetToolTipText());
}
#endif // wxUSE_TOOLTIPS

TEST_CASE_METHOD(WindowTestCase, "Window::Help", "[window]")
{
    wxHelpProvider::Set(new wxSimpleHelpProvider());

    CPPUNIT_ASSERT_EQUAL("", m_window->GetHelpText());

    m_window->SetHelpText("helptext");

    CPPUNIT_ASSERT_EQUAL("helptext", m_window->GetHelpText());
}

TEST_CASE_METHOD(WindowTestCase, "Window::Parent", "[window]")
{
    CPPUNIT_ASSERT_EQUAL(static_cast<wxWindow*>(NULL), m_window->GetGrandParent());
    CPPUNIT_ASSERT_EQUAL(wxTheApp->GetTopWindow(), m_window->GetParent());
}

TEST_CASE_METHOD(WindowTestCase, "Window::Siblings", "[window]")
{
    CPPUNIT_ASSERT_EQUAL(static_cast<wxWindow*>(NULL), m_window->GetNextSibling());
    CPPUNIT_ASSERT_EQUAL(static_cast<wxWindow*>(NULL), m_window->GetPrevSibling());

    wxWindow* newwin = new wxWindow(wxTheApp->GetTopWindow(), wxID_ANY);

    CPPUNIT_ASSERT_EQUAL(newwin, m_window->GetNextSibling());
    CPPUNIT_ASSERT_EQUAL(static_cast<wxWindow*>(NULL), m_window->GetPrevSibling());

    CPPUNIT_ASSERT_EQUAL(static_cast<wxWindow*>(NULL), newwin->GetNextSibling());
    CPPUNIT_ASSERT_EQUAL(m_window, newwin->GetPrevSibling());

    wxDELETE(newwin);
}

TEST_CASE_METHOD(WindowTestCase, "Window::Children", "[window]")
{
    CPPUNIT_ASSERT_EQUAL(0, m_window->GetChildren().GetCount());

    wxWindow* child1 = new wxWindow(m_window, wxID_ANY);

    CPPUNIT_ASSERT_EQUAL(1, m_window->GetChildren().GetCount());

    m_window->RemoveChild(child1);

    CPPUNIT_ASSERT_EQUAL(0, m_window->GetChildren().GetCount());

    child1->SetId(wxID_HIGHEST + 1);
    child1->SetName("child1");

    m_window->AddChild(child1);

    CPPUNIT_ASSERT_EQUAL(1, m_window->GetChildren().GetCount());
    CPPUNIT_ASSERT_EQUAL(child1, m_window->FindWindow(wxID_HIGHEST + 1));
    CPPUNIT_ASSERT_EQUAL(child1, m_window->FindWindow("child1"));

    m_window->DestroyChildren();

    CPPUNIT_ASSERT_EQUAL(0, m_window->GetChildren().GetCount());
}

TEST_CASE_METHOD(WindowTestCase, "Window::Focus", "[window]")
{
#ifndef __WXOSX__
    CPPUNIT_ASSERT(!m_window->HasFocus());

    if ( m_window->AcceptsFocus() )
    {
        m_window->SetFocus();
        CHECK_FOCUS_IS(m_window);
    }

    //Set the focus back to the main window
    wxTheApp->GetTopWindow()->SetFocus();

    if ( m_window->AcceptsFocusFromKeyboard() )
    {
        m_window->SetFocusFromKbd();
        CHECK_FOCUS_IS(m_window);
    }
#endif
}

TEST_CASE_METHOD(WindowTestCase, "Window::Positioning", "[window]")
{
    //Some basic tests for consistency
    int x, y;
    m_window->GetPosition(&x, &y);

    CPPUNIT_ASSERT_EQUAL(x, m_window->GetPosition().x);
    CPPUNIT_ASSERT_EQUAL(y, m_window->GetPosition().y);
    CPPUNIT_ASSERT_EQUAL(m_window->GetPosition(),
                         m_window->GetRect().GetTopLeft());

    m_window->GetScreenPosition(&x, &y);
    CPPUNIT_ASSERT_EQUAL(x, m_window->GetScreenPosition().x);
    CPPUNIT_ASSERT_EQUAL(y, m_window->GetScreenPosition().y);
    CPPUNIT_ASSERT_EQUAL(m_window->GetScreenPosition(),
                         m_window->GetScreenRect().GetTopLeft());
}

TEST_CASE_METHOD(WindowTestCase, "Window::PositioningBeyondShortLimit", "[window]")
{
#ifdef __WXMSW__
    //Positioning under MSW is limited to short relative coordinates

    //
    //Test window creation beyond SHRT_MAX
    int commonDim = 10;
    wxWindow* w = new wxWindow(m_window, wxID_ANY,
                               wxPoint(0, SHRT_MAX + commonDim),
                               wxSize(commonDim, commonDim));
    CPPUNIT_ASSERT_EQUAL(SHRT_MAX + commonDim, w->GetPosition().y);

    w->Move(0, 0);

    //
    //Test window moving beyond SHRT_MAX
    w->Move(0, SHRT_MAX + commonDim);
    CPPUNIT_ASSERT_EQUAL(SHRT_MAX + commonDim, w->GetPosition().y);

    //
    //Test window moving below SHRT_MIN
    w->Move(0, SHRT_MIN - commonDim);
    CPPUNIT_ASSERT_EQUAL(SHRT_MIN - commonDim, w->GetPosition().y);

    //
    //Test deferred move beyond SHRT_MAX
    m_window->SetVirtualSize(-1, SHRT_MAX + 2 * commonDim);
    wxWindow* bigWin = new wxWindow(m_window, wxID_ANY, wxDefaultPosition,
                                    //size is also limited by SHRT_MAX
                                    wxSize(commonDim, SHRT_MAX));
    wxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(bigWin);
    sizer->AddSpacer(commonDim); //add some space to go beyond SHRT_MAX
    sizer->Add(w);
    m_window->SetSizer(sizer);
    m_window->Layout();
    CPPUNIT_ASSERT_EQUAL(SHRT_MAX + commonDim, w->GetPosition().y);
#endif
}

TEST_CASE_METHOD(WindowTestCase, "Window::Show", "[window]")
{
    CPPUNIT_ASSERT(m_window->IsShown());

    m_window->Hide();

    CPPUNIT_ASSERT(!m_window->IsShown());

    m_window->Show();

    CPPUNIT_ASSERT(m_window->IsShown());

    m_window->Show(false);

    CPPUNIT_ASSERT(!m_window->IsShown());

    m_window->ShowWithEffect(wxSHOW_EFFECT_BLEND);

    CPPUNIT_ASSERT(m_window->IsShown());

    m_window->HideWithEffect(wxSHOW_EFFECT_BLEND);

    CPPUNIT_ASSERT(!m_window->IsShown());
}

TEST_CASE_METHOD(WindowTestCase, "Window::Enable", "[window]")
{
    CPPUNIT_ASSERT(m_window->IsEnabled());

    m_window->Disable();

    CPPUNIT_ASSERT(!m_window->IsEnabled());

    m_window->Enable();

    CPPUNIT_ASSERT(m_window->IsEnabled());

    m_window->Enable(false);

    CPPUNIT_ASSERT(!m_window->IsEnabled());
}

TEST_CASE_METHOD(WindowTestCase, "Window::FindWindowBy", "[window]")
{
    m_window->SetId(wxID_HIGHEST + 1);
    m_window->SetName("name");
    m_window->SetLabel("label");

    CPPUNIT_ASSERT_EQUAL(m_window, wxWindow::FindWindowById(wxID_HIGHEST + 1));
    CPPUNIT_ASSERT_EQUAL(m_window, wxWindow::FindWindowByName("name"));
    CPPUNIT_ASSERT_EQUAL(m_window, wxWindow::FindWindowByLabel("label"));

    CPPUNIT_ASSERT_EQUAL(static_cast<wxWindow*>(NULL),
                         wxWindow::FindWindowById(wxID_HIGHEST + 3));
    CPPUNIT_ASSERT_EQUAL(static_cast<wxWindow*>(NULL),
                         wxWindow::FindWindowByName("noname"));
    CPPUNIT_ASSERT_EQUAL(static_cast<wxWindow*>(NULL),
                         wxWindow::FindWindowByLabel("nolabel"));
}
