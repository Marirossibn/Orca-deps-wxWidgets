/////////////////////////////////////////////////////////////////////////////
// Name:        src/common/graphcmn.cpp
// Purpose:     graphics context methods common to all platforms
// Author:      Stefan Csomor
// Modified by:
// Created:
// RCS-ID:      $Id$
// Copyright:   (c) Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#if defined(__BORLANDC__)
    #pragma hdrstop
#endif

#if wxUSE_GRAPHICS_CONTEXT

#include "wx/graphics.h"
#include "wx/dcgraph.h"

#ifndef WX_PRECOMP
    #include "wx/icon.h"
    #include "wx/bitmap.h"
    #include "wx/dcmemory.h"
    #include "wx/region.h"
#endif

#ifdef __WXMAC__
#include "wx/mac/private.h"
#endif
//-----------------------------------------------------------------------------
// constants
//-----------------------------------------------------------------------------

static const double RAD2DEG = 180.0 / M_PI;

//-----------------------------------------------------------------------------
// Local functions
//-----------------------------------------------------------------------------

static inline double DegToRad(double deg)
{
    return (deg * M_PI) / 180.0;
}

//-----------------------------------------------------------------------------
// wxDC bridge class
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxGCDC, wxDC)

wxGCDC::wxGCDC(const wxWindowDC& dc)
{
    m_pimpl = new wxGCDCImpl( this, dc );
}

wxGCDC::wxGCDC( const wxMemoryDC& dc)
{
    m_pimpl = new wxGCDCImpl( this, dc );
}

wxGCDC::wxGCDC()
{
    m_pimpl = new wxGCDCImpl( this );
}

wxGraphicsContext* wxGCDC::GetGraphicsContext()
{
    if (!m_pimpl) return NULL;
    wxGCDCImpl *gc_impl = (wxGCDCImpl*) m_pimpl;
    return gc_impl->GetGraphicsContext();
}

void wxGCDC::SetGraphicsContext( wxGraphicsContext* ctx )
{
    if (!m_pimpl) return;
    wxGCDCImpl *gc_impl = (wxGCDCImpl*) m_pimpl;
    gc_impl->SetGraphicsContext( ctx );
}

IMPLEMENT_ABSTRACT_CLASS(wxGCDCImpl, wxDCImpl)

wxGCDCImpl::wxGCDCImpl( wxDC *owner ) :
   wxDCImpl( owner )
{
    Init();
}

void wxGCDCImpl::SetGraphicsContext( wxGraphicsContext* ctx )
{
    delete m_graphicContext;
    m_graphicContext = ctx;
    if ( m_graphicContext )
    {
        m_matrixOriginal = m_graphicContext->GetTransform();
        m_ok = true;
        // apply the stored transformations to the passed in context
        ComputeScaleAndOrigin();
        m_graphicContext->SetFont( m_font , m_textForegroundColour );
        m_graphicContext->SetPen( m_pen );
        m_graphicContext->SetBrush( m_brush);
    }
}

wxGCDCImpl::wxGCDCImpl( wxDC *owner, const wxWindowDC& dc ) :
   wxDCImpl( owner )
{
    Init();
    SetGraphicsContext( wxGraphicsContext::Create(dc) );
}

wxGCDCImpl::wxGCDCImpl( wxDC *owner, const wxMemoryDC& dc ) :
   wxDCImpl( owner )
{
    Init();
    SetGraphicsContext( wxGraphicsContext::Create(dc) );
}

void wxGCDCImpl::Init()
{
    m_ok = false;
    m_colour = true;
    m_mm_to_pix_x = mm2pt;
    m_mm_to_pix_y = mm2pt;

    m_pen = *wxBLACK_PEN;
    m_font = *wxNORMAL_FONT;
    m_brush = *wxWHITE_BRUSH;

    m_graphicContext = NULL;
    m_logicalFunctionSupported = true;
}


wxGCDCImpl::~wxGCDCImpl()
{
    delete m_graphicContext;
}

