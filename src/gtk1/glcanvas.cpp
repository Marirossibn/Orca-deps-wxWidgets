/////////////////////////////////////////////////////////////////////////////
// Name:        src/gtk1/glcanvas.cpp
// Purpose:     wxGLCanvas, for using OpenGL/Mesa with wxWidgets and GTK
// Author:      Robert Roebling
// Modified by:
// Created:     17/08/98
// RCS-ID:      $Id$
// Copyright:   (c) Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#if wxUSE_GLCANVAS

#include "wx/glcanvas.h"

#ifndef WX_PRECOMP
    #include "wx/app.h"
    #include "wx/frame.h"
    #include "wx/colour.h"
    #include "wx/module.h"
#endif // WX_PRECOMP

extern "C"
{
#include "gtk/gtk.h"
#include "gdk/gdk.h"
#include "gdk/gdkx.h"
}

#include "wx/gtk1/win_gtk.h"
#include "wx/gtk1/private.h"

//---------------------------------------------------------------------------
// global data
//---------------------------------------------------------------------------

XVisualInfo *g_vi = (XVisualInfo*) NULL;

//-----------------------------------------------------------------------------
// idle system
//-----------------------------------------------------------------------------

extern void wxapp_install_idle_handler();
extern bool g_isIdle;

// ----------------------------------------------------------------------------
// helper functions
// ----------------------------------------------------------------------------

// wrapper around glXMakeContextCurrent/glXMakeCurrent depending on GLX
// version
static void wxMakeContextCurrent(GLXDrawable drawable, GLXContext context)
{
    if (wxGLCanvas::GetGLXVersion() >= 13)
        glXMakeContextCurrent( GDK_DISPLAY(), drawable, drawable, context);
    else // GLX <= 1.2 doesn't have glXMakeContextCurrent()
        glXMakeCurrent( GDK_DISPLAY(), drawable, context);
}

//---------------------------------------------------------------------------
// wxGLContext
//---------------------------------------------------------------------------

IMPLEMENT_CLASS(wxGLContext,wxObject)

wxGLContext::wxGLContext(wxWindow* win, const wxGLContext* other)
{
    wxGLCanvas *gc = (wxGLCanvas*) win;

    if (wxGLCanvas::GetGLXVersion() >= 13)
    {
        GLXFBConfig *fbc = gc->m_fbc;
        wxCHECK_RET( fbc, _T("invalid GLXFBConfig for OpenGl") );
        m_glContext = glXCreateNewContext( GDK_DISPLAY(), fbc[0], GLX_RGBA_TYPE,
                                           other ? other->m_glContext : None,
                                           GL_TRUE );
    }
    else // GLX <= 1.2
    {
        XVisualInfo *vi = (XVisualInfo *) gc->m_vi;
        wxCHECK_RET( vi, _T("invalid visual for OpenGl") );
        m_glContext = glXCreateContext( GDK_DISPLAY(), vi,
                                        other ? other->m_glContext : None,
                                        GL_TRUE );
    }

    wxASSERT_MSG( m_glContext, _T("Couldn't create OpenGl context") );
}

wxGLContext::~wxGLContext()
{
    if ( !m_glContext )
        return;

    if ( m_glContext == glXGetCurrentContext() )
        wxMakeContextCurrent(None, NULL);

    glXDestroyContext( GDK_DISPLAY(), m_glContext );
}

void wxGLContext::SetCurrent(const wxGLCanvas& win) const
{
    if ( !m_glContext )
        return;

    GdkWindow *window = GTK_PIZZA(win.m_wxwindow)->bin_window;
    wxCHECK_RET( window, _T("window must be shown") );

    wxMakeContextCurrent(GDK_WINDOW_XWINDOW(window), m_glContext);
}


#if WXWIN_COMPATIBILITY_2_8

//-----------------------------------------------------------------------------
// "realize" from m_wxwindow: used to create m_glContext implicitly
//-----------------------------------------------------------------------------

extern "C" {
static gint
gtk_glwindow_realized_callback( GtkWidget *WXUNUSED(widget), wxGLCanvas *win )
{
    win->GTKInitImplicitContext();

    return FALSE;
}
}

