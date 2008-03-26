///////////////////////////////////////////////////////////////////////////////
// Name:        wx/bitmap.h
// Purpose:     wxBitmap class interface
// Author:      Vaclav Slavik
// Modified by:
// Created:     22.04.01
// RCS-ID:      $Id$
// Copyright:   (c) wxWidgets team
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_BITMAP_H_BASE_
#define _WX_BITMAP_H_BASE_

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include "wx/string.h"
#include "wx/gdicmn.h"  // for wxBitmapType
#include "wx/colour.h"

class WXDLLIMPEXP_FWD_CORE wxBitmap;
class WXDLLIMPEXP_FWD_CORE wxBitmapHandler;
class WXDLLIMPEXP_FWD_CORE wxIcon;
class WXDLLIMPEXP_FWD_CORE wxImage;
class WXDLLIMPEXP_FWD_CORE wxMask;
class WXDLLIMPEXP_FWD_CORE wxPalette;

// ----------------------------------------------------------------------------
// wxVariant support
// ----------------------------------------------------------------------------

#if wxUSE_VARIANT
#include "wx/variant.h"
DECLARE_VARIANT_OBJECT_EXPORTED(wxBitmap,WXDLLIMPEXP_CORE)
#endif

// ----------------------------------------------------------------------------
// wxMask represents the transparent area of the bitmap
// ----------------------------------------------------------------------------

// TODO: all implementation of wxMask, except the generic one,
//       do not derive from wxMaskBase,,, they should
class WXDLLIMPEXP_CORE wxMaskBase : public wxObject
{
public:
    // create the mask from bitmap pixels of the given colour
    bool Create(const wxBitmap& bitmap, const wxColour& colour);

#if wxUSE_PALETTE
    // create the mask from bitmap pixels with the given palette index
    bool Create(const wxBitmap& bitmap, int paletteIndex);
#endif // wxUSE_PALETTE

    // create the mask from the given mono bitmap
    bool Create(const wxBitmap& bitmap);

protected:
    // this function is called from Create() to free the existing mask data
    virtual void FreeData() = 0;

    // these functions must be overridden to implement the corresponding public
    // Create() methods, they shouldn't call FreeData() as it's already called
    // by the public wrappers
    virtual bool InitFromColour(const wxBitmap& bitmap,
                                const wxColour& colour) = 0;
    virtual bool InitFromMonoBitmap(const wxBitmap& bitmap) = 0;
};

#if defined(__WXMGL__) || \
    defined(__WXDFB__) || \
    defined(__WXMAC__) || \
    defined(__WXGTK__) || \
    defined(__WXCOCOA__) || \
    defined(__WXMOTIF__) || \
    defined(__WXX11__)
    #define wxUSE_BITMAP_BASE 1
#else
    #define wxUSE_BITMAP_BASE 0
#endif

// a more readable way to tell
#define wxBITMAP_SCREEN_DEPTH       (-1)


// All ports except wxMSW,wxOS2,wxPalmOS use wxBitmapHandler and wxBitmapBase as base class
// for wxBitmapHandler; wxMSW,wxOS2,wxPalmOS use wxGDIImageHandler as base class
// since it allows some code reuse there.
#if wxUSE_BITMAP_BASE

// ----------------------------------------------------------------------------
// wxBitmapHandler: class which knows how to create/load/save bitmaps in
// different formats
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxBitmapHandler : public wxObject
{
public:
    wxBitmapHandler() { m_type = wxBITMAP_TYPE_INVALID; }
    virtual ~wxBitmapHandler() { }

    // NOTE: the following functions should be pure virtuals, but they aren't
    //       because otherwise almost all ports would have to implement
    //       them as "return false"...

    virtual bool Create(wxBitmap *WXUNUSED(bitmap), const void* WXUNUSED(data),
                         wxBitmapType WXUNUSED(type), int WXUNUSED(width), int WXUNUSED(height),
                         int WXUNUSED(depth) = 1)
        { return false; }

    virtual bool LoadFile(wxBitmap *WXUNUSED(bitmap), const wxString& WXUNUSED(name),
                           wxBitmapType WXUNUSED(type), int WXUNUSED(desiredWidth),
                           int WXUNUSED(desiredHeight))
        { return false; }

    virtual bool SaveFile(const wxBitmap *WXUNUSED(bitmap), const wxString& WXUNUSED(name),
                           wxBitmapType WXUNUSED(type), const wxPalette *WXUNUSED(palette) = NULL) const
        { return false; }

    void SetName(const wxString& name)      { m_name = name; }
    void SetExtension(const wxString& ext)  { m_extension = ext; }
    void SetType(wxBitmapType type)         { m_type = type; }
    const wxString& GetName() const         { return m_name; }
    const wxString& GetExtension() const    { return m_extension; }
    wxBitmapType GetType() const            { return m_type; }

private:
    wxString      m_name;
    wxString      m_extension;
    wxBitmapType  m_type;

    DECLARE_ABSTRACT_CLASS(wxBitmapHandler)
};


