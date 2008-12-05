///////////////////////////////////////////////////////////////////////////////
// Name:        wx/generic/headerctrlg.h
// Purpose:     Generic wxHeaderCtrl implementation
// Author:      Vadim Zeitlin
// Created:     2008-12-01
// RCS-ID:      $Id$
// Copyright:   (c) 2008 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_GENERIC_HEADERCTRLG_H_
#define _WX_GENERIC_HEADERCTRLG_H_

#include "wx/event.h"
#include "wx/vector.h"

// ----------------------------------------------------------------------------
// wxHeaderCtrl
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_CORE wxHeaderCtrl : public wxHeaderCtrlBase
{
public:
    wxHeaderCtrl()
    {
        Init();
    }

    wxHeaderCtrl(wxWindow *parent,
                 wxWindowID id = wxID_ANY,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = wxHD_DEFAULT_STYLE,
                 const wxString& name = wxHeaderCtrlNameStr)
    {
        Init();

        Create(parent, id, pos, size, style, name);
    }

    bool Create(wxWindow *parent,
                wxWindowID id = wxID_ANY,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxHD_DEFAULT_STYLE,
                const wxString& name = wxHeaderCtrlNameStr);

    virtual ~wxHeaderCtrl();

private:
    // implement base class pure virtuals
    virtual unsigned int DoGetCount() const;
    virtual void DoInsert(const wxHeaderColumn& col, unsigned int idx);
    virtual void DoDelete(unsigned int idx);
    virtual void DoShowColumn(unsigned int idx, bool show);
    virtual void DoShowSortIndicator(unsigned int idx, int sortOrder);
    virtual void DoScrollHorz(int dx);

    // override wxWindow methods which must be implemented by a new control
    virtual wxSize DoGetBestSize() const;

    // common part of all ctors
    void Init();

    // event handlers
    void OnPaint(wxPaintEvent& event);
    void OnMouse(wxMouseEvent& event);

    // return the horizontal start position of the given column
    int GetColStart(unsigned int idx) const;

    // refresh the given column [only]
    void RefreshCol(unsigned int idx);

    // refresh all the controls starting from (and including) the given one
    void RefreshColsAfter(unsigned int idx);

    // all our current columns
    typedef wxVector<wxHeaderColumn> Columns;
    Columns m_cols;

    // sorting indicators for the columns: our API is such that it allows using
    // multiple columns for sorting, and even if this is not used anywhere in
    // practice right now, still support this
    //
    // the values are interpreted in the same way as ShowSortIndicator()
    // sortOrder parameter: true/false for ascending/descending sort if the
    // corresponding column is used for sorting or -1 otherwise
    wxVector<int> m_sortOrders;

    // index of the column under mouse or -1 if none
    unsigned int m_hover;

    // the horizontal scroll offset
    int m_scrollOffset;


    DECLARE_EVENT_TABLE()
    DECLARE_NO_COPY_CLASS(wxHeaderCtrl)
};

#endif // _WX_GENERIC_HEADERCTRLG_H_

