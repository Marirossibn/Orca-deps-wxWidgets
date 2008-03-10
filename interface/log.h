/////////////////////////////////////////////////////////////////////////////
// Name:        log.h
// Purpose:     interface of wxLogWindow
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxLogWindow
    @wxheader{log.h}

    This class represents a background log window: to be precise, it collects all
    log messages in the log frame which it manages but also passes them on to the
    log target which was active at the moment of its creation. This allows, for
    example, to show all the log messages in a frame but still continue to process
    them normally by showing the standard log dialog.

    @library{wxbase}
    @category{logging}

    @see wxLogTextCtrl
*/
class wxLogWindow : public wxLogInterposer
{
public:
    /**
        Creates the log frame window and starts collecting the messages in it.
        
        @param parent
            The parent window for the log frame, may be @NULL
        @param title
            The title for the log frame
        @param show
            @true to show the frame initially (default), otherwise
            Show() must be called later.
        @param passToOld
            @true to process the log messages normally in addition to
            logging them in the log frame (default), @false to only log them in the
            log frame.
    */
    wxLogWindow(wxFrame parent, const wxChar title, bool show = true,
                bool passToOld = true);

    /**
        Returns the associated log frame window. This may be used to position or resize
        it but use Show() to show or hide it.
    */
    wxFrame* GetFrame() const;

    /**
        Called if the user closes the window interactively, will not be
        called if it is destroyed for another reason (such as when program
        exits).
        Return @true from here to allow the frame to close, @false to
        prevent this from happening.
        
        @see OnFrameDelete()
    */
    virtual bool OnFrameClose(wxFrame frame);

    /**
        Called immediately after the log frame creation allowing for
        any extra initializations.
    */
    virtual void OnFrameCreate(wxFrame frame);

    /**
        Called right before the log frame is going to be deleted: will
        always be called unlike OnFrameClose().
    */
    virtual void OnFrameDelete(wxFrame frame);

    /**
        Shows or hides the frame.
    */
    void Show(bool show = true);
};



/**
    @class wxLogInterposerTemp
    @wxheader{log.h}

    A special version of wxLogChain which uses itself as the
    new log target. It forwards log messages to the previously installed one in
    addition to
    processing them itself. Unlike wxLogInterposer, it doesn't
    delete the old target which means it can be used to temporarily redirect log
    output.

    As per wxLogInterposer, this class must be derived from to implement
    wxLog::DoLog
    and/or wxLog::DoLogString methods.

    @library{wxbase}
    @category{logging}
*/
class wxLogInterposerTemp : public wxLogChain
{
public:
    /**
        The default constructor installs this object as the current active log target.
    */
};



/**
    @class wxLogChain
    @wxheader{log.h}

    This simple class allows to chain log sinks, that is to install a new sink but
    keep passing log messages to the old one instead of replacing it completely as
    wxLog::SetActiveTarget does.

    It is especially useful when you want to divert the logs somewhere (for
    example to a file or a log window) but also keep showing the error messages
    using the standard dialogs as wxLogGui does by default.

    Example of usage:

    @code
    wxLogChain *logChain = new wxLogChain(new wxLogStderr);

    // all the log messages are sent to stderr and also processed as usually
    ...

    // don't delete logChain directly as this would leave a dangling
    // pointer as active log target, use SetActiveTarget() instead
    delete wxLog::SetActiveTarget(...something else or @NULL...);
    @endcode

    @library{wxbase}
    @category{logging}
*/
class wxLogChain : public wxLog
{
public:
    /**
        Sets the specified @c logger (which may be @NULL) as the default log
        target but the log messages are also passed to the previous log target if any.
    */
    wxLogChain(wxLog* logger);

    /**
        Destroys the previous log target.
    */
    ~wxLogChain();

    /**
        Detaches the old log target so it won't be destroyed when the wxLogChain object
        is destroyed.
    */
    void DetachOldLog();

    /**
        Returns the pointer to the previously active log target (which may be @NULL).
    */
    wxLog* GetOldLog() const;

    /**
        Returns @true if the messages are passed to the previously active log
        target (default) or @false if PassMessages()
        had been called.
    */
    bool IsPassingMessages() const;

