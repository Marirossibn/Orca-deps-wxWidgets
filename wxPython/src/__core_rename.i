// A bunch of %rename directives generated by BuildRenamers in config.py
// in order to remove the wx prefix from all global scope names.

#ifndef BUILDING_RENAMERS

%rename(NOT_FOUND)                          wxNOT_FOUND;
%rename(VSCROLL)                            wxVSCROLL;
%rename(HSCROLL)                            wxHSCROLL;
%rename(CAPTION)                            wxCAPTION;
%rename(DOUBLE_BORDER)                      wxDOUBLE_BORDER;
%rename(SUNKEN_BORDER)                      wxSUNKEN_BORDER;
%rename(RAISED_BORDER)                      wxRAISED_BORDER;
%rename(BORDER)                             wxBORDER;
%rename(SIMPLE_BORDER)                      wxSIMPLE_BORDER;
%rename(STATIC_BORDER)                      wxSTATIC_BORDER;
%rename(TRANSPARENT_WINDOW)                 wxTRANSPARENT_WINDOW;
%rename(NO_BORDER)                          wxNO_BORDER;
%rename(TAB_TRAVERSAL)                      wxTAB_TRAVERSAL;
%rename(WANTS_CHARS)                        wxWANTS_CHARS;
%rename(POPUP_WINDOW)                       wxPOPUP_WINDOW;
%rename(CENTER_FRAME)                       wxCENTER_FRAME;
%rename(CENTRE_ON_SCREEN)                   wxCENTRE_ON_SCREEN;
%rename(CENTER_ON_SCREEN)                   wxCENTER_ON_SCREEN;
%rename(ED_CLIENT_MARGIN)                   wxED_CLIENT_MARGIN;
%rename(ED_BUTTONS_BOTTOM)                  wxED_BUTTONS_BOTTOM;
%rename(ED_BUTTONS_RIGHT)                   wxED_BUTTONS_RIGHT;
%rename(ED_STATIC_LINE)                     wxED_STATIC_LINE;
%rename(EXT_DIALOG_STYLE)                   wxEXT_DIALOG_STYLE;
%rename(CLIP_CHILDREN)                      wxCLIP_CHILDREN;
%rename(CLIP_SIBLINGS)                      wxCLIP_SIBLINGS;
%rename(ALWAYS_SHOW_SB)                     wxALWAYS_SHOW_SB;
%rename(RETAINED)                           wxRETAINED;
%rename(BACKINGSTORE)                       wxBACKINGSTORE;
%rename(COLOURED)                           wxCOLOURED;
%rename(FIXED_LENGTH)                       wxFIXED_LENGTH;
%rename(LB_NEEDED_SB)                       wxLB_NEEDED_SB;
%rename(LB_ALWAYS_SB)                       wxLB_ALWAYS_SB;
%rename(LB_SORT)                            wxLB_SORT;
%rename(LB_SINGLE)                          wxLB_SINGLE;
%rename(LB_MULTIPLE)                        wxLB_MULTIPLE;
%rename(LB_EXTENDED)                        wxLB_EXTENDED;
%rename(LB_OWNERDRAW)                       wxLB_OWNERDRAW;
%rename(LB_HSCROLL)                         wxLB_HSCROLL;
%rename(PROCESS_ENTER)                      wxPROCESS_ENTER;
%rename(PASSWORD)                           wxPASSWORD;
%rename(CB_SIMPLE)                          wxCB_SIMPLE;
%rename(CB_DROPDOWN)                        wxCB_DROPDOWN;
%rename(CB_SORT)                            wxCB_SORT;
%rename(CB_READONLY)                        wxCB_READONLY;
%rename(RA_HORIZONTAL)                      wxRA_HORIZONTAL;
%rename(RA_VERTICAL)                        wxRA_VERTICAL;
%rename(RA_SPECIFY_ROWS)                    wxRA_SPECIFY_ROWS;
%rename(RA_SPECIFY_COLS)                    wxRA_SPECIFY_COLS;
%rename(RB_GROUP)                           wxRB_GROUP;
%rename(RB_SINGLE)                          wxRB_SINGLE;
%rename(SL_HORIZONTAL)                      wxSL_HORIZONTAL;
%rename(SL_VERTICAL)                        wxSL_VERTICAL;
%rename(SL_AUTOTICKS)                       wxSL_AUTOTICKS;
%rename(SL_LABELS)                          wxSL_LABELS;
%rename(SL_LEFT)                            wxSL_LEFT;
%rename(SL_TOP)                             wxSL_TOP;
%rename(SL_RIGHT)                           wxSL_RIGHT;
%rename(SL_BOTTOM)                          wxSL_BOTTOM;
%rename(SL_BOTH)                            wxSL_BOTH;
%rename(SL_SELRANGE)                        wxSL_SELRANGE;
%rename(SB_HORIZONTAL)                      wxSB_HORIZONTAL;
%rename(SB_VERTICAL)                        wxSB_VERTICAL;
%rename(ST_SIZEGRIP)                        wxST_SIZEGRIP;
%rename(ST_NO_AUTORESIZE)                   wxST_NO_AUTORESIZE;
%rename(FLOOD_SURFACE)                      wxFLOOD_SURFACE;
%rename(FLOOD_BORDER)                       wxFLOOD_BORDER;
%rename(ODDEVEN_RULE)                       wxODDEVEN_RULE;
%rename(WINDING_RULE)                       wxWINDING_RULE;
%rename(TOOL_TOP)                           wxTOOL_TOP;
%rename(TOOL_BOTTOM)                        wxTOOL_BOTTOM;
%rename(TOOL_LEFT)                          wxTOOL_LEFT;
%rename(TOOL_RIGHT)                         wxTOOL_RIGHT;
%rename(OK)                                 wxOK;
%rename(YES_NO)                             wxYES_NO;
%rename(CANCEL)                             wxCANCEL;
%rename(YES)                                wxYES;
%rename(NO)                                 wxNO;
%rename(NO_DEFAULT)                         wxNO_DEFAULT;
%rename(YES_DEFAULT)                        wxYES_DEFAULT;
%rename(ICON_EXCLAMATION)                   wxICON_EXCLAMATION;
%rename(ICON_HAND)                          wxICON_HAND;
%rename(ICON_QUESTION)                      wxICON_QUESTION;
%rename(ICON_INFORMATION)                   wxICON_INFORMATION;
%rename(ICON_STOP)                          wxICON_STOP;
%rename(ICON_ASTERISK)                      wxICON_ASTERISK;
%rename(ICON_MASK)                          wxICON_MASK;
%rename(ICON_WARNING)                       wxICON_WARNING;
%rename(ICON_ERROR)                         wxICON_ERROR;
%rename(FORWARD)                            wxFORWARD;
%rename(BACKWARD)                           wxBACKWARD;
%rename(RESET)                              wxRESET;
%rename(HELP)                               wxHELP;
%rename(MORE)                               wxMORE;
%rename(SETUP)                              wxSETUP;
%rename(SIZE_AUTO_WIDTH)                    wxSIZE_AUTO_WIDTH;
%rename(SIZE_AUTO_HEIGHT)                   wxSIZE_AUTO_HEIGHT;
%rename(SIZE_AUTO)                          wxSIZE_AUTO;
%rename(SIZE_USE_EXISTING)                  wxSIZE_USE_EXISTING;
%rename(SIZE_ALLOW_MINUS_ONE)               wxSIZE_ALLOW_MINUS_ONE;
%rename(PORTRAIT)                           wxPORTRAIT;
%rename(LANDSCAPE)                          wxLANDSCAPE;
%rename(PRINT_QUALITY_HIGH)                 wxPRINT_QUALITY_HIGH;
%rename(PRINT_QUALITY_MEDIUM)               wxPRINT_QUALITY_MEDIUM;
%rename(PRINT_QUALITY_LOW)                  wxPRINT_QUALITY_LOW;
%rename(PRINT_QUALITY_DRAFT)                wxPRINT_QUALITY_DRAFT;
%rename(ID_ANY)                             wxID_ANY;
%rename(ID_SEPARATOR)                       wxID_SEPARATOR;
%rename(ID_LOWEST)                          wxID_LOWEST;
%rename(ID_OPEN)                            wxID_OPEN;
%rename(ID_CLOSE)                           wxID_CLOSE;
%rename(ID_NEW)                             wxID_NEW;
%rename(ID_SAVE)                            wxID_SAVE;
%rename(ID_SAVEAS)                          wxID_SAVEAS;
%rename(ID_REVERT)                          wxID_REVERT;
%rename(ID_EXIT)                            wxID_EXIT;
%rename(ID_UNDO)                            wxID_UNDO;
%rename(ID_REDO)                            wxID_REDO;
%rename(ID_HELP)                            wxID_HELP;
%rename(ID_PRINT)                           wxID_PRINT;
%rename(ID_PRINT_SETUP)                     wxID_PRINT_SETUP;
%rename(ID_PREVIEW)                         wxID_PREVIEW;
%rename(ID_ABOUT)                           wxID_ABOUT;
%rename(ID_HELP_CONTENTS)                   wxID_HELP_CONTENTS;
%rename(ID_HELP_COMMANDS)                   wxID_HELP_COMMANDS;
%rename(ID_HELP_PROCEDURES)                 wxID_HELP_PROCEDURES;
%rename(ID_HELP_CONTEXT)                    wxID_HELP_CONTEXT;
%rename(ID_CLOSE_ALL)                       wxID_CLOSE_ALL;
%rename(ID_PREFERENCES)                     wxID_PREFERENCES;
%rename(ID_CUT)                             wxID_CUT;
%rename(ID_COPY)                            wxID_COPY;
%rename(ID_PASTE)                           wxID_PASTE;
%rename(ID_CLEAR)                           wxID_CLEAR;
%rename(ID_FIND)                            wxID_FIND;
%rename(ID_DUPLICATE)                       wxID_DUPLICATE;
%rename(ID_SELECTALL)                       wxID_SELECTALL;
%rename(ID_DELETE)                          wxID_DELETE;
%rename(ID_REPLACE)                         wxID_REPLACE;
%rename(ID_REPLACE_ALL)                     wxID_REPLACE_ALL;
%rename(ID_PROPERTIES)                      wxID_PROPERTIES;
%rename(ID_VIEW_DETAILS)                    wxID_VIEW_DETAILS;
%rename(ID_VIEW_LARGEICONS)                 wxID_VIEW_LARGEICONS;
%rename(ID_VIEW_SMALLICONS)                 wxID_VIEW_SMALLICONS;
%rename(ID_VIEW_LIST)                       wxID_VIEW_LIST;
%rename(ID_VIEW_SORTDATE)                   wxID_VIEW_SORTDATE;
%rename(ID_VIEW_SORTNAME)                   wxID_VIEW_SORTNAME;
%rename(ID_VIEW_SORTSIZE)                   wxID_VIEW_SORTSIZE;
%rename(ID_VIEW_SORTTYPE)                   wxID_VIEW_SORTTYPE;
%rename(ID_FILE1)                           wxID_FILE1;
%rename(ID_FILE2)                           wxID_FILE2;
%rename(ID_FILE3)                           wxID_FILE3;
%rename(ID_FILE4)                           wxID_FILE4;
%rename(ID_FILE5)                           wxID_FILE5;
%rename(ID_FILE6)                           wxID_FILE6;
%rename(ID_FILE7)                           wxID_FILE7;
%rename(ID_FILE8)                           wxID_FILE8;
%rename(ID_FILE9)                           wxID_FILE9;
%rename(ID_OK)                              wxID_OK;
%rename(ID_CANCEL)                          wxID_CANCEL;
%rename(ID_APPLY)                           wxID_APPLY;
%rename(ID_YES)                             wxID_YES;
%rename(ID_NO)                              wxID_NO;
%rename(ID_STATIC)                          wxID_STATIC;
%rename(ID_FORWARD)                         wxID_FORWARD;
%rename(ID_BACKWARD)                        wxID_BACKWARD;
%rename(ID_DEFAULT)                         wxID_DEFAULT;
%rename(ID_MORE)                            wxID_MORE;
%rename(ID_SETUP)                           wxID_SETUP;
%rename(ID_RESET)                           wxID_RESET;
%rename(ID_CONTEXT_HELP)                    wxID_CONTEXT_HELP;
%rename(ID_YESTOALL)                        wxID_YESTOALL;
%rename(ID_NOTOALL)                         wxID_NOTOALL;
%rename(ID_ABORT)                           wxID_ABORT;
%rename(ID_RETRY)                           wxID_RETRY;
%rename(ID_IGNORE)                          wxID_IGNORE;
%rename(ID_ADD)                             wxID_ADD;
%rename(ID_REMOVE)                          wxID_REMOVE;
%rename(ID_UP)                              wxID_UP;
%rename(ID_DOWN)                            wxID_DOWN;
%rename(ID_HOME)                            wxID_HOME;
%rename(ID_REFRESH)                         wxID_REFRESH;
%rename(ID_STOP)                            wxID_STOP;
%rename(ID_INDEX)                           wxID_INDEX;
%rename(ID_BOLD)                            wxID_BOLD;
%rename(ID_ITALIC)                          wxID_ITALIC;
%rename(ID_JUSTIFY_CENTER)                  wxID_JUSTIFY_CENTER;
%rename(ID_JUSTIFY_FILL)                    wxID_JUSTIFY_FILL;
%rename(ID_JUSTIFY_RIGHT)                   wxID_JUSTIFY_RIGHT;
%rename(ID_JUSTIFY_LEFT)                    wxID_JUSTIFY_LEFT;
%rename(ID_UNDERLINE)                       wxID_UNDERLINE;
%rename(ID_INDENT)                          wxID_INDENT;
%rename(ID_UNINDENT)                        wxID_UNINDENT;
%rename(ID_ZOOM_100)                        wxID_ZOOM_100;
%rename(ID_ZOOM_FIT)                        wxID_ZOOM_FIT;
%rename(ID_ZOOM_IN)                         wxID_ZOOM_IN;
%rename(ID_ZOOM_OUT)                        wxID_ZOOM_OUT;
%rename(ID_UNDELETE)                        wxID_UNDELETE;
%rename(ID_REVERT_TO_SAVED)                 wxID_REVERT_TO_SAVED;
%rename(ID_HIGHEST)                         wxID_HIGHEST;
%rename(OPEN)                               wxOPEN;
%rename(SAVE)                               wxSAVE;
%rename(HIDE_READONLY)                      wxHIDE_READONLY;
%rename(OVERWRITE_PROMPT)                   wxOVERWRITE_PROMPT;
%rename(FILE_MUST_EXIST)                    wxFILE_MUST_EXIST;
%rename(MULTIPLE)                           wxMULTIPLE;
%rename(CHANGE_DIR)                         wxCHANGE_DIR;
%rename(ACCEL_ALT)                          wxACCEL_ALT;
%rename(ACCEL_CTRL)                         wxACCEL_CTRL;
%rename(ACCEL_SHIFT)                        wxACCEL_SHIFT;
%rename(ACCEL_NORMAL)                       wxACCEL_NORMAL;
%rename(PD_AUTO_HIDE)                       wxPD_AUTO_HIDE;
%rename(PD_APP_MODAL)                       wxPD_APP_MODAL;
%rename(PD_CAN_ABORT)                       wxPD_CAN_ABORT;
%rename(PD_ELAPSED_TIME)                    wxPD_ELAPSED_TIME;
%rename(PD_ESTIMATED_TIME)                  wxPD_ESTIMATED_TIME;
%rename(PD_REMAINING_TIME)                  wxPD_REMAINING_TIME;
%rename(DD_NEW_DIR_BUTTON)                  wxDD_NEW_DIR_BUTTON;
%rename(DD_DEFAULT_STYLE)                   wxDD_DEFAULT_STYLE;
%rename(MENU_TEAROFF)                       wxMENU_TEAROFF;
%rename(MB_DOCKABLE)                        wxMB_DOCKABLE;
%rename(NO_FULL_REPAINT_ON_RESIZE)          wxNO_FULL_REPAINT_ON_RESIZE;
%rename(FULL_REPAINT_ON_RESIZE)             wxFULL_REPAINT_ON_RESIZE;
%rename(LI_HORIZONTAL)                      wxLI_HORIZONTAL;
%rename(LI_VERTICAL)                        wxLI_VERTICAL;
%rename(WS_EX_VALIDATE_RECURSIVELY)         wxWS_EX_VALIDATE_RECURSIVELY;
%rename(WS_EX_BLOCK_EVENTS)                 wxWS_EX_BLOCK_EVENTS;
%rename(WS_EX_TRANSIENT)                    wxWS_EX_TRANSIENT;
%rename(WS_EX_THEMED_BACKGROUND)            wxWS_EX_THEMED_BACKGROUND;
%rename(WS_EX_PROCESS_IDLE)                 wxWS_EX_PROCESS_IDLE;
%rename(WS_EX_PROCESS_UI_UPDATES)           wxWS_EX_PROCESS_UI_UPDATES;
%rename(MM_TEXT)                            wxMM_TEXT;
%rename(MM_LOMETRIC)                        wxMM_LOMETRIC;
%rename(MM_HIMETRIC)                        wxMM_HIMETRIC;
%rename(MM_LOENGLISH)                       wxMM_LOENGLISH;
%rename(MM_HIENGLISH)                       wxMM_HIENGLISH;
%rename(MM_TWIPS)                           wxMM_TWIPS;
%rename(MM_ISOTROPIC)                       wxMM_ISOTROPIC;
%rename(MM_ANISOTROPIC)                     wxMM_ANISOTROPIC;
%rename(MM_POINTS)                          wxMM_POINTS;
%rename(MM_METRIC)                          wxMM_METRIC;
%rename(CENTRE)                             wxCENTRE;
%rename(CENTER)                             wxCENTER;
%rename(HORIZONTAL)                         wxHORIZONTAL;
%rename(VERTICAL)                           wxVERTICAL;
%rename(BOTH)                               wxBOTH;
%rename(LEFT)                               wxLEFT;
%rename(RIGHT)                              wxRIGHT;
%rename(UP)                                 wxUP;
%rename(DOWN)                               wxDOWN;
%rename(TOP)                                wxTOP;
%rename(BOTTOM)                             wxBOTTOM;
%rename(NORTH)                              wxNORTH;
%rename(SOUTH)                              wxSOUTH;
%rename(WEST)                               wxWEST;
%rename(EAST)                               wxEAST;
%rename(ALL)                                wxALL;
%rename(ALIGN_NOT)                          wxALIGN_NOT;
%rename(ALIGN_CENTER_HORIZONTAL)            wxALIGN_CENTER_HORIZONTAL;
%rename(ALIGN_CENTRE_HORIZONTAL)            wxALIGN_CENTRE_HORIZONTAL;
%rename(ALIGN_LEFT)                         wxALIGN_LEFT;
%rename(ALIGN_TOP)                          wxALIGN_TOP;
%rename(ALIGN_RIGHT)                        wxALIGN_RIGHT;
%rename(ALIGN_BOTTOM)                       wxALIGN_BOTTOM;
%rename(ALIGN_CENTER_VERTICAL)              wxALIGN_CENTER_VERTICAL;
%rename(ALIGN_CENTRE_VERTICAL)              wxALIGN_CENTRE_VERTICAL;
%rename(ALIGN_CENTER)                       wxALIGN_CENTER;
%rename(ALIGN_CENTRE)                       wxALIGN_CENTRE;
%rename(ALIGN_MASK)                         wxALIGN_MASK;
%rename(STRETCH_NOT)                        wxSTRETCH_NOT;
%rename(SHRINK)                             wxSHRINK;
%rename(GROW)                               wxGROW;
%rename(EXPAND)                             wxEXPAND;
%rename(SHAPED)                             wxSHAPED;
%rename(FIXED_MINSIZE)                      wxFIXED_MINSIZE;
%rename(TILE)                               wxTILE;
%rename(ADJUST_MINSIZE)                     wxADJUST_MINSIZE;
%rename(BORDER_DEFAULT)                     wxBORDER_DEFAULT;
%rename(BORDER_NONE)                        wxBORDER_NONE;
%rename(BORDER_STATIC)                      wxBORDER_STATIC;
%rename(BORDER_SIMPLE)                      wxBORDER_SIMPLE;
%rename(BORDER_RAISED)                      wxBORDER_RAISED;
%rename(BORDER_SUNKEN)                      wxBORDER_SUNKEN;
%rename(BORDER_DOUBLE)                      wxBORDER_DOUBLE;
%rename(BORDER_MASK)                        wxBORDER_MASK;
%rename(BG_STYLE_SYSTEM)                    wxBG_STYLE_SYSTEM;
%rename(BG_STYLE_COLOUR)                    wxBG_STYLE_COLOUR;
%rename(BG_STYLE_CUSTOM)                    wxBG_STYLE_CUSTOM;
%rename(DEFAULT)                            wxDEFAULT;
%rename(DECORATIVE)                         wxDECORATIVE;
%rename(ROMAN)                              wxROMAN;
%rename(SCRIPT)                             wxSCRIPT;
%rename(SWISS)                              wxSWISS;
%rename(MODERN)                             wxMODERN;
%rename(TELETYPE)                           wxTELETYPE;
%rename(VARIABLE)                           wxVARIABLE;
%rename(FIXED)                              wxFIXED;
%rename(NORMAL)                             wxNORMAL;
%rename(LIGHT)                              wxLIGHT;
%rename(BOLD)                               wxBOLD;
%rename(ITALIC)                             wxITALIC;
%rename(SLANT)                              wxSLANT;
%rename(SOLID)                              wxSOLID;
%rename(DOT)                                wxDOT;
%rename(LONG_DASH)                          wxLONG_DASH;
%rename(SHORT_DASH)                         wxSHORT_DASH;
%rename(DOT_DASH)                           wxDOT_DASH;
%rename(USER_DASH)                          wxUSER_DASH;
%rename(TRANSPARENT)                        wxTRANSPARENT;
%rename(STIPPLE)                            wxSTIPPLE;
%rename(BDIAGONAL_HATCH)                    wxBDIAGONAL_HATCH;
%rename(CROSSDIAG_HATCH)                    wxCROSSDIAG_HATCH;
%rename(FDIAGONAL_HATCH)                    wxFDIAGONAL_HATCH;
%rename(CROSS_HATCH)                        wxCROSS_HATCH;
%rename(HORIZONTAL_HATCH)                   wxHORIZONTAL_HATCH;
%rename(VERTICAL_HATCH)                     wxVERTICAL_HATCH;
%rename(JOIN_BEVEL)                         wxJOIN_BEVEL;
%rename(JOIN_MITER)                         wxJOIN_MITER;
%rename(JOIN_ROUND)                         wxJOIN_ROUND;
%rename(CAP_ROUND)                          wxCAP_ROUND;
%rename(CAP_PROJECTING)                     wxCAP_PROJECTING;
%rename(CAP_BUTT)                           wxCAP_BUTT;
%rename(CLEAR)                              wxCLEAR;
%rename(XOR)                                wxXOR;
%rename(INVERT)                             wxINVERT;
%rename(OR_REVERSE)                         wxOR_REVERSE;
%rename(AND_REVERSE)                        wxAND_REVERSE;
%rename(COPY)                               wxCOPY;
%rename(AND)                                wxAND;
%rename(AND_INVERT)                         wxAND_INVERT;
%rename(NO_OP)                              wxNO_OP;
%rename(NOR)                                wxNOR;
%rename(EQUIV)                              wxEQUIV;
%rename(SRC_INVERT)                         wxSRC_INVERT;
%rename(OR_INVERT)                          wxOR_INVERT;
%rename(NAND)                               wxNAND;
%rename(OR)                                 wxOR;
%rename(SET)                                wxSET;
%rename(PAPER_NONE)                         wxPAPER_NONE;
%rename(PAPER_LETTER)                       wxPAPER_LETTER;
%rename(PAPER_LEGAL)                        wxPAPER_LEGAL;
%rename(PAPER_A4)                           wxPAPER_A4;
%rename(PAPER_CSHEET)                       wxPAPER_CSHEET;
%rename(PAPER_DSHEET)                       wxPAPER_DSHEET;
%rename(PAPER_ESHEET)                       wxPAPER_ESHEET;
%rename(PAPER_LETTERSMALL)                  wxPAPER_LETTERSMALL;
%rename(PAPER_TABLOID)                      wxPAPER_TABLOID;
%rename(PAPER_LEDGER)                       wxPAPER_LEDGER;
%rename(PAPER_STATEMENT)                    wxPAPER_STATEMENT;
%rename(PAPER_EXECUTIVE)                    wxPAPER_EXECUTIVE;
%rename(PAPER_A3)                           wxPAPER_A3;
%rename(PAPER_A4SMALL)                      wxPAPER_A4SMALL;
%rename(PAPER_A5)                           wxPAPER_A5;
%rename(PAPER_B4)                           wxPAPER_B4;
%rename(PAPER_B5)                           wxPAPER_B5;
%rename(PAPER_FOLIO)                        wxPAPER_FOLIO;
%rename(PAPER_QUARTO)                       wxPAPER_QUARTO;
%rename(PAPER_10X14)                        wxPAPER_10X14;
%rename(PAPER_11X17)                        wxPAPER_11X17;
%rename(PAPER_NOTE)                         wxPAPER_NOTE;
%rename(PAPER_ENV_9)                        wxPAPER_ENV_9;
%rename(PAPER_ENV_10)                       wxPAPER_ENV_10;
%rename(PAPER_ENV_11)                       wxPAPER_ENV_11;
%rename(PAPER_ENV_12)                       wxPAPER_ENV_12;
%rename(PAPER_ENV_14)                       wxPAPER_ENV_14;
%rename(PAPER_ENV_DL)                       wxPAPER_ENV_DL;
%rename(PAPER_ENV_C5)                       wxPAPER_ENV_C5;
%rename(PAPER_ENV_C3)                       wxPAPER_ENV_C3;
%rename(PAPER_ENV_C4)                       wxPAPER_ENV_C4;
%rename(PAPER_ENV_C6)                       wxPAPER_ENV_C6;
%rename(PAPER_ENV_C65)                      wxPAPER_ENV_C65;
%rename(PAPER_ENV_B4)                       wxPAPER_ENV_B4;
%rename(PAPER_ENV_B5)                       wxPAPER_ENV_B5;
%rename(PAPER_ENV_B6)                       wxPAPER_ENV_B6;
%rename(PAPER_ENV_ITALY)                    wxPAPER_ENV_ITALY;
%rename(PAPER_ENV_MONARCH)                  wxPAPER_ENV_MONARCH;
%rename(PAPER_ENV_PERSONAL)                 wxPAPER_ENV_PERSONAL;
%rename(PAPER_FANFOLD_US)                   wxPAPER_FANFOLD_US;
%rename(PAPER_FANFOLD_STD_GERMAN)           wxPAPER_FANFOLD_STD_GERMAN;
%rename(PAPER_FANFOLD_LGL_GERMAN)           wxPAPER_FANFOLD_LGL_GERMAN;
%rename(PAPER_ISO_B4)                       wxPAPER_ISO_B4;
%rename(PAPER_JAPANESE_POSTCARD)            wxPAPER_JAPANESE_POSTCARD;
%rename(PAPER_9X11)                         wxPAPER_9X11;
%rename(PAPER_10X11)                        wxPAPER_10X11;
%rename(PAPER_15X11)                        wxPAPER_15X11;
%rename(PAPER_ENV_INVITE)                   wxPAPER_ENV_INVITE;
%rename(PAPER_LETTER_EXTRA)                 wxPAPER_LETTER_EXTRA;
%rename(PAPER_LEGAL_EXTRA)                  wxPAPER_LEGAL_EXTRA;
%rename(PAPER_TABLOID_EXTRA)                wxPAPER_TABLOID_EXTRA;
%rename(PAPER_A4_EXTRA)                     wxPAPER_A4_EXTRA;
%rename(PAPER_LETTER_TRANSVERSE)            wxPAPER_LETTER_TRANSVERSE;
%rename(PAPER_A4_TRANSVERSE)                wxPAPER_A4_TRANSVERSE;
%rename(PAPER_LETTER_EXTRA_TRANSVERSE)      wxPAPER_LETTER_EXTRA_TRANSVERSE;
%rename(PAPER_A_PLUS)                       wxPAPER_A_PLUS;
%rename(PAPER_B_PLUS)                       wxPAPER_B_PLUS;
%rename(PAPER_LETTER_PLUS)                  wxPAPER_LETTER_PLUS;
%rename(PAPER_A4_PLUS)                      wxPAPER_A4_PLUS;
%rename(PAPER_A5_TRANSVERSE)                wxPAPER_A5_TRANSVERSE;
%rename(PAPER_B5_TRANSVERSE)                wxPAPER_B5_TRANSVERSE;
%rename(PAPER_A3_EXTRA)                     wxPAPER_A3_EXTRA;
%rename(PAPER_A5_EXTRA)                     wxPAPER_A5_EXTRA;
%rename(PAPER_B5_EXTRA)                     wxPAPER_B5_EXTRA;
%rename(PAPER_A2)                           wxPAPER_A2;
%rename(PAPER_A3_TRANSVERSE)                wxPAPER_A3_TRANSVERSE;
%rename(PAPER_A3_EXTRA_TRANSVERSE)          wxPAPER_A3_EXTRA_TRANSVERSE;
%rename(DUPLEX_SIMPLEX)                     wxDUPLEX_SIMPLEX;
%rename(DUPLEX_HORIZONTAL)                  wxDUPLEX_HORIZONTAL;
%rename(DUPLEX_VERTICAL)                    wxDUPLEX_VERTICAL;
%rename(ITEM_SEPARATOR)                     wxITEM_SEPARATOR;
%rename(ITEM_NORMAL)                        wxITEM_NORMAL;
%rename(ITEM_CHECK)                         wxITEM_CHECK;
%rename(ITEM_RADIO)                         wxITEM_RADIO;
%rename(ITEM_MAX)                           wxITEM_MAX;
%rename(HT_NOWHERE)                         wxHT_NOWHERE;
%rename(HT_SCROLLBAR_FIRST)                 wxHT_SCROLLBAR_FIRST;
%rename(HT_SCROLLBAR_ARROW_LINE_1)          wxHT_SCROLLBAR_ARROW_LINE_1;
%rename(HT_SCROLLBAR_ARROW_LINE_2)          wxHT_SCROLLBAR_ARROW_LINE_2;
%rename(HT_SCROLLBAR_ARROW_PAGE_1)          wxHT_SCROLLBAR_ARROW_PAGE_1;
%rename(HT_SCROLLBAR_ARROW_PAGE_2)          wxHT_SCROLLBAR_ARROW_PAGE_2;
%rename(HT_SCROLLBAR_THUMB)                 wxHT_SCROLLBAR_THUMB;
%rename(HT_SCROLLBAR_BAR_1)                 wxHT_SCROLLBAR_BAR_1;
%rename(HT_SCROLLBAR_BAR_2)                 wxHT_SCROLLBAR_BAR_2;
%rename(HT_SCROLLBAR_LAST)                  wxHT_SCROLLBAR_LAST;
%rename(HT_WINDOW_OUTSIDE)                  wxHT_WINDOW_OUTSIDE;
%rename(HT_WINDOW_INSIDE)                   wxHT_WINDOW_INSIDE;
%rename(HT_WINDOW_VERT_SCROLLBAR)           wxHT_WINDOW_VERT_SCROLLBAR;
%rename(HT_WINDOW_HORZ_SCROLLBAR)           wxHT_WINDOW_HORZ_SCROLLBAR;
%rename(HT_WINDOW_CORNER)                   wxHT_WINDOW_CORNER;
%rename(HT_MAX)                             wxHT_MAX;
%rename(MOD_NONE)                           wxMOD_NONE;
%rename(MOD_ALT)                            wxMOD_ALT;
%rename(MOD_CONTROL)                        wxMOD_CONTROL;
%rename(MOD_SHIFT)                          wxMOD_SHIFT;
%rename(MOD_WIN)                            wxMOD_WIN;
%rename(UPDATE_UI_NONE)                     wxUPDATE_UI_NONE;
%rename(UPDATE_UI_RECURSE)                  wxUPDATE_UI_RECURSE;
%rename(UPDATE_UI_FROMIDLE)                 wxUPDATE_UI_FROMIDLE;
%rename(Object)                             wxObject;
%rename(BITMAP_TYPE_INVALID)                wxBITMAP_TYPE_INVALID;
%rename(BITMAP_TYPE_BMP)                    wxBITMAP_TYPE_BMP;
%rename(BITMAP_TYPE_ICO)                    wxBITMAP_TYPE_ICO;
%rename(BITMAP_TYPE_CUR)                    wxBITMAP_TYPE_CUR;
%rename(BITMAP_TYPE_XBM)                    wxBITMAP_TYPE_XBM;
%rename(BITMAP_TYPE_XBM_DATA)               wxBITMAP_TYPE_XBM_DATA;
%rename(BITMAP_TYPE_XPM)                    wxBITMAP_TYPE_XPM;
%rename(BITMAP_TYPE_XPM_DATA)               wxBITMAP_TYPE_XPM_DATA;
%rename(BITMAP_TYPE_TIF)                    wxBITMAP_TYPE_TIF;
%rename(BITMAP_TYPE_GIF)                    wxBITMAP_TYPE_GIF;
%rename(BITMAP_TYPE_PNG)                    wxBITMAP_TYPE_PNG;
%rename(BITMAP_TYPE_JPEG)                   wxBITMAP_TYPE_JPEG;
%rename(BITMAP_TYPE_PNM)                    wxBITMAP_TYPE_PNM;
%rename(BITMAP_TYPE_PCX)                    wxBITMAP_TYPE_PCX;
%rename(BITMAP_TYPE_PICT)                   wxBITMAP_TYPE_PICT;
%rename(BITMAP_TYPE_ICON)                   wxBITMAP_TYPE_ICON;
%rename(BITMAP_TYPE_ANI)                    wxBITMAP_TYPE_ANI;
%rename(BITMAP_TYPE_IFF)                    wxBITMAP_TYPE_IFF;
%rename(BITMAP_TYPE_MACCURSOR)              wxBITMAP_TYPE_MACCURSOR;
%rename(BITMAP_TYPE_ANY)                    wxBITMAP_TYPE_ANY;
%rename(CURSOR_NONE)                        wxCURSOR_NONE;
%rename(CURSOR_ARROW)                       wxCURSOR_ARROW;
%rename(CURSOR_RIGHT_ARROW)                 wxCURSOR_RIGHT_ARROW;
%rename(CURSOR_BULLSEYE)                    wxCURSOR_BULLSEYE;
%rename(CURSOR_CHAR)                        wxCURSOR_CHAR;
%rename(CURSOR_CROSS)                       wxCURSOR_CROSS;
%rename(CURSOR_HAND)                        wxCURSOR_HAND;
%rename(CURSOR_IBEAM)                       wxCURSOR_IBEAM;
%rename(CURSOR_LEFT_BUTTON)                 wxCURSOR_LEFT_BUTTON;
%rename(CURSOR_MAGNIFIER)                   wxCURSOR_MAGNIFIER;
%rename(CURSOR_MIDDLE_BUTTON)               wxCURSOR_MIDDLE_BUTTON;
%rename(CURSOR_NO_ENTRY)                    wxCURSOR_NO_ENTRY;
%rename(CURSOR_PAINT_BRUSH)                 wxCURSOR_PAINT_BRUSH;
%rename(CURSOR_PENCIL)                      wxCURSOR_PENCIL;
%rename(CURSOR_POINT_LEFT)                  wxCURSOR_POINT_LEFT;
%rename(CURSOR_POINT_RIGHT)                 wxCURSOR_POINT_RIGHT;
%rename(CURSOR_QUESTION_ARROW)              wxCURSOR_QUESTION_ARROW;
%rename(CURSOR_RIGHT_BUTTON)                wxCURSOR_RIGHT_BUTTON;
%rename(CURSOR_SIZENESW)                    wxCURSOR_SIZENESW;
%rename(CURSOR_SIZENS)                      wxCURSOR_SIZENS;
%rename(CURSOR_SIZENWSE)                    wxCURSOR_SIZENWSE;
%rename(CURSOR_SIZEWE)                      wxCURSOR_SIZEWE;
%rename(CURSOR_SIZING)                      wxCURSOR_SIZING;
%rename(CURSOR_SPRAYCAN)                    wxCURSOR_SPRAYCAN;
%rename(CURSOR_WAIT)                        wxCURSOR_WAIT;
%rename(CURSOR_WATCH)                       wxCURSOR_WATCH;
%rename(CURSOR_BLANK)                       wxCURSOR_BLANK;
%rename(CURSOR_DEFAULT)                     wxCURSOR_DEFAULT;
%rename(CURSOR_COPY_ARROW)                  wxCURSOR_COPY_ARROW;
%rename(CURSOR_ARROWWAIT)                   wxCURSOR_ARROWWAIT;
%rename(CURSOR_MAX)                         wxCURSOR_MAX;
%rename(Size)                               wxSize;
%rename(RealPoint)                          wxRealPoint;
%rename(Point)                              wxPoint;
%rename(Rect)                               wxRect;
%rename(IntersectRect)                      wxIntersectRect;
%rename(Point2D)                            wxPoint2D;
%rename(DefaultPosition)                    wxDefaultPosition;
%rename(DefaultSize)                        wxDefaultSize;
%rename(FromStart)                          wxFromStart;
%rename(FromCurrent)                        wxFromCurrent;
%rename(FromEnd)                            wxFromEnd;
%rename(OutputStream)                       wxOutputStream;
%rename(FSFile)                             wxFSFile;
%rename(FileSystem)                         wxFileSystem;
%rename(InternetFSHandler)                  wxInternetFSHandler;
%rename(ZipFSHandler)                       wxZipFSHandler;
%rename(MemoryFSHandler)                    wxMemoryFSHandler;
%rename(ImageHandler)                       wxImageHandler;
%rename(ImageHistogram)                     wxImageHistogram;
%rename(Image)                              wxImage;
%rename(NullImage)                          wxNullImage;
%rename(IMAGE_RESOLUTION_INCHES)            wxIMAGE_RESOLUTION_INCHES;
%rename(IMAGE_RESOLUTION_CM)                wxIMAGE_RESOLUTION_CM;
%rename(BMP_24BPP)                          wxBMP_24BPP;
%rename(BMP_8BPP)                           wxBMP_8BPP;
%rename(BMP_8BPP_GREY)                      wxBMP_8BPP_GREY;
%rename(BMP_8BPP_GRAY)                      wxBMP_8BPP_GRAY;
%rename(BMP_8BPP_RED)                       wxBMP_8BPP_RED;
%rename(BMP_8BPP_PALETTE)                   wxBMP_8BPP_PALETTE;
%rename(BMP_4BPP)                           wxBMP_4BPP;
%rename(BMP_1BPP)                           wxBMP_1BPP;
%rename(BMP_1BPP_BW)                        wxBMP_1BPP_BW;
%rename(BMPHandler)                         wxBMPHandler;
%rename(ICOHandler)                         wxICOHandler;
%rename(CURHandler)                         wxCURHandler;
%rename(ANIHandler)                         wxANIHandler;
%rename(PNGHandler)                         wxPNGHandler;
%rename(GIFHandler)                         wxGIFHandler;
%rename(PCXHandler)                         wxPCXHandler;
%rename(JPEGHandler)                        wxJPEGHandler;
%rename(PNMHandler)                         wxPNMHandler;
%rename(XPMHandler)                         wxXPMHandler;
%rename(TIFFHandler)                        wxTIFFHandler;
%rename(QUANTIZE_INCLUDE_WINDOWS_COLOURS)   wxQUANTIZE_INCLUDE_WINDOWS_COLOURS;
%rename(QUANTIZE_FILL_DESTINATION_IMAGE)    wxQUANTIZE_FILL_DESTINATION_IMAGE;
%rename(Quantize)                           wxQuantize;
%rename(EvtHandler)                         wxEvtHandler;
%rename(EVENT_PROPAGATE_NONE)               wxEVENT_PROPAGATE_NONE;
%rename(EVENT_PROPAGATE_MAX)                wxEVENT_PROPAGATE_MAX;
%rename(NewEventType)                       wxNewEventType;
%rename(Event)                              wxEvent;
%rename(PropagationDisabler)                wxPropagationDisabler;
%rename(PropagateOnce)                      wxPropagateOnce;
%rename(CommandEvent)                       wxCommandEvent;
%rename(NotifyEvent)                        wxNotifyEvent;
%rename(ScrollEvent)                        wxScrollEvent;
%rename(ScrollWinEvent)                     wxScrollWinEvent;
%rename(MOUSE_BTN_ANY)                      wxMOUSE_BTN_ANY;
%rename(MOUSE_BTN_NONE)                     wxMOUSE_BTN_NONE;
%rename(MOUSE_BTN_LEFT)                     wxMOUSE_BTN_LEFT;
%rename(MOUSE_BTN_MIDDLE)                   wxMOUSE_BTN_MIDDLE;
%rename(MOUSE_BTN_RIGHT)                    wxMOUSE_BTN_RIGHT;
%rename(MouseEvent)                         wxMouseEvent;
%rename(SetCursorEvent)                     wxSetCursorEvent;
%rename(KeyEvent)                           wxKeyEvent;
%rename(SizeEvent)                          wxSizeEvent;
%rename(MoveEvent)                          wxMoveEvent;
%rename(PaintEvent)                         wxPaintEvent;
%rename(NcPaintEvent)                       wxNcPaintEvent;
%rename(EraseEvent)                         wxEraseEvent;
%rename(FocusEvent)                         wxFocusEvent;
%rename(ChildFocusEvent)                    wxChildFocusEvent;
%rename(ActivateEvent)                      wxActivateEvent;
%rename(InitDialogEvent)                    wxInitDialogEvent;
%rename(MenuEvent)                          wxMenuEvent;
%rename(CloseEvent)                         wxCloseEvent;
%rename(ShowEvent)                          wxShowEvent;
%rename(IconizeEvent)                       wxIconizeEvent;
%rename(MaximizeEvent)                      wxMaximizeEvent;
%rename(DropFilesEvent)                     wxDropFilesEvent;
%rename(UPDATE_UI_PROCESS_ALL)              wxUPDATE_UI_PROCESS_ALL;
%rename(UPDATE_UI_PROCESS_SPECIFIED)        wxUPDATE_UI_PROCESS_SPECIFIED;
%rename(UpdateUIEvent)                      wxUpdateUIEvent;
%rename(SysColourChangedEvent)              wxSysColourChangedEvent;
%rename(MouseCaptureChangedEvent)           wxMouseCaptureChangedEvent;
%rename(DisplayChangedEvent)                wxDisplayChangedEvent;
%rename(PaletteChangedEvent)                wxPaletteChangedEvent;
%rename(QueryNewPaletteEvent)               wxQueryNewPaletteEvent;
%rename(NavigationKeyEvent)                 wxNavigationKeyEvent;
%rename(WindowCreateEvent)                  wxWindowCreateEvent;
%rename(WindowDestroyEvent)                 wxWindowDestroyEvent;
%rename(ContextMenuEvent)                   wxContextMenuEvent;
%rename(IDLE_PROCESS_ALL)                   wxIDLE_PROCESS_ALL;
%rename(IDLE_PROCESS_SPECIFIED)             wxIDLE_PROCESS_SPECIFIED;
%rename(IdleEvent)                          wxIdleEvent;
%rename(PyEvent)                            wxPyEvent;
%rename(PyCommandEvent)                     wxPyCommandEvent;
%rename(PYAPP_ASSERT_SUPPRESS)              wxPYAPP_ASSERT_SUPPRESS;
%rename(PYAPP_ASSERT_EXCEPTION)             wxPYAPP_ASSERT_EXCEPTION;
%rename(PYAPP_ASSERT_DIALOG)                wxPYAPP_ASSERT_DIALOG;
%rename(PYAPP_ASSERT_LOG)                   wxPYAPP_ASSERT_LOG;
%rename(PRINT_WINDOWS)                      wxPRINT_WINDOWS;
%rename(PRINT_POSTSCRIPT)                   wxPRINT_POSTSCRIPT;
%rename(PyApp)                              wxPyApp;
%rename(Exit)                               wxExit;
%rename(Yield)                              wxYield;
%rename(YieldIfNeeded)                      wxYieldIfNeeded;
%rename(SafeYield)                          wxSafeYield;
%rename(WakeUpIdle)                         wxWakeUpIdle;
%rename(PostEvent)                          wxPostEvent;
%rename(App_CleanUp)                        wxApp_CleanUp;
%rename(EventLoop)                          wxEventLoop;
%rename(AcceleratorEntry)                   wxAcceleratorEntry;
%rename(AcceleratorTable)                   wxAcceleratorTable;
%rename(NullAcceleratorTable)               wxNullAcceleratorTable;
%rename(GetAccelFromString)                 wxGetAccelFromString;
%rename(VisualAttributes)                   wxVisualAttributes;
%rename(WINDOW_VARIANT_NORMAL)              wxWINDOW_VARIANT_NORMAL;
%rename(WINDOW_VARIANT_SMALL)               wxWINDOW_VARIANT_SMALL;
%rename(WINDOW_VARIANT_MINI)                wxWINDOW_VARIANT_MINI;
%rename(WINDOW_VARIANT_LARGE)               wxWINDOW_VARIANT_LARGE;
%rename(WINDOW_VARIANT_MAX)                 wxWINDOW_VARIANT_MAX;
%rename(Window)                             wxWindow;
%rename(FindWindowById)                     wxFindWindowById;
%rename(FindWindowByName)                   wxFindWindowByName;
%rename(FindWindowByLabel)                  wxFindWindowByLabel;
%rename(Window_FromHWND)                    wxWindow_FromHWND;
%rename(Validator)                          wxValidator;
%rename(PyValidator)                        wxPyValidator;
%rename(DefaultValidator)                   wxDefaultValidator;
%rename(Menu)                               wxMenu;
%rename(MenuBar)                            wxMenuBar;
%rename(MenuItem)                           wxMenuItem;
%rename(Control)                            wxControl;
%rename(ItemContainer)                      wxItemContainer;
%rename(ControlWithItems)                   wxControlWithItems;
%rename(SizerItem)                          wxSizerItem;
%rename(Sizer)                              wxSizer;
%rename(PySizer)                            wxPySizer;
%rename(BoxSizer)                           wxBoxSizer;
%rename(StaticBoxSizer)                     wxStaticBoxSizer;
%rename(GridSizer)                          wxGridSizer;
%rename(FLEX_GROWMODE_NONE)                 wxFLEX_GROWMODE_NONE;
%rename(FLEX_GROWMODE_SPECIFIED)            wxFLEX_GROWMODE_SPECIFIED;
%rename(FLEX_GROWMODE_ALL)                  wxFLEX_GROWMODE_ALL;
%rename(FlexGridSizer)                      wxFlexGridSizer;
%rename(GBPosition)                         wxGBPosition;
%rename(GBSpan)                             wxGBSpan;
%rename(DefaultSpan)                        wxDefaultSpan;
%rename(GBSizerItem)                        wxGBSizerItem;
%rename(GridBagSizer)                       wxGridBagSizer;
%rename(Left)                               wxLeft;
%rename(Top)                                wxTop;
%rename(Right)                              wxRight;
%rename(Bottom)                             wxBottom;
%rename(Width)                              wxWidth;
%rename(Height)                             wxHeight;
%rename(Centre)                             wxCentre;
%rename(Center)                             wxCenter;
%rename(CentreX)                            wxCentreX;
%rename(CentreY)                            wxCentreY;
%rename(Unconstrained)                      wxUnconstrained;
%rename(AsIs)                               wxAsIs;
%rename(PercentOf)                          wxPercentOf;
%rename(Above)                              wxAbove;
%rename(Below)                              wxBelow;
%rename(LeftOf)                             wxLeftOf;
%rename(RightOf)                            wxRightOf;
%rename(SameAs)                             wxSameAs;
%rename(Absolute)                           wxAbsolute;
%rename(IndividualLayoutConstraint)         wxIndividualLayoutConstraint;
%rename(LayoutConstraints)                  wxLayoutConstraints;

#endif
