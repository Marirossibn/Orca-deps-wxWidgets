/////////////////////////////////////////////////////////////////////////////
// Name:        src/generic/datavgen.cpp
// Purpose:     wxDataViewCtrl generic implementation
// Author:      Robert Roebling
// Id:          $Id$
// Copyright:   (c) 1998 Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_DATAVIEWCTRL

#include "wx/dataview.h"

#ifdef wxUSE_GENERICDATAVIEWCTRL

#ifndef WX_PRECOMP
    #ifdef __WXMSW__
        #include "wx/msw/wrapwin.h"
    #endif
    #include "wx/sizer.h"
    #include "wx/log.h"
    #include "wx/dcclient.h"
    #include "wx/timer.h"
    #include "wx/settings.h"
#endif

#include "wx/stockitem.h"
#include "wx/calctrl.h"
#include "wx/popupwin.h"
#include "wx/renderer.h"

//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------

class wxDataViewCtrl;

//-----------------------------------------------------------------------------
// wxDataViewHeaderWindow
//-----------------------------------------------------------------------------

class wxDataViewHeaderWindow: public wxWindow
{
public:
    wxDataViewHeaderWindow( wxDataViewCtrl *parent,
                            wxWindowID id,
                            const wxPoint &pos = wxDefaultPosition,
                            const wxSize &size = wxDefaultSize,
                            const wxString &name = wxT("wxdataviewctrlheaderwindow") );
    virtual ~wxDataViewHeaderWindow();

    void SetOwner( wxDataViewCtrl* owner ) { m_owner = owner; }
    wxDataViewCtrl *GetOwner() { return m_owner; }

    void OnPaint( wxPaintEvent &event );
    void OnMouse( wxMouseEvent &event );
    void OnSetFocus( wxFocusEvent &event );

private:
    wxDataViewCtrl      *m_owner;
    wxCursor            *m_resizeCursor;

private:
    DECLARE_DYNAMIC_CLASS(wxDataViewHeaderWindow)
    DECLARE_EVENT_TABLE()
};

//-----------------------------------------------------------------------------
// wxDataViewRenameTimer
//-----------------------------------------------------------------------------

class wxDataViewRenameTimer: public wxTimer
{
private:
    wxDataViewMainWindow *m_owner;

public:
    wxDataViewRenameTimer( wxDataViewMainWindow *owner );
    void Notify();
};

//-----------------------------------------------------------------------------
// wxDataViewTextCtrlWrapper: wraps a wxTextCtrl for inline editing
//-----------------------------------------------------------------------------

class wxDataViewTextCtrlWrapper : public wxEvtHandler
{
public:
    // NB: text must be a valid object but not Create()d yet
    wxDataViewTextCtrlWrapper( wxDataViewMainWindow *owner,
                               wxTextCtrl *text,
                               wxDataViewListModel *model,
                               size_t col, size_t row,
                               wxRect cellLabel );

    wxTextCtrl *GetText() const { return m_text; }

    void AcceptChangesAndFinish();

protected:
    void OnChar( wxKeyEvent &event );
    void OnKeyUp( wxKeyEvent &event );
    void OnKillFocus( wxFocusEvent &event );

    bool AcceptChanges();
    void Finish();

private:
    wxDataViewMainWindow   *m_owner;
    wxTextCtrl             *m_text;
    wxString                m_startValue;
    wxDataViewListModel    *m_model;
    size_t                  m_col;
    size_t                  m_row;
    bool                    m_finished;
    bool                    m_aboutToFinish;

    DECLARE_EVENT_TABLE()
};

//-----------------------------------------------------------------------------
// wxDataViewMainWindow
//-----------------------------------------------------------------------------

WX_DEFINE_SORTED_ARRAY_SIZE_T( size_t, wxDataViewSelection );

class wxDataViewMainWindow: public wxWindow
{
public:
    wxDataViewMainWindow( wxDataViewCtrl *parent,
                            wxWindowID id,
                            const wxPoint &pos = wxDefaultPosition,
                            const wxSize &size = wxDefaultSize,
                            const wxString &name = wxT("wxdataviewctrlmainwindow") );
    virtual ~wxDataViewMainWindow();

    // notifications from wxDataViewListModel
    bool RowAppended();
    bool RowPrepended();
    bool RowInserted( size_t before );
    bool RowDeleted( size_t row );
    bool RowChanged( size_t row );
    bool ValueChanged( size_t col, size_t row );
    bool RowsReordered( size_t *new_order );
    bool Cleared();

    void SetOwner( wxDataViewCtrl* owner ) { m_owner = owner; }
    wxDataViewCtrl *GetOwner() { return m_owner; }

    void OnPaint( wxPaintEvent &event );
    void OnArrowChar(size_t newCurrent, const wxKeyEvent& event);
    void OnChar( wxKeyEvent &event );
    void OnMouse( wxMouseEvent &event );
    void OnSetFocus( wxFocusEvent &event );
    void OnKillFocus( wxFocusEvent &event );

    void UpdateDisplay();
    void RecalculateDisplay();
    void OnInternalIdle();

    void OnRenameTimer();
    void FinishEditing( wxTextCtrl *text );

    void ScrollWindow( int dx, int dy, const wxRect *rect );

    bool HasCurrentRow() { return m_currentRow != (size_t)-1; }
    void ChangeCurrentRow( size_t row );

    bool IsSingleSel() const { return !GetParent()->HasFlag(wxDV_MULTIPLE); };
    bool IsEmpty() { return GetRowCount() == 0; }

    int GetCountPerPage();
    int GetEndOfLastCol();
    size_t GetFirstVisibleRow();
    size_t GetLastVisibleRow();
    size_t GetRowCount();

    void SelectAllRows( bool on );
    void SelectRow( size_t row, bool on );
    void SelectRows( size_t from, size_t to, bool on );
    void ReverseRowSelection( size_t row );
    bool IsRowSelected( size_t row );

    void RefreshRow( size_t row );
    void RefreshRows( size_t from, size_t to );
    void RefreshRowsAfter( size_t firstRow );

private:
    wxDataViewCtrl             *m_owner;
    int                         m_lineHeight;
    bool                        m_dirty;

    wxDataViewColumn           *m_currentCol;
    size_t                      m_currentRow;
    wxDataViewSelection         m_selection;

    wxDataViewRenameTimer      *m_renameTimer;
    wxDataViewTextCtrlWrapper  *m_textctrlWrapper;
    bool                        m_lastOnSame;

