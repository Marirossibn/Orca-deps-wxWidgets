/////////////////////////////////////////////////////////////////////////////
// Name:        No names yet.
// Purpose:     Contrib. demo
// Author:      Aleksandras Gluchovas
// Modified by: Sebastian Haase (June 21, 2001)
// Created:     04/11/98
// RCS-ID:      $Id$
// Copyright:   (c) Aleksandras Gluchovas
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////


#ifdef __GNUG__
#pragma implementation "fl_demo1.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/textctrl.h"

// fl headers
#include "wx/fl/controlbar.h"     // core API

// extra plugins
#include "wx/fl/barhintspl.h"    // beveal for bars with "X"s and grooves
#include "wx/fl/rowdragpl.h"     // NC-look with dragable rows
#include "wx/fl/cbcustom.h"      // customization plugin
#include "wx/fl/hintanimpl.h"

// beuty-care
#include "wx/fl/gcupdatesmgr.h"  // smooth d&d
#include "wx/fl/antiflickpl.h"   // double-buffered repaint of decorations
#include "wx/fl/dyntbar.h"       // auto-layouting toolbar
#include "wx/fl/dyntbarhnd.h"    // control-bar dimension handler for it

#include "fl_demo1.h"

// comment it out if it breaks, (this is my workaround for MSDev 4.0 linker)

char  wxDummyChar;


IMPLEMENT_APP	(MyApp)

bool MyApp::OnInit(void)
{
    MyFrame *frame = new MyFrame(NULL);
    
    frame->SetBackgroundColour( wxColour(192,192,192) );
    
    wxMenu *file_menu = new wxMenu;
    
    file_menu->Append( NEW_TEST_LOAD, "&Load layouts"  );
    file_menu->Append( NEW_TEST_SAVE, "&Store layouts" );
    file_menu->Append( NEW_TEST_EXIT, "E&xit" );
    
    wxMenuBar *menu_bar = new wxMenuBar;
    
    menu_bar->Append(file_menu,   "&File");
    
    frame->SetMenuBar(menu_bar);
    
    frame->CreateStatusBar(3);
    
    frame->Show(TRUE);
    
    frame->mpClientWnd->Refresh();
    
    SetTopWindow(frame);
    
    
    wxMessageBox("Hello, this demo has a bunch of yet-not-fixed-bugs and missing functionality\n" \
                 "The ONLY purpose is to demonstrate self-layouting toolbars,\nflat-bitmapped-buttons and 2-new FL-plugins" \
                 "(cbRowDragPlugin & cbBarHintsPlugin)\n\n" \
                 "BTW, disabled images and label-text are rendered at run-time" );
    
    return TRUE;
}

/***** Implementation for class MyFrame *****/

BEGIN_EVENT_TABLE( MyFrame, wxFrame )
    // EVT_CHAR_HOOK(MyFrame::OnKeyDown)
    // EVT_PAINT( MyFrame::OnPaint )
    EVT_MENU( NEW_TEST_SAVE, MyFrame::OnSave )
    EVT_MENU( NEW_TEST_LOAD, MyFrame::OnLoad )
    EVT_MENU( NEW_TEST_EXIT, MyFrame::OnExit )
END_EVENT_TABLE()

void MyFrame::OnLoad( wxCommandEvent& event )
{						
    wxMessageBox("Hey - you found a BIG question-mark !!");
}

void MyFrame::OnSave( wxCommandEvent& event )
{
    wxMessageBox("Hey - you found another BIG question-mark !!");
}

void MyFrame::OnExit( wxCommandEvent& event )
{
    Destroy();
}

wxTextCtrl* MyFrame::CreateTextCtrl( const wxString& value )
{
    wxTextCtrl* pCtrl = 
        
        new wxTextCtrl( this, -1, value, 
        wxDefaultPosition, wxSize(0,0), wxTE_MULTILINE );
    
    pCtrl->SetBackgroundColour( wxColour( 255,255,255 ) );
    
    return pCtrl;
}