void wxGCDCImpl::DoDrawBitmap( const wxBitmap &bmp, wxCoord x, wxCoord y, bool WXUNUSED(useMask) )
{
    wxCHECK_RET( IsOk(), wxT("wxGCDC(cg)::DoDrawBitmap - invalid DC") );
    wxCHECK_RET( bmp.IsOk(), wxT("wxGCDC(cg)::DoDrawBitmap - invalid bitmap") );

    if ( bmp.GetDepth() == 1 )
    {
        m_graphicContext->SetPen(*wxTRANSPARENT_PEN);
        m_graphicContext->SetBrush( wxBrush( m_textBackgroundColour , wxSOLID ) );
        m_graphicContext->DrawRectangle( x , y , bmp.GetWidth() , bmp.GetHeight() );        
        m_graphicContext->SetBrush( wxBrush( m_textForegroundColour , wxSOLID ) );
        m_graphicContext->DrawBitmap( bmp, x , y , bmp.GetWidth() , bmp.GetHeight() );
        m_graphicContext->SetBrush( m_graphicContext->CreateBrush(m_brush));
        m_graphicContext->SetPen( m_graphicContext->CreatePen(m_pen));
    }
    else
        m_graphicContext->DrawBitmap( bmp, x , y , bmp.GetWidth() , bmp.GetHeight() );
}

void wxGCDCImpl::DoDrawIcon( const wxIcon &icon, wxCoord x, wxCoord y )
{
    wxCHECK_RET( IsOk(), wxT("wxGCDC(cg)::DoDrawIcon - invalid DC") );
    wxCHECK_RET( icon.IsOk(), wxT("wxGCDC(cg)::DoDrawIcon - invalid icon") );

    wxCoord w = icon.GetWidth();
    wxCoord h = icon.GetHeight();

    m_graphicContext->DrawIcon( icon , x, y, w, h );
}

bool wxGCDCImpl::StartDoc( const wxString& WXUNUSED(message) ) 
{
    return false;
}

void wxGCDCImpl::EndDoc() 
{
}

void wxGCDCImpl::StartPage()
{
}

void wxGCDCImpl::EndPage() 
{
}
    
void wxGCDCImpl::Flush()
{
#ifdef __WXMAC__
    CGContextFlush( (CGContextRef) m_graphicContext->GetNativeContext() );
#endif
}

void wxGCDCImpl::DoSetClippingRegion( wxCoord x, wxCoord y, wxCoord w, wxCoord h )
{
    wxCHECK_RET( IsOk(), wxT("wxGCDC(cg)::DoSetClippingRegion - invalid DC") );

    m_graphicContext->Clip( x, y, w, h );
    if ( m_clipping )
    {
        m_clipX1 = wxMax( m_clipX1, x );
        m_clipY1 = wxMax( m_clipY1, y );
        m_clipX2 = wxMin( m_clipX2, (x + w) );
        m_clipY2 = wxMin( m_clipY2, (y + h) );
    }
    else
    {
        m_clipping = true;

        m_clipX1 = x;
        m_clipY1 = y;
        m_clipX2 = x + w;
        m_clipY2 = y + h;
    }
}

void wxGCDCImpl::DoSetClippingRegionAsRegion( const wxRegion &region )
{
    // region is in device coordinates
    wxCHECK_RET( IsOk(), wxT("wxGCDC(cg)::DoSetClippingRegionAsRegion - invalid DC") );

    if (region.Empty())
    {
        //DestroyClippingRegion();
        return;
    }

    wxRegion logRegion( region );
    wxCoord x, y, w, h;

    logRegion.Offset( DeviceToLogicalX(0), DeviceToLogicalY(0) );
    logRegion.GetBox( x, y, w, h );

    m_graphicContext->Clip( logRegion );
    if ( m_clipping )
    {
        m_clipX1 = wxMax( m_clipX1, x );
        m_clipY1 = wxMax( m_clipY1, y );
        m_clipX2 = wxMin( m_clipX2, (x + w) );
        m_clipY2 = wxMin( m_clipY2, (y + h) );
    }
    else
    {
        m_clipping = true;

        m_clipX1 = x;
        m_clipY1 = y;
        m_clipX2 = x + w;
        m_clipY2 = y + h;
    }
}

void wxGCDCImpl::DestroyClippingRegion()
{
    m_graphicContext->ResetClip();
    // currently the clip eg of a window extends to the area between the scrollbars
    // so we must explicitely make sure it only covers the area we want it to draw
    int width, height ;
    GetOwner()->GetSize( &width , &height ) ;
    m_graphicContext->Clip( DeviceToLogicalX(0) , DeviceToLogicalY(0) , DeviceToLogicalXRel(width), DeviceToLogicalYRel(height) );
    
    m_graphicContext->SetPen( m_pen );
    m_graphicContext->SetBrush( m_brush );

    m_clipping = false;
}

void wxGCDCImpl::DoGetSizeMM( int* width, int* height ) const
{
    int w = 0, h = 0;

    GetOwner()->GetSize( &w, &h );
    if (width)
        *width = long( double(w) / (m_scaleX * m_mm_to_pix_x) );
    if (height)
        *height = long( double(h) / (m_scaleY * m_mm_to_pix_y) );
}