    /**
        By default, the log messages are passed to the previously active log target.
        Calling this function with @false parameter disables this behaviour
        (presumably temporarily, as you shouldn't use wxLogChain at all otherwise) and
        it can be reenabled by calling it again with @a passMessages set to @true.
    */
    void PassMessages(bool passMessages);

    /**
        Sets another log target to use (may be @NULL). The log target specified
        in the @ref ctor() constructor or in a previous call to
        this function is deleted.
        This doesn't change the old log target value (the one the messages are
        forwarded to) which still remains the same as was active when wxLogChain
        object was created.
    */
    void SetLog(wxLog* logger);
};



/**
    @class wxLogGui
    @wxheader{log.h}

    This is the default log target for the GUI wxWidgets applications. It is passed
    to wxLog::SetActiveTarget at the program
    startup and is deleted by wxWidgets during the program shut down.

    @library{wxbase}
    @category{logging}
*/
class wxLogGui : public wxLog
{
public:
    /**
        Default constructor.
    */
    wxLogGui();
};



/**
    @class wxLogStream
    @wxheader{log.h}

    This class can be used to redirect the log messages to a C++ stream.

    Please note that this class is only available if wxWidgets was compiled with
    the standard iostream library support (@c wxUSE_STD_IOSTREAM must be on).

    @library{wxbase}
    @category{logging}

    @see wxLogStderr, wxStreamToTextRedirector
*/
class wxLogStream : public wxLog
{
public:
    /**
        Constructs a log target which sends all the log messages to the given
        output stream. If it is @NULL, the messages are sent to @c cerr.
    */
    wxLogStream(std::ostream ostr = NULL);
};



/**
    @class wxLogStderr
    @wxheader{log.h}

    This class can be used to redirect the log messages to a C file stream (not to
    be confused with C++ streams). It is the default log target for the non-GUI
    wxWidgets applications which send all the output to @c stderr.

    @library{wxbase}
    @category{logging}

    @see wxLogStream
*/
class wxLogStderr : public wxLog
{
public:
    /**
        Constructs a log target which sends all the log messages to the given
        @c FILE. If it is @NULL, the messages are sent to @c stderr.
    */
    wxLogStderr(FILE fp = NULL);
};



/**
    @class wxLogBuffer
    @wxheader{log.h}

    wxLogBuffer is a very simple implementation of log sink which simply collects
    all the logged messages in a string (except the debug messages which are output
    in the usual way immediately as we're presumably not interested in collecting
    them for later). The messages from different log function calls are separated
    by the new lines.

    All the messages collected so far can be shown to the user (and the current
    buffer cleared) by calling the overloaded wxLogBuffer::Flush
    method.

    @library{wxbase}
    @category{FIXME}
*/
class wxLogBuffer : public wxLog
{
public:
    /**
        Shows all the messages collected so far to the user (using a message box in the
        GUI applications or by printing them out to the console in text mode) and
        clears the internal buffer.
    */
    virtual void Flush();

    /**
        Returns the current buffer contains. Messages from different log function calls
        are separated with the new lines in the buffer.
        The buffer can be cleared by Flush() which will
        also show the current contents to the user.
    */
    const wxString GetBuffer();
};



/**
    @class wxLogInterposer
    @wxheader{log.h}

    A special version of wxLogChain which uses itself as the
    new log target. It forwards log messages to the previously installed one in
    addition to
    processing them itself.

    Unlike wxLogChain which is usually used directly as is,
    this class must be derived from to implement wxLog::DoLog
    and/or wxLog::DoLogString methods.

    wxLogInterposer destroys the previous log target in its destructor. If you
    don't want this to happen, use wxLogInterposerTemp instead.

    @library{wxbase}
    @category{logging}
*/
class wxLogInterposer : public wxLogChain
{
public:
    /**
        The default constructor installs this object as the current active log target.
    */
};



