/////////////////////////////////////////////////////////////////////////////
// Name:        tex2rtf.cpp
// Purpose:     Converts Latex to linear/WinHelp RTF, HTML, wxHelp.
// Author:      Julian Smart
// Modified by: Wlodzimiez ABX Skiba 2003/2004 Unicode support
//              Ron Lee
// Created:     7.9.93
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #ifndef NO_GUI
        #include "wx/menu.h"
        #include "wx/textctrl.h"
        #include "wx/filedlg.h"
        #include "wx/msgdlg.h"
    #endif
#endif

#ifndef NO_GUI
    #include "wx/timer.h"
    #include "wx/help.h"
    #include "wx/cshelp.h"
    #include "wx/helphtml.h"
    #ifdef __WXMSW__
        #include "wx/msw/helpchm.h"
    #else
        #include "wx/html/helpctrl.h"
    #endif
#endif // !NO_GUI

#if wxUSE_IOSTREAMH
#include <iostream.h>
#include <fstream.h>
#else
#include <iostream>
#include <fstream>
#endif

#include <ctype.h>
#include <stdlib.h>
#include "tex2any.h"
#include "tex2rtf.h"
#include "rtfutils.h"
#include "symbols.h"

#if (defined(__WXGTK__) || defined(__WXMOTIF__) || defined(__WXMAC__) || defined(__WXX11__)) && !defined(NO_GUI)
#include "tex2rtf.xpm"
#endif

#if !WXWIN_COMPATIBILITY_2_4
static inline wxChar* copystring(const wxChar* s)
    { return wxStrcpy(new wxChar[wxStrlen(s) + 1], s); }
#endif

const float versionNo = TEX2RTF_VERSION_NUMBER;

TexChunk *currentMember = NULL;
bool startedSections = false;
wxChar *contentsString = NULL;
bool suppressNameDecoration = false;
bool OkToClose = true;
int passNumber = 1;
unsigned long errorCount = 0;

#ifndef NO_GUI

extern wxChar *BigBuffer;
extern wxChar *TexFileRoot;
extern wxChar *TexBibName;         // Bibliography output file name
extern wxChar *TexTmpBibName;      // Temporary bibliography output file name
extern wxList ColourTable;
extern TexChunk *TopLevel;

#if wxUSE_HELP
wxHelpControllerBase *HelpInstance = NULL;
#endif // wxUSE_HELP

#ifdef __WXMSW__
static wxChar *ipc_buffer = NULL;
static wxChar Tex2RTFLastStatus[100];
Tex2RTFServer *TheTex2RTFServer = NULL;
#endif // __WXMSW__

#endif // !NO_GUI

wxChar *bulletFile = NULL;

FILE *Contents = NULL;   // Contents page
FILE *Chapters = NULL;   // Chapters (WinHelp RTF) or rest of file (linear RTF)
FILE *Sections = NULL;
FILE *Subsections = NULL;
FILE *Subsubsections = NULL;
FILE *Popups = NULL;
FILE *WinHelpContentsFile = NULL;

wxChar *InputFile = NULL;
wxChar *OutputFile = NULL;
wxChar *MacroFile = copystring(_T("tex2rtf.ini"));

wxChar *FileRoot = NULL;
wxChar *ContentsName = NULL;    // Contents page from last time around
wxChar *TmpContentsName = NULL; // Current contents page
wxChar *TmpFrameContentsName = NULL; // Current frame contents page
wxChar *WinHelpContentsFileName = NULL; // WinHelp .cnt file
wxChar *RefFileName = NULL;         // Reference file name

wxChar *RTFCharset = copystring(_T("ansi"));

#ifdef __WXMSW__
int BufSize = 100;             // Size of buffer in K
#else
int BufSize = 500;
#endif

bool Go(void);
void ShowOptions(void);

wxChar wxTex2RTFBuffer[1500];

#ifdef NO_GUI
    IMPLEMENT_APP_CONSOLE(MyApp)
#else
    wxMenuBar *menuBar = NULL;
    MyFrame *frame = NULL;
    // DECLARE_APP(MyApp)
    IMPLEMENT_APP(MyApp)
#endif