void wxGCDCImpl::SetTextForeground( const wxColour &col )
{
    wxCHECK_RET( IsOk(), wxT("wxGCDC(cg)::SetTextForeground - invalid DC") );

    if ( col != m_textForegroundColour )
    {
        m_textForegroundColour = col;
        m_graphicContext->SetFont( m_font, m_textForegroundColour );
    }
}

void wxGCDCImpl::SetTextBackground( const wxColour &col )
{
    wxCHECK_RET( IsOk(), wxT("wxGCDC(cg)::SetTextBackground - invalid DC") );

    m_textBackgroundColour = col;
}

void wxGCDCImpl::SetMapMode( int mode )
{
    switch (mode)
    {
    case wxMM_TWIPS:
        SetLogicalScale( twips2mm * m_mm_to_pix_x, twips2mm * m_mm_to_pix_y );
        break;

    case wxMM_POINTS:
        SetLogicalScale( pt2mm * m_mm_to_pix_x, pt2mm * m_mm_to_pix_y );
        break;

    case wxMM_METRIC:
        SetLogicalScale( m_mm_to_pix_x, m_mm_to_pix_y );
        break;

    case wxMM_LOMETRIC:
        SetLogicalScale( m_mm_to_pix_x / 10.0, m_mm_to_pix_y / 10.0 );
        break;

    case wxMM_TEXT:
    default:
        SetLogicalScale( 1.0, 1.0 );
        break;
    }

    ComputeScaleAndOrigin();
}

void wxGCDCImpl::SetUserScale( double x, double y )
{
    // allow negative ? -> no

    m_userScaleX = x;
    m_userScaleY = y;
    ComputeScaleAndOrigin();
}

void wxGCDCImpl::SetLogicalScale( double x, double y )
{
    // allow negative ?
    m_logicalScaleX = x;
    m_logicalScaleY = y;
    ComputeScaleAndOrigin();
}

void wxGCDCImpl::SetLogicalOrigin( wxCoord x, wxCoord y )
{
    m_logicalOriginX = x * m_signX;   // is this still correct ?
    m_logicalOriginY = y * m_signY;
    ComputeScaleAndOrigin();
}

void wxGCDCImpl::SetDeviceOrigin( wxCoord x, wxCoord y )
{
    m_deviceOriginX = x;
    m_deviceOriginY = y;
    ComputeScaleAndOrigin();
}

void wxGCDCImpl::SetAxisOrientation( bool xLeftRight, bool yBottomUp )
{
    m_signX = (xLeftRight ?  1 : -1);
    m_signY = (yBottomUp ? -1 :  1);
    ComputeScaleAndOrigin();
}

wxSize wxGCDCImpl::GetPPI() const
{
    return wxSize(72, 72);
}

int wxGCDCImpl::GetDepth() const
{
    return 32;
}

void wxGCDCImpl::ComputeScaleAndOrigin()
{
    m_scaleX = m_logicalScaleX * m_userScaleX;
    m_scaleY = m_logicalScaleY * m_userScaleY;

    if ( m_graphicContext )
    {
        m_matrixCurrent = m_graphicContext->CreateMatrix();
        m_matrixCurrent.Translate( m_deviceOriginX, m_deviceOriginY );
        m_matrixCurrent.Scale( m_scaleX, m_scaleY );
        // the logical origin sets the origin to have new coordinates
        m_matrixCurrent.Translate( -m_logicalOriginX, -m_logicalOriginY );

        m_graphicContext->SetTransform( m_matrixOriginal );
        m_graphicContext->ConcatTransform( m_matrixCurrent );
    }
}

void wxGCDCImpl::SetPalette( const wxPalette& WXUNUSED(palette) )
{

}

void wxGCDCImpl::SetBackgroundMode( int mode )
{
    m_backgroundMode = mode;
}

void wxGCDCImpl::SetFont( const wxFont &font )
{
    m_font = font;
    if ( m_graphicContext )
    {
        wxFont f = font;
        if ( f.IsOk() )
            f.SetPointSize( /*LogicalToDeviceYRel*/(font.GetPointSize()));
        m_graphicContext->SetFont( f, m_textForegroundColour );
    }
}

void wxGCDCImpl::SetPen( const wxPen &pen )
{
    if ( m_pen == pen )
        return;

    m_pen = pen;
    if ( m_graphicContext )
    {
        m_graphicContext->SetPen( m_pen );
    }
}

