/////////////////////////////////////////////////////////////////////////////
// Name:        wxedit.h
// Author:      Robert Roebling
// Created:     04/07/02
/////////////////////////////////////////////////////////////////////////////

#ifndef __wxedit_H__
#define __wxedit_H__

#ifdef __GNUG__
    #pragma interface "wxedit.cpp"
#endif

// Include wxWindows' headers

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

//----------------------------------------------------------------------------
//   constants
//----------------------------------------------------------------------------

#define ID_ABOUT    100

#define ID_NEW      200
#define ID_OPEN     201
#define ID_SAVE     202
#define ID_SAVEAS   203
#define ID_QUIT     204

//----------------------------------------------------------------------------
// MyFrame
//----------------------------------------------------------------------------

class MyFrame: public wxFrame
{
public:
    // constructors and destructors
    MyFrame( wxWindow *parent, wxWindowID id, const wxString &title,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_FRAME_STYLE );
    
private:
    void CreateMyMenuBar();
    
private:
    wxTextCtrl  *m_text;
    
private:
    void OnAbout( wxCommandEvent &event );
    void OnNew( wxCommandEvent &event );
    void OnOpen( wxCommandEvent &event );
    void OnSave( wxCommandEvent &event );
    void OnSaveAs( wxCommandEvent &event );
    void OnQuit( wxCommandEvent &event );
    void OnCloseWindow( wxCloseEvent &event );
    
private:
    DECLARE_EVENT_TABLE()
};

//----------------------------------------------------------------------------
// MyApp
//----------------------------------------------------------------------------

class MyApp: public wxApp
{
public:
    MyApp();
    
    virtual bool OnInit();
    virtual int OnExit();
};

#endif
