#----------------------------------------------------------------------------
# Name:         ErrorDialogs.py
# Version:      1.0
# Created:      September--October 2001
# Author:       Chris Fama of Wholly Snakes Software,
#               Chris.Fama@whollysnakes.com
#----------------------------------------------------------------------------
"""
ErrorDialogs.py: by Christopher J. Fama (trading under the name
Wholly Snakes Software {Australian Business Number: 28379514278}).

This code is released under the auspices of the modified version of
the GNU Library Public License (LGPL) that constitutes the license
under which the GUI package wxPython is released [see the file
LICENSE.TXT accompanying that distribution).  I must also thank Graham
Boyd, of Boyd Mining, and CEO of the Minserve group of companies
(www.minserve.com.au), for kindly allowing the release of this
module under a "free" license, despite a certain part of it's
development having taken place while working for him...

Please note that this code, written for Python 2.1, is derives from
code written when Python 1.5.2 was current.  Although since Python 2.0
the sys.excepthook variable has been available, for ease and potential
backwards compatibility (or to be more realistic
backwards-PARTIAL-compatibility!), the code catches errors by
assigning a custom object to sys.stderr and monitoring calls to it's
write() method.  Such calls which take place with a new value of
sys.last_traceback stand as evidence that at interpreter error has
just occurred; please note that this means that NO OTHER OUTPUT TO
sys.stderr WILL BE DISPLAYED.  THIS INCLUDES "warnings" generated by
the interpreter.  As far as I am aware, unless your code itself writes
to sys.stderr itself, these will be the only things you miss out
on--and many, if not most or all, of these will occur before your code
even gets the opportunity to set one of these file-like objects in
place.  If this is a problem for you and you can't work around it,
please contact means or the wxPython-users mailing list.


DOCUMENTATION:

This is a module to display errors--either explicitly requested by a
script or arbitrary interpreter errors--these needs arose in the
course of a commercial (contract) project, but were not developed in
the course of work on such [well, very little, albeit concurrently]...
[NBNB.Does not currently support display of other than interpreter errors.]

Usage, after 'from wxPython.lib.ErrorDialogs import *' (all
identifiers defined in this module begin with "wxPy", and many of them
with "wxPyError_", so there should be no namespace conflicts...):

    wxPyNewErrorDialog (<win> (['frame='] <frame (can be None)>,
                                         <OPTIONS>))
                   ...
    wxPyDestroyErrorDialogIfPresent () # e.g. when top frame destroyed)

for unhandled errors, or

    returnval = wxpyNonFatalError (<frame (can be None)>,
                          <HTML message>
                          [,<OPTIONS>])
    [NOTE: NOT IMPLEMENTED {IN THIS VERSION} YET...]
or
    returnval = wxPyFatalError (<HTML message>,
                       [,<OPTIONS, an extra one of which may be
                          'frame=' <frame (defaults to None)>>])
    [NOTE: NOT IMPLEMENTED {IN THIS VERSION} YET...]
or

    wxPybNonWindowingError (message)

for explicit errors.

<win> is one of
    wxPyNonFatalErrorDialog
    wxPyFatalErrorDialog
    wxPyFatalErrorDialogWithTraceback
    wxPyNonFatalErrorDialogWithTraceback
    wxPyNonWindowingErrorHandler

and the OPTIONS (with defaults) are: (please note that the options for
wxPyNonWindowingErrorHandler / wxPyNonWindowingError are 'almost' a (small))
subset of these):

    'modal' [default 1]: block until dismissed.

    'programname' [default "Python program"]: appears inThe
    caption of the dialog, amidst it's text and (by default) in mailings.

    'whendismissed' option, if given, this should be a string, to be
    executed in a restricted environment (see rexec module) after
    dialog dismissal.  Typically, this should be Python code to "clean
    up" after what was presumably the partial execution of some
    operation started by the user, after the unexpected interruption
    by some error [which--at least the way I work, means an unexpected
    error in the code, since exceptions that may be raised by some
    foreseen (in a programmatic sense) should normally be handled by
    try...except clauses].
    NOTE THAT CURRENTLY THE rexec CODE IS NOT WORKING, SO THIS IS JUST DONE
    BY exec...

    'mailto': if None, give no "e-mail support" option, otherwise this
    is meant to be an address for 'bug reports'; a command button will
    be created for this, which pops up another dialog  [Linux], or a window of
    another program [Windows].

    On Windows, this will launch your mailer (assuming your mailer
    would start when a file with the '.eml'extension is double-clicked
    in Windows Explorer--this will be the case for Microsoft Outlook
    Express [tm and all those legal necessities] if installed, and the
    association has not been taken over by some other program.  On
    Linux, it will open a file dialog to save the message, by default
    as a '.html' file...{Please note that, on Windows and with current
    (when I got the machine I'm writing this on--early 2000) versions
    of Outlook Express, you may need to enter your e-mail address in
    the box beside the "mail support" button.)

    The template for the mail message is in the 'MessageTemplate'
    attribute of the dialog-derived class in question (e.g.,
    wxPyNonFatalErrorDialog).  Search for this in the code below to
    see the default (note that this template is in HTML format).  This
    attributes uses the '%{name}s' string conversion feature of Python
    [see sec.2 of the Python Library Reference]; allowed Lance are:
    programname version exceptionname exceptionvalue
    extraexceptioninformation traceback

    'version': str(this) will appear after 'Version' below "Error in
    <programname>" at the top of the dialog.

EXAMPLES:

    sys.stderr = wxPyNonFatalErrorWindowWithTraceback (
        parentframe,
        programname='sumthing',
        mailto='me@sumwear',
        whendismissed="from wxPython.wx import * ; wxBell()")

FOR INTERNATIONAL [NON-ENGLISH-SPEAKING] USE:
    wxPyNonFatalErrorDialog and relatives have the method
        SetText(Number NUMBER, STRING)

    where STRING is to displayed in the wxStaticText control with ID
    wxPyError_ID_TEXT<NUMBER>--see the automatically-generated code
    for information about the meaning of these...

"""

