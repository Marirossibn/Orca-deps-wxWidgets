/////////////////////////////////////////////////////////////////////////////
// Name:        regextest.cpp
// Purpose:     Application to test regular expression (wxRegEx)
// Author:      Ryan Norton
// Modified by: 
// RCS-ID:      $Id$
// Copyright:   (c) 2003 Ryan Norton <wxprojects@comcast.net>
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

//===========================================================================
//                          REGEX LIBS
//===========================================================================

//wxWindows regular expression library
#define wxUSE_WXREGEX  

//RN (Ryan Norton's) regular expression library
//#define wxUSE_RNWXRE 

//GRETA, Microsoft Research's templated regex library 
//[http://research.microsoft.com/projects/greta/] 
//Install - Get it from .net powertools, put the directory in this directory
//#define wxUSE_GRETA 

//PCRE (Perl Compatible Regular Expressions) [sourceforge.net/projects/pcre]
//Install - Get the GnuWin32 version and put the files in this directory
//#define wxUSE_PCRE  

//===========================================================================
//                          HEADERS
//===========================================================================

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/file.h"   

#ifdef wxUSE_WXREGEX
//
//  This is the required header for wxRegEx
//
#   include <wx/regex.h>
#endif

#ifdef wxUSE_RNWXRE
#   include "re.h"
#endif

#ifdef wxUSE_GRETA
#   ifdef _MSC_VER
#       pragma warning(disable:4018)
#       pragma warning(disable:4100)
#       pragma warning(disable:4146)
#       pragma warning(disable:4244)
#       pragma warning(disable:4663)
        extern "C" {
            int __cdecl _resetstkoflw(void) {return 0;}
        }
#   endif //_MSC_VER
#   include "greta/regexpr2.h"
    using namespace regex;
#endif //wxUSE_GRETA

#ifdef wxUSE_PCRE
#   include "pcre.h"
#   ifdef _MSC_VER
#       pragma comment(lib, "libpcre.a")
#   endif
#endif

//===========================================================================
//                          IMPLEMENTATION
//===========================================================================

//
//  One of the best sources for regex info is IEEE document/standard 1003.2
//  From the open group.  
//  A current link - (http://www.opengroup.org/onlinepubs/007904975/basedefs/xbd_chap09.html).
//

//---------------------------------------------------------------------------
//                          MyDialog
//---------------------------------------------------------------------------

class MyDialog : public wxFrame
{
public:

    //IDs for our controls
    enum
    {
        PatternTextID,
        SearchTextID,
        OkButtonID
    };

    //Menu IDs
    enum
    {
        ExtendedID,
        ICaseID,
        NewLineID,
        NotBolID,
        NotEolID,
        CompID
    };


    MyDialog() : wxFrame(  NULL, -1, _("regextest - wxRegEx Testing App"),
                            wxPoint(20,20), wxSize(300,400) )
    {
        //Set the background to something light gray-ish
        SetBackgroundColour(wxColour(150,150,150));

        //
        //  Create the menus (Exit & About)
        //
     #if wxUSE_MENUS
        wxMenu *FileMenu = new wxMenu;
                OptionsMenu = new wxMenu;
        wxMenu *HelpMenu = new wxMenu;

        FileMenu->Append(wxID_EXIT, _T("&Exit"), _("Quit this program"));

        OptionsMenu->AppendCheckItem(ExtendedID, _T("wxRE_EXTENDED"), _("Extended Regular Expressions?"));
        OptionsMenu->AppendCheckItem(ICaseID, _T("wxRE_ICASE"), _("Enable case-insensitive matching?"));
        OptionsMenu->AppendCheckItem(NewLineID, _T("wxRE_NEWLINE"), _("Treat \n as special?"));
        OptionsMenu->AppendSeparator();
        OptionsMenu->AppendCheckItem(NotBolID, _T("wxRE_NOTBOL"), _("Use functionality of ^ character?"));
        OptionsMenu->AppendCheckItem(NotEolID, _T("wxRE_NOTEOL"), _("Use functionality of $ character?"));
        OptionsMenu->AppendSeparator();
        OptionsMenu->AppendSeparator();
        OptionsMenu->AppendCheckItem(CompID, _T("Test Compile"), _("Added Compiling to Match Time?"));
        OptionsMenu->Check(ExtendedID, true);

        HelpMenu->Append(wxID_ABOUT, _T("&About...\tF1"), _("Show about dialog"));

        wxMenuBar *MenuBar = new wxMenuBar();
        MenuBar->Append(FileMenu, _T("&File"));
        MenuBar->Append(OptionsMenu, _T("&Options"));
        MenuBar->Append(HelpMenu, _T("&Help"));

        SetMenuBar(MenuBar);
    #endif // wxUSE_MENUS

        // Text controls
        PatternText.Create(this, PatternTextID, _(""), wxPoint(5, 30));
        SearchText.Create(this, SearchTextID, _(""), wxPoint(5, 75));
        //reset size of text controls
        wxSize TextSize = PatternText.GetSize();
        TextSize.SetWidth(200);
        PatternText.SetSize(TextSize);
        SearchText.SetSize(TextSize);
        
        // StaticText
        PatternHeader.Create(this, -1, _("Regular Expression:"), wxPoint(5, 10));
        SearchHeader.Create(this, -1, _("String to Search:"), wxPoint(5, 55));
        IterHeader.Create(this, -1, _("Iterations (Match Time):"), wxPoint(100, 100));

        ResultText.Create(this, -1, _(""), wxPoint(5, 150), wxSize(100,200), wxST_NO_AUTORESIZE);
        ResultText2.Create(this, -1, _(""), wxPoint(115, 150), wxSize(100,200), wxST_NO_AUTORESIZE);

        // Button
        OkButton.Create(this, OkButtonID, _("OK"), wxPoint(20, 120));

        NumIters.Create(this, -1, _("15000"), wxPoint(100, 120));
    }

