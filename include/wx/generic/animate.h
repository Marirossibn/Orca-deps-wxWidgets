/////////////////////////////////////////////////////////////////////////////
// Name:        wx/generic/animate.h
// Purpose:     wxAnimationGenericImpl and wxGenericAnimationCtrl
// Author:      Julian Smart and Guillermo Rodriguez Garcia
// Modified by: Francesco Montorsi
// Created:     13/8/99
// Copyright:   (c) Julian Smart and Guillermo Rodriguez Garcia
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_GENERIC_ANIMATEH__
#define _WX_GENERIC_ANIMATEH__

#include "wx/bitmap.h"

// ----------------------------------------------------------------------------
// wxGenericAnimation
// ----------------------------------------------------------------------------

WX_DECLARE_LIST_WITH_DECL(wxAnimationDecoder, wxAnimationDecoderList, class WXDLLIMPEXP_ADV);

class WXDLLIMPEXP_ADV wxAnimationGenericImpl : public wxAnimationImpl
{
public:
    wxAnimationGenericImpl() {}

    virtual wxAnimationImplType GetImplType() wxOVERRIDE
        { return wxANIMATION_IMPL_TYPE_GENERIC; }

    virtual bool IsOk() const wxOVERRIDE
        { return m_refData != NULL; }

    virtual unsigned int GetFrameCount() const wxOVERRIDE;
    virtual int GetDelay(unsigned int i) const wxOVERRIDE;
    virtual wxImage GetFrame(unsigned int i) const wxOVERRIDE;
    virtual wxSize GetSize() const wxOVERRIDE;

    virtual bool LoadFile(const wxString& filename,
                          wxAnimationType type = wxANIMATION_TYPE_ANY) wxOVERRIDE;
    virtual bool Load(wxInputStream& stream,
                      wxAnimationType type = wxANIMATION_TYPE_ANY) wxOVERRIDE;

    // extended interface used only by the generic implementation of wxAnimationCtrl
    virtual wxPoint GetFramePosition(unsigned int frame) const;
    virtual wxSize GetFrameSize(unsigned int frame) const;
    virtual wxAnimationDisposal GetDisposalMethod(unsigned int frame) const;
    virtual wxColour GetTransparentColour(unsigned int frame) const;
    virtual wxColour GetBackgroundColour() const;

protected:
    static wxAnimationDecoderList sm_handlers;

public:
    static inline wxAnimationDecoderList& GetHandlers() { return sm_handlers; }
    static void AddHandler(wxAnimationDecoder *handler);
    static void InsertHandler(wxAnimationDecoder *handler);
    static const wxAnimationDecoder *FindHandler( wxAnimationType animType );

    static void CleanUpHandlers();
    static void InitStandardHandlers();

    wxDECLARE_NO_COPY_CLASS(wxAnimationGenericImpl);
    wxDECLARE_DYNAMIC_CLASS(wxAnimationGenericImpl);
};


// ----------------------------------------------------------------------------
// wxGenericAnimationCtrl
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_ADV wxGenericAnimationCtrl: public wxAnimationCtrlBase
{
public:
    wxGenericAnimationCtrl() { Init(); }
    wxGenericAnimationCtrl(wxWindow *parent,
                           wxWindowID id,
                           const wxAnimation& anim = wxNullAnimation,
                           const wxPoint& pos = wxDefaultPosition,
                           const wxSize& size = wxDefaultSize,
                           long style = wxAC_DEFAULT_STYLE,
                           const wxString& name = wxAnimationCtrlNameStr)
    {
        Init();

        Create(parent, id, anim, pos, size, style, name);
    }

    void Init();

    bool Create(wxWindow *parent, wxWindowID id,
                const wxAnimation& anim = wxNullAnimation,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxAC_DEFAULT_STYLE,
                const wxString& name = wxAnimationCtrlNameStr);

    ~wxGenericAnimationCtrl();


public:
    virtual bool LoadFile(const wxString& filename, wxAnimationType type = wxANIMATION_TYPE_ANY) wxOVERRIDE;
    virtual bool Load(wxInputStream& stream, wxAnimationType type = wxANIMATION_TYPE_ANY) wxOVERRIDE;

    virtual void Stop() wxOVERRIDE;
    virtual bool Play() wxOVERRIDE
        { return Play(true /* looped */); }
    virtual bool IsPlaying() const wxOVERRIDE
        { return m_isPlaying; }

    void SetAnimation(const wxAnimation &animation) wxOVERRIDE;
    wxAnimation GetAnimation() const wxOVERRIDE
        { return m_animation; }

    virtual void SetInactiveBitmap(const wxBitmap &bmp) wxOVERRIDE;

    // override base class method
    virtual bool SetBackgroundColour(const wxColour& col) wxOVERRIDE;

public:     // event handlers

    void OnPaint(wxPaintEvent& event);
    void OnTimer(wxTimerEvent& event);
    void OnSize(wxSizeEvent& event);

public:     // extended API specific to this implementation of wxAnimateCtrl

    // Specify whether the animation's background colour is to be shown (the default),
    // or whether the window background should show through
    void SetUseWindowBackgroundColour(bool useWinBackground = true)
        { m_useWinBackgroundColour = useWinBackground; }
    bool IsUsingWindowBackgroundColour() const
        { return m_useWinBackgroundColour; }

    // This overload of Play() lets you specify if the animation must loop or not
    bool Play(bool looped);

    // Draw the current frame of the animation into given DC.
    // This is fast as current frame is always cached.
    void DrawCurrentFrame(wxDC& dc);

    // Returns a wxBitmap with the current frame drawn in it
    wxBitmap& GetBackingStore()
        { return m_backingStore; }

protected:      // internal utilities

    // resize this control to fit m_animation
    void FitToAnimation();

    // Draw the background; use this when e.g. previous frame had wxANIM_TOBACKGROUND disposal.
    void DisposeToBackground();
    void DisposeToBackground(wxDC& dc);
    void DisposeToBackground(wxDC& dc, const wxPoint &pos, const wxSize &sz);

    void IncrementalUpdateBackingStore();
    bool RebuildBackingStoreUpToFrame(unsigned int);
    void DrawFrame(wxDC &dc, unsigned int);

    virtual void DisplayStaticImage() wxOVERRIDE;
    virtual wxSize DoGetBestSize() const wxOVERRIDE;

    // Helpers to safely access methods in the wxAnimationGenericImpl that are
    // specific to the generic implementation
    wxPoint animation_GetFramePosition(unsigned int frame) const;
    wxSize animation_GetFrameSize(unsigned int frame) const;
    wxAnimationDisposal animation_GetDisposalMethod(unsigned int frame) const;
    wxColour animation_GetTransparentColour(unsigned int frame) const;
    wxColour animation_GetBackgroundColour() const;


protected:
    unsigned int  m_currentFrame;     // Current frame
    bool          m_looped;           // Looped, or not
    wxTimer       m_timer;            // The timer
    wxAnimation   m_animation;        // The animation

    bool          m_isPlaying;        // Is the animation playing?
    bool          m_useWinBackgroundColour; // Use animation bg colour or window bg colour?

    wxBitmap      m_backingStore;     // The frames are drawn here and then blitted
                                      // on the screen

private:
    typedef wxAnimationCtrlBase base_type;
    wxDECLARE_DYNAMIC_CLASS(wxGenericAnimationCtrl);
    wxDECLARE_EVENT_TABLE();
};

#endif // _WX_GENERIC_ANIMATEH__