/**
    @class wxLogTextCtrl
    @wxheader{log.h}

    Using these target all the log messages can be redirected to a text control.
    The text control must have been created with @c wxTE_MULTILINE style by the
    caller previously.

    @library{wxbase}
    @category{logging}

    @see wxTextCtrl, wxStreamToTextRedirector
*/
class wxLogTextCtrl : public wxLog
{
public:
    /**
        Constructs a log target which sends all the log messages to the given text
        control. The @a textctrl parameter cannot be @NULL.
    */
    wxLogTextCtrl(wxTextCtrl textctrl);
};



/**
    @class wxLog
    @wxheader{log.h}

    wxLog class defines the interface for the @e log targets used by wxWidgets
    logging functions as explained in the @ref overview_wxlogoverview "wxLog
    overview".
    The only situations when you need to directly use this class is when you want
    to derive your own log target because the existing ones don't satisfy your
    needs. Another case is if you wish to customize the behaviour of the standard
    logging classes (all of which respect the wxLog settings): for example, set
    which trace messages are logged and which are not or change (or even remove
    completely) the timestamp on the messages.

    Otherwise, it is completely hidden behind the @e wxLogXXX() functions and
    you may not even know about its existence.

    See @ref overview_wxlogoverview "log overview" for the descriptions of wxWidgets
    logging facilities.

    @library{wxcore}
    @category{logging}

    @see wxLog::RemoveTraceMask, wxLog::GetTraceMasks
*/
class wxLog
{
public:
    /**
        Add the @a mask to the list of allowed masks for
        wxLogTrace().
        
        @see RemoveTraceMask(), GetTraceMasks()
    */
    static void AddTraceMask(const wxString& mask);

    /**
        Removes all trace masks previously set with
        AddTraceMask().
        
        @see RemoveTraceMask()
    */
    static void ClearTraceMasks();

    /**
        The functions below allow some limited customization of wxLog behaviour
        without writing a new log target class (which, aside of being a matter of
        several minutes, allows you to do anything you want).
        The verbose messages are the trace messages which are not disabled in the
        release mode and are generated by wxLogVerbose(). They
        are not normally shown to the user because they present little interest, but
        may be activated, for example, in order to help the user find some program
        problem.
        As for the (real) trace messages, their handling depends on the settings of
        the (application global) @e trace mask. There are two ways to specify it:
        either by using SetTraceMask() and
        GetTraceMask() and using
        wxLogTrace() which takes an integer mask or by using
        AddTraceMask() for string trace masks.
        The difference between bit-wise and string trace masks is that a message using
        integer trace mask will only be logged if all bits of the mask are set in the
        current mask while a message using string mask will be logged simply if the
        mask had been added before to the list of allowed ones.
        For example,
        
        will do something only if the current trace mask contains both
        @c wxTraceRefCount and @c wxTraceOle, but
        
        will log the message if it was preceded by
        
        Using string masks is simpler and allows to easily add custom ones, so this is
        the preferred way of working with trace messages. The integer trace mask is
        kept for compatibility and for additional (but very rarely needed) flexibility
        only.
        The standard trace masks are given in wxLogTrace()
        documentation.
        Finally, the @e wxLog::DoLog() function automatically prepends a time stamp
        to all the messages. The format of the time stamp may be changed: it can be
        any string with % specifications fully described in the documentation of the
        standard @e strftime() function. For example, the default format is
        "[%d/%b/%y %H:%M:%S] " which gives something like "[17/Sep/98 22:10:16] "
        (without quotes) for the current date. Setting an empty string as the time
        format disables timestamping of the messages completely.
        @b NB: Timestamping is disabled for Visual C++ users in debug builds by
        default because otherwise it would be impossible to directly go to the line
        from which the log message was generated by simply clicking in the debugger
        window on the corresponding error message. If you wish to enable it, please use
        SetTimestamp() explicitly.
        AddTraceMask()
        
        RemoveTraceMask()
        
        ClearTraceMasks()
        
        GetTraceMasks()
        
        IsAllowedTraceMask()
        
        SetVerbose()
        
        GetVerbose()
        
        SetTimestamp()
        
        GetTimestamp()
        
        SetTraceMask()
        
        GetTraceMask()
        
        SetRepetitionCounting()
        
        GetRepetitionCounting()
    */


