/////////////////////////////////////////////////////////////////////////////
// Name:        thread.cpp
// Purpose:     wxWidgets thread sample
// Author:      Guilhem Lavaux, Vadim Zeitlin
// Modified by:
// Created:     06/16/98
// RCS-ID:      $Id$
// Copyright:   (c) 1998-2002 wxWidgets team
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#if !wxUSE_THREADS
    #error "This sample requires thread support!"
#endif // wxUSE_THREADS

#include "wx/thread.h"
#include "wx/dynarray.h"
#include "wx/numdlg.h"
#include "wx/progdlg.h"

// ----------------------------------------------------------------------------
// resources
// ----------------------------------------------------------------------------

#include "../sample.xpm"

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

// define this to use wxExecute in the exec tests, otherwise just use system
#define USE_EXECUTE

#ifdef USE_EXECUTE
    #define EXEC(cmd) wxExecute((cmd), wxEXEC_SYNC)
#else
    #define EXEC(cmd) system(cmd)
#endif

class MyThread;
WX_DEFINE_ARRAY_PTR(wxThread *, wxArrayThread);

// Define a new application type
class MyApp : public wxApp
{
public:
    MyApp();
    virtual ~MyApp(){};

    virtual bool OnInit();

    // critical section protects access to all of the fields below
    wxCriticalSection m_critsect;

    // all the threads currently alive - as soon as the thread terminates, it's
    // removed from the array
    wxArrayThread m_threads;

    // semaphore used to wait for the threads to exit, see MyFrame::OnQuit()
    wxSemaphore m_semAllDone;

    // indicates that we're shutting down and all threads should exit
    bool m_shuttingDown;
};

// Define a new frame type
class MyFrame: public wxFrame
{
public:
    // ctor
    MyFrame(wxFrame *frame, const wxString& title, int x, int y, int w, int h);
    virtual ~MyFrame();

    // this function is MT-safe, i.e. it can be called from worker threads
    // safely without any additional locking
    void LogThreadMessage(const wxString& text)
    {
        wxCriticalSectionLocker lock(m_csMessages);
        m_messages.push_back(text);

        // as we effectively log the messages from the idle event handler,
        // ensure it's going to be called now that we have some messages to log
        wxWakeUpIdle();
    }

    // accessors for MyWorkerThread (called in its context!)
    bool Cancelled();

private:
    // event handlers
    // --------------

    void OnQuit(wxCommandEvent& event);
    void OnClear(wxCommandEvent& event);

    void OnStartThread(wxCommandEvent& event);
    void OnStartThreads(wxCommandEvent& event);
    void OnStopThread(wxCommandEvent& event);
    void OnPauseThread(wxCommandEvent& event);
    void OnResumeThread(wxCommandEvent& event);

    void OnStartWorker(wxCommandEvent& event);

    void OnExecMain(wxCommandEvent& event);
    void OnExecThread(wxCommandEvent& event);

    void OnShowCPUs(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    void OnIdle(wxIdleEvent &event);
    void OnWorkerEvent(wxThreadEvent& event);
    void OnUpdateWorker(wxUpdateUIEvent& event);


    // thread helper functions
    // -----------------------

    // helper function - creates a new thread (but doesn't run it)
    MyThread *CreateThread();

    // update display in our status bar: called during idle handling
    void UpdateThreadStatus();

    // log the messages queued by LogThreadMessage()
    void DoLogThreadMessages();


    // internal variables
    // ------------------

    // just some place to put our messages in
    wxTextCtrl *m_txtctrl;

    // the array of pending messages to be displayed and the critical section
    // protecting it
    wxArrayString m_messages;
    wxCriticalSection m_csMessages;

    // remember the number of running threads and total number of threads
    size_t m_nRunning,
           m_nCount;

    // the progress dialog which we show while worker thread is running
    wxProgressDialog *m_dlgProgress;

    // was the worker thread cancelled by user?
    bool m_cancelled;

    // protects m_cancelled
    wxCriticalSection m_critsectWork;

    DECLARE_EVENT_TABLE()
};

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// ID for the menu commands
enum
{
    THREAD_QUIT  = wxID_EXIT,
    THREAD_ABOUT = wxID_ABOUT,
    THREAD_TEXT          = 101,
    THREAD_CLEAR,
    THREAD_START_THREAD  = 201,
    THREAD_START_THREADS,
    THREAD_STOP_THREAD,
    THREAD_PAUSE_THREAD,
    THREAD_RESUME_THREAD,
    THREAD_START_WORKER,