// `Main program' equivalent, creating windows and returning main app frame
bool MyApp::OnInit()
{
  // Use default list of macros defined in tex2any.cc
  DefineDefaultMacros();
  AddMacroDef(ltHARDY, _T("hardy"), 0);

  FileRoot = new wxChar[300];
  ContentsName = new wxChar[300];
  TmpContentsName = new wxChar[300];
  TmpFrameContentsName = new wxChar[300];
  WinHelpContentsFileName = new wxChar[300];
  RefFileName = new wxChar[300];

  ColourTable.DeleteContents(true);

  int n = 1;

  // Read input/output files
  if (argc > 1)
  {
    if (argv[1][0] != '-')
    {
      InputFile = argv[1];
      n ++;

      if (argc > 2)
      {
        if (argv[2][0] != '-')
        {
          OutputFile = argv[2];
          n ++;
        }
      }
    }
  }

#ifdef NO_GUI
  if (!InputFile || !OutputFile)
  {
    wxSTD cout << "Tex2RTF: input or output file is missing.\n";
    ShowOptions();
    exit(1);
  }
#endif

  if (InputFile)
  {
    TexPathList.EnsureFileAccessible(InputFile);
  }
  if (!InputFile || !OutputFile)
    isInteractive = true;

  int i;
  for (i = n; i < argc;)
  {
    if (wxStrcmp(argv[i], _T("-winhelp")) == 0)
    {
      i ++;
      convertMode = TEX_RTF;
      winHelp = true;
    }
#ifndef NO_GUI
    else if (wxStrcmp(argv[i], _T("-interactive")) == 0)
    {
      i ++;
      isInteractive = true;
    }
#endif
    else if (wxStrcmp(argv[i], _T("-sync")) == 0)  // Don't yield
    {
      i ++;
      isSync = true;
    }
    else if (wxStrcmp(argv[i], _T("-rtf")) == 0)
    {
      i ++;
      convertMode = TEX_RTF;
    }
    else if (wxStrcmp(argv[i], _T("-html")) == 0)
    {
      i ++;
      convertMode = TEX_HTML;
    }
    else if (wxStrcmp(argv[i], _T("-xlp")) == 0)
    {
      i ++;
      convertMode = TEX_XLP;
    }
    else if (wxStrcmp(argv[i], _T("-twice")) == 0)
    {
      i ++;
      runTwice = true;
    }
    else if (wxStrcmp(argv[i], _T("-macros")) == 0)
    {
      i ++;
      if (i < argc)
      {
        MacroFile = copystring(argv[i]);
        i ++;
      }
    }
    else if (wxStrcmp(argv[i], _T("-bufsize")) == 0)
    {
      i ++;
      if (i < argc)
      {
        BufSize = wxAtoi(argv[i]);
        i ++;
      }
    }
    else if (wxStrcmp(argv[i], _T("-charset")) == 0)
    {
      i ++;
      if (i < argc)
      {
        wxChar *s = argv[i];
        i ++;
        if (wxStrcmp(s, _T("ansi")) == 0 || wxStrcmp(s, _T("pc")) == 0 || wxStrcmp(s, _T("mac")) == 0 ||
            wxStrcmp(s, _T("pca")) == 0)
          RTFCharset = copystring(s);
        else
        {
          OnError(_T("Incorrect argument for -charset"));
          return false;
        }
      }
    }
    else if (wxStrcmp(argv[i], _T("-checkcurleybraces")) == 0)
    {
      i ++;
      checkCurleyBraces = true;
    }
    else if (wxStrcmp(argv[i], _T("-checksyntax")) == 0)
    {
      i ++;
      checkSyntax = true;
    }
    else
    {
      wxString buf;
      buf.Printf(_T("Invalid switch %s.\n"), argv[i]);
      OnError((wxChar *)buf.c_str());
#ifdef NO_GUI
      ShowOptions();
      exit(1);
#endif
      return false;
    }
  }

#if defined(__WXMSW__) && !defined(NO_GUI)
  wxDDEInitialize();
  Tex2RTFLastStatus[0] = 0; // DDE connection return value
  TheTex2RTFServer = new Tex2RTFServer;
  TheTex2RTFServer->Create(_T("TEX2RTF"));
#endif

  TexInitialize(BufSize);
  ResetContentsLevels(0);

#ifndef NO_GUI

  if (isInteractive)
  {
    wxChar buf[100];

    // Create the main frame window
    frame = new MyFrame(NULL, wxID_ANY, _T("Tex2RTF"), wxDefaultPosition, wxSize(400, 300));
    frame->CreateStatusBar(2);

    // Give it an icon
    // TODO: uncomment this when we have tex2rtf.xpm
    frame->SetIcon(wxICON(tex2rtf));

    if (InputFile)
    {
      wxSnprintf(buf, sizeof(buf), _T("Tex2RTF [%s]"), wxFileNameFromPath(InputFile));
      frame->SetTitle(buf);
    }

    // Make a menubar
    wxMenu *file_menu = new wxMenu;
    file_menu->Append(TEX_GO, _T("&Go"), _T("Run converter"));
    file_menu->Append(TEX_SET_INPUT, _T("Set &Input File"), _T("Set the LaTeX input file"));
    file_menu->Append(TEX_SET_OUTPUT, _T("Set &Output File"), _T("Set the output file"));
    file_menu->AppendSeparator();
    file_menu->Append(TEX_VIEW_LATEX, _T("View &LaTeX File"), _T("View the LaTeX input file"));
    file_menu->Append(TEX_VIEW_OUTPUT, _T("View Output &File"), _T("View output file"));
    file_menu->Append(TEX_SAVE_FILE, _T("&Save log file"), _T("Save displayed text into file"));
    file_menu->AppendSeparator();
    file_menu->Append(TEX_QUIT, _T("E&xit"), _T("Exit Tex2RTF"));

    wxMenu *macro_menu = new wxMenu;

    macro_menu->Append(TEX_LOAD_CUSTOM_MACROS, _T("&Load Custom Macros"), _T("Load custom LaTeX macro file"));
    macro_menu->Append(TEX_VIEW_CUSTOM_MACROS, _T("View &Custom Macros"), _T("View custom LaTeX macros"));

    wxMenu *mode_menu = new wxMenu;

    mode_menu->Append(TEX_MODE_RTF, _T("Output linear &RTF"), _T("Wordprocessor-compatible RTF"));
    mode_menu->Append(TEX_MODE_WINHELP, _T("Output &WinHelp RTF"), _T("WinHelp-compatible RTF"));
    mode_menu->Append(TEX_MODE_HTML, _T("Output &HTML"), _T("HTML World Wide Web hypertext file"));
    mode_menu->Append(TEX_MODE_XLP, _T("Output &XLP"), _T("wxHelp hypertext help file"));

    wxMenu *options_menu = new wxMenu;

    options_menu->Append(TEX_OPTIONS_CURLEY_BRACE, _T("Curley brace matching"), _T("Checks for mismatched curley braces"),true);
    options_menu->Append(TEX_OPTIONS_SYNTAX_CHECKING, _T("Syntax checking"), _T("Syntax checking for common errors"),true);

    options_menu->Check(TEX_OPTIONS_CURLEY_BRACE, checkCurleyBraces);
    options_menu->Check(TEX_OPTIONS_SYNTAX_CHECKING, checkSyntax);

    wxMenu *help_menu = new wxMenu;

    help_menu->Append(TEX_HELP, _T("&Help"), _T("Tex2RTF Contents Page"));
    help_menu->Append(TEX_ABOUT, _T("&About Tex2RTF"), _T("About Tex2RTF"));

    menuBar = new wxMenuBar;
    menuBar->Append(file_menu, _T("&File"));
    menuBar->Append(macro_menu, _T("&Macros"));
    menuBar->Append(mode_menu, _T("&Conversion Mode"));
    menuBar->Append(options_menu, _T("&Options"));
    menuBar->Append(help_menu, _T("&Help"));

    frame->SetMenuBar(menuBar);
    frame->textWindow = new wxTextCtrl(frame, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_READONLY|wxTE_MULTILINE);

    (*frame->textWindow) << _T("Welcome to Tex2RTF.\n");
//    ShowOptions();

#if wxUSE_HELP
#if wxUSE_MS_HTML_HELP && !defined(__WXUNIVERSAL__)
    HelpInstance = new wxCHMHelpController;
#else
    HelpInstance = new wxHtmlHelpController;
#endif
    HelpInstance->Initialize(_T("tex2rtf"));
#endif // wxUSE_HELP

    /*
     * Read macro/initialisation file
     *
     */

    wxString path = TexPathList.FindValidPath(MacroFile);
    if (path != _T(""))
      ReadCustomMacros((wxChar *)path.c_str());

    wxStrcpy(buf, _T("In "));

    if (winHelp && (convertMode == TEX_RTF))
      wxStrcat(buf, _T("WinHelp RTF"));
    else if (!winHelp && (convertMode == TEX_RTF))
      wxStrcat(buf, _T("linear RTF"));
    else if (convertMode == TEX_HTML) wxStrcat(buf, _T("HTML"));
    else if (convertMode == TEX_XLP) wxStrcat(buf, _T("XLP"));
      wxStrcat(buf, _T(" mode."));
    frame->SetStatusText(buf, 1);

    frame->Show(true);
    return true;
  }
  else
#endif // NO_GUI
  {
    /*
     * Read macro/initialisation file
     *
     */

    wxString path = TexPathList.FindValidPath(MacroFile);
    if (path != _T(""))
      ReadCustomMacros((wxChar*)path.c_str());

    Go();
    if (runTwice) 
    {
        Go();
    }
#ifdef NO_GUI
    return true;
#else
    OnExit(); // Do cleanup since OnExit won't be called now
    return false;
#endif
  }
}

