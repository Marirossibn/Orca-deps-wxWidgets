/////////////////////////////////////////////////////////////////////////////
// Name:        gdicmn.h
// Purpose:     Common GDI classes, types and declarations
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c)
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __GDICMNH__
#define __GDICMNH__

#ifdef __GNUG__
#pragma interface "gdicmn.h"
#endif

#include "wx/object.h"
#include "wx/list.h"
#include "wx/hash.h"
#include "wx/setup.h"

#ifdef __WINDOWS__
#include "wx/msw/colour.h"
#elif defined(__MOTIF__)
#include "wx/xt/colour.h"
#elif defined(__GTK__)
#include "wx/gtk/colour.h"
#endif

// Standard cursors
typedef enum {
 wxCURSOR_ARROW =  1,
 wxCURSOR_BULLSEYE,
 wxCURSOR_CHAR,
 wxCURSOR_CROSS,
 wxCURSOR_HAND,
 wxCURSOR_IBEAM,
 wxCURSOR_LEFT_BUTTON,
 wxCURSOR_MAGNIFIER,
 wxCURSOR_MIDDLE_BUTTON,
 wxCURSOR_NO_ENTRY,
 wxCURSOR_PAINT_BRUSH,
 wxCURSOR_PENCIL,
 wxCURSOR_POINT_LEFT,
 wxCURSOR_POINT_RIGHT,
 wxCURSOR_QUESTION_ARROW,
 wxCURSOR_RIGHT_BUTTON,
 wxCURSOR_SIZENESW,
 wxCURSOR_SIZENS,
 wxCURSOR_SIZENWSE,
 wxCURSOR_SIZEWE,
 wxCURSOR_SIZING,
 wxCURSOR_SPRAYCAN,
 wxCURSOR_WAIT,
 wxCURSOR_WATCH,
 wxCURSOR_BLANK
#ifdef __X__
  /* Not yet implemented for Windows */
  , wxCURSOR_CROSS_REVERSE,
  wxCURSOR_DOUBLE_ARROW,
  wxCURSOR_BASED_ARROW_UP,
  wxCURSOR_BASED_ARROW_DOWN
#endif
} _standard_cursors_t;

class WXDLLEXPORT wxSize: public wxObject
{
public:
  long x;
  long y;
  inline wxSize(void) { x = 0; y = 0; }
  inline wxSize(long xx, long yy) { x = xx; y = yy; }
  inline wxSize(const wxSize& sz) { x = sz.x; y = sz.y; }
  inline void operator = (const wxSize& sz) { x = sz.x; y = sz.y; }
  inline void Set(long xx, long yy) { x = xx; y = yy; }
  inline long GetX() const { return x; }
  inline long GetY() const { return y; }
};

// Point
class WXDLLEXPORT wxRealPoint: public wxObject
{
  DECLARE_DYNAMIC_CLASS(wxRealPoint)
 public:
  double x;
  double y;
  inline wxRealPoint(void) { x = 0.0; y = 0.0; };
  inline wxRealPoint(double the_x, double the_y) { x = the_x; y = the_y; };

  inline void operator = (const wxRealPoint& pt) { x = pt.x; y = pt.y; }
};

class WXDLLEXPORT wxPoint: public wxObject
{
  DECLARE_DYNAMIC_CLASS(wxPoint)
 public:
  long x;
  long y;
  inline wxPoint(void) { x = 0; y = 0; };
  wxPoint(long the_x, long the_y) { x = the_x; y = the_y; };
  inline void operator = (const wxPoint& pt) { x = pt.x; y = pt.y; }
};

#if WXWIN_COMPATIBILITY
#define wxIntPoint wxPoint
#define wxRectangle wxRect
#endif

class WXDLLEXPORT wxRect : public wxObject {
    DECLARE_DYNAMIC_CLASS(wxRect)
public:
   wxRect(void) ;
   wxRect(const long x, const long y, const long w, const long h);
   wxRect(const wxPoint& topLeft, const wxPoint& bottomRight);
   wxRect(const wxPoint& pos, const wxSize& size);
   wxRect(const wxRect& rect);

   inline long  GetX(void) const        { return x; }
   inline void SetX(const long X)       { x = X; }
   inline long  GetY(void) const        { return y; }
   inline void SetY(const long Y)       { y = Y; }
   inline long  GetWidth() const        { return width; }
   inline void SetWidth(const long w)   { width = w; }
   inline long  GetHeight() const       { return height; }
   inline void SetHeight(const long h)  { height = h; }

   inline wxPoint GetPosition(void) { return wxPoint(x, y); }
   inline wxSize GetSize(void) { return wxSize(width, height); }

   inline long  GetLeft(void)   const { return x; }
   inline long  GetTop(void)    const { return y; }
   inline long  GetBottom(void) const { return y + height; }
   inline long  GetRight(void)  const { return x + width; }