    /**
        Disables time stamping of the log messages.
        This function is new since wxWidgets version 2.9
    */
    void SetTimestamp(const wxString& format);

    /**
        Called to process the message of the specified severity. @a msg is the text
        of the message as specified in the call of @e wxLogXXX() function which
        generated it and @a timestamp is the moment when the message was generated.
        The base class version prepends the timestamp to the message, adds a prefix
        corresponding to the log level and then calls
        DoLogString() with the resulting string.
    */
    virtual void DoLog(wxLogLevel level, const wxString& msg,
                       time_t timestamp);

    /**
        Called to log the specified string. The timestamp is already included in the
        string but still passed to this function.
        A simple implementation may just send the string to @c stdout or, better,
        @c stderr.
    */
    virtual void DoLogString(const wxString& msg, time_t timestamp);

    /**
        Instructs wxLog to not create new log targets on the fly if there is none
        currently. (Almost) for internal use only: it is supposed to be called by the
        application shutdown code.
        Note that this function also calls
        ClearTraceMasks().
    */
    static void DontCreateOnDemand();

    /**
        Shows all the messages currently in buffer and clears it. If the buffer
        is already empty, nothing happens.
    */
    virtual void Flush();

    /**
        Flushes the current log target if any, does nothing if there is none.
        
        @see Flush()
    */
    static void FlushActive();

    /**
        Returns the pointer to the active log target (may be @NULL).
    */
    static wxLog* GetActiveTarget();

    /**
        Returns the current log level limit.
    */
    static wxLogLevel GetLogLevel();

    /**
        Returns whether the repetition counting mode is enabled.
    */
    static bool GetRepetitionCounting();

    /**
        Returns the current timestamp format string.
    */
    static const wxString GetTimestamp();

    /**
        Returns the current trace mask, see Customization() section
        for details.
    */
    static wxTraceMask GetTraceMask();

    /**
        Returns the currently allowed list of string trace masks.
        
        @see AddTraceMask().
    */
    static const wxArrayString GetTraceMasks();

    /**
        Returns whether the verbose mode is currently active.
    */
    static bool GetVerbose();

    /**
        The functions in this section work with and manipulate the active log target.
        The OnLog() is called by the @e wxLogXXX() functions
        and invokes the DoLog() of the active log target if any.
        Get/Set methods are used to install/query the current active target and,
        finally, DontCreateOnDemand() disables the
        automatic creation of a standard log target if none actually exists. It is
        only useful when the application is terminating and shouldn't be used in other
        situations because it may easily lead to a loss of messages.
        OnLog()
        
        GetActiveTarget()
        
        SetActiveTarget()
        
        DontCreateOnDemand()
        
        Suspend()
        
        Resume()
    */


    /**
        Returns @true if the @a mask is one of allowed masks for
        wxLogTrace().
        See also: AddTraceMask(),
        RemoveTraceMask()
    */
    static bool IsAllowedTraceMask(const wxString& mask);

    /**
        There are two functions which must be implemented by any derived class to
        actually process the log messages: DoLog() and
        DoLogString(). The second function receives a string
        which just has to be output in some way and the easiest way to write a new log
        target is to override just this function in the derived class. If more control
        over the output format is needed, then the first function must be overridden
        which allows to construct custom messages depending on the log level or even
        do completely different things depending on the message severity (for example,
        throw away all messages except warnings and errors, show warnings on the
        screen and forward the error messages to the user's (or programmer's) cell
        phone - maybe depending on whether the timestamp tells us if it is day or
        night in the current time zone).
        There also functions to support message buffering. Why are they needed?
        Some of wxLog implementations, most notably the standard wxLogGui class,
        buffer the messages (for example, to avoid showing the user a zillion of modal
        message boxes one after another -- which would be really annoying).
        Flush() shows them all and clears the buffer contents.
        This function doesn't do anything if the buffer is already empty.
        Flush()
        
        FlushActive()
    */


    /**
        Forwards the message at specified level to the @e DoLog() function of the
        active log target if there is any, does nothing otherwise.
    */
    static void OnLog(wxLogLevel level, const wxString& message);