_debug = 0
#_debug = 1 # uncomment to display some information (to stdout)

from wxPython.wx import *
import string, sys, traceback, time, rexec, operator, types, tempfile, os
#from wxPython.lib.createandsendHTMLmail import *# now inline
import MimeWriter, mimetools, cStringIO, smtplib

from ErrorDialogs_wdr import *

# You may see from the above line that I used the excellent RAD tool
# wxDesigner, by Robert Roebling, to accelerate development of this
# module...  The above is left for the convenience of making future
# changes with wxDesigner; also so the wxDesigner-generated codedoes
# not need to precede the "hand-generated" code in this file; finally,
# as a personal endorsement: it is truly a brilliant time-saver!
# Please note that, at the time of writing, the wxDesigner-generated
# output requires manual removal of the PythonBitmaps function--an
# appropriate version of this function will be imported from a
# similarly- named module.  Another manual change will have to be made
# to the automatically-generated source: "parent.sizerAroundText = "
# should be added [immediately] before the text similar to "item13 =
# wxStaticBoxSizer( item14, wxVERTICAL )", this sizer being the one
# containing the wxTextCtrl...  [IMPORTANT NOTE: THIS LINE SHOULD BE
# THE ONE INSTANTIATING A wxStaticBoxSizer, *NOT* THE wxStaticBox
# ITSELF...]

def wxPyDestroyErrorDialogIfPresent():
    if isinstance(sys.stderr,wxPyNonFatalErrorDialog):
        sys.stderr.Destroy()
    sys.stderr = None

def wxPyNewErrorDialog(dlg):
    wxPyDestroyErrorDialogIfPresent()
    sys.stderr = dlg

class wxPyNonWindowingErrorHandler:
    this_exception = 0
    softspace = 0
    def __init__(self,fatal=0,file=sys.__stderr__):
        self.fatal = fatal
        self.file = file
    def write(self,s):
        import sys
        if string.find(s,"Warning") <> 0\
           and  self.this_exception is not sys.last_traceback:
            wxPyNonWindowingError("The Python interpreter encountered an error "
                              "not handled by any\nexception handler--this "
                              "may represent some programming error.",
                              fatal=self.fatal,
                               stderr=self.file,
                              last=1)
        self.this_exception = sys.last_traceback