    void OnAbout(wxCommandEvent& WXUNUSED(evt))
    {
        wxChar* szBuffer;
        wxFile f;

        f.Open(_("README.txt"), wxFile::read);
        szBuffer = new wxChar[f.Length() + 1];
        f.Read(szBuffer, f.Length());
        f.Close();

        wxMessageBox(wxString::Format(
            _("%s%s%s%s"),
            _("-----------Regular Expression Test Application-----------\n"),
            szBuffer,
            _("\n\n\n(c) 2003 Ryan Norton <wxprojects@comcast.net>\n"),
            wxVERSION_STRING
                                     )
                    );

        delete szBuffer;
    }

    void OnMatch(wxCommandEvent& WXUNUSED(evt))
    {
        wxString szPattern = PatternText.GetValue();
        wxString szSearch = SearchText.GetValue();
        wxString szResult, szResult2, szResult3, szResult4; //Will be displayed in ResultText
        wxString szStatus, szStatus2, szStatus3, szStatus4;

        int nCompileFlags = 0, nCompileFlags2 = 0, nCompileFlags3 = 0, nCompileFlags4 = 0;
        int nMatchFlags = 0, nMatchFlags2 = 0, nMatchFlags3 = 0, nMatchFlags4 = 0;

        if (!(OptionsMenu->IsChecked(ExtendedID)))
        {
#ifdef wxUSE_WXREGEX
            nCompileFlags |= wxRE_BASIC;
#endif
        }
        else
        {
#ifdef wxUSE_RNWXRE
            nCompileFlags2 |= wxRe::wxRE_EXTENDED;
#endif
            //            nCompileFlags3 |= EXTENDED;
        }

        if (OptionsMenu->IsChecked(ICaseID))
        {
#ifdef wxUSE_WXREGEX
            nCompileFlags |= wxRE_ICASE;
#endif
#ifdef wxUSE_RNWXRE
            nCompileFlags2 |= wxRe::wxRE_ICASE;
#endif
#ifdef wxUSE_GRETA
            nCompileFlags3 |= NOCASE;
#endif
        }

        if (OptionsMenu->IsChecked(NewLineID))
        {
#ifdef wxUSE_WXREGEX
            nCompileFlags |= wxRE_NEWLINE;
#endif
#ifdef wxUSE_RNWXRE
            nCompileFlags2 |= wxRe::wxRE_NEWLINE;
#endif
#ifdef wxUSE_GRETA
            nCompileFlags3 |= MULTILINE;
#endif
        }

        if (OptionsMenu->IsChecked(NotBolID))
        {
#ifdef wxUSE_WXREGEX
            nMatchFlags |= wxRE_NOTBOL;
#endif
#ifdef wxUSE_RNWXRE
            nMatchFlags2 |= wxRe::wxRE_NOTBOL;
#endif
        }

        if (OptionsMenu->IsChecked(NotEolID))
        {
#ifdef wxUSE_WXREGEX
            nMatchFlags |= wxRE_NOTEOL;
#endif
#ifdef wxUSE_RNWXRE
            nMatchFlags2 |= wxRe::wxRE_NOTEOL;
#endif
        }

        //Our regular expression object

        //Success!  Here we'll display some 
        //information to the user
        size_t dwStartIndex = 0, dwEndIndex = 0,
               dwStartIndex2= 0, dwEndIndex2= 0,
               dwStartIndex3= 0, dwEndIndex3= 0,
               dwStartIndex4= 0, dwEndIndex4= 0;

        time_t dwStartTime = 0, dwEndTime = 0,
               dwStartTime2= 0, dwEndTime2= 0,
               dwStartTime3= 0, dwEndTime3= 0,
               dwStartTime4= 0, dwEndTime4= 0; 

        int i = 0;
        long n;
        if (!NumIters.GetValue().ToLong(&n))
            n = 0;


#ifdef wxUSE_WXREGEX
		wxRegEx Regex;
        //Regular Expressions must be compiled before
        //you can search a string with them, or
        //at least most implementations do.
        //(Boost and Microsoft have templated
        //versions that don't require compilation)
        //Basically compilation breaks it down into
        //something that's easier to parse, due
        //to the syntax of regular expressions
        if (!Regex.Compile(szPattern, nCompileFlags))
            szStatus += _("\nCompile Failed!\n"); 
        else
        {
            //Here's where we actually search our string
            if (!Regex.Matches(szSearch, nMatchFlags))
                szStatus += _("\nExecution/Matching Failed!\n");
            else
            {
                Regex.GetMatch(&dwStartIndex, &dwEndIndex);

                szStatus = _("Success");

                //We're going to go ahead and time the match
                //for fun (clock() is a c library routine that
                //returns the current time since a certian point
                //in milliseconds
                dwStartTime = clock();
        
                if (OptionsMenu->IsChecked(CompID))
                {
                    for(i = 0; i < n; ++i)
                    {
                        Regex.Compile(szPattern, nCompileFlags);
                        Regex.Matches(szSearch, nMatchFlags);
                    }
                }
                else
                {
                    for(i = 0; i < n; ++i)
                    {
                        Regex.Matches(szSearch, nMatchFlags);
                    }
                }
        
                //This line gets the difference in time between now
                //and when we first initialized dwStartTime.
                dwEndTime = clock() - dwStartTime;
            }
        }


        szResult = wxString::Format(
                            _("--wxRegEx--\nIndex:[%i]-[%i]\nString:%s\nMatch Time:%ums\nStatus:%s"),
                            dwStartIndex, dwEndIndex+dwStartIndex, 
                            szSearch.Mid(dwStartIndex, dwEndIndex),
                            dwEndTime, 
                            szStatus
                                    );

#endif //wxUSE_WXREGEX

#ifdef wxUSE_RNWXRE
        wxRe Re;
        wxRe::wxReError e;
        if ((e = Re.Comp(szPattern, nCompileFlags2)) != wxRe::wxRE_OK)
            szStatus2 = wxString::Format(_("\nCompile Failed!\n%s\n"), wxRe::ErrorToString(e)); 
        else
        {
            //Here's where we actually search our string
            if ((e = Re.Exec(szSearch, nMatchFlags2)) != wxRe::wxRE_OK)
                szStatus2 = wxString::Format(_("\nExecution/Matching Failed!\n%s\n"), wxRe::ErrorToString(e));
            else
            {
                dwStartIndex2 = Re.GetMatch(0).first;
                dwEndIndex2 = Re.GetMatch(0).second;

                szStatus2 = _("Success");

                dwStartTime2 = clock();
        
                if (OptionsMenu->IsChecked(CompID))
                {
                    for(i = 0; i < n; ++i)
                    {
                        Re.Comp(szPattern, nCompileFlags2);
                        Re.Exec(szSearch, nMatchFlags2);
                    }
                }
                else
                {
                    for(i = 0; i < n; ++i)
                    {
                        Re.Exec(szSearch, nMatchFlags2);
                    }
                }

                dwEndTime2 = clock() - dwStartTime2;
            }
        }
        szResult2 = wxString::Format(
                            _("--Ryan's wxRe--\nIndex:[%i]-[%i]\nString:%s\nMatch Time:%ums\nStatus:%s"),
                            dwStartIndex2, dwEndIndex2+dwStartIndex2, 
                            szSearch.Mid(dwStartIndex2, dwEndIndex2),
                            dwEndTime2, 
                            szStatus2
                                    );
#endif //wxUSE_RNWXRE

#ifdef wxUSE_GRETA
        std::string stdszPattern(szPattern);
        rpattern Greta (stdszPattern,EXTENDED,MODE_MIXED);
        match_results r;
        std::string stdszSearch(szSearch);

        //Here's where we actually search our string
        if (!Greta.match(stdszSearch, r).matched)
            szStatus3 += _("\nExecution/Matching Failed!\n");
        else
        {
            szStatus3 = _("Success");

            dwStartTime3 = clock();
    
            if (OptionsMenu->IsChecked(CompID))
            {
                for(i = 0; i < n; ++i)
                {
                    Greta = rpattern(stdszPattern,EXTENDED,MODE_MIXED);
                    Greta.match(stdszSearch, r);
                }
            }
            else
            {
                for(i = 0; i < n; ++i)
                {
                    Greta.match(stdszSearch, r);
                }
            }
    
            dwEndTime3 = clock() - dwStartTime3;
        }

        szResult3 = wxString::Format(
            _("--Greta--\nIndex:[%i]-[%i]\nString:%s\nMatch Time:%ums\nStatus:%s"),
                            r.rstart(), r.rlength() + r.rstart(), 
                            szSearch.Mid(r.rstart(), r.rlength()),
                            dwEndTime3, 
                            szStatus3);
#endif //wxUSE_GRETA

#ifdef wxUSE_PCRE
        pcre* pPcre;
        const wxChar* szError;
        int nErrOff;

        if ((pPcre = pcre_compile(szPattern, nCompileFlags4, &szError, &nErrOff, 0)) == NULL)
            szStatus4 = wxString::Format(_("\nCompile Failed!\nError:%s\nOffset:%i\n"), szError, nErrOff); 
        else
        {
            size_t msize;
	        pcre_fullinfo(pPcre, 0, PCRE_INFO_CAPTURECOUNT, &msize);
	        msize = 3*(msize+1);
	        int *m = new int[msize];

            //Here's where we actually search our string
            if (!pcre_exec(pPcre, 0, szSearch, szSearch.Length(), 0, 0, m, msize))
                szStatus4 = wxString::Format(_("\nExecution/Matching Failed!\n"));
            else
            {
                dwStartIndex4 = m[0];
                dwEndIndex4 = m[1] - m[0];

                szStatus4 = _("Success");

                dwStartTime4 = clock();
        
                if (OptionsMenu->IsChecked(CompID))
                {
                    for(i = 0; i < n; ++i)
                    {
                        pPcre = pcre_compile(szPattern, nCompileFlags4, &szError, &nErrOff, 0);
                        pcre_exec(pPcre, 0, szSearch, szSearch.Length(), 0, 0, m, msize);
                    }
                }
                else
                {
                    for(i = 0; i < n; ++i)
                    {
                        pcre_exec(pPcre, 0, szSearch, szSearch.Length(), 0, 0, m, msize);
                    }
                }

                dwEndTime4 = clock() - dwStartTime4;
            }
        }
        szResult4 = wxString::Format(
                            _("--PCRE--\nIndex:[%i]-[%i]\nString:%s\nMatch Time:%ums\nStatus:%s"),
                            dwStartIndex4, dwEndIndex4+dwStartIndex4, 
                            szSearch.Mid(dwStartIndex4, dwEndIndex4),
                            dwEndTime4, 
                            szStatus4
                                    );
#endif //wxUSE_PCRE

        ResultText.SetLabel(szResult + _("\n\n") + szResult2);
        ResultText2.SetLabel(szResult3 + _("\n\n") + szResult4);
    }

    void OnQuit(wxCommandEvent& WXUNUSED(evt))
    {
        Close(TRUE);
    }

    wxTextCtrl PatternText, SearchText, NumIters;
    wxStaticText PatternHeader, SearchHeader, IterHeader, ResultText, ResultText2;
    wxButton OkButton;

    wxMenu *OptionsMenu;

    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(MyDialog, wxFrame)

//menu
EVT_MENU(wxID_EXIT,  MyDialog::OnQuit)
EVT_MENU(wxID_ABOUT,  MyDialog::OnAbout)

//text
EVT_TEXT_ENTER(MyDialog::PatternTextID, MyDialog::OnMatch)
EVT_TEXT_ENTER(MyDialog::SearchTextID, MyDialog::OnMatch)

//button
EVT_BUTTON(MyDialog::OkButtonID, MyDialog::OnMatch)

END_EVENT_TABLE()

//---------------------------------------------------------------------------
//                          MyApp
//---------------------------------------------------------------------------

class MyApp : public wxApp
{
public:
	bool OnInit()
    {
       MyDialog* dialog = new MyDialog();
       dialog->Show();
       return true;
    }
};

IMPLEMENT_APP(MyApp)

