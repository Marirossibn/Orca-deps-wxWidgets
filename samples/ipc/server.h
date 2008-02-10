/////////////////////////////////////////////////////////////////////////////
// Name:        server.h
// Purpose:     DDE sample: server
// Author:      Julian Smart
// Modified by:
// Created:     25/01/99
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "connection.h"

#define ID_START         10000
#define ID_DISCONNECT    10001
#define ID_ADVISE         10002
#define ID_LOG          10003
#define ID_SERVERNAME    10004

// Define a new application
class MyServer;
class MyFrame;

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
    virtual int OnExit();
    MyFrame *GetFrame() { return m_frame; };

protected:
    MyFrame        *m_frame;
};

DECLARE_APP(MyApp)

// Define a new frame
class MyFrame : public wxFrame
{
public:
    MyFrame(wxFrame *frame, const wxString& title);

    void OnExit(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);

    void Enable();
    void Disconnect();

protected:
    wxButton* GetStart()  { return (wxButton*) FindWindow( ID_START ); }
    wxChoice* GetServername()  { return (wxChoice*) FindWindow( ID_SERVERNAME ); }
    wxButton* GetDisconnect()  { return (wxButton*) FindWindow( ID_DISCONNECT ); }
    wxButton* GetAdvise()  { return (wxButton*) FindWindow( ID_ADVISE ); }
    wxTextCtrl* GetLog()  { return (wxTextCtrl*) FindWindow( ID_LOG ); }


    MyServer         *m_server;

    void OnStart( wxCommandEvent &event );
    void OnServerName( wxCommandEvent &event );
    void OnDisconnect( wxCommandEvent &event );
    void OnAdvise( wxCommandEvent &event );

    DECLARE_EVENT_TABLE()
};

class MyConnection : public MyConnectionBase
{
public:
    virtual bool OnExecute(const wxString& topic, const void *data, size_t size, wxIPCFormat format);
    virtual const void *OnRequest(const wxString& topic, const wxString& item, size_t *size, wxIPCFormat format);
    virtual bool OnPoke(const wxString& topic, const wxString& item, const void *data, size_t size, wxIPCFormat format);
    virtual bool OnStartAdvise(const wxString& topic, const wxString& item);
    virtual bool OnStopAdvise(const wxString& topic, const wxString& item);
    virtual bool DoAdvise(const wxString& item, const void *data, size_t size, wxIPCFormat format);
    virtual bool OnDisconnect();

    wxString        m_sAdvise;

protected:
    wxString        m_sRequestDate;
    char            m_achRequestBytes[3];
};

class MyServer: public wxServer
{
public:
    MyServer();
    ~MyServer();
    void Disconnect();
    bool IsConnected() { return m_connection != NULL; };
    MyConnection *GetConnection() { return m_connection; };
    void Advise();
    bool CanAdvise() { return m_connection != NULL && !m_connection->m_sAdvise.IsEmpty(); };
    wxConnectionBase *OnAcceptConnection(const wxString& topic);

protected:
    MyConnection     *m_connection;
};