#ifndef NO_GUI
int MyApp::OnExit()
{
  wxNode *node = CustomMacroList.GetFirst();
  while (node)
  {
    CustomMacro *macro = (CustomMacro *)node->GetData();
    delete macro;
    delete node;
    node = CustomMacroList.GetFirst();
  }
  MacroDefs.BeginFind();
  wxHashTable::Node* mNode = MacroDefs.Next();
  while (mNode)
  {
    TexMacroDef* def = (TexMacroDef*) mNode->GetData();
    delete def;
    mNode = MacroDefs.Next();
  }
  MacroDefs.Clear();
#ifdef __WXMSW__
  delete TheTex2RTFServer;
  wxDDECleanUp();
#endif

#if wxUSE_HELP
  delete HelpInstance;
#endif // wxUSE_HELP

    if (BigBuffer)
    {
      delete BigBuffer;
      BigBuffer = NULL;
    }
    if (currentArgData)
    {
      delete currentArgData;
      currentArgData = NULL;
    }
    if (TexFileRoot)
    {
      delete TexFileRoot;
      TexFileRoot = NULL;
    }
    if (TexBibName)
    {
      delete TexBibName;
      TexBibName = NULL;
    }
    if (TexTmpBibName)
    {
      delete TexTmpBibName;
      TexTmpBibName = NULL;
    }
    if (FileRoot)
    {
      delete FileRoot;
      FileRoot = NULL;
    }
    if (ContentsName)
    {
      delete ContentsName;
      ContentsName = NULL;
    }
    if (TmpContentsName)
    {
      delete TmpContentsName;
      TmpContentsName = NULL;
    }
    if (TmpFrameContentsName)
    {
      delete TmpFrameContentsName;
      TmpFrameContentsName = NULL;
    }
    if (WinHelpContentsFileName)
    {
      delete WinHelpContentsFileName;
      WinHelpContentsFileName = NULL;
    }
    if (RefFileName)
    {
      delete RefFileName;
      RefFileName = NULL;
    }
    if (TopLevel)
    {
      delete TopLevel;
      TopLevel = NULL;
    }
    if (MacroFile)
    {
      delete MacroFile;
      MacroFile = NULL;
    }
    if (RTFCharset)
    {
      delete RTFCharset;
      RTFCharset = NULL;
    }

    delete [] PageStyle;
    delete [] BibliographyStyleString;
    delete [] DocumentStyleString;
    delete [] bitmapMethod;
    delete [] backgroundColourString;
    delete [] ContentsNameString;
    delete [] AbstractNameString;
    delete [] GlossaryNameString;
    delete [] ReferencesNameString;
    delete [] FiguresNameString;
    delete [] TablesNameString;
    delete [] FigureNameString;
    delete [] TableNameString;
    delete [] IndexNameString;
    delete [] ChapterNameString;
    delete [] SectionNameString;
    delete [] SubsectionNameString;
    delete [] SubsubsectionNameString;
    delete [] UpNameString;
    if (winHelpTitle)
      delete[] winHelpTitle;

  // TODO: this simulates zero-memory leaks!
  // Otherwise there are just too many...
#ifndef __WXGTK__
#if (defined(__WXDEBUG__) && wxUSE_MEMORY_TRACING) || wxUSE_DEBUG_CONTEXT
  wxDebugContext::SetCheckpoint();
#endif
#endif

  return 0;
}
#endif
void ShowOptions(void)
{
    wxChar buf[100];
    wxSnprintf(buf, sizeof(buf), _T("Tex2RTF version %.2f"), versionNo);
    OnInform(buf);
    OnInform(_T("Usage: tex2rtf [input] [output] [switches]\n"));
    OnInform(_T("where valid switches are"));
#ifndef NO_GUI
    OnInform(_T("    -interactive"));
#endif
    OnInform(_T("    -bufsize <size in K>"));
    OnInform(_T("    -charset <pc | pca | ansi | mac> (default ansi)"));
    OnInform(_T("    -twice"));
    OnInform(_T("    -sync"));
    OnInform(_T("    -checkcurleybraces"));
    OnInform(_T("    -checksyntax"));
    OnInform(_T("    -macros <filename>"));
    OnInform(_T("    -winhelp"));
    OnInform(_T("    -rtf"));
    OnInform(_T("    -html"));
    OnInform(_T("    -xlp\n"));
}