    bool                        m_hasFocus;

    int                         m_dragCount;
    wxPoint                     m_dragStart;

    // for double click logic
    size_t m_lineLastClicked,
           m_lineBeforeLastClicked,
           m_lineSelectSingleOnUp;

private:
    DECLARE_DYNAMIC_CLASS(wxDataViewMainWindow)
    DECLARE_EVENT_TABLE()
};

// ---------------------------------------------------------
// wxGenericDataViewListModelNotifier
// ---------------------------------------------------------

class wxGenericDataViewListModelNotifier: public wxDataViewListModelNotifier
{
public:
    wxGenericDataViewListModelNotifier( wxDataViewMainWindow *mainWindow )
        { m_mainWindow = mainWindow; }

    virtual bool RowAppended()
        { return m_mainWindow->RowAppended(); }
    virtual bool RowPrepended()
        { return m_mainWindow->RowPrepended(); }
    virtual bool RowInserted( size_t before )
        { return m_mainWindow->RowInserted( before ); }
    virtual bool RowDeleted( size_t row )
        { return m_mainWindow->RowDeleted( row ); }
    virtual bool RowChanged( size_t row )
        { return m_mainWindow->RowChanged( row ); }
    virtual bool ValueChanged( size_t col, size_t row )
        { return m_mainWindow->ValueChanged( col, row ); }
    virtual bool RowsReordered( size_t *new_order )
        { return m_mainWindow->RowsReordered( new_order ); }
    virtual bool Cleared()
        { return m_mainWindow->Cleared(); }

    wxDataViewMainWindow    *m_mainWindow;
};

// ---------------------------------------------------------
// wxDataViewCell
// ---------------------------------------------------------

IMPLEMENT_ABSTRACT_CLASS(wxDataViewCell, wxDataViewCellBase)

wxDataViewCell::wxDataViewCell( const wxString &varianttype, wxDataViewCellMode mode ) :
    wxDataViewCellBase( varianttype, mode )
{
    m_dc = NULL;
}

wxDataViewCell::~wxDataViewCell()
{
    if (m_dc)
        delete m_dc;
}

wxDC *wxDataViewCell::GetDC()
{
    if (m_dc == NULL)
    {
        if (GetOwner() == NULL)
            return NULL;
        if (GetOwner()->GetOwner() == NULL)
            return NULL;
        m_dc = new wxClientDC( GetOwner()->GetOwner() );
    }

    return m_dc;
}

// ---------------------------------------------------------
// wxDataViewCustomCell
// ---------------------------------------------------------

IMPLEMENT_ABSTRACT_CLASS(wxDataViewCustomCell, wxDataViewCell)

wxDataViewCustomCell::wxDataViewCustomCell( const wxString &varianttype,
                          wxDataViewCellMode mode ) :
    wxDataViewCell( varianttype, mode )
{
}

// ---------------------------------------------------------
// wxDataViewTextCell
// ---------------------------------------------------------

IMPLEMENT_ABSTRACT_CLASS(wxDataViewTextCell, wxDataViewCustomCell)

wxDataViewTextCell::wxDataViewTextCell( const wxString &varianttype, wxDataViewCellMode mode ) :
    wxDataViewCustomCell( varianttype, mode )
{
}

bool wxDataViewTextCell::SetValue( const wxVariant &value )
{
    m_text = value.GetString();

    return true;
}

bool wxDataViewTextCell::GetValue( wxVariant& WXUNUSED(value) )
{
    return false;
}

bool wxDataViewTextCell::Render( wxRect cell, wxDC *dc, int WXUNUSED(state) )
{
    dc->DrawText( m_text, cell.x, cell.y );

    return true;
}

wxSize wxDataViewTextCell::GetSize()
{
    return wxSize(80,20);
}

// ---------------------------------------------------------
// wxDataViewToggleCell
// ---------------------------------------------------------

IMPLEMENT_ABSTRACT_CLASS(wxDataViewToggleCell, wxDataViewCustomCell)

wxDataViewToggleCell::wxDataViewToggleCell( const wxString &varianttype,
                        wxDataViewCellMode mode ) :
    wxDataViewCustomCell( varianttype, mode )
{
    m_toggle = false;
}

bool wxDataViewToggleCell::SetValue( const wxVariant &value )
{
    m_toggle = value.GetBool();

    return true;
}

bool wxDataViewToggleCell::GetValue( wxVariant &WXUNUSED(value) )
{
    return false;
}

bool wxDataViewToggleCell::Render( wxRect cell, wxDC *dc, int WXUNUSED(state) )
{
    // User wxRenderer here

    wxRect rect;
    rect.x = cell.x + cell.width/2 - 10;
    rect.width = 20;
    rect.y = cell.y + cell.height/2 - 10;
    rect.height = 20;

    int flags = 0;
    if (m_toggle)
        flags |= wxCONTROL_CHECKED;
    if (GetMode() != wxDATAVIEW_CELL_ACTIVATABLE)
        flags |= wxCONTROL_DISABLED;

    wxRendererNative::Get().DrawCheckBox(
            GetOwner()->GetOwner(),
            *dc,
            rect,
            flags );

    return true;
}

bool wxDataViewToggleCell::Activate( wxRect WXUNUSED(cell), wxDataViewListModel *model, size_t col, size_t row )
{
    bool value = !m_toggle;
    wxVariant variant = value;
    model->SetValue( variant, col, row );
    model->ValueChanged( col, row );
    return true;
}

wxSize wxDataViewToggleCell::GetSize()
{
    return wxSize(20,20);
}

// ---------------------------------------------------------
// wxDataViewProgressCell
// ---------------------------------------------------------

IMPLEMENT_ABSTRACT_CLASS(wxDataViewProgressCell, wxDataViewCustomCell)

wxDataViewProgressCell::wxDataViewProgressCell( const wxString &label,
    const wxString &varianttype, wxDataViewCellMode mode ) :
    wxDataViewCustomCell( varianttype, mode )
{
    m_label = label;
    m_value = 0;
}

wxDataViewProgressCell::~wxDataViewProgressCell()
{
}

bool wxDataViewProgressCell::SetValue( const wxVariant &value )
{
    m_value = (long) value;

    if (m_value < 0) m_value = 0;
    if (m_value > 100) m_value = 100;

    return true;
}