    THREAD_EXEC_MAIN,
    THREAD_EXEC_THREAD,

    THREAD_SHOWCPUS,

    WORKER_EVENT = wxID_HIGHEST+1   // this one gets sent from the worker thread
};

// ----------------------------------------------------------------------------
// GUI thread
// ----------------------------------------------------------------------------

class MyThread : public wxThread
{
public:
    MyThread(MyFrame *frame);
    virtual ~MyThread();

    // thread execution starts here
    virtual void *Entry();

    // write something to the text control in the main frame
    void WriteText(const wxString& text)
    {
        m_frame->LogThreadMessage(text);
    }

public:
    unsigned m_count;
    MyFrame *m_frame;
};

// ----------------------------------------------------------------------------
// worker thread
// ----------------------------------------------------------------------------

class MyWorkerThread : public wxThread
{
public:
    MyWorkerThread(MyFrame *frame);

    // thread execution starts here
    virtual void *Entry();

    // called when the thread exits - whether it terminates normally or is
    // stopped with Delete() (but not when it is Kill()ed!)
    virtual void OnExit();

public:
    MyFrame *m_frame;
    unsigned m_count;
};

// ----------------------------------------------------------------------------
// a thread which simply calls wxExecute
// ----------------------------------------------------------------------------

class MyExecThread : public wxThread
{
public:
    MyExecThread(const wxChar *command) : wxThread(wxTHREAD_JOINABLE),
                                          m_command(command)
    {
        Create();
    }

    virtual ExitCode Entry()
    {
        return wxUIntToPtr(EXEC(m_command));
    }

private:
    wxString m_command;
};

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

// Create a new application object
IMPLEMENT_APP(MyApp)

MyApp::MyApp()
{
    m_shuttingDown = false;
}

// `Main program' equivalent, creating windows and returning main app frame
bool MyApp::OnInit()
{
    if ( !wxApp::OnInit() )
        return false;

    // uncomment this to get some debugging messages from the trace code
    // on the console (or just set WXTRACE env variable to include "thread")
    wxLog::AddTraceMask("thread");

    // Create the main frame window
    MyFrame *frame = new MyFrame((wxFrame *)NULL, _T("wxWidgets threads sample"),
                                 50, 50, 450, 340);
    SetTopWindow(frame);

    // Show the frame
    frame->Show(true);

    return true;
}

// ----------------------------------------------------------------------------
// MyFrame
// ----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(THREAD_QUIT, MyFrame::OnQuit)
    EVT_MENU(THREAD_CLEAR, MyFrame::OnClear)
    EVT_MENU(THREAD_START_THREAD, MyFrame::OnStartThread)
    EVT_MENU(THREAD_START_THREADS, MyFrame::OnStartThreads)
    EVT_MENU(THREAD_STOP_THREAD, MyFrame::OnStopThread)
    EVT_MENU(THREAD_PAUSE_THREAD, MyFrame::OnPauseThread)
    EVT_MENU(THREAD_RESUME_THREAD, MyFrame::OnResumeThread)

    EVT_MENU(THREAD_EXEC_MAIN, MyFrame::OnExecMain)
    EVT_MENU(THREAD_EXEC_THREAD, MyFrame::OnExecThread)

    EVT_MENU(THREAD_SHOWCPUS, MyFrame::OnShowCPUs)
    EVT_MENU(THREAD_ABOUT, MyFrame::OnAbout)

    EVT_MENU(THREAD_START_WORKER, MyFrame::OnStartWorker)