void wxGCDCImpl::SetBrush( const wxBrush &brush )
{
    if (m_brush == brush)
        return;

    m_brush = brush;
    if ( m_graphicContext )
    {
        m_graphicContext->SetBrush( m_brush );
    }
}
 
void wxGCDCImpl::SetBackground( const wxBrush &brush )
{
    if (m_backgroundBrush == brush)
        return;

    m_backgroundBrush = brush;
    if (!m_backgroundBrush.IsOk())
        return;
}

void wxGCDCImpl::SetLogicalFunction( int function )
{
    if (m_logicalFunction == function)
        return;

    m_logicalFunction = function;
    if ( m_graphicContext->SetLogicalFunction( function ) )
        m_logicalFunctionSupported=true;
    else
        m_logicalFunctionSupported=false;
}

bool wxGCDCImpl::DoFloodFill(wxCoord WXUNUSED(x), wxCoord WXUNUSED(y),
                         const wxColour& WXUNUSED(col), int WXUNUSED(style))
{
    return false;
}

bool wxGCDCImpl::DoGetPixel( wxCoord WXUNUSED(x), wxCoord WXUNUSED(y), wxColour *WXUNUSED(col) ) const
{
    //  wxCHECK_MSG( 0 , false, wxT("wxGCDC(cg)::DoGetPixel - not implemented") );
    return false;
}

void wxGCDCImpl::DoDrawLine( wxCoord x1, wxCoord y1, wxCoord x2, wxCoord y2 )
{
    wxCHECK_RET( IsOk(), wxT("wxGCDC(cg)::DoDrawLine - invalid DC") );

    if ( !m_logicalFunctionSupported )
        return;

    m_graphicContext->StrokeLine(x1,y1,x2,y2);

    CalcBoundingBox(x1, y1);
    CalcBoundingBox(x2, y2);
}

void wxGCDCImpl::DoCrossHair( wxCoord x, wxCoord y )
{
    wxCHECK_RET( IsOk(), wxT("wxGCDC(cg)::DoCrossHair - invalid DC") );

    if ( !m_logicalFunctionSupported )
        return;

    int w = 0, h = 0;

    GetOwner()->GetSize( &w, &h );

    m_graphicContext->StrokeLine(0,y,w,y);
    m_graphicContext->StrokeLine(x,0,x,h);

    CalcBoundingBox(0, 0);
    CalcBoundingBox(0+w, 0+h);
}

void wxGCDCImpl::DoDrawArc( wxCoord x1, wxCoord y1,
                        wxCoord x2, wxCoord y2,
                        wxCoord xc, wxCoord yc )
{
    wxCHECK_RET( IsOk(), wxT("wxGCDC(cg)::DoDrawArc - invalid DC") );

    if ( !m_logicalFunctionSupported )
        return;

    double dx = x1 - xc;
    double dy = y1 - yc;
    double radius = sqrt((double)(dx * dx + dy * dy));
    wxCoord rad = (wxCoord)radius;
    double sa, ea;
    if (x1 == x2 && y1 == y2)
    {
        sa = 0.0;
        ea = 360.0;
    }
    else if (radius == 0.0)
    {
        sa = ea = 0.0;
    }
    else
    {
        sa = (x1 - xc == 0) ?
     (y1 - yc < 0) ? 90.0 : -90.0 :
             -atan2(double(y1 - yc), double(x1 - xc)) * RAD2DEG;
        ea = (x2 - xc == 0) ?
     (y2 - yc < 0) ? 90.0 : -90.0 :
             -atan2(double(y2 - yc), double(x2 - xc)) * RAD2DEG;
    }

    bool fill = m_brush.GetStyle() != wxTRANSPARENT;

    wxGraphicsPath path = m_graphicContext->CreatePath();
    if ( fill && ((x1!=x2)||(y1!=y2)) )
        path.MoveToPoint( xc, yc );
    // since these angles (ea,sa) are measured counter-clockwise, we invert them to
    // get clockwise angles
    path.AddArc( xc, yc , rad , DegToRad(-sa) , DegToRad(-ea), false );
    if ( fill && ((x1!=x2)||(y1!=y2)) )
        path.AddLineToPoint( xc, yc );
    m_graphicContext->DrawPath(path);
}

