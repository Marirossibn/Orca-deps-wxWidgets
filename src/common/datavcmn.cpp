/////////////////////////////////////////////////////////////////////////////
// Name:        src/common/datavcmn.cpp
// Purpose:     wxDataViewCtrl base classes and common parts
// Author:      Robert Roebling
// Created:     2006/02/20
// RCS-ID:      $Id$
// Copyright:   (c) 2006, Robert Roebling
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_DATAVIEWCTRL

#include "wx/dataview.h"

#ifndef WX_PRECOMP
    #include "wx/log.h"
    #include "wx/icon.h"
#endif

const wxChar wxDataViewCtrlNameStr[] = wxT("dataviewCtrl");


bool operator == (const wxDataViewItem &left, const wxDataViewItem &right)
{
    return (left.GetID() == right.GetID() );
}


// ---------------------------------------------------------
// wxDataViewModelNotifier
// ---------------------------------------------------------

#include "wx/listimpl.cpp"
WX_DEFINE_LIST(wxDataViewModelNotifiers);

bool wxDataViewModelNotifier::ItemsAdded( const wxDataViewItem &parent, const wxDataViewItemArray &items )
{
    size_t count = items.GetCount();
    size_t i;
    for (i = 0; i < count; i++)
        if (!ItemAdded( parent, items[i] )) return false;
        
    return true;
}

bool wxDataViewModelNotifier::ItemsDeleted( const wxDataViewItem &parent, const wxDataViewItemArray &items )
{
    size_t count = items.GetCount();
    size_t i;
    for (i = 0; i < count; i++)
        if (!ItemDeleted( parent, items[i] )) return false;
    
    return true;
}

bool wxDataViewModelNotifier::ItemsChanged( const wxDataViewItemArray &items )
{
    size_t count = items.GetCount();
    size_t i;
    for (i = 0; i < count; i++)
        if (!ItemChanged( items[i] )) return false;
        
    return true;
}

// ---------------------------------------------------------
// wxDataViewModel
// ---------------------------------------------------------

wxDataViewModel::wxDataViewModel()
{
    m_notifiers.DeleteContents( true );
}

bool wxDataViewModel::ItemAdded( const wxDataViewItem &parent, const wxDataViewItem &item )
{
    bool ret = true;

    wxDataViewModelNotifiers::iterator iter;
    for (iter = m_notifiers.begin(); iter != m_notifiers.end(); ++iter)
    {
        wxDataViewModelNotifier* notifier = *iter;
        if (!notifier->ItemAdded( parent, item ))
            ret = false;
    }

    return ret;
}

bool wxDataViewModel::ItemDeleted( const wxDataViewItem &parent, const wxDataViewItem &item )
{
    bool ret = true;

    wxDataViewModelNotifiers::iterator iter;
    for (iter = m_notifiers.begin(); iter != m_notifiers.end(); ++iter)
    {
        wxDataViewModelNotifier* notifier = *iter;
        if (!notifier->ItemDeleted( parent, item ))
            ret = false;
    }

    return ret;
}

bool wxDataViewModel::ItemChanged( const wxDataViewItem &item )
{
    bool ret = true;

    wxDataViewModelNotifiers::iterator iter;
    for (iter = m_notifiers.begin(); iter != m_notifiers.end(); ++iter)
    {
        wxDataViewModelNotifier* notifier = *iter;
        if (!notifier->ItemChanged( item ))
            ret = false;
    }

    return ret;
}

bool wxDataViewModel::ItemsAdded( const wxDataViewItem &parent, const wxDataViewItemArray &items )
{
    bool ret = true;

    wxDataViewModelNotifiers::iterator iter;
    for (iter = m_notifiers.begin(); iter != m_notifiers.end(); ++iter)
    {
        wxDataViewModelNotifier* notifier = *iter;
        if (!notifier->ItemsAdded( parent, items ))
            ret = false;
    }

    return ret;
}

bool wxDataViewModel::ItemsDeleted( const wxDataViewItem &parent, const wxDataViewItemArray &items )
{
    bool ret = true;

    wxDataViewModelNotifiers::iterator iter;
    for (iter = m_notifiers.begin(); iter != m_notifiers.end(); ++iter)
    {
        wxDataViewModelNotifier* notifier = *iter;
        if (!notifier->ItemsDeleted( parent, items ))
            ret = false;
    }

    return ret;
}

bool wxDataViewModel::ItemsChanged( const wxDataViewItemArray &items )
{
    bool ret = true;

    wxDataViewModelNotifiers::iterator iter;
    for (iter = m_notifiers.begin(); iter != m_notifiers.end(); ++iter)
    {
        wxDataViewModelNotifier* notifier = *iter;
        if (!notifier->ItemsChanged( items ))
            ret = false;
    }

    return ret;
}

bool wxDataViewModel::ValueChanged( const wxDataViewItem &item, unsigned int col )
{
    bool ret = true;

    wxDataViewModelNotifiers::iterator iter;
    for (iter = m_notifiers.begin(); iter != m_notifiers.end(); ++iter)
    {
        wxDataViewModelNotifier* notifier = *iter;
        if (!notifier->ValueChanged( item, col ))
            ret = false;
    }

    return ret;
}

bool wxDataViewModel::Cleared()
{
    bool ret = true;

    wxDataViewModelNotifiers::iterator iter;
    for (iter = m_notifiers.begin(); iter != m_notifiers.end(); ++iter)
    {
        wxDataViewModelNotifier* notifier = *iter;
        if (!notifier->Cleared())
            ret = false;
    }

    return ret;
}