    EVT_UPDATE_UI(THREAD_START_WORKER, MyFrame::OnUpdateWorker)
    EVT_THREAD(WORKER_EVENT, MyFrame::OnWorkerEvent)
    EVT_IDLE(MyFrame::OnIdle)
END_EVENT_TABLE()

// My frame constructor
MyFrame::MyFrame(wxFrame *frame, const wxString& title,
                 int x, int y, int w, int h)
       : wxFrame(frame, wxID_ANY, title, wxPoint(x, y), wxSize(w, h))
{
    SetIcon(wxIcon(sample_xpm));

    // Make a menubar
    wxMenuBar *menuBar = new wxMenuBar;

    wxMenu *menuFile = new wxMenu;
    menuFile->Append(THREAD_CLEAR, _T("&Clear log\tCtrl-L"));
    menuFile->AppendSeparator();
    menuFile->Append(THREAD_QUIT, _T("E&xit\tAlt-X"));
    menuBar->Append(menuFile, _T("&File"));

    wxMenu *menuThread = new wxMenu;
    menuThread->Append(THREAD_START_THREAD, _T("&Start a new thread\tCtrl-N"));
    menuThread->Append(THREAD_START_THREADS, _T("Start &many threads at once"));
    menuThread->Append(THREAD_STOP_THREAD, _T("S&top the last spawned thread\tCtrl-S"));
    menuThread->AppendSeparator();
    menuThread->Append(THREAD_PAUSE_THREAD, _T("&Pause the last spawned running thread\tCtrl-P"));
    menuThread->Append(THREAD_RESUME_THREAD, _T("&Resume the first suspended thread\tCtrl-R"));
    menuThread->AppendSeparator();
    menuThread->Append(THREAD_START_WORKER, _T("Start a &worker thread\tCtrl-W"));
    menuBar->Append(menuThread, _T("&Thread"));

    wxMenu *menuExec = new wxMenu;
    menuExec->Append(THREAD_EXEC_MAIN, _T("&Launch a program from main thread\tF5"));
    menuExec->Append(THREAD_EXEC_THREAD, _T("L&aunch a program from a thread\tCtrl-F5"));
    menuBar->Append(menuExec, _T("&Execute"));

    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(THREAD_SHOWCPUS, _T("&Show CPU count"));
    menuHelp->AppendSeparator();
    menuHelp->Append(THREAD_ABOUT, _T("&About..."));
    menuBar->Append(menuHelp, _T("&Help"));

    SetMenuBar(menuBar);

    m_nRunning = m_nCount = 0;

    m_dlgProgress = (wxProgressDialog *)NULL;

#if wxUSE_STATUSBAR
    CreateStatusBar(2);
#endif // wxUSE_STATUSBAR

    m_txtctrl = new wxTextCtrl(this, wxID_ANY, _T(""), wxPoint(0, 0), wxSize(0, 0),
                               wxTE_MULTILINE | wxTE_READONLY);
}

MyFrame::~MyFrame()
{
    // NB: although the OS will terminate all the threads anyhow when the main
    //     one exits, it's good practice to do it ourselves -- even if it's not
    //     completely trivial in this example

    // tell all the threads to terminate: note that they can't terminate while
    // we're deleting them because they will block in their OnExit() -- this is
    // important as otherwise we might access invalid array elements

    {
        wxCriticalSectionLocker locker(wxGetApp().m_critsect);

        // check if we have any threads running first
        const wxArrayThread& threads = wxGetApp().m_threads;
        size_t count = threads.GetCount();

        if ( !count )
            return;

        // set the flag indicating that all threads should exit
        wxGetApp().m_shuttingDown = true;
    }

    // now wait for them to really terminate
    wxGetApp().m_semAllDone.Wait();
}

MyThread *MyFrame::CreateThread()
{
    MyThread *thread = new MyThread(this);

    if ( thread->Create() != wxTHREAD_NO_ERROR )
    {
        wxLogError(wxT("Can't create thread!"));
    }

    wxCriticalSectionLocker enter(wxGetApp().m_critsect);
    wxGetApp().m_threads.Add(thread);

    return thread;
}

void MyFrame::OnStartThreads(wxCommandEvent& WXUNUSED(event) )
{
    static long s_num;

    s_num = wxGetNumberFromUser(_T("How many threads to start: "), _T(""),
                                _T("wxThread sample"), s_num, 1, 10000, this);
    if ( s_num == -1 )
    {
        s_num = 10;

        return;
    }

    unsigned count = unsigned(s_num), n;

    wxArrayThread threads;

    // first create them all...
    for ( n = 0; n < count; n++ )
    {
        wxThread *thr = CreateThread();

        // we want to show the effect of SetPriority(): the first thread will
        // have the lowest priority, the second - the highest, all the rest
        // the normal one
        if ( n == 0 )
            thr->SetPriority(WXTHREAD_MIN_PRIORITY);
        else if ( n == 1 )
            thr->SetPriority(WXTHREAD_MAX_PRIORITY);
        else
            thr->SetPriority(WXTHREAD_DEFAULT_PRIORITY);

        threads.Add(thr);
    }

#if wxUSE_STATUSBAR
    wxString msg;
    msg.Printf(wxT("%d new threads created."), count);
    SetStatusText(msg, 1);
#endif // wxUSE_STATUSBAR

    // ...and then start them
    for ( n = 0; n < count; n++ )
    {
        threads[n]->Run();
    }
}

void MyFrame::OnStartThread(wxCommandEvent& WXUNUSED(event) )
{
    MyThread *thread = CreateThread();

    if ( thread->Run() != wxTHREAD_NO_ERROR )
    {
        wxLogError(wxT("Can't start thread!"));
    }

#if wxUSE_STATUSBAR
    SetStatusText(_T("New thread started."), 1);
#endif // wxUSE_STATUSBAR
}

void MyFrame::OnStopThread(wxCommandEvent& WXUNUSED(event) )
{
    wxCriticalSectionLocker enter(wxGetApp().m_critsect);

    // stop the last thread
    if ( wxGetApp().m_threads.IsEmpty() )
    {
        wxLogError(wxT("No thread to stop!"));
    }
    else
    {
        wxGetApp().m_threads.Last()->Delete();

#if wxUSE_STATUSBAR
        SetStatusText(_T("Last thread stopped."), 1);
#endif // wxUSE_STATUSBAR
    }
}

void MyFrame::OnResumeThread(wxCommandEvent& WXUNUSED(event) )
{
    wxCriticalSectionLocker enter(wxGetApp().m_critsect);

    // resume first suspended thread
    size_t n = 0, count = wxGetApp().m_threads.Count();
    while ( n < count && !wxGetApp().m_threads[n]->IsPaused() )
        n++;

    if ( n == count )
    {
        wxLogError(wxT("No thread to resume!"));
    }
    else
    {
        wxGetApp().m_threads[n]->Resume();

#if wxUSE_STATUSBAR
        SetStatusText(_T("Thread resumed."), 1);
#endif // wxUSE_STATUSBAR
    }
}

void MyFrame::OnPauseThread(wxCommandEvent& WXUNUSED(event) )
{
    wxCriticalSectionLocker enter(wxGetApp().m_critsect);

    // pause last running thread
    int n = wxGetApp().m_threads.Count() - 1;
    while ( n >= 0 && !wxGetApp().m_threads[n]->IsRunning() )
        n--;

    if ( n < 0 )
    {
        wxLogError(wxT("No thread to pause!"));
    }
    else
    {
        wxGetApp().m_threads[n]->Pause();

#if wxUSE_STATUSBAR
        SetStatusText(_T("Thread paused."), 1);
#endif // wxUSE_STATUSBAR
    }
}

void MyFrame::OnIdle(wxIdleEvent& event)
{
    DoLogThreadMessages();

    UpdateThreadStatus();

    event.Skip();
}

void MyFrame::DoLogThreadMessages()
{
    wxCriticalSectionLocker lock(m_csMessages);

    const size_t count = m_messages.size();
    for ( size_t n = 0; n < count; n++ )
    {
        m_txtctrl->AppendText(m_messages[n]);
    }

    m_messages.clear();
}

void MyFrame::UpdateThreadStatus()
{
    wxCriticalSectionLocker enter(wxGetApp().m_critsect);

    // update the counts of running/total threads
    size_t nRunning = 0,
           nCount = wxGetApp().m_threads.Count();
    for ( size_t n = 0; n < nCount; n++ )
    {
        if ( wxGetApp().m_threads[n]->IsRunning() )
            nRunning++;
    }

    if ( nCount != m_nCount || nRunning != m_nRunning )
    {
        m_nRunning = nRunning;
        m_nCount = nCount;

        wxLogStatus(this, wxT("%u threads total, %u running."), unsigned(nCount), unsigned(nRunning));
    }
    //else: avoid flicker - don't print anything
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event) )
{
    Close(true);
}