void wxGCDCImpl::DoDrawEllipticArc( wxCoord x, wxCoord y, wxCoord w, wxCoord h,
                                double sa, double ea )
{
    wxCHECK_RET( IsOk(), wxT("wxGCDC(cg)::DoDrawEllipticArc - invalid DC") );

    if ( !m_logicalFunctionSupported )
        return;

    m_graphicContext->PushState();
    m_graphicContext->Translate(x+w/2.0,y+h/2.0);
    wxDouble factor = ((wxDouble) w) / h;
    m_graphicContext->Scale( factor , 1.0);

    // since these angles (ea,sa) are measured counter-clockwise, we invert them to
    // get clockwise angles
    if ( m_brush.GetStyle() != wxTRANSPARENT )
    {
        wxGraphicsPath path = m_graphicContext->CreatePath();
        path.MoveToPoint( 0, 0 );
        path.AddLineToPoint( h / 2.0 * cos(DegToRad(sa)) , h / 2.0 * sin(DegToRad(-sa)) );
        path.AddLineToPoint( h / 2.0 * cos(DegToRad(ea)) , h / 2.0 * sin(DegToRad(-ea)) );
        path.AddLineToPoint( 0, 0 );
        m_graphicContext->FillPath( path );

        path = m_graphicContext->CreatePath();
        path.AddArc( 0, 0, h/2.0 , DegToRad(-sa) , DegToRad(-ea), sa > ea );
        m_graphicContext->FillPath( path );
        m_graphicContext->StrokePath( path );
    }
    else
    {
        wxGraphicsPath path = m_graphicContext->CreatePath();
    path.AddArc( 0, 0, h/2.0 , DegToRad(-sa) , DegToRad(-ea), sa > ea );
    m_graphicContext->DrawPath( path );
    }

    m_graphicContext->PopState();
}

void wxGCDCImpl::DoDrawPoint( wxCoord x, wxCoord y )
{
    wxCHECK_RET( IsOk(), wxT("wxGCDC(cg)::DoDrawPoint - invalid DC") );

    DoDrawLine( x , y , x + 1 , y + 1 );
}

void wxGCDCImpl::DoDrawLines(int n, wxPoint points[],
                         wxCoord xoffset, wxCoord yoffset)
{
    wxCHECK_RET( IsOk(), wxT("wxGCDC(cg)::DoDrawLines - invalid DC") );

    if ( !m_logicalFunctionSupported )
        return;

    wxPoint2DDouble* pointsD = new wxPoint2DDouble[n];
    for( int i = 0; i < n; ++i)
    {
        pointsD[i].m_x = points[i].x + xoffset;
        pointsD[i].m_y = points[i].y + yoffset;
    }

    m_graphicContext->StrokeLines( n , pointsD);
    delete[] pointsD;
}

#if wxUSE_SPLINES
void wxGCDCImpl::DoDrawSpline(const wxPointList *points)
{
    wxCHECK_RET( IsOk(), wxT("wxGCDC(cg)::DoDrawSpline - invalid DC") );

    if ( !m_logicalFunctionSupported )
        return;

    wxGraphicsPath path = m_graphicContext->CreatePath();

    wxPointList::compatibility_iterator node = points->GetFirst();
    if (node == wxPointList::compatibility_iterator())
        // empty list
        return;

    wxPoint *p = node->GetData();

    wxCoord x1 = p->x;
    wxCoord y1 = p->y;

    node = node->GetNext();
    p = node->GetData();

    wxCoord x2 = p->x;
    wxCoord y2 = p->y;
    wxCoord cx1 = ( x1 + x2 ) / 2;
    wxCoord cy1 = ( y1 + y2 ) / 2;

    path.MoveToPoint( x1 , y1 );
    path.AddLineToPoint( cx1 , cy1 );
#if !wxUSE_STL

    while ((node = node->GetNext()) != NULL)
#else

    while ((node = node->GetNext()))
#endif // !wxUSE_STL

    {
        p = node->GetData();
        x1 = x2;
        y1 = y2;
        x2 = p->x;
        y2 = p->y;
        wxCoord cx4 = (x1 + x2) / 2;
        wxCoord cy4 = (y1 + y2) / 2;

        path.AddQuadCurveToPoint(x1 , y1 ,cx4 , cy4 );

        cx1 = cx4;
        cy1 = cy4;
    }

    path.AddLineToPoint( x2 , y2 );

    m_graphicContext->StrokePath( path );
}
#endif // wxUSE_SPLINES

