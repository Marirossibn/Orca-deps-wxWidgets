/*
 * Program: scroll
 *
 * Author: Robert Roebling
 *
 * Copyright: (C) 1998, Robert Roebling
 *
 */

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/image.h"
#include "wx/listctrl.h"

// derived classes

class MyFrame;
class MyApp;

// MyCanvas

class MyCanvas: public wxScrolledWindow
{
public:
    MyCanvas() {};
    MyCanvas( wxWindow *parent, wxWindowID, const wxPoint &pos, const wxSize &size );
    ~MyCanvas();
    void OnPaint( wxPaintEvent &event );

    DECLARE_DYNAMIC_CLASS(MyCanvas)
    DECLARE_EVENT_TABLE()
};

// MyFrame

class MyFrame: public wxFrame
{
public:
    MyFrame();

    void OnAbout( wxCommandEvent &event );
    void OnQuit( wxCommandEvent &event );

    MyCanvas         *m_canvas;

    DECLARE_DYNAMIC_CLASS(MyFrame)
    DECLARE_EVENT_TABLE()
};

// MyApp

class MyApp: public wxApp
{
public:
    virtual bool OnInit();
};

// main program

IMPLEMENT_APP(MyApp)

// MyCanvas

IMPLEMENT_DYNAMIC_CLASS(MyCanvas, wxScrolledWindow)

BEGIN_EVENT_TABLE(MyCanvas, wxScrolledWindow)
  EVT_PAINT(MyCanvas::OnPaint)
END_EVENT_TABLE()

MyCanvas::MyCanvas( wxWindow *parent, wxWindowID id,
                    const wxPoint &pos, const wxSize &size )
        : wxScrolledWindow( parent, id, pos, size, wxSUNKEN_BORDER | wxTAB_TRAVERSAL )
{
    wxString choices[] =
    {
        "This",
        "is one of my",
        "really",
        "wonderful",
        "examples."
    };
  
  (void) new wxButton( this, -1, "wxButton", wxPoint(10,10) );
  
  (void) new wxTextCtrl( this, -1, "wxTextCtrl", wxPoint(10,50) );
  
  (void) new wxRadioButton( this, -1, "Disable", wxPoint(10,90) );
  
  (void) new wxComboBox( this, -1, "This", wxPoint(10,130), wxDefaultSize, 5, choices );
  
  (void) new wxRadioBox( this, -1, "This", wxPoint(10,200), wxDefaultSize, 5, choices );

  wxListCtrl *m_listCtrl = new wxListCtrl(
	this, -1, wxPoint(200, 10), wxSize(180, 120),
	wxLC_REPORT | wxSUNKEN_BORDER | wxLC_SINGLE_SEL );

  m_listCtrl->InsertColumn(0, "First", wxLIST_FORMAT_LEFT, 90);
  m_listCtrl->InsertColumn(1, "Last", wxLIST_FORMAT_LEFT, 90);

  for ( int i=0; i < 30; i++)
  {
      char buf[20];
      sprintf(buf, "Item %d", i);
      m_listCtrl->InsertItem(i, buf);
  }
  m_listCtrl->SetItemState( 3, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED );

  (void) new wxListBox( this, -1, wxPoint(200,180), wxSize(180,120), 5, choices, wxLB_ALWAYS_SB );
  
  SetBackgroundColour( "WHEAT" );
}

MyCanvas::~MyCanvas()
{
}

void MyCanvas::OnPaint( wxPaintEvent &WXUNUSED(event) )
{
  wxPaintDC dc( this );
  PrepareDC( dc );

  dc.DrawText( "Some text", 110, 10 );
  
  dc.DrawRectangle( 50, 30, 200, 200 );
}

// MyFrame

const int ID_QUIT  = 108;
const int ID_ABOUT = 109;

IMPLEMENT_DYNAMIC_CLASS( MyFrame, wxFrame )

BEGIN_EVENT_TABLE(MyFrame,wxFrame)
  EVT_MENU    (ID_ABOUT, MyFrame::OnAbout)
  EVT_MENU    (ID_QUIT,  MyFrame::OnQuit)
END_EVENT_TABLE()

MyFrame::MyFrame()
       : wxFrame( (wxFrame *)NULL, -1, "wxScrolledWindow sample",
                  wxPoint(20,20), wxSize(470,360) )
{
  wxMenu *file_menu = new wxMenu();
  file_menu->Append( ID_ABOUT, "&About..");
  file_menu->Append( ID_QUIT, "E&xit\tAlt-X");

  wxMenuBar *menu_bar = new wxMenuBar();
  menu_bar->Append(file_menu, "&File");

  SetMenuBar( menu_bar );

  CreateStatusBar(2);
  int widths[] = { -1, 100 };
  SetStatusWidths( 2, widths );

  m_canvas = new MyCanvas( this, -1, wxPoint(0,0), wxSize(10,10) );
  m_canvas->SetScrollbars( 10, 10, 50, 100 );
}

void MyFrame::OnQuit( wxCommandEvent &WXUNUSED(event) )
{
  Close( TRUE );
}

void MyFrame::OnAbout( wxCommandEvent &WXUNUSED(event) )
{
  (void)wxMessageBox( "wxScroll demo\n"
                      "Robert Roebling (c) 1998",
                      "About wxScroll Demo", wxICON_INFORMATION | wxOK );
}

//-----------------------------------------------------------------------------
// MyApp
//-----------------------------------------------------------------------------

bool MyApp::OnInit()
{
  wxFrame *frame = new MyFrame();
  frame->Show( TRUE );

  return TRUE;
}