#endif // WXWIN_COMPATIBILITY_2_8

//-----------------------------------------------------------------------------
// "map" from m_wxwindow
//-----------------------------------------------------------------------------

extern "C" {
static gint
gtk_glwindow_map_callback( GtkWidget * WXUNUSED(widget), wxGLCanvas *win )
{
    wxPaintEvent event( win->GetId() );
    event.SetEventObject( win );
    win->GetEventHandler()->ProcessEvent( event );

    win->GetUpdateRegion().Clear();

    return FALSE;
}
}

//-----------------------------------------------------------------------------
// "expose_event" of m_wxwindow
//-----------------------------------------------------------------------------

extern "C" {
static void
gtk_glwindow_expose_callback( GtkWidget *WXUNUSED(widget), GdkEventExpose *gdk_event, wxGLCanvas *win )
{
    if (g_isIdle)
        wxapp_install_idle_handler();

    win->GetUpdateRegion().Union( gdk_event->area.x,
                                  gdk_event->area.y,
                                  gdk_event->area.width,
                                  gdk_event->area.height );
}
}

//-----------------------------------------------------------------------------
// "draw" of m_wxwindow
//-----------------------------------------------------------------------------

extern "C" {
static void
gtk_glwindow_draw_callback( GtkWidget *WXUNUSED(widget), GdkRectangle *rect, wxGLCanvas *win )
{
    if (g_isIdle)
        wxapp_install_idle_handler();

    win->GetUpdateRegion().Union( rect->x, rect->y,
                                  rect->width, rect->height );
}
}

//-----------------------------------------------------------------------------
// "size_allocate" of m_wxwindow
//-----------------------------------------------------------------------------

extern "C" {
static void
gtk_glcanvas_size_callback( GtkWidget *WXUNUSED(widget), GtkAllocation* alloc, wxGLCanvas *win )
{
    if (g_isIdle)
        wxapp_install_idle_handler();

    if (!win->m_hasVMT)
        return;

    wxSizeEvent event( wxSize(win->m_width,win->m_height), win->GetId() );
    event.SetEventObject( win );
    win->GetEventHandler()->ProcessEvent( event );
}
}

//---------------------------------------------------------------------------
// wxGlCanvas
//---------------------------------------------------------------------------

IMPLEMENT_CLASS(wxGLCanvas, wxWindow)

wxGLCanvas::wxGLCanvas(wxWindow *parent,
                       wxWindowID id,
                       const int *attribList,
                       const wxPoint& pos,
                       const wxSize& size,
                       long style,
                       const wxString& name,
                       const wxPalette& palette)
#if WXWIN_COMPATIBILITY_2_8
    : m_createImplicitContext(false)
#endif
{
    Create(parent, id, pos, size, style, name, attribList, palette);
}

#if WXWIN_COMPATIBILITY_2_8

wxGLCanvas::wxGLCanvas(wxWindow *parent,
                       wxWindowID id,
                       const wxPoint& pos,
                       const wxSize& size,
                       long style,
                       const wxString& name,
                       const int *attribList,
                       const wxPalette& palette)
    : m_createImplicitContext(true)
{
    Create(parent, id, pos, size, style, name, attribList, palette);
}

wxGLCanvas::wxGLCanvas(wxWindow *parent,
                       const wxGLContext *shared,
                       wxWindowID id,
                       const wxPoint& pos,
                       const wxSize& size,
                       long style,
                       const wxString& name,
                       const int *attribList,
                       const wxPalette& palette)
    : m_createImplicitContext(true)
{
    m_sharedContext = wx_const_cast(wxGLContext *, shared);

    Create(parent, id, pos, size, style, name, attribList, palette);
}

wxGLCanvas::wxGLCanvas(wxWindow *parent,
                       const wxGLCanvas *shared,
                       wxWindowID id,
                       const wxPoint& pos, const wxSize& size,
                       long style, const wxString& name,
                       const int *attribList,
                       const wxPalette& palette )
    : m_createImplicitContext(true)
{
    m_sharedContextOf = wx_const_cast(wxGLCanvas *, shared);

    Create(parent, id, pos, size, style, name, attribList, palette);
}