def wxPyNonWindowingError(msg,#output=None,errors=None,
                          stderr=sys.__stderr__,
                          fatal=1,
                          last=None):
    if os.path.exists("wxPyNonWindowingErrors.txt"):
        mode = 'a+'
    else:
        mode = 'w'
    fl = open("wxPyNonWindowingErrors.txt",mode)
    if stderr is not None:
        l = [fl,stderr] # so that the error will be written to the file
            # before any potential error in stderr.write()... (this is largely
            # for my own sake in developing...)
    else:
        l = [fl]
    for f in l:
        f.write(time.ctime (time.time ()) + ": ")
        f.write(msg)
        #f.flush()
        if sys.exc_info () [0] is not None:
            if last:
                f.write('Currently handled exception:\n')
                f.flush()
            traceback.print_exc(file=f)
            if last:
                f.write('\nPrevious (?) error:\n')
        elif last or sys.last_traceback:
            f.write("\n\n(For wizards only) ")
        if last:
            if type(last) <> types.ListType or len(last) < 3:
                if (hasattr(sys,"last_traceback") and sys.last_traceback):
                    last = [sys.last_type ,sys.last_value,sys.last_traceback]
            if type(last) == types.ListType:
                traceback.print_exception(last[0],last[1],last[2],
                                          None,f)
         #f.flush()
        if f is sys.__stderr__:
            s = ' (see the file "wxPyNonWindowingErrors.txt")'
        else:
            s = ""
        f.write("Please contact the author with a copy of this\n"
                "message%s.\n" % s)
        #f.flush()
    fl.close()
    if fatal and stderr is sys.__stderr__:
        if sys.platform in ["windows",'nt',"win32"]:
            sys.__stderr__.write(
                "\nYou may have to manually close this window to exit.")
        sys.exit()

class wxPythonRExec (rexec.RExec):
    def __init__(self,securityhole=0,*args,**kwargs):
        apply(rexec.RExec.__init__, (self,) + args, kwargs)
        if securityhole:
            self.ok_builtin_modules = self.ok_builtin_modules + \
            ('wxPython', 'wxPython.wxc','wxPython.wx','wxPython.misc',
             'wxPython.misc2', 'wxPython.windows', 'wxPython.gdi',
             'wxPython.clip_dnd', 'wxPython.events', 'wxPython.mdi',
             'wxPython.frames', 'wxPython.stattool', 'wxPython.controls',
             'wxPython.controls2', 'wxPython.windows2', 'wxPython.cmndlgs',
             'wxPython.windows3', 'wxPython.image', 'wxPython.printfw',
             'wxc','misc', 'misc2', 'windows', 'gdi', 'clip_dnd', 'events',
             'mdi', 'frames', 'stattool', 'controls', 'controls2', 'windows2',
             'cmndlgs', 'windows3', 'image', 'printfw', 'wx')
            # possible security hole!

##def wxPyFatalError(msg,frame=None,**kwargs):
##    kwargs.update({'fatal' : 1})
##    apply(wxPyNonFatalError,
##          (frame,msg),
##          kwargs)