#ifndef NO_GUI

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_CLOSE(MyFrame::OnCloseWindow)
    EVT_MENU(TEX_QUIT, MyFrame::OnExit)
    EVT_MENU(TEX_GO, MyFrame::OnGo)
    EVT_MENU(TEX_SET_INPUT, MyFrame::OnSetInput)
    EVT_MENU(TEX_SET_OUTPUT, MyFrame::OnSetOutput)
    EVT_MENU(TEX_SAVE_FILE, MyFrame::OnSaveFile)
    EVT_MENU(TEX_VIEW_LATEX, MyFrame::OnViewLatex)
    EVT_MENU(TEX_VIEW_OUTPUT, MyFrame::OnViewOutput)
    EVT_MENU(TEX_VIEW_CUSTOM_MACROS, MyFrame::OnShowMacros)
    EVT_MENU(TEX_LOAD_CUSTOM_MACROS, MyFrame::OnLoadMacros)
    EVT_MENU(TEX_MODE_RTF, MyFrame::OnModeRTF)
    EVT_MENU(TEX_MODE_WINHELP, MyFrame::OnModeWinHelp)
    EVT_MENU(TEX_MODE_HTML, MyFrame::OnModeHTML)
    EVT_MENU(TEX_MODE_XLP, MyFrame::OnModeXLP)
    EVT_MENU(TEX_OPTIONS_CURLEY_BRACE, MyFrame::OnOptionsCurleyBrace)
    EVT_MENU(TEX_OPTIONS_SYNTAX_CHECKING, MyFrame::OnOptionsSyntaxChecking)
    EVT_MENU(TEX_HELP, MyFrame::OnHelp)
    EVT_MENU(TEX_ABOUT, MyFrame::OnAbout)
END_EVENT_TABLE()

// My frame constructor
MyFrame::MyFrame(wxFrame *frame, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size):
  wxFrame(frame, id, title, pos, size)
{}

