/////////////////////////////////////////////////////////////////////////////
// Name:        textctrl.h
// Purpose:     wxTextCtrl class
// Author:      David Webster
// Modified by:
// Created:     10/17/99
// RCS-ID:      $Id$
// Copyright:   (c) David Webster
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_TEXTCTRL_H_
#define _WX_TEXTCTRL_H_

class WXDLLEXPORT wxTextCtrl : public wxTextCtrlBase
{
public:
    // creation
    // --------

    wxTextCtrl();
    wxTextCtrl( wxWindow*          pParent
               ,wxWindowID         vId
               ,const wxString&    rsValue = wxEmptyString
               ,const wxPoint&     rPos = wxDefaultPosition
               ,const wxSize&      rSize = wxDefaultSize
               ,long               lStyle = 0
#if wxUSE_VALIDATORS
               ,const wxValidator& rValidator = wxDefaultValidator
#endif
               ,const wxString&    rsName = wxTextCtrlNameStr
              )
    {
        Create(pParent, vId, rsValue, rPos, rSize, lStyle, rValidator, rsName);
    }

    bool Create( wxWindow*          pParent
                ,wxWindowID         vId
                ,const wxString&    rsValue = wxEmptyString
                ,const wxPoint&     rPos = wxDefaultPosition
                ,const wxSize&      rSize = wxDefaultSize
                ,long               lStyle = 0
#if wxUSE_VALIDATORS
                ,const wxValidator& rValidator = wxDefaultValidator
#endif
                ,const wxString&    rsName = wxTextCtrlNameStr
               );

    // implement base class pure virtuals
    // ----------------------------------

    virtual      wxString GetValue(void) const;
    virtual void SetValue(const wxString& rsValue);

    virtual int      GetLineLength(long nLineNo) const;
    virtual wxString GetLineText(long nLineNo) const;
    virtual int      GetNumberOfLines(void) const;

    virtual bool IsModified(void) const;
    virtual bool IsEditable(void) const;

    // If the return values from and to are the same, there is no selection.
    virtual void GetSelection( long* pFrom
                              ,long* pTo
                             ) const;

    // operations
    // ----------

    // editing
    virtual void Clear(void);
    virtual void Replace( long            lFrom
                         ,long            lTo
                         ,const wxString& rsValue
                        );
    virtual void Remove( long lFrom
                        ,long lTo
                       );

    // load the controls contents from the file
    virtual bool LoadFile(const wxString& rsFile);

    // clears the dirty flag
    virtual void DiscardEdits(void);

    // writing text inserts it at the current position, appending always
    // inserts it at the end
    virtual void WriteText(const wxString& rsText);
    virtual void AppendText(const wxString& rsText);

    // translate between the position (which is just an index in the text ctrl
    // considering all its contents as a single strings) and (x, y) coordinates
    // which represent column and line.
    virtual long XYToPosition( long lX
                              ,long lY
                             ) const;
    virtual bool PositionToXY( long  lPos
                              ,long* plX
                              ,long* plY
                             ) const;

    virtual void ShowPosition(long lPos);

    // Clipboard operations
    virtual void Copy(void);
    virtual void Cut(void);
    virtual void Paste(void);

    virtual bool CanCopy(void) const;
    virtual bool CanCut(void) const;
    virtual bool CanPaste(void) const;

    // Undo/redo
    virtual void Undo(void);
    virtual void Redo(void);

    virtual bool CanUndo(void) const;
    virtual bool CanRedo(void) const;

    // Insertion point
    virtual void SetInsertionPoint(long lPos);
    virtual void SetInsertionPointEnd(void);
    virtual long GetInsertionPoint(void) const;
    virtual long GetLastPosition(void) const;

    virtual void SetSelection( long lFrom
                              ,long lTo
                             );
    virtual void SetEditable(bool bEditable);

    //
    // Implementation from now on
    // --------------------------
    //
    virtual void Command(wxCommandEvent& rEvent);
    virtual bool OS2Command( WXUINT uParam
                            ,WXWORD wId
                           );

    virtual WXHBRUSH OnCtlColor( WXHDC    hDC
                                ,WXHWND   pWnd
                                ,WXUINT   nCtlColor
                                ,WXUINT   message
                                ,WXWPARAM wParam
                                ,WXLPARAM lParam
                               );

    virtual bool SetBackgroundColour(const wxColour& colour);
    virtual bool SetForegroundColour(const wxColour& colour);

    virtual void AdoptAttributesFromHWND(void);
    virtual void SetupColours(void);

    virtual bool AcceptsFocus(void) const;

    // callbacks
    void OnDropFiles(wxDropFilesEvent& rEvent);
    void OnChar(wxKeyEvent& rEvent); // Process 'enter' if required

    void OnCut(wxCommandEvent& rEvent);
    void OnCopy(wxCommandEvent& rEvent);
    void OnPaste(wxCommandEvent& rEvent);
    void OnUndo(wxCommandEvent& rEvent);
    void OnRedo(wxCommandEvent& rEvent);

    void OnUpdateCut(wxUpdateUIEvent& rEvent);
    void OnUpdateCopy(wxUpdateUIEvent& rEvent);
    void OnUpdatePaste(wxUpdateUIEvent& rEvent);
    void OnUpdateUndo(wxUpdateUIEvent& rEvent);
    void OnUpdateRedo(wxUpdateUIEvent& rEvent);

    inline bool IsMLE(void) {return m_bIsMLE;}
    inline void SetMLE(bool bIsMLE) {m_bIsMLE = bIsMLE;}

protected:
    //
    // call this to increase the size limit (will do nothing if the current
    // limit is big enough)
    //
    void           AdjustSpaceLimit(void);
    virtual wxSize DoGetBestSize(void) const;

private:
    bool                            m_bIsMLE;
    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(wxTextCtrl)
}; // end of CLASS wxTextCtrl

#endif
    // _WX_TEXTCTRL_H_