void wxDataViewModel::Resort()
{
    wxDataViewModelNotifiers::iterator iter;
    for (iter = m_notifiers.begin(); iter != m_notifiers.end(); ++iter)
    {
        wxDataViewModelNotifier* notifier = *iter;
        notifier->Resort();
    }
}

void wxDataViewModel::AddNotifier( wxDataViewModelNotifier *notifier )
{
    m_notifiers.push_back( notifier );
    notifier->SetOwner( this );
}

void wxDataViewModel::RemoveNotifier( wxDataViewModelNotifier *notifier )
{
    m_notifiers.DeleteObject( notifier );
}

int wxDataViewModel::Compare( const wxDataViewItem &item1, const wxDataViewItem &item2,
                              unsigned int column, bool ascending )
{
    // sort branches before leaves
    bool item1_is_container = IsContainer(item1);
    bool item2_is_container = IsContainer(item2);

    if (item1_is_container && !item2_is_container)
        return 1;
    if (item2_is_container && !item1_is_container)
        return -1;

    wxVariant value1,value2;
    GetValue( value1, item1, column );
    GetValue( value2, item2, column );

    if (!ascending)
    {
        wxVariant temp = value1;
        value1 = value2;
        value2 = temp;
    }

    if (value1.GetType() == wxT("string"))
    {
        wxString str1 = value1.GetString();
        wxString str2 = value2.GetString();
        int res = str1.Cmp( str2 );
        if (res) return res;
    } else
    if (value1.GetType() == wxT("long"))
    {
        long l1 = value1.GetLong();
        long l2 = value2.GetLong();
        long res = l1-l2;
        if (res) return res;
    } else
    if (value1.GetType() == wxT("double"))
    {
        double d1 = value1.GetDouble();
        double d2 = value2.GetDouble();
        if (d1 < d2) return 1;
        if (d1 > d2) return -1;
    } else
    if (value1.GetType() == wxT("datetime"))
    {
        wxDateTime dt1 = value1.GetDateTime();
        wxDateTime dt2 = value2.GetDateTime();
        if (dt1.IsEarlierThan(dt2)) return 1;
        if (dt2.IsEarlierThan(dt1)) return -11;
    }

    // items must be different
    unsigned long litem1 = (unsigned long) item1.GetID();
    unsigned long litem2 = (unsigned long) item2.GetID();

    if (!ascending)
        return litem2-litem2;

    return litem1-litem2;
}

// ---------------------------------------------------------
// wxDataViewIndexListModel
// ---------------------------------------------------------

wxDataViewIndexListModel::wxDataViewIndexListModel( unsigned int initial_size )
{
    // build initial index
    unsigned int i;
    for (i = 1; i < initial_size+1; i++)
        m_hash.Add( (void*) i );
    m_lastIndex = initial_size + 1;
}

wxDataViewIndexListModel::~wxDataViewIndexListModel()
{
}

void wxDataViewIndexListModel::RowPrepended()
{
    unsigned int id = m_lastIndex++;
    m_hash.Insert( (void*) id, 0 );
    wxDataViewItem item( (void*) id );
    ItemAdded( wxDataViewItem(0), item );
}

void wxDataViewIndexListModel::RowInserted( unsigned int before )
{
    unsigned int id = m_lastIndex++;
    m_hash.Insert( (void*) id, before );
    wxDataViewItem item( (void*) id );
    ItemAdded( wxDataViewItem(0), item );
}

void wxDataViewIndexListModel::RowAppended()
{
    unsigned int id = m_lastIndex++;
    m_hash.Add( (void*) id );
    wxDataViewItem item( (void*) id );
    ItemAdded( wxDataViewItem(0), item );
}

void wxDataViewIndexListModel::RowDeleted( unsigned int row )
{
    wxDataViewItem item( m_hash[row] );
    wxDataViewModel::ItemDeleted( wxDataViewItem(0), item );
    m_hash.RemoveAt( row );
}

void wxDataViewIndexListModel::RowChanged( unsigned int row )
{
    wxDataViewModel::ItemChanged( GetItem(row) );
}

void wxDataViewIndexListModel::RowValueChanged( unsigned int row, unsigned int col )
{
    wxDataViewModel::ValueChanged( GetItem(row), col );
}

unsigned int wxDataViewIndexListModel::GetRow( const wxDataViewItem &item ) const
{
    // assert for not found
    return (unsigned int) m_hash.Index( item.GetID() );
}

wxDataViewItem wxDataViewIndexListModel::GetItem( unsigned int row ) const
{
    wxASSERT( row < m_hash.GetCount() );
    return wxDataViewItem( m_hash[row] );
}

int wxDataViewIndexListModel::Compare(const wxDataViewItem& item1,
                                      const wxDataViewItem& item2,
                                      unsigned int WXUNUSED(column),
                                      bool ascending)
{
    if (ascending)
        return GetRow(item1) - GetRow(item2);

    return GetRow(item2) - GetRow(item1);
}

void wxDataViewIndexListModel::GetValue( wxVariant &variant,
                           const wxDataViewItem &item, unsigned int col ) const
{
    GetValue( variant, GetRow(item), col );
}

bool wxDataViewIndexListModel::SetValue( const wxVariant &variant,
                           const wxDataViewItem &item, unsigned int col )
{
    return SetValue( variant, GetRow(item), col );
}

wxDataViewItem wxDataViewIndexListModel::GetParent( const wxDataViewItem & WXUNUSED(item) ) const
{
    return wxDataViewItem(0);
}

bool wxDataViewIndexListModel::IsContainer( const wxDataViewItem &item ) const
{
    // only the invisible root item has children
    if (!item.IsOk())
        return true;

    return false;
}