class wxPyNonFatalErrorDialogWithTraceback(wxDialog):
    this_exception = 0
    populate_function = populate_wxNonFatalErrorDialogWithTraceback
    no_continue_button = false
    fatal = false
    modal = true
    exitjustreturns = false # really only for testing!

    def __init__(self, parent, id,
                 pos = wxPyDefaultPosition, size = wxPyDefaultSize,
                 style = wxDEFAULT_DIALOG_STYLE,
                 programname = "Python program",
                 version = "?",
                 mailto = None,
                 whendismissed = "",
                 disable_exit_button = false):

        if _debug:
            sys.stdout.write('\nwxPyNonFatalErrorWindow.__init__: '
                                        'STARTING...\n\n')

        if self.fatal:
            whetherNF = ""
        else:
            whetherNF = "non-"
        title = "A (%sfatal) error has occurred in %s!"\
                % (whetherNF,programname)
        self.programname = programname # save for later use
        self.mailto = mailto # save for later use
        self.parent = parent # save for later use
        self.whendismissed = whendismissed # save for later use
        self.dialogtitle = title # save for later use

        wxDialog.__init__(self, parent, id, title, pos, size, style)

        self.topsizer = self.populate_function( false,true )

        self.SetProgramName(programname)
        self.SetVersion(version)

        if not self.no_continue_button:
            EVT_BUTTON(self, wxPyError_ID_CONTINUE, self.OnContinue)
        if mailto:
            disable_mail_button = 0
        else:
            disable_mail_button = 1
        if not disable_mail_button:
            EVT_BUTTON(self, wxPyError_ID_MAIL, self.OnMail)
        else:
            self.GetMailButton().Enable(false)
        if not disable_exit_button:
            EVT_BUTTON(self, wxPyError_ID_EXIT, self.OnExit)

        self.nonwindowingerror = wxPyNonWindowingErrorHandler(file=sys.__stderr__,
                                                              fatal=0)

        if _debug:
            sys.stdout.write('\nwxPyNonFatalErrorWindow.__init__: '
                             'DONE.\n\n')
    def GetExtraInformation(self):
        return self.extraexceptioninformation

    def SetExtraInformation(self,value):
        self.extraexceptioninformation = value
        c = self.GetExtraInformationCtrl()
        if c is not None:
            c.SetLabel(str(value))
            self.topsizer.Layout()

    def GetExtraInformationCtrl(self):
        return self.FindWindowById(wxPyError_ID_EXTRAINFORMATION)

    def GetExceptionName(self):
        return str(self.exceptiontype)

    def SetExceptionName(self,value):
        self.exceptiontype = str(value)
        c = self.GetExceptionNameCtrl()
        if c is not None:
            c.SetLabel(str(value))
            self.topsizer.Layout()

    def GetExceptionNameCtrl(self):
        return self.FindWindowById(wxPyError_ID_EXCEPTIONNAME)

    def GetTraceback(self):
        try:
            return self.traceback
        except AttributeError:
            return None

    def SetTraceback(self,value):
        self.traceback = value
        c = self.GetTracebackCtrl()
        if c is not None:
            s,cs = c.GetSize(), c.GetClientSize()
            if value[-1] == '\n':
                value = value[:-1]
            if _debug:
                print "%s.SetTraceback(): ...SetValue('%s' (^M=\\r; ^J=\\n))"\
                      % (self,string.replace(value,'\n',"^J"))
            c.SetValue(value)

            # Despite using the wxADJUST_MINSIZE flag in the
            # appropriate AddWindow method of the sizer, this doesn't
            # size the control appropriately... evidently the control's
            # GetBestSize method is not returning the "correct"
            # value...  So we perform a rather ugly "fix"... note that
            # this also requires that we remove the wxADJUST_MINSIZE
            # flag from the AddWindow method of the sizer containing
            # the wxTextCtrl, which adds the wxTextCtrl...  (this
            # amounts, as of wxDesigner 2.6, to only a few mouse
            # clicks...)

            if _debug:
                size = c.GetBestSize()
                print "%s.SetTraceback(): %s.GetBestSize() = (%s,%s)"\
                      % (self,c,size.width,size.height)
            w,h = 0,0
            for v in string.split(value,"\n"):
                pw,ph,d,e = t = c.GetFullTextExtent(v)
                if _debug:
                    print v, t
                h = h + ph + e# + d# + e
                pw = pw + wxSystemSettings_GetSystemMetric(wxSYS_VSCROLL_X)
                if pw > w:
                    w = pw
            w = w + s.width - cs.width
            h = h + s.height - cs.height
            if _debug:
                print "%s.SetTraceback(): calculated w,h =" % c,\
                      w,h,"and sys.platform = '%s'" % sys.platform
            self.sizerAroundText.SetItemMinSize (c,w,h)
            c.SetSize ((w,h))
            c.SetSizeHints (w,h,w,h)
            c.Refresh()#.SetAutoLayout(FALSE)

            #^ the reason we need the above seems to be to replace the
            #faulty GetBestSize of wxTextCtrl...
            #self.sizerAroundText.Layout()
            self.topsizer.Layout()

    def GetTracebackCtrl(self):
        return self.FindWindowById(wxPyError_ID_TEXTCTRL)

    def GetVersion(self):
        return self.version

    def SetVersion(self,value):
        self.version = value
        c = self.GetVersionNumberCtrl()
        if c is not None:
            c.SetLabel(value)
            self.topsizer.Layout()

    def GetVersionNumberCtrl(self):
        return self.FindWindowById(wxPyError_ID_VERSIONNUMBER)

    def GetProgramName(self):
        return self.programname

    def SetProgramName(self,value):
        self.programname = value
        c = self.GetProgramNameCtrl()
        if c is not None:
            c.SetLabel(value)
            self.topsizer.Layout()

    def GetProgramNameCtrl(self):
        return self.FindWindowById(wxPyError_ID_PROGRAMNAME)

    def GetContinueButton(self):
        return self.FindWindowById(wxPyError_ID_CONTINUE)

    def GetMailButton(self):
        return self.FindWindowById(wxPyError_ID_MAIL)

    def GetExitButton(self):
        return self.FindWindowById(wxPyError_ID_EXIT)

    # write handler (which is really the guts of the thing...
    # [Note that this doesn't use sys.excepthook because I already had a
    # working body of code...

    def write(self,s):
        if self.this_exception is not sys.last_traceback:
          if not wxThread_IsMain():
              # Aquire the GUI mutex before making GUI calls.  Mutex is released
              # when locker is deleted at the end of this function.
              locker = wxMutexGuiLocker()

          self.this_exception = sys.last_traceback
            # this is meant to be done once per traceback's sys.stderr.write's
            # - on the first in fact.....
          try:
            #from wxPython.wx import wxBell
            wxBell()

            if _debug:
                sys.stdout.write(
                    'in  %s.write(): ' % self)

            self.exceptiontype = sys.last_type
            self.extraexceptioninformation = sys.last_value
            c = cStringIO.StringIO()
            traceback.print_last(None,c)
            self.traceback = c.getvalue()

            if _debug:
                #import traceback
                traceback.print_last(None,sys.stdout)

            self.SetExceptionName(str(self.exceptiontype))
            self.SetExtraInformation(str(self.extraexceptioninformation))
            self.SetTraceback(str(self.traceback))

            self.topsizer.Fit(self)
            self.topsizer.SetSizeHints(self)

            if self.modal:
                self.ShowModal()
            else:
                self.Show(true)

          except:
              if not locals().has_key("c"):
                  c = cStringIO.StringIO()
                  c.write("[Exception occurred before data from "
                          "sys.last_traceback available]")