void MyFrame::OnCloseWindow(wxCloseEvent& WXUNUSED(event))
{
  if (!stopRunning && !OkToClose)
  {
    stopRunning = true;
    runTwice = false;
    return;
  }
  else if (OkToClose)
  {
    this->Destroy();
  }
}

void MyFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
  Close();
//    this->Destroy();
}

void MyFrame::OnGo(wxCommandEvent& WXUNUSED(event))
{
      passNumber = 1;
      errorCount = 0;
      menuBar->EnableTop(0, false);
      menuBar->EnableTop(1, false);
      menuBar->EnableTop(2, false);
      menuBar->EnableTop(3, false);
      textWindow->Clear();
      Tex2RTFYield(true);
      Go();

      if (stopRunning)
      {
        SetStatusText(_T("Build aborted!"));
        wxString errBuf;
        errBuf.Printf(_T("\nErrors encountered during this pass: %lu\n"), errorCount);
        OnInform((wxChar *)errBuf.c_str());
      }


      if (runTwice && !stopRunning)
      {
        Tex2RTFYield(true);
        Go();
      }
      menuBar->EnableTop(0, true);
      menuBar->EnableTop(1, true);
      menuBar->EnableTop(2, true);
      menuBar->EnableTop(3, true);
}

void MyFrame::OnSetInput(wxCommandEvent& WXUNUSED(event))
{
      ChooseInputFile(true);
}

void MyFrame::OnSetOutput(wxCommandEvent& WXUNUSED(event))
{
      ChooseOutputFile(true);
}

void MyFrame::OnSaveFile(wxCommandEvent& WXUNUSED(event))
{
      wxString s = wxFileSelector(_T("Save text to file"), _T(""), _T(""), _T("txt"), _T("*.txt"));
      if (s != _T(""))
      {
        textWindow->SaveFile(s);
        wxChar buf[350];
        wxSnprintf(buf, sizeof(buf), _T("Saved text to %s"), (const wxChar*) s.c_str());
        frame->SetStatusText(buf, 0);
      }
}

void MyFrame::OnViewOutput(wxCommandEvent& WXUNUSED(event))
{
      ChooseOutputFile();
      if (OutputFile && wxFileExists(OutputFile))
      {
        textWindow->LoadFile(OutputFile);
        wxChar buf[300];
        wxString str(wxFileNameFromPath(OutputFile));
        wxSnprintf(buf, sizeof(buf), _T("Tex2RTF [%s]"), (const wxChar*) str.c_str());
        frame->SetTitle(buf);
      }
}

void MyFrame::OnViewLatex(wxCommandEvent& WXUNUSED(event))
{
      ChooseInputFile();
      if (InputFile && wxFileExists(InputFile))
      {
        textWindow->LoadFile(InputFile);
        wxChar buf[300];
        wxString str(wxFileNameFromPath(OutputFile));
        wxSnprintf(buf, sizeof(buf), _T("Tex2RTF [%s]"), (const wxChar*) str.c_str());
        frame->SetTitle(buf);
      }
}

void MyFrame::OnLoadMacros(wxCommandEvent& WXUNUSED(event))
{
      textWindow->Clear();
      wxString s = wxFileSelector(_T("Choose custom macro file"), wxPathOnly(MacroFile), wxFileNameFromPath(MacroFile), _T("ini"), _T("*.ini"));
      if (s != _T("") && wxFileExists(s))
      {
        MacroFile = copystring(s);
        ReadCustomMacros((wxChar *)s.c_str());
        ShowCustomMacros();
      }
}

void MyFrame::OnShowMacros(wxCommandEvent& WXUNUSED(event))
{
      textWindow->Clear();
      Tex2RTFYield(true);
      ShowCustomMacros();
}

void MyFrame::OnModeRTF(wxCommandEvent& WXUNUSED(event))
{
      convertMode = TEX_RTF;
      winHelp = false;
      InputFile = NULL;
      OutputFile = NULL;
      SetStatusText(_T("In linear RTF mode."), 1);
}

void MyFrame::OnModeWinHelp(wxCommandEvent& WXUNUSED(event))
{
      convertMode = TEX_RTF;
      winHelp = true;
      InputFile = NULL;
      OutputFile = NULL;
      SetStatusText(_T("In WinHelp RTF mode."), 1);
}

void MyFrame::OnModeHTML(wxCommandEvent& WXUNUSED(event))
{
      convertMode = TEX_HTML;
      winHelp = false;
      InputFile = NULL;
      OutputFile = NULL;
      SetStatusText(_T("In HTML mode."), 1);
}

void MyFrame::OnModeXLP(wxCommandEvent& WXUNUSED(event))
{
      convertMode = TEX_XLP;
      InputFile = NULL;
      OutputFile = NULL;
      SetStatusText(_T("In XLP mode."), 1);
}

void MyFrame::OnOptionsCurleyBrace(wxCommandEvent& WXUNUSED(event))
{
    checkCurleyBraces = !checkCurleyBraces;
    if (checkCurleyBraces)
    {
        SetStatusText(_T("Checking curley braces: YES"), 1);
    }
    else
    {
        SetStatusText(_T("Checking curley braces: NO"), 1);
    }
}