unsigned int wxDataViewIndexListModel::GetChildren( const wxDataViewItem &item, wxDataViewItemArray &children ) const
{
    if (item.IsOk())
        return 0;

    children = m_hash;

    return m_hash.GetCount();
}

//-----------------------------------------------------------------------------
// wxDataViewIconText
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxDataViewIconText,wxObject)

IMPLEMENT_VARIANT_OBJECT_EXPORTED(wxDataViewIconText, WXDLLIMPEXP_ADV)

bool operator == (const wxDataViewIconText &one, const wxDataViewIconText &two)
{
    if (one.GetText() != two.GetText()) return false;
    if (one.IsSameAs(two)) return false;
    return true;
}

// ---------------------------------------------------------
// wxDataViewRendererBase
// ---------------------------------------------------------

IMPLEMENT_ABSTRACT_CLASS(wxDataViewRendererBase, wxObject)

wxDataViewRendererBase::wxDataViewRendererBase( const wxString &varianttype,
                                                wxDataViewCellMode WXUNUSED(mode),
                                                int WXUNUSED(align) )
{
    m_variantType = varianttype;
    m_editorCtrl = NULL;
}

const wxDataViewCtrl* wxDataViewRendererBase::GetView() const
{
    return wx_const_cast(wxDataViewRendererBase*, this)->GetOwner()->GetOwner();
}

bool wxDataViewRendererBase::StartEditing( const wxDataViewItem &item, wxRect labelRect )
{
    m_item = item; // remember for later

    unsigned int col = GetOwner()->GetModelColumn();
    wxVariant value;
    GetOwner()->GetOwner()->GetModel()->GetValue( value, item, col );

    m_editorCtrl = CreateEditorCtrl( GetOwner()->GetOwner()->GetMainWindow(), labelRect, value );

    wxDataViewEditorCtrlEvtHandler *handler =
        new wxDataViewEditorCtrlEvtHandler( m_editorCtrl, (wxDataViewRenderer*) this );

    m_editorCtrl->PushEventHandler( handler );

#if defined(__WXGTK20__) && !defined(wxUSE_GENERICDATAVIEWCTRL)
    handler->SetFocusOnIdle();
#else
    m_editorCtrl->SetFocus();
#endif

    // Now we should send Editing Started event
    wxDataViewEvent event( wxEVT_COMMAND_DATAVIEW_ITEM_EDITING_STARTED, GetOwner()->GetOwner()->GetId() );
    event.SetDataViewColumn( GetOwner() );
    event.SetModel( GetOwner()->GetOwner()->GetModel() );
    event.SetItem( item );
    GetOwner()->GetOwner()->GetEventHandler()->ProcessEvent( event );

    return true;
}

void wxDataViewRendererBase::CancelEditing()
{
    wxPendingDelete.Append( m_editorCtrl );

    GetOwner()->GetOwner()->GetMainWindow()->SetFocus();

    // m_editorCtrl->PopEventHandler( true );
}

bool wxDataViewRendererBase::FinishEditing()
{
    wxVariant value;
    GetValueFromEditorCtrl( m_editorCtrl, value );

    wxPendingDelete.Append( m_editorCtrl );

    GetOwner()->GetOwner()->GetMainWindow()->SetFocus();

    if (!Validate(value))
        return false;

    unsigned int col = GetOwner()->GetModelColumn();
    GetOwner()->GetOwner()->GetModel()->SetValue( value, m_item, col );
    GetOwner()->GetOwner()->GetModel()->ValueChanged( m_item, col );

    // m_editorCtrl->PopEventHandler( true );

    // Now we should send Editing Done event
    wxDataViewEvent event( wxEVT_COMMAND_DATAVIEW_ITEM_EDITING_DONE, GetOwner()->GetOwner()->GetId() );
    event.SetDataViewColumn( GetOwner() );
    event.SetModel( GetOwner()->GetOwner()->GetModel() );
    event.SetItem( m_item );
    GetOwner()->GetOwner()->GetEventHandler()->ProcessEvent( event );

    return true;
}

//-----------------------------------------------------------------------------
// wxDataViewEditorCtrlEvtHandler
//-----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(wxDataViewEditorCtrlEvtHandler, wxEvtHandler)
    EVT_CHAR           (wxDataViewEditorCtrlEvtHandler::OnChar)
    EVT_KILL_FOCUS     (wxDataViewEditorCtrlEvtHandler::OnKillFocus)
    EVT_IDLE           (wxDataViewEditorCtrlEvtHandler::OnIdle)
END_EVENT_TABLE()

wxDataViewEditorCtrlEvtHandler::wxDataViewEditorCtrlEvtHandler(
                                wxControl *editorCtrl,
                                wxDataViewRenderer *owner )
{
    m_owner = owner;
    m_editorCtrl = editorCtrl;

    m_finished = false;
}

void wxDataViewEditorCtrlEvtHandler::OnIdle( wxIdleEvent &event )
{
    if (m_focusOnIdle)
    {
        m_focusOnIdle = false;
        if (wxWindow::FindFocus() != m_editorCtrl)
            m_editorCtrl->SetFocus();
    }

    event.Skip();
}

void wxDataViewEditorCtrlEvtHandler::OnChar( wxKeyEvent &event )
{
    switch ( event.m_keyCode )
    {
        case WXK_RETURN:
            m_finished = true;
            m_owner->FinishEditing();
            break;

        case WXK_ESCAPE:
            m_finished = true;
            m_owner->CancelEditing();
            break;

        default:
            event.Skip();
    }
}

void wxDataViewEditorCtrlEvtHandler::OnKillFocus( wxFocusEvent &event )
{
    if (!m_finished)
    {
        m_finished = true;
        m_owner->FinishEditing();
    }

    event.Skip();
}