   wxRect& operator = (const wxRect& rect);
   bool operator == (const wxRect& rect);
   bool operator != (const wxRect& rect);
public:
   long x, y, width, height;
};

class WXDLLEXPORT wxBrush;
class WXDLLEXPORT wxPen;
class WXDLLEXPORT wxBitmap;
class WXDLLEXPORT wxIcon;
class WXDLLEXPORT wxCursor;
class WXDLLEXPORT wxFont;
class WXDLLEXPORT wxPalette;
class WXDLLEXPORT wxPalette;

/*
 * Bitmap flags
 */

// Hint to indicate filetype
#define wxBITMAP_TYPE_BMP               1
#define wxBITMAP_TYPE_BMP_RESOURCE      2
#define wxBITMAP_TYPE_ICO               3
#define wxBITMAP_TYPE_ICO_RESOURCE      4
#define wxBITMAP_TYPE_CUR               5
#define wxBITMAP_TYPE_CUR_RESOURCE      6
#define wxBITMAP_TYPE_XBM               7
#define wxBITMAP_TYPE_XBM_DATA          8
#define wxBITMAP_TYPE_XPM               9
#define wxBITMAP_TYPE_XPM_DATA          10
#define wxBITMAP_TYPE_TIF               11
#define wxBITMAP_TYPE_TIF_RESOURCE      12
#define wxBITMAP_TYPE_GIF               13
#define wxBITMAP_TYPE_GIF_RESOURCE      14
#define wxBITMAP_TYPE_PNG               15
#define wxBITMAP_TYPE_PNG_RESOURCE      16
#define wxBITMAP_TYPE_ANY               50

#define wxBITMAP_TYPE_RESOURCE wxBITMAP_TYPE_BMP_RESOURCE

class WXDLLEXPORT wxBitmap;
class WXDLLEXPORT wxCursor;
class WXDLLEXPORT wxIcon;

// Management of pens, brushes and fonts
class WXDLLEXPORT wxPenList: public wxList
{
  DECLARE_DYNAMIC_CLASS(wxPenList)
 public:
  inline wxPenList(void)
    { }
  ~wxPenList(void);
  void AddPen(wxPen *pen);
  void RemovePen(wxPen *pen);
  wxPen *FindOrCreatePen(const wxColour& colour, const int width, const int style);
  wxPen *FindOrCreatePen(const wxString& colour, const int width, const int style);
};

class WXDLLEXPORT wxBrushList: public wxList
{
  DECLARE_DYNAMIC_CLASS(wxBrushList)
 public:
  inline wxBrushList(void)
    { }
  ~wxBrushList(void);
  void AddBrush(wxBrush *brush);
  void RemoveBrush(wxBrush *brush);
  wxBrush *FindOrCreateBrush(const wxColour& colour, const int style);
  wxBrush *FindOrCreateBrush(const wxString& colour, const int style);
};

WXDLLEXPORT_DATA(extern const char*) wxEmptyString;

class WXDLLEXPORT wxFontList: public wxList
{
  DECLARE_DYNAMIC_CLASS(wxFontList)
 public:
  inline wxFontList(void)
    { }
  ~wxFontList(void);
  void AddFont(wxFont *font);
  void RemoveFont(wxFont *font);
  wxFont *FindOrCreateFont(const int pointSize, const int family, const int style, const int weight,
    const bool underline = FALSE, const wxString& face = wxEmptyString);
};

class WXDLLEXPORT wxColourDatabase: public wxList
{
  DECLARE_CLASS(wxColourDatabase)
 public:
  wxColourDatabase(int type);
  ~wxColourDatabase(void) ;
  // Not const because it may add a name to the database
  wxColour *FindColour(const wxString& colour) ;
  wxString FindName(const wxColour& colour) const;
  void Initialize(void);
};

class WXDLLEXPORT wxBitmapList: public wxList
{
  DECLARE_DYNAMIC_CLASS(wxBitmapList)
 public:
   wxBitmapList(void);
  ~wxBitmapList(void);

  void AddBitmap(wxBitmap *bitmap);
  void RemoveBitmap(wxBitmap *bitmap);
};

// Lists of GDI objects
WXDLLEXPORT_DATA(extern wxPenList*) 	wxThePenList;
WXDLLEXPORT_DATA(extern wxBrushList*) 	wxTheBrushList;
WXDLLEXPORT_DATA(extern wxFontList*)		wxTheFontList;
WXDLLEXPORT_DATA(extern wxBitmapList*)	wxTheBitmapList;