    /**
        Remove the @a mask from the list of allowed masks for
        wxLogTrace().
        See also: AddTraceMask()
    */
    static void RemoveTraceMask(const wxString& mask);

    /**
        Resumes logging previously suspended by a call to
        Suspend(). All messages logged in the meanwhile will be
        flushed soon.
    */
    static void Resume();

    /**
        Sets the specified log target as the active one. Returns the pointer to the
        previous active log target (may be @NULL).  To suppress logging use a new
        instance of wxLogNull not @NULL.  If the active log target is set to @NULL a
        new default log target will be created when logging occurs.
    */
    static wxLog* SetActiveTarget(wxLog* logtarget);

    /**
        Specifies that log messages with level  logLevel should be ignored
        and not sent to the active log target.
    */
    static void SetLogLevel(wxLogLevel logLevel);

    /**
        Enables logging mode in which a log message is logged once, and in case exactly
        the same message successively repeats one or more times, only the number of
        repetitions is logged.
    */
    static void SetRepetitionCounting(bool repetCounting = true);

    /**
        Sets the timestamp format prepended by the default log targets to all
        messages. The string may contain any normal characters as well as %
        prefixed format specificators, see @e strftime() manual for details.
        Passing an empty string to this function disables message time stamping.
    */
    static void SetTimestamp(const wxString& format);

    /**
        Sets the trace mask, see Customization()
        section for details.
    */
    static void SetTraceMask(wxTraceMask mask);

    /**
        Activates or deactivates verbose mode in which the verbose messages are
        logged as the normal ones instead of being silently dropped.
    */
    static void SetVerbose(bool verbose = true);

    /**
        Suspends the logging until Resume() is called. Note that
        the latter must be called the same number of times as the former to undo it,
        i.e. if you call Suspend() twice you must call Resume() twice as well.
        Note that suspending the logging means that the log sink won't be be flushed
        periodically, it doesn't have any effect if the current log target does the
        logging immediately without waiting for Flush() to be
        called (the standard GUI log target only shows the log dialog when it is
        flushed, so Suspend() works as expected with it).
        
        @see Resume(), wxLogNull
    */
    static void Suspend();
};



/**
    @class wxLogNull
    @wxheader{log.h}

    This class allows to temporarily suspend logging. All calls to the log
    functions during the life time of an object of this class are just ignored.

    In particular, it can be used to suppress the log messages given by wxWidgets
    itself but it should be noted that it is rarely the best way to cope with this
    problem as @b all log messages are suppressed, even if they indicate a
    completely different error than the one the programmer wanted to suppress.

    For instance, the example of the overview:

    @code
    wxFile file;

      // wxFile.Open() normally complains if file can't be opened, we don't want it
      {
        wxLogNull logNo;
        if ( !file.Open("bar") )
          ... process error ourselves ...
      } // ~wxLogNull called, old log sink restored

      wxLogMessage("..."); // ok
    @endcode

    would be better written as:

    @code
    wxFile file;

      // don't try to open file if it doesn't exist, we are prepared to deal with
      // this ourselves - but all other errors are not expected
      if ( wxFile::Exists("bar") )
      {
          // gives an error message if the file couldn't be opened
          file.Open("bar");
      }
      else
      {
          ...
      }
    @endcode


    @library{wxbase}
    @category{logging}
*/
class wxLogNull : public wxLog
{
public:
    /**
        Suspends logging.
    */
    wxLogNull();

    /**
        Resumes logging.
    */
};



// ============================================================================
// Global functions/macros
// ============================================================================

/**
    This function shows a message to the user in a safe way and should be safe to
    call even before the application has been initialized or if it is currently in
    some other strange state (for example, about to crash). Under Windows this
    function shows a message box using a native dialog instead of
    wxMessageBox() (which might be unsafe to call), elsewhere
    it simply prints the message to the standard output using the title as prefix.

    @param title
        The title of the message box shown to the user or the prefix
        of the message string
    @param text
        The text to show to the user

    @see wxLogFatalError()
*/
void wxSafeShowMessage(const wxString& title,
                       const wxString& text);