bool wxDataViewProgressCell::Render( wxRect cell, wxDC *dc, int WXUNUSED(state) )
{
    double pct = (double)m_value / 100.0;
    wxRect bar = cell;
    bar.width = (int)(cell.width * pct);
    dc->SetPen( *wxTRANSPARENT_PEN );
    dc->SetBrush( *wxBLUE_BRUSH );
    dc->DrawRectangle( bar );

    dc->SetBrush( *wxTRANSPARENT_BRUSH );
    dc->SetPen( *wxBLACK_PEN );
    dc->DrawRectangle( cell );

    return true;
}

wxSize wxDataViewProgressCell::GetSize()
{
    return wxSize(40,12);
}

// ---------------------------------------------------------
// wxDataViewDateCell
// ---------------------------------------------------------

class wxDataViewDateCellPopupTransient: public wxPopupTransientWindow
{
public:
    wxDataViewDateCellPopupTransient( wxWindow* parent, wxDateTime *value,
        wxDataViewListModel *model, size_t col, size_t row ) :
        wxPopupTransientWindow( parent, wxBORDER_SIMPLE )
    {
        m_model = model;
        m_col = col;
        m_row = row;
        m_cal = new wxCalendarCtrl( this, wxID_ANY, *value );
        wxBoxSizer *sizer = new wxBoxSizer( wxHORIZONTAL );
        sizer->Add( m_cal, 1, wxGROW );
        SetSizer( sizer );
        sizer->Fit( this );
    }

    void OnCalendar( wxCalendarEvent &event );