void wxGCDCImpl::DoDrawPolygon( int n, wxPoint points[],
                            wxCoord xoffset, wxCoord yoffset,
                            int fillStyle )
{
    wxCHECK_RET( IsOk(), wxT("wxGCDC(cg)::DoDrawPolygon - invalid DC") );

    if ( n <= 0 || (m_brush.GetStyle() == wxTRANSPARENT && m_pen.GetStyle() == wxTRANSPARENT ) )
        return;
    if ( !m_logicalFunctionSupported )
        return;

    bool closeIt = false;
    if (points[n-1] != points[0])
        closeIt = true;

    wxPoint2DDouble* pointsD = new wxPoint2DDouble[n+(closeIt?1:0)];
    for( int i = 0; i < n; ++i)
    {
        pointsD[i].m_x = points[i].x + xoffset;
        pointsD[i].m_y = points[i].y + yoffset;
    }
    if ( closeIt )
        pointsD[n] = pointsD[0];

    m_graphicContext->DrawLines( n+(closeIt?1:0) , pointsD, fillStyle);
    delete[] pointsD;
}

void wxGCDCImpl::DoDrawPolyPolygon(int n,
                               int count[],
                               wxPoint points[],
                               wxCoord xoffset,
                               wxCoord yoffset,
                               int fillStyle)
{
    wxASSERT(n > 1);
    wxGraphicsPath path = m_graphicContext->CreatePath();

    int i = 0;
    for ( int j = 0; j < n; ++j)
    {
        wxPoint start = points[i];
        path.MoveToPoint( start.x+ xoffset, start.y+ yoffset);
        ++i;
        int l = count[j];
        for ( int k = 1; k < l; ++k)
        {
            path.AddLineToPoint( points[i].x+ xoffset, points[i].y+ yoffset);
            ++i;
        }
        // close the polygon
        if ( start != points[i-1])
            path.AddLineToPoint( start.x+ xoffset, start.y+ yoffset);
    }
    m_graphicContext->DrawPath( path , fillStyle);
}

void wxGCDCImpl::DoDrawRectangle(wxCoord x, wxCoord y, wxCoord w, wxCoord h)
{
    wxCHECK_RET( IsOk(), wxT("wxGCDC(cg)::DoDrawRectangle - invalid DC") );

    if ( !m_logicalFunctionSupported )
        return;

    // CMB: draw nothing if transformed w or h is 0
    if (w == 0 || h == 0)
        return;

    if ( m_graphicContext->ShouldOffset() )
    {
        // if we are offsetting the entire rectangle is moved 0.5, so the
        // border line gets off by 1
        w -= 1;
        h -= 1;
    }
    m_graphicContext->DrawRectangle(x,y,w,h);
}

void wxGCDCImpl::DoDrawRoundedRectangle(wxCoord x, wxCoord y,
                                    wxCoord w, wxCoord h,
                                    double radius)
{
    wxCHECK_RET( IsOk(), wxT("wxGCDC(cg)::DoDrawRoundedRectangle - invalid DC") );

    if ( !m_logicalFunctionSupported )
        return;

    if (radius < 0.0)
        radius = - radius * ((w < h) ? w : h);

    // CMB: draw nothing if transformed w or h is 0
    if (w == 0 || h == 0)
        return;

    if ( m_graphicContext->ShouldOffset() )
    {
        // if we are offsetting the entire rectangle is moved 0.5, so the
        // border line gets off by 1
        w -= 1;
        h -= 1;
    }
    m_graphicContext->DrawRoundedRectangle( x,y,w,h,radius);
}

void wxGCDCImpl::DoDrawEllipse(wxCoord x, wxCoord y, wxCoord w, wxCoord h)
{
    wxCHECK_RET( IsOk(), wxT("wxGCDC(cg)::DoDrawEllipse - invalid DC") );

    if ( !m_logicalFunctionSupported )
        return;

    if ( m_graphicContext->ShouldOffset() )
    {
        // if we are offsetting the entire rectangle is moved 0.5, so the
        // border line gets off by 1
        w -= 1;
        h -= 1;
    }
    m_graphicContext->DrawEllipse(x,y,w,h);
}

bool wxGCDCImpl::CanDrawBitmap() const
{
    return true;
}

bool wxGCDCImpl::DoBlit(
    wxCoord xdest, wxCoord ydest, wxCoord width, wxCoord height,
    wxDC *source, wxCoord xsrc, wxCoord ysrc, int logical_func , bool useMask,
    wxCoord xsrcMask, wxCoord ysrcMask )
{
    return DoStretchBlit( xdest, ydest, width, height,
        source, xsrc, ysrc, width, height, logical_func, useMask,
        xsrcMask,ysrcMask );
}

