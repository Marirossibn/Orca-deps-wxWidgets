#-----------------------------------------------------------------------------
# Python source generated by wxDesigner from file: ErrorDialogs.wdr
# Do not modify this file, all changes will be lost!
#-----------------------------------------------------------------------------

# Include wxWindows' modules
from wxPython.wx import *

# Custom source
from wxPython.lib.PythonBitmaps import *
from wxPython.html import *
from wxPython.lib.ClickableHtmlWindow import *

# Window functions

wxPyError_ID_TEXT1 = 10000
wxPyError_ID_PROGRAMNAME = 10001
wxPyError_ID_TEXT2 = 10002
wxPyError_ID_VERSIONNUMBER = 10003
wxPyError_ID_EXTRA_VERSION_INFORMATION = 10004
wxPyError_ID_STATICBITMAP1 = 10005
wxPyError_ID_STATICBITMAP2 = 10006
wxPyError_ID_TEXT3 = 10007
wxPyError_ID_TEXT4 = 10008
wxPyError_ID_TEXTCTRL = 10009
wxPyError_ID_TEXT5 = 10010
wxPyError_ID_CONTINUE = 10011
wxPyError_ID_MAIL = 10012
wxPyError_ID_TEXT6 = 10013
wxPyError_ID_ADDRESS = 10014
wxPyError_ID_EXIT = 10015
wxPyError_ID_TEXT7 = 10016
wxPyError_ID_TEXT8 = 10017
wxPyError_ID_TEXT9 = 10018
wxPyError_ID_TEXT10 = 10019
wxPyError_ID_TEXT11 = 10020
wxPyError_ID_TEXT12 = 10021