    wxCalendarCtrl      *m_cal;
    wxDataViewListModel *m_model;
    size_t               m_col;
    size_t               m_row;

protected:
    virtual void OnDismiss()
    {
    }

private:
    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(wxDataViewDateCellPopupTransient,wxPopupTransientWindow)
    EVT_CALENDAR( wxID_ANY, wxDataViewDateCellPopupTransient::OnCalendar )
END_EVENT_TABLE()

void wxDataViewDateCellPopupTransient::OnCalendar( wxCalendarEvent &event )
{
    wxDateTime date = event.GetDate();
    wxVariant value = date;
    m_model->SetValue( value, m_col, m_row );
    m_model->ValueChanged( m_col, m_row );
    DismissAndNotify();
}

IMPLEMENT_ABSTRACT_CLASS(wxDataViewDateCell, wxDataViewCustomCell)

wxDataViewDateCell::wxDataViewDateCell( const wxString &varianttype,
                        wxDataViewCellMode mode ) :
    wxDataViewCustomCell( varianttype, mode )
{
}

bool wxDataViewDateCell::SetValue( const wxVariant &value )
{
    m_date = value.GetDateTime();

    return true;
}

bool wxDataViewDateCell::Render( wxRect cell, wxDC *dc, int WXUNUSED(state) )
{
    dc->SetFont( GetOwner()->GetOwner()->GetFont() );
    wxString tmp = m_date.FormatDate();
    dc->DrawText( tmp, cell.x, cell.y );

    return true;
}

wxSize wxDataViewDateCell::GetSize()
{
    wxDataViewCtrl* view = GetOwner()->GetOwner();
    wxString tmp = m_date.FormatDate();
    wxCoord x,y,d;
    view->GetTextExtent( tmp, &x, &y, &d );
    return wxSize(x,y+d);
}

bool wxDataViewDateCell::Activate( wxRect WXUNUSED(cell), wxDataViewListModel *model, size_t col, size_t row )
{
    wxVariant variant;
    model->GetValue( variant, col, row );
    wxDateTime value = variant.GetDateTime();

    wxDataViewDateCellPopupTransient *popup = new wxDataViewDateCellPopupTransient(
        GetOwner()->GetOwner()->GetParent(), &value, model, col, row );
    wxPoint pos = wxGetMousePosition();
    popup->Move( pos );
    popup->Layout();
    popup->Popup( popup->m_cal );

    return true;
}

// ---------------------------------------------------------
// wxDataViewColumn
// ---------------------------------------------------------

IMPLEMENT_ABSTRACT_CLASS(wxDataViewColumn, wxDataViewColumnBase)

wxDataViewColumn::wxDataViewColumn( const wxString &title, wxDataViewCell *cell, size_t model_column,
        int fixed_width, wxDataViewColumnSizing sizing, int flags ) :
    wxDataViewColumnBase( title, cell, model_column, flags )
{
    m_sizing = sizing;

    m_width = fixed_width;
    m_fixedWidth = fixed_width;
}

wxDataViewColumn::~wxDataViewColumn()
{
}

void wxDataViewColumn::SetTitle( const wxString &title )
{
    wxDataViewColumnBase::SetTitle( title );

}

int wxDataViewColumn::GetWidth()
{
    return m_width;
}

void wxDataViewColumn::SetFixedWidth( int width )
{
    m_fixedWidth = width;

    if (m_sizing == wxDATAVIEW_COL_WIDTH_FIXED)
    {
        m_width = width;
        // Set dirty
    }
}

int wxDataViewColumn::GetFixedWidth()
{
    return m_fixedWidth;
}

//-----------------------------------------------------------------------------
// wxDataViewHeaderWindow
//-----------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_CLASS(wxDataViewHeaderWindow, wxWindow)

BEGIN_EVENT_TABLE(wxDataViewHeaderWindow,wxWindow)
    EVT_PAINT         (wxDataViewHeaderWindow::OnPaint)
    EVT_MOUSE_EVENTS  (wxDataViewHeaderWindow::OnMouse)
    EVT_SET_FOCUS     (wxDataViewHeaderWindow::OnSetFocus)
END_EVENT_TABLE()

wxDataViewHeaderWindow::wxDataViewHeaderWindow( wxDataViewCtrl *parent, wxWindowID id,
     const wxPoint &pos, const wxSize &size, const wxString &name ) :
    wxWindow( parent, id, pos, size, 0, name )
{
    SetOwner( parent );

    m_resizeCursor = new wxCursor( wxCURSOR_SIZEWE );

    wxVisualAttributes attr = wxPanel::GetClassDefaultAttributes();
    SetOwnForegroundColour( attr.colFg );
    SetOwnBackgroundColour( attr.colBg );
    if (!m_hasFont)
        SetOwnFont( attr.font );
}

wxDataViewHeaderWindow::~wxDataViewHeaderWindow()
{
    delete m_resizeCursor;
}

void wxDataViewHeaderWindow::OnPaint( wxPaintEvent &WXUNUSED(event) )
{
    int w, h;
    GetClientSize( &w, &h );

    wxPaintDC dc( this );

    int xpix;
    m_owner->GetScrollPixelsPerUnit( &xpix, NULL );

    int x;
    m_owner->GetViewStart( &x, NULL );

    // account for the horz scrollbar offset
    dc.SetDeviceOrigin( -x * xpix, 0 );

    dc.SetFont( GetFont() );

    size_t cols = GetOwner()->GetNumberOfColumns();
    size_t i;
    int xpos = 0;
    for (i = 0; i < cols; i++)
    {
        wxDataViewColumn *col = GetOwner()->GetColumn( i );
        int width = col->GetWidth();

        int cw = width;
        int ch = h;

        wxRendererNative::Get().DrawHeaderButton
                                (
                                    this,
                                    dc,
                                    wxRect(xpos, 0, cw, ch-1),
                                    m_parent->IsEnabled() ? 0
                                                          : (int)wxCONTROL_DISABLED
                                );

        dc.DrawText( col->GetTitle(), xpos+3, 3 );

        xpos += width;
    }
}

void wxDataViewHeaderWindow::OnMouse( wxMouseEvent &WXUNUSED(event) )
{
}

void wxDataViewHeaderWindow::OnSetFocus( wxFocusEvent &event )
{
    GetParent()->SetFocus();
    event.Skip();
}

//-----------------------------------------------------------------------------
// wxDataViewRenameTimer
//-----------------------------------------------------------------------------

wxDataViewRenameTimer::wxDataViewRenameTimer( wxDataViewMainWindow *owner )
{
    m_owner = owner;
}

void wxDataViewRenameTimer::Notify()
{
    m_owner->OnRenameTimer();
}

//-----------------------------------------------------------------------------
// wxDataViewTextCtrlWrapper: wraps a wxTextCtrl for inline editing
//-----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(wxDataViewTextCtrlWrapper, wxEvtHandler)
    EVT_CHAR           (wxDataViewTextCtrlWrapper::OnChar)
    EVT_KEY_UP         (wxDataViewTextCtrlWrapper::OnKeyUp)
    EVT_KILL_FOCUS     (wxDataViewTextCtrlWrapper::OnKillFocus)
END_EVENT_TABLE()

wxDataViewTextCtrlWrapper::wxDataViewTextCtrlWrapper(
                        wxDataViewMainWindow *owner,
                        wxTextCtrl *text,
                        wxDataViewListModel *model,
                        size_t col, size_t row,
                        wxRect rectLabel )
{
    m_owner = owner;
    m_model = model;
    m_row = row;
    m_col = col;
    m_text = text;

    m_finished = false;
    m_aboutToFinish = false;

    wxVariant value;
    model->GetValue( value, col, row );
    m_startValue = value.GetString();

    m_owner->GetOwner()->CalcScrolledPosition(
        rectLabel.x, rectLabel.y, &rectLabel.x, &rectLabel.y );

    m_text->Create( owner, wxID_ANY, m_startValue,
                    wxPoint(rectLabel.x-2,rectLabel.y-2),
                    wxSize(rectLabel.width+7,rectLabel.height+4) );
    m_text->SetFocus();

    m_text->PushEventHandler(this);
}

void wxDataViewTextCtrlWrapper::AcceptChangesAndFinish()
{
    m_aboutToFinish = true;

    // Notify the owner about the changes
    AcceptChanges();

    // Even if vetoed, close the control (consistent with MSW)
    Finish();
}

void wxDataViewTextCtrlWrapper::OnChar( wxKeyEvent &event )
{
    switch ( event.m_keyCode )
    {
        case WXK_RETURN:
            AcceptChangesAndFinish();
            break;

        case WXK_ESCAPE:
            // m_owner->OnRenameCancelled( m_itemEdited );
            Finish();
            break;

        default:
            event.Skip();
    }
}

void wxDataViewTextCtrlWrapper::OnKeyUp( wxKeyEvent &event )
{
    if (m_finished)
    {
        event.Skip();
        return;
    }

    // auto-grow the textctrl
    wxSize parentSize = m_owner->GetSize();
    wxPoint myPos = m_text->GetPosition();
    wxSize mySize = m_text->GetSize();
    int sx, sy;
    m_text->GetTextExtent(m_text->GetValue() + _T("MM"), &sx, &sy);
    if (myPos.x + sx > parentSize.x)
        sx = parentSize.x - myPos.x;
    if (mySize.x > sx)
        sx = mySize.x;
    m_text->SetSize(sx, wxDefaultCoord);

    event.Skip();
}

void wxDataViewTextCtrlWrapper::OnKillFocus( wxFocusEvent &event )
{
    if ( !m_finished && !m_aboutToFinish )
    {
        AcceptChanges();
        //if ( !AcceptChanges() )
        //    m_owner->OnRenameCancelled( m_itemEdited );

        Finish();
    }

    // We must let the native text control handle focus
    event.Skip();
}

bool wxDataViewTextCtrlWrapper::AcceptChanges()
{
    const wxString value = m_text->GetValue();

    if ( value == m_startValue )
        // nothing changed, always accept
        return true;

//    if ( !m_owner->OnRenameAccept(m_itemEdited, value) )
        // vetoed by the user
//        return false;

    // accepted, do rename the item
    wxVariant variant;
    variant = value;
    m_model->SetValue( variant, m_col, m_row );
    m_model->ValueChanged( m_col, m_row );

    return true;
}

void wxDataViewTextCtrlWrapper::Finish()
{
    if ( !m_finished )
    {
        m_finished = true;

        m_text->RemoveEventHandler(this);
        m_owner->FinishEditing(m_text);

        // delete later
        wxPendingDelete.Append( this );
    }
}

//-----------------------------------------------------------------------------
// wxDataViewMainWindow
//-----------------------------------------------------------------------------

int LINKAGEMODE wxDataViewSelectionCmp( size_t row1, size_t row2 )
{
    if (row1 > row2) return 1;
    if (row1 == row2) return 0;
    return -1;
}


IMPLEMENT_ABSTRACT_CLASS(wxDataViewMainWindow, wxWindow)

BEGIN_EVENT_TABLE(wxDataViewMainWindow,wxWindow)
    EVT_PAINT         (wxDataViewMainWindow::OnPaint)
    EVT_MOUSE_EVENTS  (wxDataViewMainWindow::OnMouse)
    EVT_SET_FOCUS     (wxDataViewMainWindow::OnSetFocus)
    EVT_KILL_FOCUS    (wxDataViewMainWindow::OnKillFocus)
    EVT_CHAR          (wxDataViewMainWindow::OnChar)
END_EVENT_TABLE()

wxDataViewMainWindow::wxDataViewMainWindow( wxDataViewCtrl *parent, wxWindowID id,
    const wxPoint &pos, const wxSize &size, const wxString &name ) :
    wxWindow( parent, id, pos, size, wxWANTS_CHARS, name ),
    m_selection( wxDataViewSelectionCmp )

{
    SetOwner( parent );

    m_lastOnSame = false;
    m_renameTimer = new wxDataViewRenameTimer( this );
    m_textctrlWrapper = NULL;

    // TODO: user better initial values/nothing selected
    m_currentCol = NULL;
    m_currentRow = 0;

    // TODO: we need to calculate this smartly
    m_lineHeight = 20;

    m_dragCount = 0;
    m_dragStart = wxPoint(0,0);
    m_lineLastClicked = (size_t) -1;
    m_lineBeforeLastClicked = (size_t) -1;
    m_lineSelectSingleOnUp = (size_t) -1;

    m_hasFocus = false;

    SetBackgroundColour( *wxWHITE );

    UpdateDisplay();
}

wxDataViewMainWindow::~wxDataViewMainWindow()
{
    delete m_renameTimer;
}

void wxDataViewMainWindow::OnRenameTimer()
{
    // We have to call this here because changes may just have
    // been made and no screen update taken place.
    if ( m_dirty )
        wxSafeYield();


    int xpos = 0;
    size_t cols = GetOwner()->GetNumberOfColumns();
    size_t i;
    for (i = 0; i < cols; i++)
    {
        wxDataViewColumn *c = GetOwner()->GetColumn( i );
        if (c == m_currentCol)
            break;
        xpos += c->GetWidth();
    }
    wxRect labelRect( xpos, m_currentRow * m_lineHeight, m_currentCol->GetWidth(), m_lineHeight );

    wxClassInfo *textControlClass = CLASSINFO(wxTextCtrl);

    wxTextCtrl * const text = (wxTextCtrl *)textControlClass->CreateObject();
    m_textctrlWrapper = new wxDataViewTextCtrlWrapper(this, text, GetOwner()->GetModel(),
        m_currentCol->GetModelColumn(), m_currentRow, labelRect );
}

void wxDataViewMainWindow::FinishEditing( wxTextCtrl *text )
{
    delete text;
    m_textctrlWrapper = NULL;
    SetFocus();
  //  SetFocusIgnoringChildren();
}

bool wxDataViewMainWindow::RowAppended()
{
    return false;
}

bool wxDataViewMainWindow::RowPrepended()
{
    return false;
}

bool wxDataViewMainWindow::RowInserted( size_t WXUNUSED(before) )
{
    return false;
}

bool wxDataViewMainWindow::RowDeleted( size_t WXUNUSED(row) )
{
    return false;
}

bool wxDataViewMainWindow::RowChanged( size_t WXUNUSED(row) )
{
    return false;
}

bool wxDataViewMainWindow::ValueChanged( size_t WXUNUSED(col), size_t row )
{
    wxRect rect( 0, row*m_lineHeight, 10000, m_lineHeight );
    m_owner->CalcScrolledPosition( rect.x, rect.y, &rect.x, &rect.y );
    Refresh( true, &rect );

    return true;
}

bool wxDataViewMainWindow::RowsReordered( size_t *WXUNUSED(new_order) )
{
    Refresh();

    return true;
}

bool wxDataViewMainWindow::Cleared()
{
    return false;
}

void wxDataViewMainWindow::UpdateDisplay()
{
    m_dirty = true;
}

void wxDataViewMainWindow::OnInternalIdle()
{
    wxWindow::OnInternalIdle();

    if (m_dirty)
    {
        RecalculateDisplay();
        m_dirty = false;
    }
}

void wxDataViewMainWindow::RecalculateDisplay()
{
    wxDataViewListModel *model = GetOwner()->GetModel();
    if (!model)
    {
        Refresh();
        return;
    }

    int width = 0;
    size_t cols = GetOwner()->GetNumberOfColumns();
    size_t i;
    for (i = 0; i < cols; i++)
    {
        wxDataViewColumn *col = GetOwner()->GetColumn( i );
        width += col->GetWidth();
    }

    int height = model->GetNumberOfRows() * m_lineHeight;

    SetVirtualSize( width, height );
    GetOwner()->SetScrollRate( 10, m_lineHeight );

    Refresh();
}

void wxDataViewMainWindow::ScrollWindow( int dx, int dy, const wxRect *rect )
{
    wxWindow::ScrollWindow( dx, dy, rect );
    GetOwner()->m_headerArea->ScrollWindow( dx, 0 );
}

void wxDataViewMainWindow::OnPaint( wxPaintEvent &WXUNUSED(event) )
{
    wxPaintDC dc( this );

    GetOwner()->PrepareDC( dc );

    dc.SetFont( GetFont() );

    wxRect update = GetUpdateRegion().GetBox();
    m_owner->CalcUnscrolledPosition( update.x, update.y, &update.x, &update.y );

    wxDataViewListModel *model = GetOwner()->GetModel();

    size_t item_start = wxMax( 0, (update.y / m_lineHeight) );
    size_t item_count = wxMin( (int)(((update.y + update.height) / m_lineHeight) - item_start + 1),
                               (int)(model->GetNumberOfRows()-item_start) );



    size_t item;
    for (item = item_start; item < item_start+item_count; item++)
    {
        if (m_selection.Index( item ) != wxNOT_FOUND)
        {
            int flags = wxCONTROL_SELECTED;
            if (item == m_currentRow)
                flags |= wxCONTROL_CURRENT;
            if (m_hasFocus)
                flags |= wxCONTROL_FOCUSED;
            wxRect rect( 0, item*m_lineHeight+1, GetEndOfLastCol(), m_lineHeight-2 );
            wxRendererNative::Get().DrawItemSelectionRect
                                (
                                    this,
                                    dc,
                                    rect,
                                    flags
                                );
        }
        else
        {
            if (item == m_currentRow)
            {
                int flags = wxCONTROL_CURRENT;
                if (m_hasFocus)
                    flags |= wxCONTROL_FOCUSED;  // should have no effect
                wxRect rect( 0, item*m_lineHeight+1, GetEndOfLastCol(), m_lineHeight-2 );
                wxRendererNative::Get().DrawItemSelectionRect
                                (
                                    this,
                                    dc,
                                    rect,
                                    flags
                                );

            }
        }
    }

    wxRect cell_rect;
    cell_rect.x = 0;
    cell_rect.height = m_lineHeight;
    size_t cols = GetOwner()->GetNumberOfColumns();
    size_t i;
    for (i = 0; i < cols; i++)
    {
        wxDataViewColumn *col = GetOwner()->GetColumn( i );
        wxDataViewCell *cell = col->GetCell();
        cell_rect.width = col->GetWidth();

        for (item = item_start; item < item_start+item_count; item++)
        {
            cell_rect.y = item*m_lineHeight;
            wxVariant value;
            model->GetValue( value, col->GetModelColumn(), item );
            cell->SetValue( value );
            wxSize size = cell->GetSize();
            // cannot be bigger than allocated space
            size.x = wxMin( size.x, cell_rect.width );
            size.y = wxMin( size.y, cell_rect.height );
            // TODO: check for left/right/centre alignment here
            wxRect item_rect;
            // for now: centre
            item_rect.x = cell_rect.x + (cell_rect.width / 2) - (size.x / 2);
            item_rect.y = cell_rect.y + (cell_rect.height / 2) - (size.y / 2);

            item_rect.width = size.x;
            item_rect.height= size.y;
            cell->Render( item_rect, &dc, 0 );
        }

        cell_rect.x += cell_rect.width;
    }
}

int wxDataViewMainWindow::GetCountPerPage()
{
    wxSize size = GetClientSize();
    return size.y / m_lineHeight;
}

int wxDataViewMainWindow::GetEndOfLastCol()
{
    int width = 0;
    size_t i;
    for (i = 0; i < GetOwner()->GetNumberOfColumns(); i++)
    {
        wxDataViewColumn *c = GetOwner()->GetColumn( i );
        width += c->GetWidth();
    }
    return width;
}

size_t wxDataViewMainWindow::GetFirstVisibleRow()
{
    int x = 0;
    int y = 0;
    m_owner->CalcUnscrolledPosition( x, y, &x, &y );

    return y / m_lineHeight;
}

size_t wxDataViewMainWindow::GetLastVisibleRow()
{
    wxSize client_size = GetClientSize();
    m_owner->CalcUnscrolledPosition( client_size.x, client_size.y, &client_size.x, &client_size.y );

    return wxMin( GetRowCount()-1, ((unsigned)client_size.y/m_lineHeight)+1 );
}

size_t wxDataViewMainWindow::GetRowCount()
{
    return GetOwner()->GetModel()->GetNumberOfRows();
}

void wxDataViewMainWindow::ChangeCurrentRow( size_t row )
{
    m_currentRow = row;

    // send event
}

void wxDataViewMainWindow::SelectAllRows( bool on )
{
    if (IsEmpty())
        return;

    if (on)
    {
        m_selection.Clear();
        for (size_t i = 0; i < GetRowCount(); i++)
            m_selection.Add( i );
        Refresh();
    }
    else
    {
        size_t first_visible = GetFirstVisibleRow();
        size_t last_visible = GetLastVisibleRow();
        size_t i;
        for (i = 0; i < m_selection.GetCount(); i++)
        {
            size_t row = m_selection[i];
            if ((row >= first_visible) && (row <= last_visible))
                RefreshRow( row );
        }
        m_selection.Clear();
    }
}

void wxDataViewMainWindow::SelectRow( size_t row, bool on )
{
    if (m_selection.Index( row ) == wxNOT_FOUND)
    {
        if (on)
        {
            m_selection.Add( row );
            RefreshRow( row );
        }
    }
    else
    {
        if (!on)
        {
            m_selection.Remove( row );
            RefreshRow( row );
        }
    }
}

void wxDataViewMainWindow::SelectRows( size_t from, size_t to, bool on )
{
    if (from > to)
    {
        size_t tmp = from;
        from = to;
        to = tmp;
    }

    size_t i;
    for (i = from; i <= to; i++)
    {
        if (m_selection.Index( i ) == wxNOT_FOUND)
        {
            if (on)
                m_selection.Add( i );
        }
        else
        {
            if (!on)
                m_selection.Remove( i );
        }
    }
    RefreshRows( from, to );
}

void wxDataViewMainWindow::ReverseRowSelection( size_t row )
{
    if (m_selection.Index( row ) == wxNOT_FOUND)
        m_selection.Add( row );
    else
        m_selection.Remove( row );
    RefreshRow( row );
}

bool wxDataViewMainWindow::IsRowSelected( size_t row )
{
    return (m_selection.Index( row ) != wxNOT_FOUND);
}

void wxDataViewMainWindow::RefreshRow( size_t row )
{
    wxRect rect( 0, row*m_lineHeight, GetEndOfLastCol(), m_lineHeight );
    m_owner->CalcScrolledPosition( rect.x, rect.y, &rect.x, &rect.y );

    wxSize client_size = GetClientSize();
    wxRect client_rect( 0, 0, client_size.x, client_size.y );
    wxRect intersect_rect = client_rect.Intersect( rect );
    if (intersect_rect.width > 0)
        Refresh( true, &intersect_rect );
}

void wxDataViewMainWindow::RefreshRows( size_t from, size_t to )
{
    if (from > to)
    {
        size_t tmp = to;
        to = from;
        from = tmp;
    }

    wxRect rect( 0, from*m_lineHeight, GetEndOfLastCol(), (to-from+1) * m_lineHeight );
    m_owner->CalcScrolledPosition( rect.x, rect.y, &rect.x, &rect.y );

    wxSize client_size = GetClientSize();
    wxRect client_rect( 0, 0, client_size.x, client_size.y );
    wxRect intersect_rect = client_rect.Intersect( rect );
    if (intersect_rect.width > 0)
        Refresh( true, &intersect_rect );
}

void wxDataViewMainWindow::RefreshRowsAfter( size_t firstRow )
{
    size_t count = GetRowCount();
    if (firstRow > count)
        return;

    wxRect rect( 0, firstRow*m_lineHeight, GetEndOfLastCol(), count * m_lineHeight );
    m_owner->CalcScrolledPosition( rect.x, rect.y, &rect.x, &rect.y );

    wxSize client_size = GetClientSize();
    wxRect client_rect( 0, 0, client_size.x, client_size.y );
    wxRect intersect_rect = client_rect.Intersect( rect );
    if (intersect_rect.width > 0)
        Refresh( true, &intersect_rect );
}

void wxDataViewMainWindow::OnArrowChar(size_t newCurrent, const wxKeyEvent& event)
{
    wxCHECK_RET( newCurrent < GetRowCount(),
                 _T("invalid item index in OnArrowChar()") );

    // if there is no selection, we cannot move it anywhere
    if (!HasCurrentRow())
        return;

    size_t oldCurrent = m_currentRow;

    // in single selection we just ignore Shift as we can't select several
    // items anyhow
    if ( event.ShiftDown() && !IsSingleSel() )
    {
        RefreshRow( oldCurrent );

        ChangeCurrentRow( newCurrent );

        // select all the items between the old and the new one
        if ( oldCurrent > newCurrent )
        {
            newCurrent = oldCurrent;
            oldCurrent = m_currentRow;
        }

        SelectRows( oldCurrent, newCurrent, true );
    }
    else // !shift
    {
        RefreshRow( oldCurrent );

        // all previously selected items are unselected unless ctrl is held
        if ( !event.ControlDown() )
            SelectAllRows(false);

        ChangeCurrentRow( newCurrent );

        if ( !event.ControlDown() )
            SelectRow( m_currentRow, true );
        else
            RefreshRow( m_currentRow );
    }

    // MoveToFocus();
}

void wxDataViewMainWindow::OnChar( wxKeyEvent &event )
{
    if (event.GetKeyCode() == WXK_TAB)
    {
        wxNavigationKeyEvent nevent;
        nevent.SetWindowChange( event.ControlDown() );
        nevent.SetDirection( !event.ShiftDown() );
        nevent.SetEventObject( GetParent()->GetParent() );
        nevent.SetCurrentFocus( m_parent );
        if (GetParent()->GetParent()->GetEventHandler()->ProcessEvent( nevent ))
            return;
    }

    // no item -> nothing to do
    if (!HasCurrentRow())
    {
        event.Skip();
        return;
    }

    // don't use m_linesPerPage directly as it might not be computed yet
    const int pageSize = GetCountPerPage();
    wxCHECK_RET( pageSize, _T("should have non zero page size") );

    switch ( event.GetKeyCode() )
    {
        case WXK_UP:
            if ( m_currentRow > 0 )
                OnArrowChar( m_currentRow - 1, event );
            break;

        case WXK_DOWN:
            if ( m_currentRow < GetRowCount() - 1 )
                OnArrowChar( m_currentRow + 1, event );
            break;

        case WXK_END:
            if (!IsEmpty())
                OnArrowChar( GetRowCount() - 1, event );
            break;

        case WXK_HOME:
            if (!IsEmpty())
                OnArrowChar( 0, event );
            break;

        case WXK_PAGEUP:
            {
                int steps = pageSize - 1;
                int index = m_currentRow - steps;
                if (index < 0)
                    index = 0;

                OnArrowChar( index, event );
            }
            break;

        case WXK_PAGEDOWN:
            {
                int steps = pageSize - 1;
                size_t index = m_currentRow + steps;
                size_t count = GetRowCount();
                if ( index >= count )
                    index = count - 1;

                OnArrowChar( index, event );
            }
            break;

        default:
            event.Skip();
    }
}

void wxDataViewMainWindow::OnMouse( wxMouseEvent &event )
{
    if (event.GetEventType() == wxEVT_MOUSEWHEEL)
    {
        // let the base handle mouse wheel events.
        event.Skip();
        return;
    }

    int x = event.GetX();
    int y = event.GetY();
    m_owner->CalcUnscrolledPosition( x, y, &x, &y );

    wxDataViewColumn *col = NULL;

    int xpos = 0;
    size_t cols = GetOwner()->GetNumberOfColumns();
    size_t i;
    for (i = 0; i < cols; i++)
    {
        wxDataViewColumn *c = GetOwner()->GetColumn( i );
        if (x < xpos + c->GetWidth())
        {
            col = c;
            break;
        }
        xpos += c->GetWidth();
    }
    if (!col)
        return;
    wxDataViewCell *cell = col->GetCell();

    size_t current = y / m_lineHeight;

    if ((current > GetRowCount()) || (x > GetEndOfLastCol()))
    {
        // Unselect all if below the last row ?
        return;
    }

    wxDataViewListModel *model = GetOwner()->GetModel();

    if (event.Dragging())
    {
        if (m_dragCount == 0)
        {
            // we have to report the raw, physical coords as we want to be
            // able to call HitTest(event.m_pointDrag) from the user code to
            // get the item being dragged
            m_dragStart = event.GetPosition();
        }

        m_dragCount++;

        if (m_dragCount != 3)
            return;

        if (event.LeftIsDown())
        {
            // Notify cell about drag
        }
        return;
    }
    else
    {
        m_dragCount = 0;
    }

    bool forceClick = false;

    if (event.ButtonDClick())
    {
        m_renameTimer->Stop();
        m_lastOnSame = false;
    }

    if (event.LeftDClick())
    {
        if ( current == m_lineLastClicked )
        {
            if (cell->GetMode() == wxDATAVIEW_CELL_ACTIVATABLE)
            {
                wxVariant value;
                model->GetValue( value, col->GetModelColumn(), current );
                cell->SetValue( value );
                wxRect cell_rect( xpos, current * m_lineHeight, col->GetWidth(), m_lineHeight );
                cell->Activate( cell_rect, model, col->GetModelColumn(), current );
            }
            return;
        }
        else
        {
            // The first click was on another item, so don't interpret this as
            // a double click, but as a simple click instead
            forceClick = true;
        }
    }

    if (event.LeftUp())
    {
        if (m_lineSelectSingleOnUp != (size_t)-1)
        {
            // select single line
            SelectAllRows( false );
            SelectRow( m_lineSelectSingleOnUp, true );
        }

        if (m_lastOnSame)
        {
            if ((col == m_currentCol) && (current == m_currentRow) &&
                (cell->GetMode() == wxDATAVIEW_CELL_EDITABLE) )
            {
                m_renameTimer->Start( 100, true );
            }
        }

        m_lastOnSame = false;
        m_lineSelectSingleOnUp = (size_t)-1;
    }
    else
    {
        // This is necessary, because after a DnD operation in
        // from and to ourself, the up event is swallowed by the
        // DnD code. So on next non-up event (which means here and
        // now) m_lineSelectSingleOnUp should be reset.
        m_lineSelectSingleOnUp = (size_t)-1;
    }

    if (event.RightDown())
    {
        m_lineBeforeLastClicked = m_lineLastClicked;
        m_lineLastClicked = current;

        // If the item is already selected, do not update the selection.
        // Multi-selections should not be cleared if a selected item is clicked.
        if (!IsRowSelected(current))
        {
            SelectAllRows(false);
            ChangeCurrentRow(current);
            SelectRow(m_currentRow,true);
        }

        // notify cell about right click
        // cell->...

        // Allow generation of context menu event
        event.Skip();
    }
    else if (event.MiddleDown())
    {
        // notify cell about middle click
        // cell->...
    }
    if (event.LeftDown() || forceClick)
    {
#ifdef __WXMSW__
        SetFocus();
#endif

        m_lineBeforeLastClicked = m_lineLastClicked;
        m_lineLastClicked = current;

        size_t oldCurrentRow = m_currentRow;
        bool oldWasSelected = IsRowSelected(m_currentRow);

        bool cmdModifierDown = event.CmdDown();
        if ( IsSingleSel() || !(cmdModifierDown || event.ShiftDown()) )
        {
            if ( IsSingleSel() || !IsRowSelected(current) )
            {
                SelectAllRows( false );

                ChangeCurrentRow(current);

                SelectRow(m_currentRow,true);
            }
            else // multi sel & current is highlighted & no mod keys
            {
                m_lineSelectSingleOnUp = current;
                ChangeCurrentRow(current); // change focus
            }
        }
        else // multi sel & either ctrl or shift is down
        {
            if (cmdModifierDown)
            {
                ChangeCurrentRow(current);

                ReverseRowSelection(m_currentRow);
            }
            else if (event.ShiftDown())
            {
                ChangeCurrentRow(current);

                size_t lineFrom = oldCurrentRow,
                       lineTo = current;

                if ( lineTo < lineFrom )
                {
                    lineTo = lineFrom;
                    lineFrom = m_currentRow;
                }

                SelectRows(lineFrom, lineTo, true);
            }
            else // !ctrl, !shift
            {
                // test in the enclosing if should make it impossible
                wxFAIL_MSG( _T("how did we get here?") );
            }
        }

        if (m_currentRow != oldCurrentRow)
            RefreshRow( oldCurrentRow );

        wxDataViewColumn *oldCurrentCol = m_currentCol;

        // Update selection here...
        m_currentCol = col;

        m_lastOnSame = !forceClick && ((col == oldCurrentCol) && (current == oldCurrentRow)) && oldWasSelected;
    }
}

void wxDataViewMainWindow::OnSetFocus( wxFocusEvent &event )
{
    m_hasFocus = true;

    if (HasCurrentRow())
        Refresh();

    event.Skip();
}

void wxDataViewMainWindow::OnKillFocus( wxFocusEvent &event )
{
    m_hasFocus = false;

    if (HasCurrentRow())
        Refresh();

    event.Skip();
}

//-----------------------------------------------------------------------------
// wxDataViewCtrl
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxDataViewCtrl, wxDataViewCtrlBase)