##              c2 = cStringIO.StringIO()
##              traceback.print_exception(None,c2)
              wxPyNonWindowingError("Warning: "
                                "a %s error was encountered trying to "
                                "handle the exception\n%s\nThis was:"#%s\n"
                                % (sys.exc_type, c.getvalue()),#, c2.getvalue()),
                                stderr=sys.__stderr__,
                                last=0)


    # button handlers:

    def OnContinue(self, event):
     try:
        if self.whendismissed:
            parent = self.parent # so whendismissed can refer to "parent"
            if 1:
                if _debug:
                    sys.stdout.write("exec '''%s''': "
                                         % (self.whendismissed))
                exec self.whendismissed
                if _debug: print "\n",
            else:
                if _debug:
                    sys.stdout.write("wxPythonRExec(%s).r_exec('''%s'''): "
                                         % (self.securityhole,
                                            self.whendismissed))
                wxPythonRExec(self.securityhole).r_exec(self.whendismissed)
                if _debug: print "\n",
        if self.modal:
            self.EndModal(wxID_OK)
        else:
            self.Close ()
        if _debug: print "reimporting ",
        for  m in sys.modules.values():
            if m and m.__dict__["__name__"][0] in string.uppercase:#hack!
                if _debug:
                    print m.__dict__["__name__"],
                reload (m)
                if _debug:
                    print ' ',
        if _debug:
            print '\nENDING %s.OnContinue()..\n\n' % (self,),
     except:
      wxPyNonWindowingError("Warning: the following exception information"
                        " may not be the full story.. (because "
                        "a %s(%s) error was encountered trying to "
                        "handle the exception)\n\n"
                        % tuple(sys.exc_info()[:2]),
                        stderr=sys.__stderr__,
                        last=0)

    MessageTemplate = "<head>"\
                 "</head>"\
                 '<body text="#000000" bgcolor="#FFFFFF">'\
                 "<p>"\
                 "<i><b>Hello,</b></i>\n<p>\n"\
                 '<p><h2><font color="#CC6C00">%(programname)s</font>'\
                 " error.</h2>\n"\
                 "I encountered the following error when running your "\
                 'program <font color="#CC6C00">%(programname)s</font>,'\
                 "at %(date)s.\n<p>\n"\
                 "<p>"\
                 "<h2>Traceback (automatically generated):</h2>\n"\
                 '<p><font size="-1">\n<pre>%(traceback)s</pre>\n<p></font><p>'\
                 "\n<p>\n<h2>More information follows:</h2>\n<p>\n"\
                 '<font color="#CC6C00">'\
                 '<i>[Insert more '\
                 "information about the error here, such as what you were "\
                 "trying to do at the time of the error.  Please "\
                 "understand that failure to fill in this field will be "\
                 "interpreted as an invitation to consign this e-mail "\
                 "straight to the trash can!]\n</i><p>\n"\
                 "</font><p>\n"\
                 '<i><b>Yours sincerely,</b></i>\n<p>'\
                 '<font color="#CC6C00">'\
                 "[insert your name here]\n"\
                 "</font>"\
                 "</body>"

    def OnMail(self,event):
      try:
        if _debug:
            print 'Attempting to write mail message.\n',
        gmtdate = time.asctime(time.gmtime(time.time())) + ' GMT'
        tm = time.localtime(time.time())
        date = time.asctime(tm) + ' ' +\
               time.strftime("%Z",tm)
        programname = self.programname
        traceback = self.traceback
        mailto = self.mailto
        message = self.MessageTemplate % vars()
        subject = "Un-caught exception when running %s." % programname
        if _debug:
            print 'message:',message
            print 'subject:,',subject
            print 'sent to:',mailto
        mailfrom = self.FindWindowById (wxPyError_ID_ADDRESS)
        if mailfrom:
            mailfrom = mailfrom.GetValue()
        if _startmailerwithhtml(mailto,subject,message,text="",mailfrom=mailfrom):
          if not (hasattr(self,"fatal") and self.fatal):
            self.OnContinue(event) # if ok, then act as if "Continue" selected
      except:
              wxPyNonWindowingError("Warning: the following exception information"
                                " may not be the full story... (because "
                                "a %s error was encountered trying to "
                                "handle the original exception)\n\n"#%s"
                                % (sys.exc_type,),#self.msg),
                                stderr=sys.__stderr__,
                                last=0)

    def OnExit(self, event):
        if self.IsModal():
            self.EndModal(wxID_CANCEL)
        if self.exitjustreturns:
            return
        wxGetApp().ExitMainLoop()