#endif // WXWIN_COMPATIBILITY_2_8

bool wxGLCanvas::Create(wxWindow *parent,
                        wxWindowID id,
                        const wxPoint& pos,
                        const wxSize& size,
                        long style,
                        const wxString& name,
                        const int *attribList,
                        const wxPalette& palette)
{
    m_noExpose = true;
    m_nativeSizeEvent = true;
    m_fbc = NULL;
    m_vi = NULL;

    if (wxGLCanvas::GetGLXVersion() >= 13)
    {
        // GLX >= 1.3 uses a GLXFBConfig
        GLXFBConfig * fbc = NULL;
        if (wxTheApp->m_glFBCInfo != NULL)
        {
            fbc = (GLXFBConfig *) wxTheApp->m_glFBCInfo;
            m_canFreeFBC = false; // owned by wxTheApp - don't free upon destruction
        }
        else
        {
            fbc = (GLXFBConfig *) wxGLCanvas::ChooseGLFBC(attribList);
            m_canFreeFBC = true;
        }
        m_fbc = fbc;  // save for later use
        wxCHECK_MSG( m_fbc, false, _T("required FBConfig couldn't be found") );
    }

    XVisualInfo *vi = NULL;
    if (wxTheApp->m_glVisualInfo != NULL)
    {
        vi = (XVisualInfo *)wxTheApp->m_glVisualInfo;
        m_canFreeVi = false; // owned by wxTheApp - don't free upon destruction
    }
    else
    {
        if (wxGLCanvas::GetGLXVersion() >= 13)
        // GLX >= 1.3
            vi = glXGetVisualFromFBConfig(GDK_DISPLAY(), m_fbc[0]);
        else
            // GLX <= 1.2
            vi = (XVisualInfo *) ChooseGLVisual(attribList);

        m_canFreeVi = true;
    }

    m_vi = vi;  // save for later use

    wxCHECK_MSG( m_vi, false, _T("required visual couldn't be found") );
    GdkVisual *visual;
    GdkColormap *colormap;

    visual = gdkx_visual_get( vi->visualid );
    colormap = gdk_colormap_new( visual, TRUE );

    gtk_widget_push_colormap( colormap );
    gtk_widget_push_visual( visual );

    wxWindow::Create( parent, id, pos, size, style, name );
    m_glWidget = m_wxwindow;

    gtk_pizza_set_clear( GTK_PIZZA(m_wxwindow), FALSE );

#if WXWIN_COMPATIBILITY_2_8
    gtk_signal_connect( GTK_OBJECT(m_wxwindow), "realize",
                            GTK_SIGNAL_FUNC(gtk_glwindow_realized_callback), (gpointer) this);
#endif // WXWIN_COMPATIBILITY_2_8

    gtk_signal_connect( GTK_OBJECT(m_wxwindow), "map",
                            GTK_SIGNAL_FUNC(gtk_glwindow_map_callback), (gpointer) this);

    gtk_signal_connect( GTK_OBJECT(m_wxwindow), "expose_event",
                            GTK_SIGNAL_FUNC(gtk_glwindow_expose_callback), (gpointer) this);

    gtk_signal_connect( GTK_OBJECT(m_wxwindow), "draw",
                            GTK_SIGNAL_FUNC(gtk_glwindow_draw_callback), (gpointer) this);

    gtk_signal_connect( GTK_OBJECT(m_widget), "size_allocate",
                            GTK_SIGNAL_FUNC(gtk_glcanvas_size_callback), (gpointer) this);

    gtk_widget_pop_visual();

    gtk_widget_pop_colormap();

#if WXWIN_COMPATIBILITY_2_8
    // if our parent window is already visible, we had been realized before we
    // connected to the "realize" signal and hence our m_glContext hasn't been
    // initialized yet and we have to do it now
    if (GTK_WIDGET_REALIZED(m_wxwindow))
        gtk_glwindow_realized_callback( m_wxwindow, this );
#endif // WXWIN_COMPATIBILITY_2_8

    if (GTK_WIDGET_MAPPED(m_wxwindow))
        gtk_glwindow_map_callback( m_wxwindow, this );

    return true;
}