BEGIN_EVENT_TABLE(wxDataViewCtrl, wxDataViewCtrlBase)
    EVT_SIZE(wxDataViewCtrl::OnSize)
END_EVENT_TABLE()

wxDataViewCtrl::~wxDataViewCtrl()
{
    if (m_notifier)
        GetModel()->RemoveNotifier( m_notifier );
}

void wxDataViewCtrl::Init()
{
    m_notifier = NULL;
}

bool wxDataViewCtrl::Create(wxWindow *parent, wxWindowID id,
           const wxPoint& pos, const wxSize& size,
           long style, const wxValidator& validator )
{
    if (!wxControl::Create( parent, id, pos, size, style | wxScrolledWindowStyle|wxSUNKEN_BORDER, validator))
        return false;

    Init();

#ifdef __WXMAC__
    MacSetClipChildren( true ) ;
#endif

    m_clientArea = new wxDataViewMainWindow( this, wxID_ANY );
#ifdef __WXMSW__
    m_headerArea = new wxDataViewHeaderWindow( this, wxID_ANY, wxDefaultPosition, wxSize(wxDefaultCoord,22) );
#else
    m_headerArea = new wxDataViewHeaderWindow( this, wxID_ANY, wxDefaultPosition, wxSize(wxDefaultCoord,25) );
#endif

    SetTargetWindow( m_clientArea );

    wxBoxSizer *sizer = new wxBoxSizer( wxVERTICAL );
    sizer->Add( m_headerArea, 0, wxGROW );
    sizer->Add( m_clientArea, 1, wxGROW );
    SetSizer( sizer );

    return true;
}