##         if isinstance(sys.stderr,wxPyNonFatalErrorDialogWithTraceback):
##             if sys.stderr == self:
##                 selfdestroyed = 1
##             sys.stderr.Destroy()
##         sys.stderr = wxPyNonWindowingErrorHandler(sys.__stderr__)
##         wxSafeYield() # so remaining events are processed...
##         if self.parent not in [None,NULL]:
##             self.parent.Destroy()
##         elif "selfdestroyed" not  in locals().keys():
##             self.Destroy()

    def SetText(self,number,string):
        self.FindWindowById(eval("wxPyError_ID_TEXT%d"
                                 % number)).SetLabel(string)
        self.topsizer.Layout()

class wxPyFatalErrorDialogWithTraceback(wxPyNonFatalErrorDialogWithTraceback):
    populate_function = populate_wxFatalErrorDialogWithTraceback
    no_continue_button = true
    fatal = true

class wxPyNonFatalErrorDialog(wxPyNonFatalErrorDialogWithTraceback):
    populate_function = populate_wxNonFatalErrorDialog

class wxPyFatalErrorDialog(wxPyFatalErrorDialogWithTraceback):
    populate_function = populate_wxFatalErrorDialog


def _startmailerwithhtml(mailto,subject,html,text,mailfrom=None):
    if sys.platform in ["windows",'nt',"win32"] and sys.hexversion >= 0x02000000:
        name = tempfile.mktemp(".eml")
        f = open(name,"w")
        f.write(_createhtmlmail(html,text,subject,to=mailto,
                               mailfrom=mailfrom))
        f.close()
        try:
            os.startfile(name)
        except WindowsError:
            # probably no association with eml files
            return _writehtmlmessage(mailto,subject,html,text,mailfrom=mailfrom)
        return 1
    else:
        return _writehtmlmessage(mailto,subject,html,text,mailfrom=mailfrom)

