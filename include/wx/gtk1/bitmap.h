/////////////////////////////////////////////////////////////////////////////
// Name:        bitmap.h
// Purpose:
// Author:      Robert Roebling
// RCS-ID:      $Id$
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#ifndef __GTKBITMAPH__
#define __GTKBITMAPH__

#ifdef __GNUG__
#pragma interface
#endif

#include "wx/defs.h"
#include "wx/object.h"
#include "wx/string.h"
#include "wx/palette.h"

//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------

class wxMask;
class wxBitmap;

//-----------------------------------------------------------------------------
// also implemented here from wxImage
//-----------------------------------------------------------------------------

/*
class wxImage : wxGDIObject
{
  wxImage( const wxBitmap &bitmap );
  wxBitmap ConvertToBitmap() const;
}
*/

//-----------------------------------------------------------------------------
// wxMask
//-----------------------------------------------------------------------------

class wxMask: public wxObject
{
  DECLARE_DYNAMIC_CLASS(wxMask)

public:
  wxMask();
  wxMask( const wxBitmap& bitmap, const wxColour& colour );
  wxMask( const wxBitmap& bitmap, int paletteIndex );
  wxMask( const wxBitmap& bitmap );
  ~wxMask();

 // implementation

  GdkBitmap   *m_bitmap;
  GdkBitmap *GetBitmap() const;
};

//-----------------------------------------------------------------------------
// wxBitmap
//-----------------------------------------------------------------------------

class wxBitmap: public wxObject
{
  DECLARE_DYNAMIC_CLASS(wxBitmap)

public:
  wxBitmap();
  wxBitmap( int width, int height, int depth = -1 );
  wxBitmap( const char bits[], int width, int height, int depth = 1 );
  wxBitmap( const char **bits );
  wxBitmap( char **bits );
  wxBitmap( const wxBitmap& bmp );
  wxBitmap( const wxString &filename, int type = wxBITMAP_TYPE_XPM );
  ~wxBitmap();
  wxBitmap& operator = ( const wxBitmap& bmp );
  bool operator == ( const wxBitmap& bmp );
  bool operator != ( const wxBitmap& bmp );
  bool Ok() const;

  int GetHeight() const;
  int GetWidth() const;
  int GetDepth() const;

  wxMask *GetMask() const;
  void SetMask( wxMask *mask );

  bool SaveFile( const wxString &name, int type, wxPalette *palette = (wxPalette *) NULL );
  bool LoadFile( const wxString &name, int type = wxBITMAP_TYPE_XPM );

  wxPalette *GetPalette() const;
  wxPalette *GetColourMap() const
    { return GetPalette(); };

// implementation

  void SetHeight( int height );
  void SetWidth( int width );
  void SetDepth( int depth );
  void SetPixmap( GdkPixmap *pixmap );

  GdkPixmap *GetPixmap() const;
  GdkBitmap *GetBitmap() const;

    // no data :-)
};

#endif // __GTKBITMAPH__