void MyFrame::OnExecMain(wxCommandEvent& WXUNUSED(event))
{
    wxLogMessage(wxT("The exit code from the main program is %ld"),
                 EXEC(_T("/bin/echo \"main program\"")));
}

void MyFrame::OnExecThread(wxCommandEvent& WXUNUSED(event))
{
    MyExecThread thread(wxT("/bin/echo \"child thread\""));
    thread.Run();

    wxLogMessage(wxT("The exit code from a child thread is %ld"),
                 (long)wxPtrToUInt(thread.Wait()));
}

void MyFrame::OnShowCPUs(wxCommandEvent& WXUNUSED(event))
{
    wxString msg;

    int nCPUs = wxThread::GetCPUCount();
    switch ( nCPUs )
    {
        case -1:
            msg = _T("Unknown number of CPUs");
            break;

        case 0:
            msg = _T("WARNING: you're running without any CPUs!");
            break;

        case 1:
            msg = _T("This system only has one CPU.");
            break;

        default:
            msg.Printf(wxT("This system has %d CPUs"), nCPUs);
    }

    wxLogMessage(msg);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event) )
{
    wxMessageDialog dialog(this,
                           _T("wxWidgets multithreaded application sample\n")
                           _T("(c) 1998 Julian Smart, Guilhem Lavaux\n")
                           _T("(c) 1999 Vadim Zeitlin\n")
                           _T("(c) 2000 Robert Roebling"),
                           _T("About wxThread sample"),
                           wxOK | wxICON_INFORMATION);

    dialog.ShowModal();
}