wxGLCanvas::~wxGLCanvas()
{
    GLXFBConfig * fbc = (GLXFBConfig *) m_fbc;
    if (fbc && m_canFreeFBC)
        XFree( fbc );

    XVisualInfo *vi = (XVisualInfo *) m_vi;
    if (vi && m_canFreeVi)
        XFree( vi );
}

void* wxGLCanvas::ChooseGLVisual(const int *attribList)
{
    int data[512];
    GetGLAttribListFromWX( attribList, data );

    Display *dpy = GDK_DISPLAY();

    return glXChooseVisual( dpy, DefaultScreen(dpy), data );
}

void* wxGLCanvas::ChooseGLFBC(const int *attribList)
{
    int data[512];
    GetGLAttribListFromWX( attribList, data );

    int returned;
    return glXChooseFBConfig( GDK_DISPLAY(), DefaultScreen(GDK_DISPLAY()),
                              data, &returned );
}


void
wxGLCanvas::GetGLAttribListFromWX(const int *wx_attribList, int *gl_attribList)
{
    if ( !wx_attribList )
    {
        if (wxGLCanvas::GetGLXVersion() >= 13)
        {
            // leave GLX >= 1.3 choose the default attributes
            gl_attribList[0] = 0;
        }
        else // GLX < 1.3
        {
            int i = 0;
            // default settings if attriblist = 0
            gl_attribList[i++] = GLX_RGBA;
            gl_attribList[i++] = GLX_DOUBLEBUFFER;
            gl_attribList[i++] = GLX_DEPTH_SIZE;   gl_attribList[i++] = 1;
            gl_attribList[i++] = GLX_RED_SIZE;     gl_attribList[i++] = 1;
            gl_attribList[i++] = GLX_GREEN_SIZE;   gl_attribList[i++] = 1;
            gl_attribList[i++] = GLX_BLUE_SIZE;    gl_attribList[i++] = 1;
            gl_attribList[i++] = GLX_ALPHA_SIZE;   gl_attribList[i++] = 0;
            gl_attribList[i++] = None;
        }
    }
    else // have non-default attributes
    {
        int arg=0, p=0;
        while( (wx_attribList[arg]!=0) && (p<510) )
        {
            switch( wx_attribList[arg++] )
            {
                case WX_GL_RGBA:
                    if (wxGLCanvas::GetGLXVersion() <= 12)
                    {
                        // for GLX >= 1.3, GLX_RGBA is useless (setting this flags will crash on most opengl implm)
                        gl_attribList[p++] = GLX_RGBA;
                    }
                    break;
                case WX_GL_BUFFER_SIZE:
                    gl_attribList[p++] = GLX_BUFFER_SIZE;
                    gl_attribList[p++] = wx_attribList[arg++];
                    break;
                case WX_GL_LEVEL:
                    gl_attribList[p++] = GLX_LEVEL;
                    gl_attribList[p++] = wx_attribList[arg++];
                    break;
                case WX_GL_DOUBLEBUFFER:
                    gl_attribList[p++] = GLX_DOUBLEBUFFER;
                    gl_attribList[p++] = 1;
                    break;
                case WX_GL_STEREO:
                    gl_attribList[p++] = GLX_STEREO;
                    break;
                case WX_GL_AUX_BUFFERS:
                    gl_attribList[p++] = GLX_AUX_BUFFERS;
                    gl_attribList[p++] = wx_attribList[arg++];
                    break;
                case WX_GL_MIN_RED:
                    gl_attribList[p++] = GLX_RED_SIZE;
                    gl_attribList[p++] = wx_attribList[arg++];
                    break;
                case WX_GL_MIN_GREEN:
                    gl_attribList[p++] = GLX_GREEN_SIZE;
                    gl_attribList[p++] = wx_attribList[arg++];
                    break;
                case WX_GL_MIN_BLUE:
                    gl_attribList[p++] = GLX_BLUE_SIZE;
                    gl_attribList[p++] = wx_attribList[arg++];
                    break;
                case WX_GL_MIN_ALPHA:
                    gl_attribList[p++] = GLX_ALPHA_SIZE;
                    gl_attribList[p++] = wx_attribList[arg++];
                    break;
                case WX_GL_DEPTH_SIZE:
                    gl_attribList[p++] = GLX_DEPTH_SIZE;
                    gl_attribList[p++] = wx_attribList[arg++];
                    break;
                case WX_GL_STENCIL_SIZE:
                    gl_attribList[p++] = GLX_STENCIL_SIZE;
                    gl_attribList[p++] = wx_attribList[arg++];
                    break;
                case WX_GL_MIN_ACCUM_RED:
                    gl_attribList[p++] = GLX_ACCUM_RED_SIZE;
                    gl_attribList[p++] = wx_attribList[arg++];
                    break;
                case WX_GL_MIN_ACCUM_GREEN:
                    gl_attribList[p++] = GLX_ACCUM_GREEN_SIZE;
                    gl_attribList[p++] = wx_attribList[arg++];
                    break;
                case WX_GL_MIN_ACCUM_BLUE:
                    gl_attribList[p++] = GLX_ACCUM_BLUE_SIZE;
                    gl_attribList[p++] = wx_attribList[arg++];
                    break;
                case WX_GL_MIN_ACCUM_ALPHA:
                    gl_attribList[p++] = GLX_ACCUM_ALPHA_SIZE;
                    gl_attribList[p++] = wx_attribList[arg++];
                    break;
                default:
                    break;
            }
        }

        gl_attribList[p] = 0;
    }
}