void MyFrame::OnOptionsSyntaxChecking(wxCommandEvent& WXUNUSED(event))
{
    checkSyntax = !checkSyntax;
    if (checkSyntax)
    {
        SetStatusText(_T("Checking syntax: YES"), 1);
    }
    else
    {
        SetStatusText(_T("Checking syntax: NO"), 1);
    }
}


void MyFrame::OnHelp(wxCommandEvent& WXUNUSED(event))
{
#if wxUSE_HELP
      HelpInstance->LoadFile();
      HelpInstance->DisplayContents();
#endif // wxUSE_HELP
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
      wxChar buf[300];
#ifdef __WIN32__
      wxChar *platform = _T(" (32-bit)");
#else
#ifdef __WXMSW__
      wxChar *platform = _T(" (16-bit)");
#else
      wxChar *platform = _T("");
#endif
#endif
      wxSnprintf(buf, sizeof(buf), _T("Tex2RTF Version %.2f%s\nLaTeX to RTF, WinHelp, and HTML Conversion\n\n(c) Julian Smart, George Tasker and others, 1999-2002"), versionNo, platform);
      wxMessageBox(buf, _T("About Tex2RTF"));
}

void ChooseInputFile(bool force)
{
  if (force || !InputFile)
  {
    wxString s = wxFileSelector(_T("Choose LaTeX input file"), wxPathOnly(InputFile), wxFileNameFromPath(InputFile), _T("tex"), _T("*.tex"));
    if (s != _T(""))
    {
      // Different file, so clear index entries.
      ClearKeyWordTable();
      ResetContentsLevels(0);
      passNumber = 1;
      errorCount = 0;

      InputFile = copystring(s);
      wxString str = wxFileNameFromPath(InputFile);
      wxString buf;
      buf.Printf(_T("Tex2RTF [%s]"), str.c_str());
      frame->SetTitle((wxChar *)buf.c_str());
      OutputFile = NULL;
    }
  }
}

void ChooseOutputFile(bool force)
{
  wxChar extensionBuf[10];
  wxChar wildBuf[10];
  wxStrcpy(wildBuf, _T("*."));
  wxString path;
  if (OutputFile)
    path = wxPathOnly(OutputFile);
  else if (InputFile)
    path = wxPathOnly(InputFile);

  switch (convertMode)
  {
    case TEX_RTF:
    {
      wxStrcpy(extensionBuf, _T("rtf"));
      wxStrcat(wildBuf, _T("rtf"));
      break;
    }
    case TEX_XLP:
    {
      wxStrcpy(extensionBuf, _T("xlp"));
      wxStrcat(wildBuf, _T("xlp"));
      break;
    }
    case TEX_HTML:
    {
      wxStrcpy(extensionBuf, _T("html"));
      wxStrcat(wildBuf, _T("html"));
      break;
    }
  }
  if (force || !OutputFile)
  {
    wxString s = wxFileSelector(_T("Choose output file"), path, wxFileNameFromPath(OutputFile),
                   extensionBuf, wildBuf);
    if (s != _T(""))
      OutputFile = copystring(s);
  }
}
#endif