// ----------------------------------------------------------------------------
// wxBitmap: class which represents platform-dependent bitmap (unlike wxImage)
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxBitmapBase : public wxGDIObject
{
public:
    /*
    Derived class must implement these:

    wxBitmap();
    wxBitmap(const wxBitmap& bmp);
    wxBitmap(const char bits[], int width, int height, int depth = 1);
    wxBitmap(int width, int height, int depth = wxBITMAP_SCREEN_DEPTH);
    wxBitmap(const char* const* bits);
    wxBitmap(const wxString &filename, wxBitmapType type = wxBITMAP_TYPE_XPM);
    wxBitmap(const wxImage& image, int depth = wxBITMAP_SCREEN_DEPTH);

    static void InitStandardHandlers();
    */

    virtual bool Create(int width, int height, int depth = wxBITMAP_SCREEN_DEPTH) = 0;

    virtual int GetHeight() const = 0;
    virtual int GetWidth() const = 0;
    virtual int GetDepth() const = 0;

#if wxUSE_IMAGE
    virtual wxImage ConvertToImage() const = 0;
#endif // wxUSE_IMAGE

    virtual wxMask *GetMask() const = 0;
    virtual void SetMask(wxMask *mask) = 0;

    virtual wxBitmap GetSubBitmap(const wxRect& rect) const = 0;

    virtual bool SaveFile(const wxString &name, wxBitmapType type,
                          const wxPalette *palette = (wxPalette *)NULL) const = 0;
    virtual bool LoadFile(const wxString &name, wxBitmapType type) = 0;

    /*
       If raw bitmap access is supported (see wx/rawbmp.h), the following
       methods should be implemented:

       virtual bool GetRawData(wxRawBitmapData *data) = 0;
       virtual void UngetRawData(wxRawBitmapData *data) = 0;
     */

#if wxUSE_PALETTE
    virtual wxPalette *GetPalette() const = 0;
    virtual void SetPalette(const wxPalette& palette) = 0;
#endif // wxUSE_PALETTE

    // copies the contents and mask of the given (colour) icon to the bitmap
    virtual bool CopyFromIcon(const wxIcon& icon) = 0;

    // implementation:
    virtual void SetHeight(int height) = 0;
    virtual void SetWidth(int width) = 0;
    virtual void SetDepth(int depth) = 0;

    // Format handling
    static inline wxList& GetHandlers() { return sm_handlers; }
    static void AddHandler(wxBitmapHandler *handler);
    static void InsertHandler(wxBitmapHandler *handler);
    static bool RemoveHandler(const wxString& name);
    static wxBitmapHandler *FindHandler(const wxString& name);
    static wxBitmapHandler *FindHandler(const wxString& extension, wxBitmapType bitmapType);
    static wxBitmapHandler *FindHandler(wxBitmapType bitmapType);

    //static void InitStandardHandlers();
    //  (wxBitmap must implement this one)

    static void CleanUpHandlers();

    // this method is only used by the generic implementation of wxMask
    // currently but could be useful elsewhere in the future: it can be
    // overridden to quantize the colour to correspond to bitmap colour depth
    // if necessary; default implementation simply returns the colour as is
    virtual wxColour QuantizeColour(const wxColour& colour) const
    {
        return colour;
    }

protected:
    static wxList sm_handlers;

    DECLARE_ABSTRACT_CLASS(wxBitmapBase)
};

#endif // wxUSE_BITMAP_BASE

#if defined(__WXPALMOS__)
    #include "wx/palmos/bitmap.h"
#elif defined(__WXMSW__)
    #include "wx/msw/bitmap.h"
#elif defined(__WXMOTIF__)
    #include "wx/x11/bitmap.h"
#elif defined(__WXGTK20__)
    #include "wx/gtk/bitmap.h"
#elif defined(__WXGTK__)
    #include "wx/gtk1/bitmap.h"
#elif defined(__WXX11__)
    #include "wx/x11/bitmap.h"
#elif defined(__WXMGL__)
    #include "wx/mgl/bitmap.h"
#elif defined(__WXDFB__)
    #include "wx/dfb/bitmap.h"
#elif defined(__WXMAC__)
    #include "wx/mac/bitmap.h"
#elif defined(__WXCOCOA__)
    #include "wx/cocoa/bitmap.h"
#elif defined(__WXPM__)
    #include "wx/os2/bitmap.h"
#endif

// we must include generic mask.h after wxBitmap definition
#if defined(__WXMGL__) || defined(__WXDFB__)
    #define wxUSE_GENERIC_MASK 1
#else
    #define wxUSE_GENERIC_MASK 0
#endif

#if wxUSE_GENERIC_MASK
    #include "wx/generic/mask.h"
#endif

#endif // _WX_BITMAP_H_BASE_