// ---------------------------------------------------------
// wxDataViewColumnBase
// ---------------------------------------------------------

IMPLEMENT_ABSTRACT_CLASS(wxDataViewColumnBase, wxObject)

wxDataViewColumnBase::wxDataViewColumnBase(const wxString& WXUNUSED(title),
                                           wxDataViewRenderer *renderer,
                                           unsigned int model_column,
                                           int WXUNUSED(width),
                                           wxAlignment WXUNUSED(align),
                                           int WXUNUSED(flags))
{
    m_renderer = renderer;
    m_model_column = model_column;
    m_owner = NULL;
    m_renderer->SetOwner( (wxDataViewColumn*) this );

    // NOTE: the wxDataViewColumn's ctor must store the width, align, flags
    //       parameters inside the native control!
}

wxDataViewColumnBase::wxDataViewColumnBase(const wxBitmap& bitmap,
                                           wxDataViewRenderer *renderer,
                                           unsigned int model_column,
                                           int WXUNUSED(width),
                                           wxAlignment WXUNUSED(align),
                                           int WXUNUSED(flags) )
{
    m_renderer = renderer;
    m_model_column = model_column;
    m_bitmap = bitmap;
    m_owner = NULL;
    m_renderer->SetOwner( (wxDataViewColumn*) this );
}

wxDataViewColumnBase::~wxDataViewColumnBase()
{
    if (m_renderer)
        delete m_renderer;
}

int wxDataViewColumnBase::GetFlags() const
{
    int ret = 0;

    if (IsSortable())
        ret |= wxDATAVIEW_COL_SORTABLE;
    if (IsResizeable())
        ret |= wxDATAVIEW_COL_RESIZABLE;
    if (IsHidden())
        ret |= wxDATAVIEW_COL_HIDDEN;

    return ret;
}

void wxDataViewColumnBase::SetFlags(int flags)
{
    SetSortable((flags & wxDATAVIEW_COL_SORTABLE) != 0);
    SetResizeable((flags & wxDATAVIEW_COL_RESIZABLE) != 0);
    SetHidden((flags & wxDATAVIEW_COL_HIDDEN) != 0);
}

// ---------------------------------------------------------
// wxDataViewCtrlBase
// ---------------------------------------------------------

IMPLEMENT_ABSTRACT_CLASS(wxDataViewCtrlBase, wxControl)

wxDataViewCtrlBase::wxDataViewCtrlBase()
{
    m_model = NULL;
    m_expander_column = 0;
    m_indent = 8;
}

wxDataViewCtrlBase::~wxDataViewCtrlBase()
{
    if (m_model)
    {
        m_model->DecRef();
        m_model = NULL;
    }
}

bool wxDataViewCtrlBase::AssociateModel( wxDataViewModel *model )
{
    if (m_model)
    {
        m_model->DecRef();   // discard old model, if any
    }

    // add our own reference to the new model:
    m_model = model;
    if (m_model)
    {
        m_model->IncRef();
    }

    return true;
}

wxDataViewModel* wxDataViewCtrlBase::GetModel()
{
    return m_model;
}

wxDataViewColumn *
wxDataViewCtrlBase::AppendTextColumn( const wxString &label, unsigned int model_column,
                            wxDataViewCellMode mode, int width, wxAlignment align, int flags )
{
    wxDataViewColumn *ret = new wxDataViewColumn( label,
        new wxDataViewTextRenderer( wxT("string"), mode, (int)align ),
        model_column, width, align, flags );
    AppendColumn( ret );
    return ret;
}

wxDataViewColumn *
wxDataViewCtrlBase::AppendIconTextColumn( const wxString &label, unsigned int model_column,
                            wxDataViewCellMode mode, int width, wxAlignment align, int flags )
{
    wxDataViewColumn *ret = new wxDataViewColumn( label,
        new wxDataViewIconTextRenderer( wxT("wxDataViewIconText"), mode, (int)align ),
        model_column, width, align, flags );
    AppendColumn( ret );
    return ret;
}

wxDataViewColumn *
wxDataViewCtrlBase::AppendToggleColumn( const wxString &label, unsigned int model_column,
                            wxDataViewCellMode mode, int width, wxAlignment align, int flags )
{

    wxDataViewColumn *ret = new wxDataViewColumn( label,
        new wxDataViewToggleRenderer( wxT("bool"), mode, (int)align ),
        model_column, width, align, flags );
    AppendColumn( ret );
    return ret;
}

wxDataViewColumn *
wxDataViewCtrlBase::AppendProgressColumn( const wxString &label, unsigned int model_column,
                            wxDataViewCellMode mode, int width, wxAlignment align, int flags )
{
    wxDataViewColumn *ret = new wxDataViewColumn( label,
        new wxDataViewProgressRenderer( wxEmptyString, wxT("long"), mode, (int)align ),
        model_column, width, align, flags );
    AppendColumn( ret );
    return ret;
}

wxDataViewColumn *
wxDataViewCtrlBase::AppendDateColumn( const wxString &label, unsigned int model_column,
                            wxDataViewCellMode mode, int width, wxAlignment align, int flags )
{
    wxDataViewColumn *ret = new wxDataViewColumn( label,
        new wxDataViewDateRenderer( wxT("datetime"), mode, (int)align ),
        model_column, width, align, flags );
    AppendColumn( ret );
    return ret;
}

wxDataViewColumn *
wxDataViewCtrlBase::AppendBitmapColumn( const wxString &label, unsigned int model_column,
                            wxDataViewCellMode mode, int width, wxAlignment align, int flags )
{
    wxDataViewColumn *ret = new wxDataViewColumn( label,
        new wxDataViewBitmapRenderer( wxT("wxBitmap"), mode, (int)align ),
        model_column, width, align, flags );
    AppendColumn( ret );
    return ret;
}