bool wxGCDCImpl::DoStretchBlit(
    wxCoord xdest, wxCoord ydest, wxCoord dstWidth, wxCoord dstHeight,
    wxDC *source, wxCoord xsrc, wxCoord ysrc, wxCoord srcWidth, wxCoord srcHeight,
    int logical_func , bool WXUNUSED(useMask),
    wxCoord xsrcMask, wxCoord ysrcMask )
{
    wxCHECK_MSG( IsOk(), false, wxT("wxGCDC(cg)::DoStretchBlit - invalid DC") );
    wxCHECK_MSG( source->IsOk(), false, wxT("wxGCDC(cg)::DoStretchBlit - invalid source DC") );

    if ( logical_func == wxNO_OP )
        return true;
    else if ( !m_graphicContext->SetLogicalFunction( logical_func ) )
    
    {
        wxFAIL_MSG( wxT("Blitting is only supported with wxCOPY logical operation.") );
        return false;
    }

    if (xsrcMask == -1 && ysrcMask == -1)
    {
        xsrcMask = xsrc;
        ysrcMask = ysrc;
    }

    wxRect subrect(source->LogicalToDeviceX(xsrc),
                   source->LogicalToDeviceY(ysrc),
                   source->LogicalToDeviceXRel(srcWidth),
                   source->LogicalToDeviceYRel(srcHeight));

    // if needed clip the subrect down to the size of the source DC
    wxCoord sw, sh;
    source->GetSize(&sw, &sh);
    sw = source->LogicalToDeviceXRel(sw);
    sh = source->LogicalToDeviceYRel(sh);
    if (subrect.x + subrect.width > sw)
        subrect.width = sw - subrect.x;
    if (subrect.y + subrect.height > sh)
        subrect.height = sh - subrect.y;

    wxBitmap blit = source->GetAsBitmap( &subrect );

    if ( blit.IsOk() )
    {
        m_graphicContext->DrawBitmap( blit, xdest, ydest,
                                      dstWidth, dstHeight);
    }
    else
    {
        wxFAIL_MSG( wxT("Cannot Blit. Unable to get contents of DC as bitmap.") );
        return false;
    }

    // reset logical function
    m_graphicContext->SetLogicalFunction( m_logicalFunction );

    return true;
}

void wxGCDCImpl::DoDrawRotatedText(const wxString& str, wxCoord x, wxCoord y,
                               double angle)
{
    wxCHECK_RET( IsOk(), wxT("wxGCDC(cg)::DoDrawRotatedText - invalid DC") );

    if ( str.length() == 0 )
        return;
    if ( !m_logicalFunctionSupported )
        return;

    if ( m_backgroundMode == wxTRANSPARENT )
        m_graphicContext->DrawText( str, x ,y , DegToRad(angle ));
    else
        m_graphicContext->DrawText( str, x ,y , DegToRad(angle ), m_graphicContext->CreateBrush( wxBrush(m_textBackgroundColour,wxSOLID) ) );
}

void wxGCDCImpl::DoDrawText(const wxString& str, wxCoord x, wxCoord y)
{
    wxCHECK_RET( IsOk(), wxT("wxGCDC(cg)::DoDrawRotatedText - invalid DC") );

    if ( str.length() == 0 )
        return;

    if ( !m_logicalFunctionSupported )
        return;

    if ( m_backgroundMode == wxTRANSPARENT )
        m_graphicContext->DrawText( str, x ,y);
    else
        m_graphicContext->DrawText( str, x ,y , m_graphicContext->CreateBrush( wxBrush(m_textBackgroundColour,wxSOLID) ) );
}

bool wxGCDCImpl::CanGetTextExtent() const
{
    wxCHECK_MSG( IsOk(), false, wxT("wxGCDC(cg)::CanGetTextExtent - invalid DC") );

    return true;
}

void wxGCDCImpl::DoGetTextExtent( const wxString &str, wxCoord *width, wxCoord *height,
                              wxCoord *descent, wxCoord *externalLeading ,
                              const wxFont *theFont ) const
{
    wxCHECK_RET( IsOk(), wxT("wxGCDC(cg)::DoGetTextExtent - invalid DC") );

    if ( theFont )
    {
        m_graphicContext->SetFont( *theFont, m_textForegroundColour );
    }

    wxDouble h , d , e , w;

    m_graphicContext->GetTextExtent( str, &w, &h, &d, &e );

    if ( height )
        *height = (wxCoord)(h+0.5);
    if ( descent )
        *descent = (wxCoord)(d+0.5);
    if ( externalLeading )
        *externalLeading = (wxCoord)(e+0.5);
    if ( width )
        *width = (wxCoord)(w+0.5);

    if ( theFont )
    {
        m_graphicContext->SetFont( m_font, m_textForegroundColour );
    }
}