void MyFrame::OnClear(wxCommandEvent& WXUNUSED(event))
{
    m_txtctrl->Clear();
}

void MyFrame::OnUpdateWorker(wxUpdateUIEvent& event)
{
    event.Enable( m_dlgProgress == NULL );
}

void MyFrame::OnStartWorker(wxCommandEvent& WXUNUSED(event))
{
    MyWorkerThread *thread = new MyWorkerThread(this);

    if ( thread->Create() != wxTHREAD_NO_ERROR )
    {
        wxLogError(wxT("Can't create thread!"));
        return;
    }

    m_dlgProgress = new wxProgressDialog
                        (
                         _T("Progress dialog"),
                         _T("Wait until the thread terminates or press [Cancel]"),
                         100,
                         this,
                         wxPD_CAN_ABORT |
                         wxPD_APP_MODAL |
                         wxPD_ELAPSED_TIME |
                         wxPD_ESTIMATED_TIME |
                         wxPD_REMAINING_TIME
                        );

    // thread is not running yet, no need for crit sect
    m_cancelled = false;

    thread->Run();
}

void MyFrame::OnWorkerEvent(wxThreadEvent& event)
{
    int n = event.GetInt();
    if ( n == -1 )
    {
        m_dlgProgress->Destroy();
        m_dlgProgress = (wxProgressDialog *)NULL;

        // the dialog is aborted because the event came from another thread, so
        // we may need to wake up the main event loop for the dialog to be
        // really closed
        wxWakeUpIdle();
    }
    else
    {
        if ( !m_dlgProgress->Update(n) )
        {
            wxCriticalSectionLocker lock(m_critsectWork);

            m_cancelled = true;
        }
    }
}

