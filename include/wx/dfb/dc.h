/////////////////////////////////////////////////////////////////////////////
// Name:        wx/dfb/dc.h
// Purpose:     wxDC class
// Author:      Vaclav Slavik
// Created:     2006-08-07
// RCS-ID:      $Id$
// Copyright:   (c) 2006 REA Elektronik GmbH
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_DFB_DC_H_
#define _WX_DFB_DC_H_

#include "wx/defs.h"
#include "wx/region.h"
#include "wx/dfb/dfbptr.h"

wxDFB_DECLARE_INTERFACE(IDirectFBSurface);

//-----------------------------------------------------------------------------
// wxDC
//-----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxDC : public wxDCBase
{
public:
    wxDC();

    // Ctor.
    wxDC(const wxIDirectFBSurfacePtr& surface);

public:
    // implement base class pure virtuals
    // ----------------------------------

    virtual void Clear();

    virtual bool StartDoc(const wxString& message);
    virtual void EndDoc();

    virtual void StartPage();
    virtual void EndPage();

    virtual void SetFont(const wxFont& font);
    virtual void SetPen(const wxPen& pen);
    virtual void SetBrush(const wxBrush& brush);
    virtual void SetBackground(const wxBrush& brush);
    virtual void SetBackgroundMode(int mode);
#if wxUSE_PALETTE
    virtual void SetPalette(const wxPalette& palette);
#endif

    virtual void DestroyClippingRegion();

    virtual wxCoord GetCharHeight() const;
    virtual wxCoord GetCharWidth() const;
    virtual void DoGetTextExtent(const wxString& string,
                                 wxCoord *x, wxCoord *y,
                                 wxCoord *descent = NULL,
                                 wxCoord *externalLeading = NULL,
                                 wxFont *theFont = NULL) const;

    virtual bool CanDrawBitmap() const { return true; }
    virtual bool CanGetTextExtent() const { return true; }
    virtual int GetDepth() const;
    virtual wxSize GetPPI() const;

    virtual void SetMapMode(int mode);
    virtual void SetUserScale(double x, double y);
    virtual void SetLogicalScale(double x, double y);
    virtual void SetLogicalOrigin(wxCoord x, wxCoord y);
    virtual void SetDeviceOrigin(wxCoord x, wxCoord y);
    virtual void SetAxisOrientation(bool xLeftRight, bool yBottomUp);
    virtual void SetLogicalFunction(int function);

    // implementation from now on
    // --------------------------

    virtual void ComputeScaleAndOrigin();

    wxCoord XDEV2LOG(wxCoord x) const
    {
        wxCoord new_x = x - m_deviceOriginX;
        if (new_x > 0)
            return (wxCoord)((double)(new_x) / m_scaleX + 0.5) * m_signX + m_logicalOriginX;
        else
            return (wxCoord)((double)(new_x) / m_scaleX - 0.5) * m_signX + m_logicalOriginX;
    }
    wxCoord XDEV2LOGREL(wxCoord x) const
    {
        if (x > 0)
            return (wxCoord)((double)(x) / m_scaleX + 0.5);
        else
            return (wxCoord)((double)(x) / m_scaleX - 0.5);
    }
    wxCoord YDEV2LOG(wxCoord y) const
    {
        wxCoord new_y = y - m_deviceOriginY;
        if (new_y > 0)
            return (wxCoord)((double)(new_y) / m_scaleY + 0.5) * m_signY + m_logicalOriginY;
        else
            return (wxCoord)((double)(new_y) / m_scaleY - 0.5) * m_signY + m_logicalOriginY;
    }
    wxCoord YDEV2LOGREL(wxCoord y) const
    {
        if (y > 0)
            return (wxCoord)((double)(y) / m_scaleY + 0.5);
        else
            return (wxCoord)((double)(y) / m_scaleY - 0.5);
    }
    wxCoord XLOG2DEV(wxCoord x) const
    {
        wxCoord new_x = x - m_logicalOriginX;
        if (new_x > 0)
            return (wxCoord)((double)(new_x) * m_scaleX + 0.5) * m_signX + m_deviceOriginX;
        else
            return (wxCoord)((double)(new_x) * m_scaleX - 0.5) * m_signX + m_deviceOriginX;
    }
    wxCoord XLOG2DEVREL(wxCoord x) const
    {
        if (x > 0)
            return (wxCoord)((double)(x) * m_scaleX + 0.5);
        else
            return (wxCoord)((double)(x) * m_scaleX - 0.5);
    }
    wxCoord YLOG2DEV(wxCoord y) const
    {
        wxCoord new_y = y - m_logicalOriginY;
        if (new_y > 0)
            return (wxCoord)((double)(new_y) * m_scaleY + 0.5) * m_signY + m_deviceOriginY;
        else
            return (wxCoord)((double)(new_y) * m_scaleY - 0.5) * m_signY + m_deviceOriginY;
    }
    wxCoord YLOG2DEVREL(wxCoord y) const
    {
        if (y > 0)
            return (wxCoord)((double)(y) * m_scaleY + 0.5);
        else
            return (wxCoord)((double)(y) * m_scaleY - 0.5);
    }

    // Returns the surface (and increases its ref count)
    wxIDirectFBSurfacePtr GetDirectFBSurface() const { return m_surface; }

protected:
    // initializes the DC from a surface, must be called if default ctor
    // was used
    void Init(const wxIDirectFBSurfacePtr& surface);

    virtual bool DoFloodFill(wxCoord x, wxCoord y, const wxColour& col,
                             int style = wxFLOOD_SURFACE);

    virtual bool DoGetPixel(wxCoord x, wxCoord y, wxColour *col) const;

    virtual void DoDrawPoint(wxCoord x, wxCoord y);
    virtual void DoDrawLine(wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2);

    virtual void DoDrawArc(wxCoord x1, wxCoord y1,
                           wxCoord x2, wxCoord y2,
                           wxCoord xc, wxCoord yc);
    virtual void DoDrawEllipticArc(wxCoord x, wxCoord y, wxCoord w, wxCoord h,
                                   double sa, double ea);

    virtual void DoDrawRectangle(wxCoord x, wxCoord y, wxCoord width, wxCoord height);
    virtual void DoDrawRoundedRectangle(wxCoord x, wxCoord y,
                                        wxCoord width, wxCoord height,
                                        double radius);
    virtual void DoDrawEllipse(wxCoord x, wxCoord y, wxCoord width, wxCoord height);

    virtual void DoCrossHair(wxCoord x, wxCoord y);

    virtual void DoDrawIcon(const wxIcon& icon, wxCoord x, wxCoord y);
    virtual void DoDrawBitmap(const wxBitmap &bmp, wxCoord x, wxCoord y,
                              bool useMask = false);

    virtual void DoDrawText(const wxString& text, wxCoord x, wxCoord y);
    virtual void DoDrawRotatedText(const wxString& text, wxCoord x, wxCoord y,
                                   double angle);

    virtual bool DoBlit(wxCoord xdest, wxCoord ydest, wxCoord width, wxCoord height,
                        wxDC *source, wxCoord xsrc, wxCoord ysrc,
                        int rop = wxCOPY, bool useMask = false, wxCoord xsrcMask = -1, wxCoord ysrcMask = -1);

    // this is gnarly - we can't even call this function DoSetClippingRegion()
    // because of virtual function hiding
    virtual void DoSetClippingRegionAsRegion(const wxRegion& region);
    virtual void DoSetClippingRegion(wxCoord x, wxCoord y,
                                     wxCoord width, wxCoord height);

    virtual void DoGetSize(int *width, int *height) const;
    virtual void DoGetSizeMM(int* width, int* height) const;

    virtual void DoDrawLines(int n, wxPoint points[],
                             wxCoord xoffset, wxCoord yoffset);
    virtual void DoDrawPolygon(int n, wxPoint points[],
                               wxCoord xoffset, wxCoord yoffset,
                               int fillStyle = wxODDEVEN_RULE);

    // implementation from now on:

private:
    // Unified implementation of DrawIcon, DrawBitmap and Blit:
    void DoDrawSubBitmap(const wxBitmap &bmp,
                         wxCoord x, wxCoord y, wxCoord w, wxCoord h,
                         wxCoord destx, wxCoord desty, int rop, bool useMask);
    bool DoBlitFromSurface(const wxIDirectFBSurfacePtr& src,
                           wxCoord srcx, wxCoord srcy,
                           wxCoord w, wxCoord h,
                           wxCoord dstx, wxCoord dsty);

    // selects colour into surface's state
    void SelectColour(const wxColour& clr);

protected:
    wxIDirectFBSurfacePtr m_surface;

    double            m_mm_to_pix_x, m_mm_to_pix_y;

    friend class WXDLLIMPEXP_CORE wxOverlayImpl; // for Init

    DECLARE_DYNAMIC_CLASS(wxDC)
};

#endif // _WX_DFB_DC_H_