// Stock objects
WXDLLEXPORT_DATA(extern wxFont*)			wxNORMAL_FONT;
WXDLLEXPORT_DATA(extern wxFont*)			wxSMALL_FONT;
WXDLLEXPORT_DATA(extern wxFont*)			wxITALIC_FONT;
WXDLLEXPORT_DATA(extern wxFont*)			wxSWISS_FONT;

WXDLLEXPORT_DATA(extern wxPen*)			wxRED_PEN;
WXDLLEXPORT_DATA(extern wxPen*)			wxCYAN_PEN;
WXDLLEXPORT_DATA(extern wxPen*)			wxGREEN_PEN;
WXDLLEXPORT_DATA(extern wxPen*)			wxBLACK_PEN;
WXDLLEXPORT_DATA(extern wxPen*)			wxWHITE_PEN;
WXDLLEXPORT_DATA(extern wxPen*)			wxTRANSPARENT_PEN;
WXDLLEXPORT_DATA(extern wxPen*)			wxBLACK_DASHED_PEN;
WXDLLEXPORT_DATA(extern wxPen*)			wxGREY_PEN;
WXDLLEXPORT_DATA(extern wxPen*)			wxMEDIUM_GREY_PEN;
WXDLLEXPORT_DATA(extern wxPen*)			wxLIGHT_GREY_PEN;

WXDLLEXPORT_DATA(extern wxBrush*)		wxBLUE_BRUSH;
WXDLLEXPORT_DATA(extern wxBrush*)		wxGREEN_BRUSH;
WXDLLEXPORT_DATA(extern wxBrush*)		wxWHITE_BRUSH;
WXDLLEXPORT_DATA(extern wxBrush*)		wxBLACK_BRUSH;
WXDLLEXPORT_DATA(extern wxBrush*)		wxGREY_BRUSH;
WXDLLEXPORT_DATA(extern wxBrush*)		wxMEDIUM_GREY_BRUSH;
WXDLLEXPORT_DATA(extern wxBrush*)		wxLIGHT_GREY_BRUSH;
WXDLLEXPORT_DATA(extern wxBrush*)		wxTRANSPARENT_BRUSH;
WXDLLEXPORT_DATA(extern wxBrush*)		wxCYAN_BRUSH;
WXDLLEXPORT_DATA(extern wxBrush*)		wxRED_BRUSH;

WXDLLEXPORT_DATA(extern wxColour*)		wxBLACK;
WXDLLEXPORT_DATA(extern wxColour*)		wxWHITE;
WXDLLEXPORT_DATA(extern wxColour*)		wxRED;
WXDLLEXPORT_DATA(extern wxColour*)		wxBLUE;
WXDLLEXPORT_DATA(extern wxColour*)		wxGREEN;
WXDLLEXPORT_DATA(extern wxColour*)		wxCYAN;
WXDLLEXPORT_DATA(extern wxColour*)		wxLIGHT_GREY;

// 'Null' objects
WXDLLEXPORT_DATA(extern wxBitmap) 		wxNullBitmap;
WXDLLEXPORT_DATA(extern wxIcon)   		wxNullIcon;
WXDLLEXPORT_DATA(extern wxCursor) 		wxNullCursor;
WXDLLEXPORT_DATA(extern wxPen)    		wxNullPen;
WXDLLEXPORT_DATA(extern wxBrush)  		wxNullBrush;
WXDLLEXPORT_DATA(extern wxPalette) 		wxNullPalette;
WXDLLEXPORT_DATA(extern wxFont)   		wxNullFont;
WXDLLEXPORT_DATA(extern wxColour) 		wxNullColour;

// Stock cursors types
WXDLLEXPORT_DATA(extern wxCursor*)		wxSTANDARD_CURSOR;
WXDLLEXPORT_DATA(extern wxCursor*)		wxHOURGLASS_CURSOR;
WXDLLEXPORT_DATA(extern wxCursor*)		wxCROSS_CURSOR;

WXDLLEXPORT_DATA(extern wxColourDatabase*)	wxTheColourDatabase;
extern void WXDLLEXPORT wxInitializeStockObjects(void);
extern void WXDLLEXPORT wxDeleteStockObjects(void);

extern bool WXDLLEXPORT wxColourDisplay(void);

// Returns depth of screen
extern int WXDLLEXPORT wxDisplayDepth(void);

extern void WXDLLEXPORT wxDisplaySize(int *width, int *height);

extern void WXDLLEXPORT wxSetCursor(const wxCursor& cursor);

// Useful macro for create icons portably

#ifdef __WINDOWS__
# define wxICON(X) wxIcon(X##_icon);
#elif defined(__X__)
# define wxICON(X) wxIcon(X##_bits, X##_width, X##_height);
#else
# define wxICON    wxIcon
#endif

/*
  Example:
  #define wxbuild_icon "wxbuild"
 
  wxIcon *icon = new wxICON(wxbuild);
 */

#endif
    // __GDICMNH__