bool Go(void)
{
#ifndef NO_GUI
  ChooseInputFile();
  ChooseOutputFile();
#endif

  if (!InputFile || !OutputFile || stopRunning)
    return false;

#ifndef NO_GUI
  if (isInteractive)
  {
    wxChar buf[300];
    wxString str = wxFileNameFromPath(InputFile);

    wxSnprintf(buf, sizeof(buf), _T("Tex2RTF [%s]"), (const wxChar*) str);
    frame->SetTitle(buf);
  }

  wxStartTimer();
#endif

  // Find extension-less filename
  wxStrcpy(FileRoot, OutputFile);
  StripExtension(FileRoot);

  if (truncateFilenames && convertMode == TEX_HTML)
  {
    // Truncate to five characters. This ensures that
    // we can generate DOS filenames such as thing999. But 1000 files
    // may not be enough, of course...
    wxChar* sName = wxFileNameFromPath( FileRoot);  // this Julian's method is non-destructive reference

    if(sName)
      if(wxStrlen( sName) > 5)
        sName[5] = '\0';  // that should do!
  }

  wxSnprintf(ContentsName, 300, _T("%s.con"), FileRoot);
  wxSnprintf(TmpContentsName, 300, _T("%s.cn1"), FileRoot);
  wxSnprintf(TmpFrameContentsName, 300, _T("%s.frc"), FileRoot);
  wxSnprintf(WinHelpContentsFileName, 300, _T("%s.cnt"), FileRoot);
  wxSnprintf(RefFileName, 300, _T("%s.ref"), FileRoot);

  TexPathList.EnsureFileAccessible(InputFile);
  if (!bulletFile)
  {
    wxString s = TexPathList.FindValidPath(_T("bullet.bmp"));
    if (s != _T(""))
    {
      wxString str = wxFileNameFromPath(s);
      bulletFile = copystring(str);
    }
  }

  if (wxFileExists(RefFileName))
    ReadTexReferences(RefFileName);

  bool success = false;

  if (InputFile && OutputFile)
  {
    if (!wxFileExists(InputFile))
    {
      OnError(_T("Cannot open input file!"));
      TexCleanUp();
      return false;
    }
#ifndef NO_GUI
    if (isInteractive)
    {
      wxString buf;
      buf.Printf(_T("Working, pass %d...Click CLOSE to abort"), passNumber);
      frame->SetStatusText((wxChar *)buf.c_str());
    }
#endif
    OkToClose = false;
    OnInform(_T("Reading LaTeX file..."));
    TexLoadFile(InputFile);

    if (stopRunning)
    {
        OkToClose = true;
        return false;
    }

    switch (convertMode)
    {
      case TEX_RTF:
      {
        success = RTFGo();
        break;
      }
      case TEX_XLP:
      {
        success = XLPGo();
        break;
      }
      case TEX_HTML:
      {
        success = HTMLGo();
        break;
      }
    }
  }
  if (stopRunning)
  {
    OnInform(_T("*** Aborted by user."));
    success = false;
    stopRunning = false;
    OkToClose = true;
  }

  if (success)
  {
    WriteTexReferences(RefFileName);
    TexCleanUp();
    startedSections = false;

    wxString buf;
#ifndef NO_GUI
    long tim = wxGetElapsedTime();
    buf.Printf(_T("Finished PASS #%d in %ld seconds.\n"), passNumber, (long)(tim/1000.0));
    OnInform((wxChar *)buf.c_str());

    if (errorCount)
    {
        buf.Printf(_T("Errors encountered during this pass: %lu\n"), errorCount);
        OnInform((wxChar *)buf.c_str());
    }

    if (isInteractive)
    {
      buf.Printf(_T("Done, %d %s."), passNumber, (passNumber > 1) ? _T("passes") : _T("pass"));
      frame->SetStatusText((wxChar *)buf.c_str());
    }
#else
    buf.Printf(_T("Done, %d %s."), passNumber, (passNumber > 1) ? _T("passes") : _T("pass"));
    OnInform((wxChar *)buf.c_str());
    if (errorCount)
    {
        buf.Printf(_T("Errors encountered during this pass: %lu\n"), errorCount);
        OnInform((wxChar *)buf.c_str());
    }
#endif
    passNumber ++;
    errorCount = 0;
    OkToClose = true;
    return true;
  }

  TexCleanUp();
  startedSections = false;

#ifndef NO_GUI
  frame->SetStatusText(_T("Aborted by user."));
#endif // GUI

  OnInform(_T("Sorry, unsuccessful."));
  OkToClose = true;
  return false;
}

void OnError(const wxChar *msg)
{
  wxString msg_string = msg;
  errorCount++;

#ifdef NO_GUI
  wxSTD cerr << "Error: " << msg_string.mb_str() << "\n";
  wxSTD cerr.flush();
#else
  if (isInteractive && frame)
    (*frame->textWindow) << _T("Error: ") << msg << _T("\n");
  else
#ifdef __UNIX__
  {
    wxSTD cerr << "Error: " << msg_string.mb_str() << "\n";
    wxSTD cerr.flush();
  }
#endif

#ifdef __WXMSW__
    wxLogError(msg);
#endif
  Tex2RTFYield(true);
#endif // NO_GUI
}

void OnInform(const wxChar *msg)
{
  wxString msg_string = msg;
#ifdef NO_GUI
  wxSTD cout << msg_string.mb_str() << "\n";
  wxSTD cout.flush();
#else
  if (isInteractive && frame)
    (*frame->textWindow) << msg << _T("\n");
/* This whole block of code is just wrong I think.  It would behave
   completely wrong under anything other than MSW due to the ELSE
   with no statement, and the cout calls would fail under MSW, as
   the code in this block is compiled if !NO_GUI This code has been 
   here since v1.1 of this file too. - gt
  else
#ifdef __WXMSW__
  {
    wxSTD cout << msg_string.mb_str() << "\n";
    wxSTD cout.flush();
  }
#endif
#ifdef __WXMSW__
    {}
#endif
*/
  if (isInteractive)
  {
    Tex2RTFYield(true);
  }
#endif // NO_GUI
}

void OnMacro(int macroId, int no_args, bool start)
{
  switch (convertMode)
  {
    case TEX_RTF:
    {
      RTFOnMacro(macroId, no_args, start);
      break;
    }
    case TEX_XLP:
    {
      XLPOnMacro(macroId, no_args, start);
      break;
    }
    case TEX_HTML:
    {
      HTMLOnMacro(macroId, no_args, start);
      break;
    }
  }
}

bool OnArgument(int macroId, int arg_no, bool start)
{
  switch (convertMode)
  {
    case TEX_RTF:
    {
      return RTFOnArgument(macroId, arg_no, start);
      // break;
    }
    case TEX_XLP:
    {
      return XLPOnArgument(macroId, arg_no, start);
      // break;
    }
    case TEX_HTML:
    {
      return HTMLOnArgument(macroId, arg_no, start);
      // break;
    }
  }
  return true;
}