/* static */
int wxGLCanvas::GetGLXVersion()
{
    static int s_glxVersion = 0;
    if ( s_glxVersion == 0 )
    {
        // check the GLX version
        int glxMajorVer, glxMinorVer;
        bool ok = glXQueryVersion(GDK_DISPLAY(), &glxMajorVer, &glxMinorVer);
        wxASSERT_MSG( ok, _T("GLX version not found") );
        if (!ok)
            s_glxVersion = 10; // 1.0 by default
        else
            s_glxVersion = glxMajorVer*10 + glxMinorVer;
    }

    return s_glxVersion;
}

void wxGLCanvas::SwapBuffers()
{
    GdkWindow *window = GTK_PIZZA(m_wxwindow)->bin_window;
    glXSwapBuffers( GDK_DISPLAY(), GDK_WINDOW_XWINDOW( window ) );
}

void wxGLCanvas::OnInternalIdle()
{
    if (!m_updateRegion.IsEmpty())
    {
        wxPaintEvent event( GetId() );
        event.SetEventObject( this );
        GetEventHandler()->ProcessEvent( event );

        GetUpdateRegion().Clear();
    }

    wxWindow::OnInternalIdle();
}

#if WXWIN_COMPATIBILITY_2_8

void wxGLCanvas::GTKInitImplicitContext()
{
    if ( !m_glContext && m_createImplicitContext )
    {
        wxGLContext *share = m_sharedContext;
        if ( !share && m_sharedContextOf )
            share = m_sharedContextOf->m_glContext;

        m_glContext = new wxGLContext(this, share);
    }
}

#endif // WXWIN_COMPATIBILITY_2_8

//---------------------------------------------------------------------------
// wxGLApp
//---------------------------------------------------------------------------

bool wxGLApp::InitGLVisual(const int *attribList)
{
    if ( wxGLCanvas::GetGLXVersion() >= 13 )
    {
        if (m_glFBCInfo)
            XFree(m_glFBCInfo);
        m_glFBCInfo = wxGLCanvas::ChooseGLFBC(attribList);

        if ( !m_glFBCInfo )
            return false;

        if (m_glVisualInfo)
            XFree(m_glVisualInfo);
        m_glVisualInfo = glXGetVisualFromFBConfig(GDK_DISPLAY(), ((GLXFBConfig *)m_glFBCInfo)[0]);
    }
    else // GLX <= 1.2
    {
        if (m_glVisualInfo)
            XFree(m_glVisualInfo);
        m_glVisualInfo = wxGLCanvas::ChooseGLVisual(attribList);
    }

    return m_glVisualInfo != NULL;
}

#endif // wxUSE_GLCANVAS