wxDataViewColumn *
wxDataViewCtrlBase::AppendTextColumn( const wxBitmap &label, unsigned int model_column,
                            wxDataViewCellMode mode, int width, wxAlignment align, int flags )
{
    wxDataViewColumn *ret = new wxDataViewColumn( label,
        new wxDataViewTextRenderer( wxT("string"), mode, (int)align ),
        model_column, width, align, flags );
    AppendColumn( ret );
    return ret;
}

wxDataViewColumn *
wxDataViewCtrlBase::AppendIconTextColumn( const wxBitmap &label, unsigned int model_column,
                            wxDataViewCellMode mode, int width, wxAlignment align, int flags )
{
    wxDataViewColumn *ret = new wxDataViewColumn( label,
        new wxDataViewIconTextRenderer( wxT("wxDataViewIconText"), mode, (int)align ),
        model_column, width, align, flags );
    AppendColumn( ret );
    return ret;
}

wxDataViewColumn *
wxDataViewCtrlBase::AppendToggleColumn( const wxBitmap &label, unsigned int model_column,
                            wxDataViewCellMode mode, int width, wxAlignment align, int flags )
{
    wxDataViewColumn *ret = new wxDataViewColumn( label,
        new wxDataViewToggleRenderer( wxT("bool"), mode, (int)align ),
        model_column, width, align, flags );
    AppendColumn( ret );
    return ret;
}

wxDataViewColumn *
wxDataViewCtrlBase::AppendProgressColumn( const wxBitmap &label, unsigned int model_column,
                            wxDataViewCellMode mode, int width, wxAlignment align, int flags )
{
    wxDataViewColumn *ret = new wxDataViewColumn( label,
        new wxDataViewProgressRenderer( wxEmptyString, wxT("long"), mode, (int)align ),
        model_column, width, align, flags );
    AppendColumn( ret );
    return ret;
}

wxDataViewColumn *
wxDataViewCtrlBase::AppendDateColumn( const wxBitmap &label, unsigned int model_column,
                            wxDataViewCellMode mode, int width, wxAlignment align, int flags )
{
    wxDataViewColumn *ret = new wxDataViewColumn( label,
        new wxDataViewDateRenderer( wxT("datetime"), mode, (int)align ),
        model_column, width, align, flags );
    AppendColumn( ret );
    return ret;
}

wxDataViewColumn *
wxDataViewCtrlBase::AppendBitmapColumn( const wxBitmap &label, unsigned int model_column,
                            wxDataViewCellMode mode, int width, wxAlignment align, int flags )
{
    wxDataViewColumn *ret = new wxDataViewColumn( label,
        new wxDataViewBitmapRenderer( wxT("wxBitmap"), mode, (int)align ),
        model_column, width, align, flags );
    AppendColumn( ret );
    return ret;
}

wxDataViewColumn *
wxDataViewCtrlBase::PrependTextColumn( const wxString &label, unsigned int model_column,
                            wxDataViewCellMode mode, int width, wxAlignment align, int flags )
{
    wxDataViewColumn *ret = new wxDataViewColumn( label,
        new wxDataViewTextRenderer( wxT("string"), mode, (int)align ),
        model_column, width, align, flags );
    PrependColumn( ret );
    return ret;
}

wxDataViewColumn *
wxDataViewCtrlBase::PrependIconTextColumn( const wxString &label, unsigned int model_column,
                            wxDataViewCellMode mode, int width, wxAlignment align, int flags )
{
    wxDataViewColumn *ret = new wxDataViewColumn( label,
        new wxDataViewIconTextRenderer( wxT("wxDataViewIconText"), mode, (int)align ),
        model_column, width, align, flags );
    PrependColumn( ret );
    return ret;
}

wxDataViewColumn *
wxDataViewCtrlBase::PrependToggleColumn( const wxString &label, unsigned int model_column,
                            wxDataViewCellMode mode, int width, wxAlignment align, int flags )
{

    wxDataViewColumn *ret = new wxDataViewColumn( label,
        new wxDataViewToggleRenderer( wxT("bool"), mode, (int)align ),
        model_column, width, align, flags );
    PrependColumn( ret );
    return ret;
}

wxDataViewColumn *
wxDataViewCtrlBase::PrependProgressColumn( const wxString &label, unsigned int model_column,
                            wxDataViewCellMode mode, int width, wxAlignment align, int flags )
{
    wxDataViewColumn *ret = new wxDataViewColumn( label,
        new wxDataViewProgressRenderer( wxEmptyString, wxT("long"), mode, (int)align ),
        model_column, width, align, flags );
    PrependColumn( ret );
    return ret;
}

wxDataViewColumn *
wxDataViewCtrlBase::PrependDateColumn( const wxString &label, unsigned int model_column,
                            wxDataViewCellMode mode, int width, wxAlignment align, int flags )
{
    wxDataViewColumn *ret = new wxDataViewColumn( label,
        new wxDataViewDateRenderer( wxT("datetime"), mode, (int)align ),
        model_column, width, align, flags );
    PrependColumn( ret );
    return ret;
}

wxDataViewColumn *
wxDataViewCtrlBase::PrependBitmapColumn( const wxString &label, unsigned int model_column,
                            wxDataViewCellMode mode, int width, wxAlignment align, int flags )
{
    wxDataViewColumn *ret = new wxDataViewColumn( label,
        new wxDataViewBitmapRenderer( wxT("wxBitmap"), mode, (int)align ),
        model_column, width, align, flags );
    PrependColumn( ret );
    return ret;
}