/*
 * DDE Stuff
 */
#if defined(__WXMSW__) && !defined(NO_GUI)

/*
 * Server
 */

wxConnectionBase *Tex2RTFServer::OnAcceptConnection(const wxString& topic)
{
  if (topic == _T("TEX2RTF"))
  {
    if (!ipc_buffer)
      ipc_buffer = new wxChar[1000];

    return new Tex2RTFConnection(ipc_buffer, 4000);
  }
  else
    return NULL;
}

 /*
  * Connection
  */

Tex2RTFConnection::Tex2RTFConnection(wxChar *buf, int size):wxDDEConnection(buf, size)
{
}

Tex2RTFConnection::~Tex2RTFConnection(void)
{
}

bool SplitCommand(wxChar *data, wxChar *firstArg, wxChar *secondArg)
{
  firstArg[0] = 0;
  secondArg[0] = 0;
  int i = 0;
  bool stop = false;
  // Find first argument (command name)
  while (!stop)
  {
    if (data[i] == ' ' || data[i] == 0)
      stop = true;
    else
    {
      firstArg[i] = data[i];
      i ++;
    }
  }
  firstArg[i] = 0;
  if (data[i] == ' ')
  {
    // Find second argument
    i ++;
    int j = 0;
    while (data[i] != 0)
    {
      secondArg[j] = data[i];
      i ++;
      j ++;
    }
    secondArg[j] = 0;
  }
  return true;
}

bool Tex2RTFConnection::OnExecute(const wxString& WXUNUSED(topic), wxChar *data, int WXUNUSED(size), wxIPCFormat WXUNUSED(format))
{
  wxStrcpy(Tex2RTFLastStatus, _T("OK"));

  wxChar firstArg[50];
  wxChar secondArg[300];
  if (SplitCommand(data, firstArg, secondArg))
  {
    bool hasArg = (wxStrlen(secondArg) > 0);
    if (wxStrcmp(firstArg, _T("INPUT")) == 0 && hasArg)
    {
      if (InputFile) delete[] InputFile;
      InputFile = copystring(secondArg);
      if (frame)
      {
        wxChar buf[100];
        wxString str = wxFileNameFromPath(InputFile);
        wxSnprintf(buf, sizeof(buf), _T("Tex2RTF [%s]"), (const wxChar*) str);
        frame->SetTitle(buf);
      }
    }
    else if (wxStrcmp(firstArg, _T("OUTPUT")) == 0 && hasArg)
    {
      if (OutputFile) delete[] OutputFile;
      OutputFile = copystring(secondArg);
    }
    else if (wxStrcmp(firstArg, _T("GO")) == 0)
    {
      wxStrcpy(Tex2RTFLastStatus, _T("WORKING"));
      if (!Go())
        wxStrcpy(Tex2RTFLastStatus, _T("CONVERSION ERROR"));
      else
        wxStrcpy(Tex2RTFLastStatus, _T("OK"));
    }
    else if (wxStrcmp(firstArg, _T("EXIT")) == 0)
    {
      if (frame) frame->Close();
    }
    else if (wxStrcmp(firstArg, _T("MINIMIZE")) == 0 || wxStrcmp(firstArg, _T("ICONIZE")) == 0)
    {
      if (frame)
        frame->Iconize(true);
    }
    else if (wxStrcmp(firstArg, _T("SHOW")) == 0 || wxStrcmp(firstArg, _T("RESTORE")) == 0)
    {
      if (frame)
      {
        frame->Iconize(false);
        frame->Show(true);
      }
    }
    else
    {
      // Try for a setting
      wxStrcpy(Tex2RTFLastStatus, RegisterSetting(firstArg, secondArg, false));
#ifndef NO_GUI
      if (frame && wxStrcmp(firstArg, _T("conversionMode")) == 0)
      {
        wxChar buf[100];
        wxStrcpy(buf, _T("In "));

        if (winHelp && (convertMode == TEX_RTF))
          wxStrcat(buf, _T("WinHelp RTF"));
        else if (!winHelp && (convertMode == TEX_RTF))
          wxStrcat(buf, _T("linear RTF"));
        else if (convertMode == TEX_HTML) wxStrcat(buf, _T("HTML"));
        else if (convertMode == TEX_XLP) wxStrcat(buf, _T("XLP"));
          wxStrcat(buf, _T(" mode."));
        frame->SetStatusText(buf, 1);
      }
#endif
    }
  }
  return true;
}

wxChar *Tex2RTFConnection::OnRequest(const wxString& WXUNUSED(topic), const wxString& WXUNUSED(item), int *WXUNUSED(size), wxIPCFormat WXUNUSED(format))
{
  return Tex2RTFLastStatus;
}

#endif

#ifndef NO_GUI
#ifndef __WXGTK__
//void wxObject::Dump(wxSTD ostream& str)
//{
//  if (GetClassInfo() && GetClassInfo()->GetClassName())
//    str << GetClassInfo()->GetClassName();
//  else
//    str << "unknown object class";
//}
#endif
#endif