def populate_wxPyNonFatalErrorDialogWithTraceback( parent, call_fit = True, set_sizer = True ):
    item0 = wxBoxSizer( wxVERTICAL )

    item1 = wxBoxSizer( wxHORIZONTAL )

    item3 = wxStaticBox( parent, -1, "Non-fatal" )
    item3.SetFont( wxFont( 9, wxSWISS, wxNORMAL, wxBOLD ) )
    item2 = wxStaticBoxSizer( item3, wxVERTICAL )

    item4 = wxBoxSizer( wxHORIZONTAL )

    item5 = wxStaticText( parent, wxPyError_ID_TEXT1, "Error in ", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE )
    item5.SetForegroundColour( wxWHITE )
    item5.SetBackgroundColour( wxRED )
    item5.SetFont( wxFont( 21, wxSCRIPT, wxNORMAL, wxBOLD ) )
    item4.AddWindow( item5, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item6 = wxStaticText( parent, wxPyError_ID_PROGRAMNAME, "wxPyError_ID_PROGRAMNAME", wxDefaultPosition, wxDefaultSize, 0 )
    item6.SetFont( wxFont( 21, wxROMAN, wxITALIC, wxNORMAL ) )
    item4.AddWindow( item6, 1, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item2.AddSizer( item4, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item7 = wxBoxSizer( wxHORIZONTAL )

    item8 = wxStaticText( parent, wxPyError_ID_TEXT2, "Version ", wxDefaultPosition, wxDefaultSize, 0 )
    item8.SetFont( wxFont( 9, wxROMAN, wxNORMAL, wxNORMAL ) )
    item7.AddWindow( item8, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item9 = wxStaticText( parent, wxPyError_ID_VERSIONNUMBER, "wxPyError_ID_VERSIONNUMBER", wxDefaultPosition, wxDefaultSize, 0 )
    item9.SetFont( wxFont( 12, wxROMAN, wxNORMAL, wxBOLD ) )
    item7.AddWindow( item9, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item2.AddSizer( item7, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item10 = wxStaticText( parent, wxPyError_ID_EXTRA_VERSION_INFORMATION, "wxPyError_ID_EXTRA_VERSION_INFORMATION", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE )
    item10.SetFont( wxFont( 7, wxROMAN, wxITALIC, wxNORMAL ) )
    item2.AddWindow( item10, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item1.AddSizer( item2, 1, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item11 = wxStaticBitmap( parent, wxPyError_ID_STATICBITMAP1, PythonBitmaps( 0 ), wxDefaultPosition, wxDefaultSize )
    item1.AddWindow( item11, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item12 = wxStaticBitmap( parent, wxPyError_ID_STATICBITMAP2, PythonBitmaps( 1 ), wxDefaultPosition, wxDefaultSize )
    item1.AddWindow( item12, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item0.AddSizer( item1, 0, wxADJUST_MINSIZE|wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 )

    item13 = wxStaticText( parent, wxPyError_ID_TEXT3, "The Python interpreter has encountered a so-called \"un-caught exception\".", wxDefaultPosition, wxDefaultSize, 0 )
    item0.AddWindow( item13, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item15 = wxStaticBox( parent, -1, "Traceback" )
    item15.SetFont( wxFont( 6, wxSWISS, wxITALIC, wxNORMAL ) )
    parent.sizerAroundText = item14 = wxStaticBoxSizer( item15, wxVERTICAL )

    item16 = wxStaticText( parent, wxPyError_ID_TEXT4,
        "Please don't worry if this doesn't mean anything to you.\n"
        "It will be included in the \"bug report\" mentioned below.",
        wxDefaultPosition, wxDefaultSize, 0 )
    item16.SetFont( wxFont( 8, wxROMAN, wxNORMAL, wxNORMAL ) )
    item14.AddWindow( item16, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item17 = wxTextCtrl( parent, wxPyError_ID_TEXTCTRL, "wxPyError_ID_TEXTCTRL", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY )
    item17.SetFont( wxFont( 9, wxSWISS, wxNORMAL, wxNORMAL ) )
    item17.SetToolTip( wxToolTip("A \"traceback\" reports the nature and location of a Python error.") )
    item14.AddWindow( item17, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 )

    item0.AddSizer( item14, 1, wxALIGN_CENTRE|wxALL, 5 )

    item18 = wxStaticText( parent, wxPyError_ID_TEXT5, "Please select one of the options below.", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE )
    item18.SetFont( wxFont( 8, wxROMAN, wxITALIC, wxNORMAL ) )
    item0.AddWindow( item18, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item19 = wxFlexGridSizer( 3, 0, 0, 6 )
    item19.AddGrowableCol( 0 )
    item19.AddGrowableCol( 1 )
    item19.AddGrowableCol( 2 )

    item20 = wxButton( parent, wxPyError_ID_CONTINUE, "Continue", wxDefaultPosition, wxDefaultSize, 0 )
    item20.SetDefault()
    item19.AddWindow( item20, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item21 = wxBoxSizer( wxHORIZONTAL )

    item22 = wxButton( parent, wxPyError_ID_MAIL, "E-mail support", wxDefaultPosition, wxDefaultSize, 0 )
    item21.AddWindow( item22, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item23 = wxBoxSizer( wxVERTICAL )

    item24 = wxStaticText( parent, wxPyError_ID_TEXT6, "Your e-mail address:", wxDefaultPosition, wxDefaultSize, 0 )
    item24.SetFont( wxFont( 8, wxROMAN, wxITALIC, wxNORMAL ) )
    item23.AddWindow( item24, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item25 = wxTextCtrl( parent, wxPyError_ID_ADDRESS, "", wxDefaultPosition, wxSize(80,-1), 0 )
    item23.AddWindow( item25, 2, wxADJUST_MINSIZE|wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 )

    item21.AddSizer( item23, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item19.AddSizer( item21, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item26 = wxButton( parent, wxPyError_ID_EXIT, "Exit immediately", wxDefaultPosition, wxDefaultSize, 0 )
    item19.AddWindow( item26, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item27 = wxStaticText( parent, wxPyError_ID_TEXT7, "Attempt to continue.", wxDefaultPosition, wxDefaultSize, 0 )
    item19.AddWindow( item27, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item28 = wxStaticText( parent, wxPyError_ID_TEXT8, "E-mail a \"bug report\" (if this is indeed a bug!).", wxDefaultPosition, wxDefaultSize, 0 )
    item19.AddWindow( item28, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item29 = wxStaticText( parent, wxPyError_ID_TEXT9, "Attempt to exit immediately.", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT )
    item19.AddWindow( item29, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE, 5 )

    item30 = wxStaticText( parent, wxPyError_ID_TEXT10, "", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT )
    item30.SetFont( wxFont( 7, wxROMAN, wxNORMAL, wxBOLD ) )
    item19.AddWindow( item30, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item31 = wxStaticText( parent, wxPyError_ID_TEXT11, "(Please read any accompanying documentation first!)", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE )
    item31.SetFont( wxFont( 7, wxROMAN, wxNORMAL, wxBOLD ) )
    item19.AddWindow( item31, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item32 = wxStaticText( parent, wxPyError_ID_TEXT12, "(Please note that no attempt to save unsaved data will be made.)", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT )
    item32.SetFont( wxFont( 7, wxROMAN, wxNORMAL, wxBOLD ) )
    item19.AddWindow( item32, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item0.AddSizer( item19, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    if set_sizer == True:
        parent.SetAutoLayout( True )
        parent.SetSizer( item0 )
        if call_fit == True:
            item0.Fit( parent )
            item0.SetSizeHints( parent )

    return item0

wxPyError_wxPyError_ID_TEXT3 = 10022
wxPyError_ID_EXCEPTIONNAME = 10023
wxPyError_ID_EXTRAINFORMATION = 10024
wxPyError_ID_TEXT13 = 10025
wxPyError_ID_TEXT14 = 10026

def populate_wxPyNonFatalErrorDialog( parent, call_fit = True, set_sizer = True ):
    item0 = wxBoxSizer( wxVERTICAL )

    item1 = wxBoxSizer( wxHORIZONTAL )

    item2 = wxBoxSizer( wxHORIZONTAL )

    item4 = wxStaticBox( parent, -1, "Non-fatal" )
    item4.SetFont( wxFont( 9, wxSWISS, wxNORMAL, wxBOLD ) )
    item3 = wxStaticBoxSizer( item4, wxVERTICAL )

    item5 = wxBoxSizer( wxHORIZONTAL )

    item6 = wxStaticText( parent, wxPyError_ID_TEXT1, "Error in ", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE )
    item6.SetForegroundColour( wxWHITE )
    item6.SetBackgroundColour( wxRED )
    item6.SetFont( wxFont( 21, wxSCRIPT, wxNORMAL, wxBOLD ) )
    item5.AddWindow( item6, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item7 = wxStaticText( parent, wxPyError_ID_PROGRAMNAME, "wxPyError_ID_PROGRAMNAME", wxDefaultPosition, wxDefaultSize, 0 )
    item7.SetFont( wxFont( 21, wxROMAN, wxITALIC, wxNORMAL ) )
    item5.AddWindow( item7, 1, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item3.AddSizer( item5, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item8 = wxBoxSizer( wxHORIZONTAL )

    item9 = wxStaticText( parent, wxPyError_ID_TEXT2, "Version ", wxDefaultPosition, wxDefaultSize, 0 )
    item9.SetFont( wxFont( 9, wxROMAN, wxNORMAL, wxNORMAL ) )
    item8.AddWindow( item9, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item10 = wxStaticText( parent, wxPyError_ID_VERSIONNUMBER, "wxPyError_ID_VERSIONNUMBER", wxDefaultPosition, wxDefaultSize, 0 )
    item10.SetFont( wxFont( 12, wxROMAN, wxNORMAL, wxBOLD ) )
    item8.AddWindow( item10, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item3.AddSizer( item8, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item11 = wxStaticText( parent, wxPyError_ID_EXTRA_VERSION_INFORMATION, "wxPyError_ID_EXTRA_VERSION_INFORMATION", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE )
    item11.SetFont( wxFont( 7, wxROMAN, wxITALIC, wxNORMAL ) )
    item3.AddWindow( item11, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item2.AddSizer( item3, 1, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item12 = wxStaticBitmap( parent, wxPyError_ID_STATICBITMAP1, PythonBitmaps( 0 ), wxDefaultPosition, wxDefaultSize )
    item2.AddWindow( item12, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item13 = wxStaticBitmap( parent, wxPyError_ID_STATICBITMAP2, PythonBitmaps( 1 ), wxDefaultPosition, wxDefaultSize )
    item2.AddWindow( item13, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item1.AddSizer( item2, 1, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item0.AddSizer( item1, 1, wxADJUST_MINSIZE|wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 )

    item14 = wxStaticText( parent, wxPyError_wxPyError_ID_TEXT3, "The Python interpreter has encountered a so-called \"un-caught exception\".", wxDefaultPosition, wxDefaultSize, 0 )
    item0.AddWindow( item14, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item16 = wxStaticBox( parent, -1, "Exception information" )
    item16.SetFont( wxFont( 6, wxSWISS, wxITALIC, wxNORMAL ) )
    item15 = wxStaticBoxSizer( item16, wxVERTICAL )

    item17 = wxStaticText( parent, wxPyError_ID_TEXT4,
        "Please don't worry if this doesn't mean anything to you.\n"
        "It will be included in the \"bug report\" mentioned below, along with a \"stack traceback\".",
        wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE )
    item17.SetFont( wxFont( 8, wxROMAN, wxNORMAL, wxNORMAL ) )
    item15.AddWindow( item17, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item18 = wxFlexGridSizer( 2, 0, 1, 1 )
    item18.AddGrowableCol( 1 )

    item19 = wxStaticText( parent, wxPyError_ID_TEXT5, "Name:", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT )
    item19.SetFont( wxFont( 10, wxROMAN, wxITALIC, wxNORMAL ) )
    item19.SetToolTip( wxToolTip("This gives the type of the error.") )
    item18.AddWindow( item19, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item20 = wxStaticText( parent, wxPyError_ID_EXCEPTIONNAME, "wxPyError_ID_EXCEPTIONNAME", wxDefaultPosition, wxDefaultSize, 0 )
    item18.AddWindow( item20, 0, wxADJUST_MINSIZE|wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 )

    item21 = wxStaticText( parent, wxPyError_ID_TEXT6,
        "Extra\n"
        "information:",
        wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE )
    item21.SetFont( wxFont( 10, wxROMAN, wxITALIC, wxNORMAL ) )
    item18.AddWindow( item21, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item22 = wxStaticText( parent, wxPyError_ID_EXTRAINFORMATION, "wxPyError_ID_EXTRAINFORMATION", wxDefaultPosition, wxDefaultSize, 0 )
    item18.AddWindow( item22, 0, wxADJUST_MINSIZE|wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 )

    item15.AddSizer( item18, 1, wxADJUST_MINSIZE|wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 )

    item0.AddSizer( item15, 0, wxADJUST_MINSIZE|wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 10 )

    item23 = wxStaticText( parent, wxPyError_ID_TEXT7, "Please select one of the options below.", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE )
    item23.SetFont( wxFont( 8, wxROMAN, wxITALIC, wxNORMAL ) )
    item0.AddWindow( item23, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item24 = wxFlexGridSizer( 3, 0, 0, 6 )
    item24.AddGrowableCol( 0 )
    item24.AddGrowableCol( 1 )
    item24.AddGrowableCol( 2 )

    item25 = wxButton( parent, wxPyError_ID_CONTINUE, "Continue", wxDefaultPosition, wxDefaultSize, 0 )
    item25.SetDefault()
    item24.AddWindow( item25, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item26 = wxBoxSizer( wxHORIZONTAL )

    item27 = wxButton( parent, wxPyError_ID_MAIL, "E-mail support", wxDefaultPosition, wxDefaultSize, 0 )
    item26.AddWindow( item27, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item28 = wxBoxSizer( wxVERTICAL )

    item29 = wxStaticText( parent, wxPyError_ID_TEXT8, "Your e-mail address:", wxDefaultPosition, wxDefaultSize, 0 )
    item29.SetFont( wxFont( 8, wxROMAN, wxITALIC, wxNORMAL ) )
    item28.AddWindow( item29, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item30 = wxTextCtrl( parent, wxPyError_ID_ADDRESS, "", wxDefaultPosition, wxSize(80,-1), 0 )
    item28.AddWindow( item30, 2, wxADJUST_MINSIZE|wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 )

    item26.AddSizer( item28, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item24.AddSizer( item26, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item31 = wxButton( parent, wxPyError_ID_EXIT, "Exit immediately", wxDefaultPosition, wxDefaultSize, 0 )
    item24.AddWindow( item31, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item32 = wxStaticText( parent, wxPyError_ID_TEXT9, "Attempt to continue.", wxDefaultPosition, wxDefaultSize, 0 )
    item24.AddWindow( item32, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item33 = wxStaticText( parent, wxPyError_ID_TEXT10, "E-mail a \"bug report\" (if this is indeed a bug!).", wxDefaultPosition, wxDefaultSize, 0 )
    item24.AddWindow( item33, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item34 = wxStaticText( parent, wxPyError_ID_TEXT11, "Attempt to exit immediately.", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT )
    item24.AddWindow( item34, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE, 5 )

    item35 = wxStaticText( parent, wxPyError_ID_TEXT12, "", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE )
    item35.SetFont( wxFont( 7, wxROMAN, wxNORMAL, wxBOLD ) )
    item24.AddWindow( item35, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item36 = wxStaticText( parent, wxPyError_ID_TEXT13, "(Please read any accompanying documentation first!)", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE )
    item36.SetFont( wxFont( 7, wxROMAN, wxNORMAL, wxBOLD ) )
    item24.AddWindow( item36, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item37 = wxStaticText( parent, wxPyError_ID_TEXT14, "(Please note that no attempt to save unsaved data will be made.)", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT )
    item37.SetFont( wxFont( 7, wxROMAN, wxNORMAL, wxBOLD ) )
    item24.AddWindow( item37, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item0.AddSizer( item24, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    if set_sizer == True:
        parent.SetAutoLayout( True )
        parent.SetSizer( item0 )
        if call_fit == True:
            item0.Fit( parent )
            item0.SetSizeHints( parent )

    return item0


def populate_wxPyFatalErrorDialogWithTraceback( parent, call_fit = True, set_sizer = True ):
    item0 = wxBoxSizer( wxVERTICAL )

    item1 = wxBoxSizer( wxHORIZONTAL )

    item3 = wxStaticBox( parent, -1, "Fatal" )
    item3.SetFont( wxFont( 9, wxSWISS, wxNORMAL, wxBOLD ) )
    item2 = wxStaticBoxSizer( item3, wxVERTICAL )

    item4 = wxBoxSizer( wxHORIZONTAL )

    item5 = wxStaticText( parent, wxPyError_ID_TEXT1, "Error in ", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE )
    item5.SetForegroundColour( wxWHITE )
    item5.SetBackgroundColour( wxRED )
    item5.SetFont( wxFont( 21, wxSCRIPT, wxNORMAL, wxBOLD ) )
    item4.AddWindow( item5, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item6 = wxStaticText( parent, wxPyError_ID_PROGRAMNAME, "wxPyError_ID_PROGRAMNAME", wxDefaultPosition, wxDefaultSize, 0 )
    item6.SetFont( wxFont( 21, wxROMAN, wxITALIC, wxNORMAL ) )
    item4.AddWindow( item6, 1, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item2.AddSizer( item4, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item7 = wxBoxSizer( wxHORIZONTAL )

    item8 = wxStaticText( parent, wxPyError_ID_TEXT2, "Version ", wxDefaultPosition, wxDefaultSize, 0 )
    item8.SetFont( wxFont( 9, wxROMAN, wxNORMAL, wxNORMAL ) )
    item7.AddWindow( item8, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item9 = wxStaticText( parent, wxPyError_ID_VERSIONNUMBER, "wxPyError_ID_VERSIONNUMBER", wxDefaultPosition, wxDefaultSize, 0 )
    item9.SetFont( wxFont( 12, wxROMAN, wxNORMAL, wxBOLD ) )
    item7.AddWindow( item9, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item2.AddSizer( item7, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item10 = wxStaticText( parent, wxPyError_ID_EXTRA_VERSION_INFORMATION, "wxPyError_ID_EXTRA_VERSION_INFORMATION", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE )
    item10.SetFont( wxFont( 7, wxROMAN, wxITALIC, wxNORMAL ) )
    item2.AddWindow( item10, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item1.AddSizer( item2, 1, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item11 = wxStaticBitmap( parent, wxPyError_ID_STATICBITMAP1, PythonBitmaps( 0 ), wxDefaultPosition, wxDefaultSize )
    item1.AddWindow( item11, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item12 = wxStaticBitmap( parent, wxPyError_ID_STATICBITMAP2, PythonBitmaps( 1 ), wxDefaultPosition, wxDefaultSize )
    item1.AddWindow( item12, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item0.AddSizer( item1, 1, wxADJUST_MINSIZE|wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 )

    item13 = wxStaticText( parent, wxPyError_ID_TEXT3, "The Python interpreter has encountered a so-called \"un-caught exception\".", wxDefaultPosition, wxDefaultSize, 0 )
    item0.AddWindow( item13, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item15 = wxStaticBox( parent, -1, "Traceback" )
    item15.SetFont( wxFont( 6, wxSWISS, wxITALIC, wxNORMAL ) )
    parent.sizerAroundText = item14 = wxStaticBoxSizer( item15, wxVERTICAL )

    item16 = wxStaticText( parent, wxPyError_ID_TEXT4,
        "Please don't worry if this doesn't mean anything to you.\n"
        "It will be included in the \"bug report\" mentioned below.",
        wxDefaultPosition, wxDefaultSize, 0 )
    item16.SetFont( wxFont( 8, wxROMAN, wxNORMAL, wxNORMAL ) )
    item14.AddWindow( item16, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item17 = wxTextCtrl( parent, wxPyError_ID_TEXTCTRL, "wxPyError_ID_TEXTCTRL", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY )
    item17.SetFont( wxFont( 9, wxSWISS, wxNORMAL, wxNORMAL ) )
    item17.SetToolTip( wxToolTip("A \"traceback\" reports the nature and location of a Python error.") )
    item14.AddWindow( item17, 0, wxALIGN_CENTRE|wxALL, 5 )

    item0.AddSizer( item14, 0, wxALIGN_CENTRE|wxALL, 5 )

    item18 = wxStaticText( parent, wxPyError_ID_TEXT5, "Please select one of the options below.", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE )
    item18.SetFont( wxFont( 8, wxROMAN, wxITALIC, wxNORMAL ) )
    item0.AddWindow( item18, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item19 = wxFlexGridSizer( 3, 0, 0, 6 )
    item19.AddGrowableCol( 0 )
    item19.AddGrowableCol( 1 )
    item19.AddGrowableCol( 2 )

    item20 = wxBoxSizer( wxHORIZONTAL )

    item21 = wxButton( parent, wxPyError_ID_MAIL, "E-mail support", wxDefaultPosition, wxDefaultSize, 0 )
    item20.AddWindow( item21, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item22 = wxBoxSizer( wxVERTICAL )

    item23 = wxStaticText( parent, wxPyError_ID_TEXT6, "Your e-mail address:", wxDefaultPosition, wxDefaultSize, 0 )
    item23.SetFont( wxFont( 8, wxROMAN, wxITALIC, wxNORMAL ) )
    item22.AddWindow( item23, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item24 = wxTextCtrl( parent, wxPyError_ID_ADDRESS, "", wxDefaultPosition, wxSize(80,-1), 0 )
    item22.AddWindow( item24, 2, wxADJUST_MINSIZE|wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 )

    item20.AddSizer( item22, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item19.AddSizer( item20, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item25 = wxButton( parent, wxPyError_ID_EXIT, "Exit immediately", wxDefaultPosition, wxDefaultSize, 0 )
    item25.SetDefault()
    item19.AddWindow( item25, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item26 = wxStaticText( parent, wxPyError_ID_TEXT7, "E-mail a \"bug report\" (if this is indeed a bug!).", wxDefaultPosition, wxDefaultSize, 0 )
    item19.AddWindow( item26, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item27 = wxStaticText( parent, wxPyError_ID_TEXT8, "Attempt to exit immediately.", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT )
    item19.AddWindow( item27, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE, 5 )

    item28 = wxStaticText( parent, wxPyError_ID_TEXT9, "(Please read any accompanying documentation first!)", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE )
    item28.SetFont( wxFont( 7, wxROMAN, wxNORMAL, wxBOLD ) )
    item19.AddWindow( item28, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item29 = wxStaticText( parent, wxPyError_ID_TEXT10, "(Please note that no attempt to save unsaved data will be made.)", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT )
    item29.SetFont( wxFont( 7, wxROMAN, wxNORMAL, wxBOLD ) )
    item19.AddWindow( item29, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item0.AddSizer( item19, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    if set_sizer == True:
        parent.SetAutoLayout( True )
        parent.SetSizer( item0 )
        if call_fit == True:
            item0.Fit( parent )
            item0.SetSizeHints( parent )

    return item0


def populate_wxPyFatalErrorDialog( parent, call_fit = True, set_sizer = True ):
    item0 = wxBoxSizer( wxVERTICAL )

    item1 = wxBoxSizer( wxHORIZONTAL )

    item3 = wxStaticBox( parent, -1, "Fatal" )
    item3.SetFont( wxFont( 9, wxSWISS, wxNORMAL, wxBOLD ) )
    item2 = wxStaticBoxSizer( item3, wxVERTICAL )

    item4 = wxBoxSizer( wxHORIZONTAL )

    item5 = wxStaticText( parent, wxPyError_ID_TEXT1, "Error in ", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE )
    item5.SetForegroundColour( wxWHITE )
    item5.SetBackgroundColour( wxRED )
    item5.SetFont( wxFont( 21, wxSCRIPT, wxNORMAL, wxBOLD ) )
    item4.AddWindow( item5, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item6 = wxStaticText( parent, wxPyError_ID_PROGRAMNAME, "wxPyError_ID_PROGRAMNAME", wxDefaultPosition, wxDefaultSize, 0 )
    item6.SetFont( wxFont( 21, wxROMAN, wxITALIC, wxNORMAL ) )
    item4.AddWindow( item6, 1, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item2.AddSizer( item4, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item7 = wxBoxSizer( wxHORIZONTAL )

    item8 = wxStaticText( parent, wxPyError_ID_TEXT2, "Version ", wxDefaultPosition, wxDefaultSize, 0 )
    item8.SetFont( wxFont( 9, wxROMAN, wxNORMAL, wxNORMAL ) )
    item7.AddWindow( item8, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item9 = wxStaticText( parent, wxPyError_ID_VERSIONNUMBER, "wxPyError_ID_VERSIONNUMBER", wxDefaultPosition, wxDefaultSize, 0 )
    item9.SetFont( wxFont( 12, wxROMAN, wxNORMAL, wxBOLD ) )
    item7.AddWindow( item9, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item2.AddSizer( item7, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item10 = wxStaticText( parent, wxPyError_ID_EXTRA_VERSION_INFORMATION, "wxPyError_ID_EXTRA_VERSION_INFORMATION", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE )
    item10.SetFont( wxFont( 7, wxROMAN, wxITALIC, wxNORMAL ) )
    item2.AddWindow( item10, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item1.AddSizer( item2, 1, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item11 = wxStaticBitmap( parent, wxPyError_ID_STATICBITMAP1, PythonBitmaps( 0 ), wxDefaultPosition, wxDefaultSize )
    item1.AddWindow( item11, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item12 = wxStaticBitmap( parent, wxPyError_ID_STATICBITMAP2, PythonBitmaps( 1 ), wxDefaultPosition, wxDefaultSize )
    item1.AddWindow( item12, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item0.AddSizer( item1, 1, wxADJUST_MINSIZE|wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 )

    item13 = wxStaticText( parent, wxPyError_ID_TEXT3, "The Python interpreter has encountered a so-called \"un-caught exception\".", wxDefaultPosition, wxDefaultSize, 0 )
    item0.AddWindow( item13, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item15 = wxStaticBox( parent, -1, "Exception information" )
    item15.SetFont( wxFont( 6, wxSWISS, wxITALIC, wxNORMAL ) )
    item14 = wxStaticBoxSizer( item15, wxVERTICAL )

    item16 = wxStaticText( parent, wxPyError_ID_TEXT4,
        "Please don't worry if this doesn't mean anything to you.\n"
        "It will be included in the \"bug report\" mentioned below, along with a \"stack traceback\".",
        wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE )
    item16.SetFont( wxFont( 8, wxROMAN, wxNORMAL, wxNORMAL ) )
    item14.AddWindow( item16, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item17 = wxFlexGridSizer( 2, 0, 1, 1 )
    item17.AddGrowableCol( 1 )

    item18 = wxStaticText( parent, wxPyError_ID_TEXT5, "Name:", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT )
    item18.SetFont( wxFont( 10, wxROMAN, wxITALIC, wxNORMAL ) )
    item18.SetToolTip( wxToolTip("This gives the type of the error.") )
    item17.AddWindow( item18, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item19 = wxStaticText( parent, wxPyError_ID_EXCEPTIONNAME, "wxPyError_ID_EXCEPTIONNAME", wxDefaultPosition, wxDefaultSize, 0 )
    item17.AddWindow( item19, 0, wxADJUST_MINSIZE|wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 )

    item20 = wxStaticText( parent, wxPyError_ID_TEXT6,
        "Extra\n"
        "information:",
        wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE )
    item20.SetFont( wxFont( 10, wxROMAN, wxITALIC, wxNORMAL ) )
    item17.AddWindow( item20, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item21 = wxStaticText( parent, wxPyError_ID_EXTRAINFORMATION, "wxPyError_ID_EXTRAINFORMATION", wxDefaultPosition, wxDefaultSize, 0 )
    item17.AddWindow( item21, 0, wxADJUST_MINSIZE|wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 )

    item14.AddSizer( item17, 0, wxADJUST_MINSIZE|wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 )

    item0.AddSizer( item14, 1, wxADJUST_MINSIZE|wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 10 )

    item22 = wxStaticText( parent, wxPyError_ID_TEXT7, "Please select one of the options below.", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE )
    item22.SetFont( wxFont( 8, wxROMAN, wxITALIC, wxNORMAL ) )
    item0.AddWindow( item22, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item23 = wxFlexGridSizer( 3, 0, 0, 6 )
    item23.AddGrowableCol( 0 )
    item23.AddGrowableCol( 1 )
    item23.AddGrowableCol( 2 )

    item24 = wxBoxSizer( wxHORIZONTAL )

    item25 = wxButton( parent, wxPyError_ID_MAIL, "E-mail support", wxDefaultPosition, wxDefaultSize, 0 )
    item24.AddWindow( item25, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item26 = wxBoxSizer( wxVERTICAL )

    item27 = wxStaticText( parent, wxPyError_ID_TEXT8, "Your e-mail address:", wxDefaultPosition, wxDefaultSize, 0 )
    item27.SetFont( wxFont( 8, wxROMAN, wxITALIC, wxNORMAL ) )
    item26.AddWindow( item27, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item28 = wxTextCtrl( parent, wxPyError_ID_ADDRESS, "", wxDefaultPosition, wxSize(80,-1), 0 )
    item26.AddWindow( item28, 2, wxADJUST_MINSIZE|wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 )

    item24.AddSizer( item26, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item23.AddSizer( item24, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item29 = wxButton( parent, wxPyError_ID_EXIT, "Exit immediately", wxDefaultPosition, wxDefaultSize, 0 )
    item29.SetDefault()
    item23.AddWindow( item29, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item30 = wxStaticText( parent, wxPyError_ID_TEXT9, "E-mail a \"bug report\" (if this is indeed a bug!).", wxDefaultPosition, wxDefaultSize, 0 )
    item23.AddWindow( item30, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item31 = wxStaticText( parent, wxPyError_ID_TEXT10, "Attempt to exit immediately.", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT )
    item23.AddWindow( item31, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE, 5 )

    item32 = wxStaticText( parent, wxPyError_ID_TEXT11, "(Please read any accompanying documentation first!)", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE )
    item32.SetFont( wxFont( 7, wxROMAN, wxNORMAL, wxBOLD ) )
    item23.AddWindow( item32, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item33 = wxStaticText( parent, wxPyError_ID_TEXT12, "(Please note that no attempt to save unsaved data will be made.)", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT )
    item33.SetFont( wxFont( 7, wxROMAN, wxNORMAL, wxBOLD ) )
    item23.AddWindow( item33, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item0.AddSizer( item23, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    if set_sizer == True:
        parent.SetAutoLayout( True )
        parent.SetSizer( item0 )
        if call_fit == True:
            item0.Fit( parent )
            item0.SetSizeHints( parent )

    return item0

wxPyError_ID_HTML = 10027

def populate_wxPyNonFatalError( parent, call_fit = True, set_sizer = True ):
    item0 = wxBoxSizer( wxVERTICAL )

    item1 = wxBoxSizer( wxHORIZONTAL )

    item3 = wxStaticBox( parent, -1, "Non-fatal" )
    item3.SetFont( wxFont( 9, wxSWISS, wxNORMAL, wxBOLD ) )
    item2 = wxStaticBoxSizer( item3, wxVERTICAL )

    item4 = wxBoxSizer( wxHORIZONTAL )

    item5 = wxStaticText( parent, wxPyError_ID_TEXT1, "Error in ", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE )
    item5.SetForegroundColour( wxWHITE )
    item5.SetBackgroundColour( wxRED )
    item5.SetFont( wxFont( 21, wxSCRIPT, wxNORMAL, wxBOLD ) )
    item4.AddWindow( item5, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item6 = wxStaticText( parent, wxPyError_ID_PROGRAMNAME, "wxPyError_ID_PROGRAMNAME", wxDefaultPosition, wxDefaultSize, 0 )
    item6.SetFont( wxFont( 21, wxROMAN, wxITALIC, wxNORMAL ) )
    item4.AddWindow( item6, 1, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item2.AddSizer( item4, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item7 = wxBoxSizer( wxHORIZONTAL )

    item8 = wxStaticText( parent, wxPyError_ID_TEXT2, "Version ", wxDefaultPosition, wxDefaultSize, 0 )
    item8.SetFont( wxFont( 9, wxROMAN, wxNORMAL, wxNORMAL ) )
    item7.AddWindow( item8, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item9 = wxStaticText( parent, wxPyError_ID_VERSIONNUMBER, "wxPyError_ID_VERSIONNUMBER", wxDefaultPosition, wxDefaultSize, 0 )
    item9.SetFont( wxFont( 12, wxROMAN, wxNORMAL, wxBOLD ) )
    item7.AddWindow( item9, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item2.AddSizer( item7, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item10 = wxStaticText( parent, wxPyError_ID_EXTRA_VERSION_INFORMATION, "wxPyError_ID_EXTRA_VERSION_INFORMATION", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE )
    item10.SetFont( wxFont( 7, wxROMAN, wxITALIC, wxNORMAL ) )
    item2.AddWindow( item10, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item1.AddSizer( item2, 1, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item11 = wxStaticBitmap( parent, wxPyError_ID_STATICBITMAP1, PythonBitmaps( 0 ), wxDefaultPosition, wxDefaultSize )
    item1.AddWindow( item11, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item12 = wxStaticBitmap( parent, wxPyError_ID_STATICBITMAP2, PythonBitmaps( 1 ), wxDefaultPosition, wxDefaultSize )
    item1.AddWindow( item12, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item0.AddSizer( item1, 0, wxADJUST_MINSIZE|wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 )

    parent.sizerAroundText = item13 = wxBoxSizer( wxVERTICAL )

    item14 = parent.HTMLWindow = wxPyClickableHtmlWindow( parent, wxPyError_ID_HTML )
    item13.AddWindow( item14, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item0.AddSizer( item13, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item15 = wxFlexGridSizer( 3, 0, 0, 6 )
    item15.AddGrowableCol( 0 )
    item15.AddGrowableCol( 1 )
    item15.AddGrowableCol( 2 )

    item16 = wxButton( parent, wxID_OK, "OK", wxDefaultPosition, wxDefaultSize, 0 )
    item16.SetDefault()
    item15.AddWindow( item16, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item17 = wxButton( parent, wxID_CANCEL, "Cancel", wxDefaultPosition, wxDefaultSize, 0 )
    item15.AddWindow( item17, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item18 = wxStaticText( parent, wxPyError_ID_TEXT1, "", wxDefaultPosition, wxDefaultSize, 0 )
    item15.AddWindow( item18, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item19 = wxStaticText( parent, wxPyError_ID_TEXT2, "", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT )
    item15.AddWindow( item19, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE, 5 )

    item0.AddSizer( item15, 1, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    if set_sizer == True:
        parent.SetAutoLayout( True )
        parent.SetSizer( item0 )
        if call_fit == True:
            item0.Fit( parent )
            item0.SetSizeHints( parent )

    return item0


def populate_wxPyFatalError( parent, call_fit = True, set_sizer = True ):
    item0 = wxBoxSizer( wxVERTICAL )

    item1 = wxBoxSizer( wxHORIZONTAL )

    item3 = wxStaticBox( parent, -1, "Fatal" )
    item3.SetFont( wxFont( 9, wxSWISS, wxNORMAL, wxBOLD ) )
    item2 = wxStaticBoxSizer( item3, wxVERTICAL )

    item4 = wxBoxSizer( wxHORIZONTAL )

    item5 = wxStaticText( parent, wxPyError_ID_TEXT1, "Error in ", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE )
    item5.SetForegroundColour( wxWHITE )
    item5.SetBackgroundColour( wxRED )
    item5.SetFont( wxFont( 21, wxSCRIPT, wxNORMAL, wxBOLD ) )
    item4.AddWindow( item5, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item6 = wxStaticText( parent, wxPyError_ID_PROGRAMNAME, "wxPyError_ID_PROGRAMNAME", wxDefaultPosition, wxDefaultSize, 0 )
    item6.SetFont( wxFont( 21, wxROMAN, wxITALIC, wxNORMAL ) )
    item4.AddWindow( item6, 1, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item2.AddSizer( item4, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item7 = wxBoxSizer( wxHORIZONTAL )

    item8 = wxStaticText( parent, wxPyError_ID_TEXT2, "Version ", wxDefaultPosition, wxDefaultSize, 0 )
    item8.SetFont( wxFont( 9, wxROMAN, wxNORMAL, wxNORMAL ) )
    item7.AddWindow( item8, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item9 = wxStaticText( parent, wxPyError_ID_VERSIONNUMBER, "wxPyError_ID_VERSIONNUMBER", wxDefaultPosition, wxDefaultSize, 0 )
    item9.SetFont( wxFont( 12, wxROMAN, wxNORMAL, wxBOLD ) )
    item7.AddWindow( item9, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item2.AddSizer( item7, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item10 = wxStaticText( parent, wxPyError_ID_EXTRA_VERSION_INFORMATION, "wxPyError_ID_EXTRA_VERSION_INFORMATION", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE )
    item10.SetFont( wxFont( 7, wxROMAN, wxITALIC, wxNORMAL ) )
    item2.AddWindow( item10, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item1.AddSizer( item2, 1, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item11 = wxStaticBitmap( parent, wxPyError_ID_STATICBITMAP1, PythonBitmaps( 0 ), wxDefaultPosition, wxDefaultSize )
    item1.AddWindow( item11, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item12 = wxStaticBitmap( parent, wxPyError_ID_STATICBITMAP2, PythonBitmaps( 1 ), wxDefaultPosition, wxDefaultSize )
    item1.AddWindow( item12, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item0.AddSizer( item1, 1, wxADJUST_MINSIZE|wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 )

    parent.sizerAroundText = item13 = wxBoxSizer( wxVERTICAL )

    item14 = parent.HTMLWindow = wxPyClickableHtmlWindow( parent, wxPyError_ID_HTML )
    item13.AddWindow( item14, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item0.AddSizer( item13, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item15 = wxFlexGridSizer( 2, 0, 0, 6 )
    item15.AddGrowableCol( 0 )

    item16 = wxButton( parent, wxID_OK, "OK", wxDefaultPosition, wxDefaultSize, 0 )
    item16.SetDefault()
    item15.AddWindow( item16, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item18 = wxStaticText( parent, wxPyError_ID_TEXT3, "", wxDefaultPosition, wxDefaultSize, 0 )
    item15.AddWindow( item18, 0, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    item0.AddSizer( item15, 1, wxADJUST_MINSIZE|wxALIGN_CENTRE|wxALL, 5 )

    if set_sizer == True:
        parent.SetAutoLayout( True )
        parent.SetSizer( item0 )
        if call_fit == True:
            item0.Fit( parent )
            item0.SetSizeHints( parent )

    return item0

# End of generated file