wxDataViewColumn *
wxDataViewCtrlBase::PrependTextColumn( const wxBitmap &label, unsigned int model_column,
                            wxDataViewCellMode mode, int width, wxAlignment align, int flags )
{
    wxDataViewColumn *ret = new wxDataViewColumn( label,
        new wxDataViewTextRenderer( wxT("string"), mode, (int)align ),
        model_column, width, align, flags );
    PrependColumn( ret );
    return ret;
}

wxDataViewColumn *
wxDataViewCtrlBase::PrependIconTextColumn( const wxBitmap &label, unsigned int model_column,
                            wxDataViewCellMode mode, int width, wxAlignment align, int flags )
{
    wxDataViewColumn *ret = new wxDataViewColumn( label,
        new wxDataViewIconTextRenderer( wxT("wxDataViewIconText"), mode, (int)align ),
        model_column, width, align, flags );
    PrependColumn( ret );
    return ret;
}

wxDataViewColumn *
wxDataViewCtrlBase::PrependToggleColumn( const wxBitmap &label, unsigned int model_column,
                            wxDataViewCellMode mode, int width, wxAlignment align, int flags )
{
    wxDataViewColumn *ret = new wxDataViewColumn( label,
        new wxDataViewToggleRenderer( wxT("bool"), mode, (int)align ),
        model_column, width, align, flags );
    PrependColumn( ret );
    return ret;
}

wxDataViewColumn *
wxDataViewCtrlBase::PrependProgressColumn( const wxBitmap &label, unsigned int model_column,
                            wxDataViewCellMode mode, int width, wxAlignment align, int flags )
{
    wxDataViewColumn *ret = new wxDataViewColumn( label,
        new wxDataViewProgressRenderer( wxEmptyString, wxT("long"), mode, (int)align ),
        model_column, width, align, flags );
    PrependColumn( ret );
    return ret;
}

wxDataViewColumn *
wxDataViewCtrlBase::PrependDateColumn( const wxBitmap &label, unsigned int model_column,
                            wxDataViewCellMode mode, int width, wxAlignment align, int flags )
{
    wxDataViewColumn *ret = new wxDataViewColumn( label,
        new wxDataViewDateRenderer( wxT("datetime"), mode, (int)align ),
        model_column, width, align, flags );
    PrependColumn( ret );
    return ret;
}

wxDataViewColumn *
wxDataViewCtrlBase::PrependBitmapColumn( const wxBitmap &label, unsigned int model_column,
                            wxDataViewCellMode mode, int width, wxAlignment align, int flags )
{
    wxDataViewColumn *ret = new wxDataViewColumn( label,
        new wxDataViewBitmapRenderer( wxT("wxBitmap"), mode, (int)align ),
        model_column, width, align, flags );
    PrependColumn( ret );
    return ret;
}

bool
wxDataViewCtrlBase::AppendColumn( wxDataViewColumn *col )
{
    col->SetOwner( (wxDataViewCtrl*) this );
    return true;
}

bool
wxDataViewCtrlBase::PrependColumn( wxDataViewColumn *col )
{
    col->SetOwner( (wxDataViewCtrl*) this );
    return true;
}

// ---------------------------------------------------------
// wxDataViewEvent
// ---------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxDataViewEvent,wxNotifyEvent)

DEFINE_EVENT_TYPE(wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED)

DEFINE_EVENT_TYPE(wxEVT_COMMAND_DATAVIEW_ITEM_ACTIVATED)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_DATAVIEW_ITEM_COLLAPSING)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_DATAVIEW_ITEM_COLLAPSED)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_DATAVIEW_ITEM_EXPANDING)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_DATAVIEW_ITEM_EXPANDED)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_DATAVIEW_ITEM_EDITING_STARTED)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_DATAVIEW_ITEM_EDITING_DONE)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_DATAVIEW_ITEM_VALUE_CHANGED)

DEFINE_EVENT_TYPE(wxEVT_COMMAND_DATAVIEW_COLUMN_HEADER_CLICK)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_DATAVIEW_COLUMN_HEADER_RIGHT_CLICK)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_DATAVIEW_COLUMN_SORTED)

//-----------------------------------------------------------------------------
// wxDataViewTreeStore
//-----------------------------------------------------------------------------

wxDataViewTreeStoreNode::wxDataViewTreeStoreNode( 
        wxDataViewTreeStoreNode *parent, 
        const wxString &text, const wxIcon &icon, wxClientData *data )
{
    m_parent = parent;
    m_text = text;
    m_icon = icon;
    m_data = data;
}

wxDataViewTreeStoreNode::~wxDataViewTreeStoreNode()
{
    if (m_data)
        delete m_data;
}
    
#include "wx/listimpl.cpp"
WX_DEFINE_LIST(wxDataViewTreeStoreNodeList);

wxDataViewTreeStoreContainerNode::wxDataViewTreeStoreContainerNode( 
        wxDataViewTreeStoreNode *parent, const wxString &text, 
        const wxIcon &icon, const wxIcon &expanded, wxClientData *data ) :
    wxDataViewTreeStoreNode( parent, text, icon, data )
{
    m_iconExpanded = expanded;
    m_children.DeleteContents(true);
}

wxDataViewTreeStoreContainerNode::~wxDataViewTreeStoreContainerNode()
{
}

//-----------------------------------------------------------------------------

wxDataViewTreeStore::wxDataViewTreeStore()
{
    m_root = new wxDataViewTreeStoreContainerNode( NULL, wxEmptyString );
}

wxDataViewTreeStore::~wxDataViewTreeStore()
{
    delete m_root;
}