#ifdef __WXMSW__
WXLRESULT wxDataViewCtrl::MSWWindowProc(WXUINT nMsg,
                                       WXWPARAM wParam,
                                       WXLPARAM lParam)
{
    WXLRESULT rc = wxDataViewCtrlBase::MSWWindowProc(nMsg, wParam, lParam);

#ifndef __WXWINCE__
    // we need to process arrows ourselves for scrolling
    if ( nMsg == WM_GETDLGCODE )
    {
        rc |= DLGC_WANTARROWS;
    }
#endif

    return rc;
}
#endif

void wxDataViewCtrl::OnSize( wxSizeEvent &WXUNUSED(event) )
{
    // We need to override OnSize so that our scrolled
    // window a) does call Layout() to use sizers for
    // positioning the controls but b) does not query
    // the sizer for their size and use that for setting
    // the scrollable area as set that ourselves by
    // calling SetScrollbar() further down.

    Layout();

    AdjustScrollbars();
}

bool wxDataViewCtrl::AssociateModel( wxDataViewListModel *model )
{
    if (!wxDataViewCtrlBase::AssociateModel( model ))
        return false;

    m_notifier = new wxGenericDataViewListModelNotifier( m_clientArea );

    model->AddNotifier( m_notifier );

    m_clientArea->UpdateDisplay();

    return true;
}

bool wxDataViewCtrl::AppendColumn( wxDataViewColumn *col )
{
    if (!wxDataViewCtrlBase::AppendColumn(col))
        return false;

    m_clientArea->UpdateDisplay();

    return true;
}

#endif
    // !wxUSE_GENERICDATAVIEWCTRL

#endif
    // wxUSE_DATAVIEWCTRL