def _writehtmlmessage(mailto,subject,html,text=None,parent=None,mailfrom=None):
    dlg = wxFileDialog (parent,
                        "Please choose a a file to save the message to...",
                        ".",
                        "bug-report",
                        "HTML files (*.htm,*.html)|*.htm,*.html|"
                        "All files (*)|*",
                        wxSAVE | wxHIDE_READONLY)
    if dlg.ShowModal() <> wxID_CANCEL:
        f = open(dlg.GetPath(),"w")
        dlg.Destroy()
        f.write(_createhtmlmail(html,text,subject,to=mailto,mailfrom=mailfrom))
        f.close()
        return 1
    else:
        return 0

# PLEASE NOTE THAT THE CODE BELOW FOR STARTING MAILER WITH A GIVEN
#(HTML) MESSAGE IS BY ART GILLESPIE [with slight modifications by yours truly].

def _createhtmlmail (html, text, subject, to=None, mailfrom=None):
    """Create a mime-message that will render HTML in popular
       MUAs, text in better ones (if indeed text is not untrue (e.g. None)
       """
    # imported above #import MimeWriter, mimetools, cStringIO

    out = cStringIO.StringIO() # output buffer for our message
    htmlin = cStringIO.StringIO(html)
    if text:
            txtin = cStringIO.StringIO(text)

    writer = MimeWriter.MimeWriter(out)
    #
    # set up some basic headers... we put subject here
    # because smtplib.sendmail expects it to be in the
    # message body
    #
    if mailfrom:
            writer.addheader("From", mailfrom)
            #writer.addheader("Reply-to", mailfrom)
    writer.addheader("Subject", subject)
    if to:
            writer.addheader("To", to)
    writer.addheader("MIME-Version", "1.0")
    #
    # start the multipart section of the message
    # multipart/alternative seems to work better
    # on some MUAs than multipart/mixed
    #
    writer.startmultipartbody("alternative")
    writer.flushheaders()
    #
    # the plain text section
    #
    if text:
        subpart = writer.nextpart()
        subpart.addheader("Content-Transfer-Encoding", "quoted-printable")
        pout = subpart.startbody("text/plain", [("charset", 'us-ascii')])
        mimetools.encode(txtin, pout, 'quoted-printable')
        txtin.close()
    #
    # start the html subpart of the message
    #
    subpart = writer.nextpart()
    subpart.addheader("Content-Transfer-Encoding", "quoted-printable")
    pout = subpart.startbody("text/html", [("charset", 'us-ascii')])
    mimetools.encode(htmlin, pout, 'quoted-printable')
    htmlin.close()
    #
    # Now that we're done, close our writer and
    # return the message body
    #
    writer.lastpart()
    msg = out.getvalue()
    out.close()
    return msg

def _sendmail(mailto,subject,html,text):# currently unused
    """For illustration only--this function is not actually used."""
    message = _createhtmlmail(html, text, subject)
    server = smtplib.SMTP("localhost")
    server.sendmail(mailto, subject, message)
    server.quit()