bool MyFrame::Cancelled()
{
    wxCriticalSectionLocker lock(m_critsectWork);

    return m_cancelled;
}


// ----------------------------------------------------------------------------
// MyThread
// ----------------------------------------------------------------------------

MyThread::MyThread(MyFrame *frame)
        : wxThread()
{
    m_count = 0;
    m_frame = frame;
}

MyThread::~MyThread()
{
    wxCriticalSectionLocker locker(wxGetApp().m_critsect);

    wxArrayThread& threads = wxGetApp().m_threads;
    threads.Remove(this);

    if ( threads.IsEmpty() )
    {
        // signal the main thread that there are no more threads left if it is
        // waiting for us
        if ( wxGetApp().m_shuttingDown )
        {
            wxGetApp().m_shuttingDown = false;

            wxGetApp().m_semAllDone.Post();
        }
    }
}

void *MyThread::Entry()
{
    wxString text;

    text.Printf(wxT("Thread %p started (priority = %u).\n"),
                GetId(), GetPriority());
    WriteText(text);
    // wxLogMessage(text); -- test wxLog thread safeness

    for ( m_count = 0; m_count < 10; m_count++ )
    {
        // check if the application is shutting down: in this case all threads
        // should stop a.s.a.p.
        {
            wxCriticalSectionLocker locker(wxGetApp().m_critsect);
            if ( wxGetApp().m_shuttingDown )
                return NULL;
        }

        // check if just this thread was asked to exit
        if ( TestDestroy() )
            break;

        text.Printf(wxT("[%u] Thread %p here.\n"), m_count, GetId());
        WriteText(text);

        // wxSleep() can't be called from non-GUI thread!
        wxThread::Sleep(1000);
    }

    text.Printf(wxT("Thread %p finished.\n"), GetId());
    WriteText(text);
    // wxLogMessage(text); -- test wxLog thread safeness

    return NULL;
}


// ----------------------------------------------------------------------------
// MyWorkerThread
// ----------------------------------------------------------------------------

MyWorkerThread::MyWorkerThread(MyFrame *frame)
        : wxThread()
{
    m_frame = frame;
    m_count = 0;
}

void MyWorkerThread::OnExit()
{
}

// define this symbol to 1 to test if the YieldFor() call in the wxProgressDialog::Update
// function provokes a race condition in which the second wxThreadEvent posted by
// MyWorkerThread::Entry is processed by the YieldFor() call of wxProgressDialog::Update
// and results in the destruction of the progress dialog itself, resulting in a crash later.
#define TEST_YIELD_RACE_CONDITION       0

void *MyWorkerThread::Entry()
{
#if TEST_YIELD_RACE_CONDITION
    if ( TestDestroy() )
        return NULL;

    wxThreadEvent event( wxEVT_COMMAND_THREAD, WORKER_EVENT );

    event.SetInt( 50 );
    wxQueueEvent( m_frame, event.Clone() );

    event.SetInt(-1);
    wxQueueEvent( m_frame, event.Clone() );
#else
    for ( m_count = 0; !m_frame->Cancelled() && (m_count < 100); m_count++ )
    {
        // check if we were asked to exit
        if ( TestDestroy() )
            break;

        // create any type of command event here
        wxThreadEvent event( wxEVT_COMMAND_THREAD, WORKER_EVENT );
        event.SetInt( m_count );

        // send in a thread-safe way
        wxQueueEvent( m_frame, event.Clone() );

        wxMilliSleep(200);
    }

    wxThreadEvent event( wxEVT_COMMAND_THREAD, WORKER_EVENT );
    event.SetInt(-1); // that's all
    wxQueueEvent( m_frame, event.Clone() );
#endif

    return NULL;
}