wxDataViewItem wxDataViewTreeStore::AppendItem( const wxDataViewItem& parent, 
        const wxString &text, const wxIcon &icon, wxClientData *data )
{
    wxDataViewTreeStoreContainerNode *parent_node = FindContainerNode( parent );
    if (!parent_node) return wxDataViewItem(0);
    
    wxDataViewTreeStoreNode *node = 
        new wxDataViewTreeStoreNode( parent_node, text, icon, data );
    parent_node->GetChildren().Append( node );
    
    // notify control
    ItemAdded( parent, node->GetItem() );
    
    return node->GetItem();
}

wxDataViewItem wxDataViewTreeStore::PrependItem( const wxDataViewItem& parent,
        const wxString &text, const wxIcon &icon, wxClientData *data )
{
    wxDataViewTreeStoreContainerNode *parent_node = FindContainerNode( parent );
    if (!parent_node) return wxDataViewItem(0);
    
    wxDataViewTreeStoreNode *node = 
        new wxDataViewTreeStoreNode( parent_node, text, icon, data );
    parent_node->GetChildren().Insert( node );
    
    // notify control
    ItemAdded( parent, node->GetItem() );
    
    return node->GetItem();
}

wxDataViewItem wxDataViewTreeStore::InsertItem( const wxDataViewItem& parent, const wxDataViewItem& previous,
        const wxString &text, const wxIcon &icon, wxClientData *data )
{
    return wxDataViewItem(0);
}

wxDataViewItem wxDataViewTreeStore::PrependContainer( const wxDataViewItem& parent, 
        const wxString &text, const wxIcon &icon, const wxIcon &expanded, 
        wxClientData *data )
{
    wxDataViewTreeStoreContainerNode *parent_node = FindContainerNode( parent );
    if (!parent_node) return wxDataViewItem(0);
    
    wxDataViewTreeStoreContainerNode *node = 
        new wxDataViewTreeStoreContainerNode( parent_node, text, icon, expanded, data );
    parent_node->GetChildren().Insert( node );
    
    // notify control
    ItemAdded( parent, node->GetItem() );
    
    return node->GetItem();
}

wxDataViewItem wxDataViewTreeStore::AppendContainer( const wxDataViewItem& parent, 
        const wxString &text, const wxIcon &icon, const wxIcon &expanded, 
        wxClientData *data )
{
    wxDataViewTreeStoreContainerNode *parent_node = FindContainerNode( parent );
    if (!parent_node) return wxDataViewItem(0);
    
    wxDataViewTreeStoreContainerNode *node = 
        new wxDataViewTreeStoreContainerNode( parent_node, text, icon, expanded, data );
    parent_node->GetChildren().Append( node );
    
    // notify control
    ItemAdded( parent, node->GetItem() );
    
    return node->GetItem();
}

wxDataViewItem wxDataViewTreeStore::InsertContainer( const wxDataViewItem& parent, const wxDataViewItem& previous,
        const wxString &text, const wxIcon &icon, const wxIcon &expanded, 
        wxClientData *data )
{
    return wxDataViewItem(0);
}

wxDataViewItem wxDataViewTreeStore::GetNthChild( const wxDataViewItem& parent, unsigned int pos ) const
{
    wxDataViewTreeStoreContainerNode *parent_node = FindContainerNode( parent );
    if (!parent_node) return wxDataViewItem(0);
    
    wxDataViewTreeStoreNodeList::compatibility_iterator node = parent_node->GetChildren().Item( pos );
    if (node)
        return node->GetData();
        
    return wxDataViewItem(0);
}

int wxDataViewTreeStore::GetChildCount( const wxDataViewItem& parent ) const
{
    wxDataViewTreeStoreNode *node = FindNode( parent );
    if (!node) return -1;
    
    if (!node->IsContainer())
        return 0;
        
    wxDataViewTreeStoreContainerNode *container_node = (wxDataViewTreeStoreContainerNode*) node;
    return (int) container_node->GetChildren().GetCount();    
}

void wxDataViewTreeStore::SetItemText( const wxDataViewItem& item, const wxString &text )
{
    wxDataViewTreeStoreNode *node = FindNode( item );
    if (!node) return;
    
    node->SetText( text );
    
    // notify control
    ValueChanged( item, 0 );
}

wxString wxDataViewTreeStore::GetItemText( const wxDataViewItem& item ) const
{
    wxDataViewTreeStoreNode *node = FindNode( item );
    if (!node) return wxEmptyString;
    
    return node->GetText();
}

void wxDataViewTreeStore::SetItemIcon( const wxDataViewItem& item, const wxIcon &icon )
{
    wxDataViewTreeStoreNode *node = FindNode( item );
    if (!node) return;
    
    node->SetIcon( icon );
    
    // notify control
    ValueChanged( item, 0 );
}

const wxIcon &wxDataViewTreeStore::GetItemIcon( const wxDataViewItem& item ) const
{
    wxDataViewTreeStoreNode *node = FindNode( item );
    if (!node) return wxNullIcon;
    
    return node->GetIcon();
}

void wxDataViewTreeStore::SetItemExpandedIcon( const wxDataViewItem& item, const wxIcon &icon )
{
    wxDataViewTreeStoreContainerNode *node = FindContainerNode( item );
    if (!node) return;
    
    node->SetExpandedIcon( icon );
    
    // notify control
    ValueChanged( item, 0 );
}

const wxIcon &wxDataViewTreeStore::GetItemExpandedIcon( const wxDataViewItem& item ) const
{
    wxDataViewTreeStoreContainerNode *node = FindContainerNode( item );
    if (!node) return wxNullIcon;
    
    return node->GetExpandedIcon();
}