bool wxGCDCImpl::DoGetPartialTextExtents(const wxString& text, wxArrayInt& widths) const
{
    wxCHECK_MSG( IsOk(), false, wxT("wxGCDC(cg)::DoGetPartialTextExtents - invalid DC") );
    widths.Clear();
    widths.Add(0,text.Length());
    if ( text.IsEmpty() )
        return true;

    wxArrayDouble widthsD;

    m_graphicContext->GetPartialTextExtents( text, widthsD );
    for ( size_t i = 0; i < widths.GetCount(); ++i )
        widths[i] = (wxCoord)(widthsD[i] + 0.5);

    return true;
}

wxCoord wxGCDCImpl::GetCharWidth(void) const
{
    wxCoord width;
    DoGetTextExtent( wxT("g") , &width , NULL , NULL , NULL , NULL );

    return width;
}

wxCoord wxGCDCImpl::GetCharHeight(void) const
{
    wxCoord height;
    DoGetTextExtent( wxT("g") , NULL , &height , NULL , NULL , NULL );

    return height;
}

void wxGCDCImpl::Clear(void)
{
    wxCHECK_RET( IsOk(), wxT("wxGCDC(cg)::Clear - invalid DC") );
    // TODO better implementation / incorporate size info into wxGCDC or context
    m_graphicContext->SetBrush( m_backgroundBrush );
    wxPen p = *wxTRANSPARENT_PEN;
    m_graphicContext->SetPen( p );
    DoDrawRectangle( 0, 0, 32000 , 32000 );
    m_graphicContext->SetPen( m_pen );
    m_graphicContext->SetBrush( m_brush );
}

void wxGCDCImpl::DoGetSize(int *width, int *height) const
{
    *width = 10000;
    *height = 10000;
}

void wxGCDCImpl::DoGradientFillLinear(const wxRect& rect,
                                  const wxColour& initialColour,
                                  const wxColour& destColour,
                                  wxDirection nDirection )
{
    wxPoint start;
    wxPoint end;
    switch( nDirection)
    {
    case wxWEST :
        start = rect.GetRightBottom();
        start.x++;
        end = rect.GetLeftBottom();
        break;
    case wxEAST :
        start = rect.GetLeftBottom();
        end = rect.GetRightBottom();
        end.x++;
        break;
    case wxNORTH :
        start = rect.GetLeftBottom();
        start.y++;
        end = rect.GetLeftTop();
        break;
    case wxSOUTH :
        start = rect.GetLeftTop();
        end = rect.GetLeftBottom();
        end.y++;
        break;
    default :
        break;
    }

    if (rect.width == 0 || rect.height == 0)
        return;

    m_graphicContext->SetBrush( m_graphicContext->CreateLinearGradientBrush(
        start.x,start.y,end.x,end.y, initialColour, destColour));
    m_graphicContext->SetPen(*wxTRANSPARENT_PEN);
    m_graphicContext->DrawRectangle(rect.x,rect.y,rect.width,rect.height);
    m_graphicContext->SetPen(m_pen);
}

void wxGCDCImpl::DoGradientFillConcentric(const wxRect& rect,
                                      const wxColour& initialColour,
                                      const wxColour& destColour,
                                      const wxPoint& circleCenter)
{
    //Radius
    wxInt32 cx = rect.GetWidth() / 2;
    wxInt32 cy = rect.GetHeight() / 2;
    wxInt32 nRadius;
    if (cx < cy)
        nRadius = cx;
    else
        nRadius = cy;

    // make sure the background is filled (todo move into specific platform implementation ?)
    m_graphicContext->SetPen(*wxTRANSPARENT_PEN);
    m_graphicContext->SetBrush( wxBrush( destColour) );
    m_graphicContext->DrawRectangle(rect.x,rect.y,rect.width,rect.height);

    m_graphicContext->SetBrush( m_graphicContext->CreateRadialGradientBrush(
        rect.x+circleCenter.x,rect.y+circleCenter.y,
        rect.x+circleCenter.x,rect.y+circleCenter.y,
        nRadius,initialColour,destColour));

    m_graphicContext->DrawRectangle(rect.x,rect.y,rect.width,rect.height);
    m_graphicContext->SetPen(m_pen);
}

void wxGCDCImpl::DoDrawCheckMark(wxCoord x, wxCoord y,
                             wxCoord width, wxCoord height)
{
    wxDCImpl::DoDrawCheckMark(x,y,width,height);
}

#endif // wxUSE_GRAPHICS_CONTEXT