MyFrame::MyFrame(wxFrame *frame)
    : wxFrame( frame, -1, "wxWindows 2.0 wxFrameLayout Test Application", wxDefaultPosition, 
          wxSize( 700, 500 ), 
          wxCLIP_CHILDREN | wxMINIMIZE_BOX | wxMAXIMIZE_BOX | 
          wxTHICK_FRAME   | wxSYSTEM_MENU  | wxCAPTION, 
          "freimas" )
{
    mpClientWnd = CreateTextCtrl( "Client window" );
    
    mpLayout = new wxFrameLayout( this, mpClientWnd );
    
#ifdef __WXGTK__
    cbCommonPaneProperties props;
    mpLayout->GetPaneProperties( props );
    
    props.mRealTimeUpdatesOn = FALSE; // real-time OFF!!!
    
    mpLayout->SetPaneProperties( props, wxALL_PANES );
#endif
    
    mpLayout->SetUpdatesManager( new cbGCUpdatesMgr() );
    
    // this is now default...
    //mpLayout->SetMargins( 1,1,1,1 ); // gaps for vertical/horizontal/right/left panes
    
    // setup plugins for testing
    mpLayout->PushDefaultPlugins();
    
    mpLayout->AddPlugin( CLASSINFO( cbBarHintsPlugin ) ); // facny "X"es and beveal for bars
    mpLayout->AddPlugin( CLASSINFO( cbHintAnimationPlugin ) );
    mpLayout->AddPlugin( CLASSINFO( cbRowDragPlugin  ) );
    mpLayout->AddPlugin( CLASSINFO( cbAntiflickerPlugin ) );
    mpLayout->AddPlugin( CLASSINFO( cbSimpleCustomizationPlugin ) );
    
    // drop in some bars
    cbDimInfo sizes0( 200,45, // when docked horizontally      
                      200,85, // when docked vertically        
                      175,35, // when floated                  
                      FALSE,  // the bar is not fixed-size
                      4,      // vertical gap (bar border)
                      4       // horizontal gap (bar border)
                    ); 
    
    cbDimInfo sizes1( 150,35, // when docked horizontally      
                      150,85, // when docked vertically        
                      175,35, // when floated                  
                      TRUE,   // the bar is not fixed-size
                      4,      // vertical gap (bar border)
                      4       // horizontal gap (bar border)
                    ); 
    
    cbDimInfo sizes2( 175,45, // when docked horizontally      
                      175,37, // when docked vertically        
                      170,35, // when floated                  
                      TRUE,   // the bar is not fixed-size
                      4,      // vertical gap (bar border)
                      4,      // horizontal gap (bar border)
                      new cbDynToolBarDimHandler()
                    ); 
    
    mpLayout->AddBar( CreateTextCtrl("Hello"),  // bar window
                      sizes0, FL_ALIGN_TOP,     // alignment ( 0-top,1-bottom, etc)
                      0,                        // insert into 0th row (vert. position)
                      0,                        // offset from the start of row (in pixels)
                      "InfoViewer1",            // name to refere in customization pop-ups
                      TRUE
                    );
    
    mpLayout->AddBar( CreateTextCtrl("Bye"),    // bar window
                      sizes0, FL_ALIGN_TOP,     // alignment ( 0-top,1-bottom, etc)
                      1,                        // insert into 0th row (vert. position)
                      0,                        // offset from the start of row (in pixels)
                      "InfoViewer2",            // name to refere in customization pop-ups
                      TRUE
                    );
    
    mpLayout->AddBar( CreateTextCtrl("Fixed0"), // bar window
                      sizes1, FL_ALIGN_TOP,     // alignment ( 0-top,1-bottom, etc)
                      0,                        // insert into 0th row (vert. position)
                      0,                        // offset from the start of row (in pixels)
                      "ToolBar1",               // name to refer in customization pop-ups
                      TRUE
                    );
    
    wxDynamicToolBar* pToolBar = new wxDynamicToolBar();
    
    pToolBar->Create( this, -1 );
    
    // 1001-1006 ids of command events fired by added tool-buttons
    
    pToolBar->AddTool( 1001, BMP_DIR "new.bmp" );
    pToolBar->AddTool( 1002, BMP_DIR "open.bmp" );
    pToolBar->AddTool( 1003, BMP_DIR "save.bmp" );
    
    pToolBar->AddTool( 1004, BMP_DIR "cut.bmp" );
    pToolBar->AddTool( 1005, BMP_DIR "copy.bmp" );
    pToolBar->AddTool( 1006, BMP_DIR "paste.bmp" );
    
    
    mpLayout->AddBar( pToolBar,             // bar window (can be NULL)
                      sizes2, FL_ALIGN_TOP, // alignment ( 0-top,1-bottom, etc)
                      0,                    // insert into 0th row (vert. position)
                      0,                    // offset from the start of row (in pixels)
                      "ToolBar2",           // name to refere in customization pop-ups
                      FALSE
                    );
    
    mpLayout->EnableFloating( TRUE ); // off, thinking bout wxGtk...
}

MyFrame::~MyFrame()
{
    if ( mpLayout) 
        delete mpLayout; // should be destroyed manually
}

