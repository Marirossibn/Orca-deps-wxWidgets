/////////////////////////////////////////////////////////////////////////////
// Name:        wx/mgl/toplevel.h
// Purpose:     Top level window, abstraction of wxFrame and wxDialog
// Author:      Vaclav Slavik
// Id:          $Id$
// Copyright:   (c) 2001 SciTech Software, Inc. (www.scitechsoft.com)
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#ifndef __WX_TOPLEVEL_H__
#define __WX_TOPLEVEL_H__

#ifdef __GNUG__
    #pragma interface "toplevel.h"
#endif


//-----------------------------------------------------------------------------
// wxTopLevelWindowMGL
//-----------------------------------------------------------------------------

class wxTopLevelWindowMGL : public wxTopLevelWindowBase
{
public:
    // construction
    wxTopLevelWindowMGL() { Init(); }
    wxTopLevelWindowMGL(wxWindow *parent,
                        wxWindowID id,
                        const wxString& title,
                        const wxPoint& pos = wxDefaultPosition,
                        const wxSize& size = wxDefaultSize,
                        long style = wxDEFAULT_FRAME_STYLE,
                        const wxString& name = wxFrameNameStr)
    {
        Init();

        Create(parent, id, title, pos, size, style, name);
    }

    bool Create(wxWindow *parent,
                wxWindowID id,
                const wxString& title,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxDEFAULT_FRAME_STYLE,
                const wxString& name = wxFrameNameStr);

    virtual ~wxTopLevelWindowMGL();

    // implement base class pure virtuals
    virtual void Maximize(bool maximize = TRUE);
    virtual bool IsMaximized() const;
    virtual void Iconize(bool iconize = TRUE);
    virtual bool IsIconized() const;
    virtual void Restore();

    virtual bool ShowFullScreen(bool show, long style = wxFULLSCREEN_ALL);
    virtual bool IsFullScreen() const { return m_fsIsShowing; }

    virtual bool Show(bool show = TRUE);

    virtual void SetTitle(const wxString &title) { m_title = title; }
    virtual wxString GetTitle() const { return m_title; }

    // implementation from now on
    // --------------------------

protected:
    // common part of all ctors
    void Init();

    wxString      m_title;
    bool          m_fsIsShowing;         /* full screen */
    long          m_fsSaveStyle;
    long          m_fsSaveFlag;
    wxRect        m_fsSaveFrame;

    // is the frame currently iconized?
    bool          m_isIconized;
    // and maximized?
    bool          m_isMaximized;
    wxRect        m_savedFrame;
};

#endif // __WX_TOPLEVEL_H__