void wxDataViewTreeStore::SetItemData( const wxDataViewItem& item, wxClientData *data )
{
    wxDataViewTreeStoreNode *node = FindNode( item );
    if (!node) return;
    
    node->SetData( data );
    
    // notify control? only sensible when sorting on client data
    // ValueChanged( item, 0 );
}

wxClientData *wxDataViewTreeStore::GetItemData( const wxDataViewItem& item ) const
{
    wxDataViewTreeStoreNode *node = FindNode( item );
    if (!node) return NULL;
    
    return node->GetData();
}

void wxDataViewTreeStore::DeleteItem( const wxDataViewItem& item )
{
    if (!item.IsOk()) return;

    wxDataViewItem parent_item = GetParent( item );

    wxDataViewTreeStoreContainerNode *parent_node = FindContainerNode( parent_item );
    if (!parent_node) return;
    
    wxDataViewTreeStoreContainerNode *node = FindContainerNode( item );
    if (!node) return;
    
    parent_node->GetChildren().DeleteObject( node );
    
    // notify control
    ItemDeleted( parent_item, item );
}

void wxDataViewTreeStore::DeleteChildren( const wxDataViewItem& item )
{
    wxDataViewTreeStoreContainerNode *node = FindContainerNode( item );
    if (!node) return;
    
    wxDataViewItemArray array;
    wxDataViewTreeStoreNodeList::iterator iter;
    for (iter = node->GetChildren().begin(); iter != node->GetChildren().end(); iter++)
    {
        wxDataViewTreeStoreNode* child = *iter;
        array.Add( child->GetItem() );
    }
    
    node->GetChildren().clear();
    
    // notify control
    ItemsDeleted( item, array );
}

void wxDataViewTreeStore::DeleteAllItems()
{
    // TODO
}

void wxDataViewTreeStore::GetValue( wxVariant &variant, 
                           const wxDataViewItem &item, unsigned int col ) const
{
    // if (col != 0) return;
    
    wxDataViewTreeStoreNode *node = FindNode( item );
    if (!node) return;
    
    wxDataViewIconText data( node->GetText(), node->GetIcon() );
    
    variant << data;
}

bool wxDataViewTreeStore::SetValue( const wxVariant &variant, 
                           const wxDataViewItem &item, unsigned int col )
{
    // if (col != 0) return false;
    
    wxDataViewTreeStoreNode *node = FindNode( item );
    if (!node) return false;
    
    wxDataViewIconText data;
    
    data << variant;
    
    node->SetText( data.GetText() );
    node->SetIcon( data.GetIcon() );
    
    return true;
}

wxDataViewItem wxDataViewTreeStore::GetParent( const wxDataViewItem &item ) const
{
    wxDataViewTreeStoreNode *node = FindNode( item );
    if (!node) return wxDataViewItem(0);
    
    wxDataViewTreeStoreNode *parent = node->GetParent();
    if (!parent) return wxDataViewItem(0);
    
    if (parent == m_root)
        return wxDataViewItem(0);
    
    return parent->GetItem();
}

bool wxDataViewTreeStore::IsContainer( const wxDataViewItem &item ) const
{
    wxDataViewTreeStoreNode *node = FindNode( item );
    if (!node) return false;
    
    return node->IsContainer();
}

unsigned int wxDataViewTreeStore::GetChildren( const wxDataViewItem &item, wxDataViewItemArray &children ) const
{
    wxDataViewTreeStoreContainerNode *node = FindContainerNode( item );
    if (!node) return 0;

    wxDataViewTreeStoreNodeList::iterator iter;
    for (iter = node->GetChildren().begin(); iter != node->GetChildren().end(); iter++)
    {
        wxDataViewTreeStoreNode* child = *iter;
        children.Add( child->GetItem() );
    }
    
    return node->GetChildren().GetCount();
}

int wxDataViewTreeStore::Compare( const wxDataViewItem &item1, const wxDataViewItem &item2, 
                         unsigned int WXUNUSED(column), bool WXUNUSED(ascending) )
{
    wxDataViewTreeStoreNode *node1 = FindNode( item1 );
    wxDataViewTreeStoreNode *node2 = FindNode( item2 );
    
    if (!node1 || !node2)
        return 0;
    
    wxDataViewTreeStoreContainerNode* parent1 = 
        (wxDataViewTreeStoreContainerNode*) node1->GetParent();
    wxDataViewTreeStoreContainerNode* parent2 = 
        (wxDataViewTreeStoreContainerNode*) node2->GetParent();
        
    if (parent1 != parent2)
    {
        wxLogError( wxT("Comparing items with different parent.") );
        return 0;
    }
        
    if (node1->IsContainer() && !!node2->IsContainer())
        return 1;
        
    if (node2->IsContainer() && !!node1->IsContainer())
        return -1;
        
    return parent1->GetChildren().IndexOf( node1 ) - parent1->GetChildren().IndexOf( node2 );
}

wxDataViewTreeStoreNode *wxDataViewTreeStore::FindNode( const wxDataViewItem &item ) const
{
    if (!item.IsOk())
        return m_root;
        
    return (wxDataViewTreeStoreNode*) item.GetID();
}

wxDataViewTreeStoreContainerNode *wxDataViewTreeStore::FindContainerNode( const wxDataViewItem &item ) const
{
    if (!item.IsOk())
        return (wxDataViewTreeStoreContainerNode*) m_root;

    wxDataViewTreeStoreNode* node = (wxDataViewTreeStoreNode*) item.GetID();
    
    if (!node->IsContainer())
        return NULL;
        
    return (wxDataViewTreeStoreContainerNode*) node;
}

#endif

