///////////////////////////////////////////////////////////////////////////////
// Name:        src/msw/headerctrl.cpp
// Purpose:     implementation of wxHeaderCtrl for wxMSW
// Author:      Vadim Zeitlin
// Created:     2008-12-01
// RCS-ID:      $Id$
// Copyright:   (c) 2008 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// for compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/log.h"
#endif // WX_PRECOMP

#include "wx/headerctrl.h"
#include "wx/imaglist.h"

#include "wx/msw/wrapcctl.h"
#include "wx/msw/private.h"

// from src/msw/listctrl.cpp
extern int WXDLLIMPEXP_CORE wxMSWGetColumnClicked(NMHDR *nmhdr, POINT *ptClick);

// ============================================================================
// wxHeaderCtrl implementation
// ============================================================================

// ----------------------------------------------------------------------------
// wxHeaderCtrl construction/destruction
// ----------------------------------------------------------------------------

void wxHeaderCtrl::Init()
{
    m_imageList = NULL;
}

bool wxHeaderCtrl::Create(wxWindow *parent,
                          wxWindowID id,
                          const wxPoint& pos,
                          const wxSize& size,
                          long style,
                          const wxString& name)
{
    // notice that we don't need InitCommonControlsEx(ICC_LISTVIEW_CLASSES)
    // here as we already call InitCommonControls() in wxApp initialization
    // code which covers this

    if ( !CreateControl(parent, id, pos, size, style, wxDefaultValidator, name) )
        return false;

    if ( !MSWCreateControl(WC_HEADER, _T(""), pos, size) )
        return false;

    return true;
}

WXDWORD wxHeaderCtrl::MSWGetStyle(long style, WXDWORD *exstyle) const
{
    WXDWORD msStyle = wxControl::MSWGetStyle(style, exstyle);

    if ( style & wxHD_DRAGDROP )
        msStyle |= HDS_DRAGDROP;

    // the control looks nicer with these styles and there doesn't seem to be
    // any reason to not use them so we always do (as for HDS_HORZ it is 0
    // anyhow but include it for clarity)
    msStyle |= HDS_HORZ | HDS_BUTTONS | HDS_FLAT | HDS_FULLDRAG | HDS_HOTTRACK;

    return msStyle;
}

wxHeaderCtrl::~wxHeaderCtrl()
{
    delete m_imageList;
}

// ----------------------------------------------------------------------------
// wxHeaderCtrl scrolling
// ----------------------------------------------------------------------------

void wxHeaderCtrl::DoScrollHorz(int dx)
{
    // as the native control doesn't support offsetting its contents, we use a
    // hack here to make it appear correctly when the parent is scrolled:
    // instead of scrolling or repainting we simply move the control window
    // itself: to be precise, offset it by the scroll increment to the left and
    // increment its width to still extend to the right boundary to compensate
    // for it (notice that dx is negative when scrolling to the right)
    SetSize(GetPosition().x + dx, -1, GetSize().x - dx, -1, wxSIZE_USE_EXISTING);
}

// ----------------------------------------------------------------------------
// wxHeaderCtrl geometry calculation
// ----------------------------------------------------------------------------

wxSize wxHeaderCtrl::DoGetBestSize() const
{
    RECT rc = wxGetClientRect(GetHwndOf(GetParent()));
    WINDOWPOS wpos;
    HDLAYOUT layout = { &rc, &wpos };
    if ( !Header_Layout(GetHwnd(), &layout) )
    {
        wxLogLastError(_T("Header_Layout"));
        return wxControl::DoGetBestSize();
    }

    return wxSize(wpos.cx, wpos.cy);
}

// ----------------------------------------------------------------------------
// wxHeaderCtrl columns managements
// ----------------------------------------------------------------------------

unsigned int wxHeaderCtrl::DoGetCount() const
{
    return Header_GetItemCount(GetHwnd());
}

void wxHeaderCtrl::DoSetCount(unsigned int count)
{
    unsigned n;

    // first delete all old columns
    const unsigned countOld = DoGetCount();
    for ( n = 0; n < countOld; n++ )
    {
        if ( !Header_DeleteItem(GetHwnd(), 0) )
        {
            wxLogLastError(_T("Header_DeleteItem"));
        }
    }

    // and add the new ones
    for ( n = 0; n < count; n++ )
    {
        DoSetOrInsertItem(Insert, n);
    }
}

void wxHeaderCtrl::DoUpdate(unsigned int idx)
{
    DoSetOrInsertItem(Set, idx);
}

void wxHeaderCtrl::DoSetOrInsertItem(Operation oper, unsigned int idx)
{
    const wxHeaderColumnBase& col = GetColumn(idx);

    wxHDITEM hdi;

    // notice that we need to store the string we use the pointer to until we
    // pass it to the control
    wxWxCharBuffer buf;
    if ( !col.GetTitle().empty() )
    {
        hdi.mask |= HDI_TEXT;

        buf = col.GetTitle().wx_str();
        hdi.pszText = buf.data();
        hdi.cchTextMax = wxStrlen(buf);
    }

    const wxBitmap bmp = col.GetBitmap();
    if ( bmp.IsOk() )
    {
        const int bmpWidth = bmp.GetWidth(),
                  bmpHeight = bmp.GetHeight();

        if ( !m_imageList )
        {
            m_imageList = new wxImageList(bmpWidth, bmpHeight);
            Header_SetImageList(GetHwnd(), GetHimagelistOf(m_imageList));
        }
        else // already have an image list
        {
            // check that all bitmaps we use have the same size
            int imageWidth,
                imageHeight;
            m_imageList->GetSize(0, imageWidth, imageHeight);

            wxASSERT_MSG( imageWidth == bmpWidth && imageHeight == bmpHeight,
                          "all column bitmaps must have the same size" );
        }

        m_imageList->Add(bmp);
        hdi.mask |= HDI_IMAGE;
        hdi.iImage = m_imageList->GetImageCount() - 1;
    }

    if ( col.GetAlignment() != wxALIGN_NOT )
    {
        hdi.mask |= HDI_FORMAT;
        switch ( col.GetAlignment() )
        {
            case wxALIGN_LEFT:
                hdi.fmt |= HDF_LEFT;
                break;

            case wxALIGN_CENTER:
            case wxALIGN_CENTER_HORIZONTAL:
                hdi.fmt |= HDF_CENTER;
                break;

            case wxALIGN_RIGHT:
                hdi.fmt |= HDF_RIGHT;
                break;

            default:
                wxFAIL_MSG( "invalid column header alignment" );
        }
    }

    if ( col.IsSortKey() )
    {
        hdi.mask |= HDI_FORMAT;
        hdi.fmt |= col.IsSortOrderAscending() ? HDF_SORTUP : HDF_SORTDOWN;
    }

    if ( col.GetWidth() != wxCOL_WIDTH_DEFAULT || col.IsHidden() )
    {
        hdi.mask |= HDI_WIDTH;
        hdi.cxy = col.IsHidden() ? 0 : col.GetWidth();
    }

    const LRESULT rc = ::SendMessage(GetHwnd(),
                                     oper == Set ? HDM_SETITEM : HDM_INSERTITEM,
                                     idx,
                                     (LPARAM)&hdi);
    if ( oper == Set )
    {
        if ( !rc )
            wxLogLastError(_T("Header_SetItem()"));
    }
    else // Insert
    {
        if ( rc == -1 )
            wxLogLastError(_T("Header_InsertItem()"));
    }
}

// ----------------------------------------------------------------------------
// wxHeaderCtrl events
// ----------------------------------------------------------------------------

bool wxHeaderCtrl::SendClickEvent(bool dblclk, unsigned int idx, int button)
{
    wxEventType evtType;
    switch ( button )
    {
        case 0:
            evtType = dblclk ? wxEVT_COMMAND_HEADER_DCLICK
                             : wxEVT_COMMAND_HEADER_CLICK;
            break;

        case 1:
            evtType = dblclk ? wxEVT_COMMAND_HEADER_RIGHT_DCLICK
                             : wxEVT_COMMAND_HEADER_RIGHT_CLICK;
            break;

        case 2:
            evtType = dblclk ? wxEVT_COMMAND_HEADER_MIDDLE_DCLICK
                             : wxEVT_COMMAND_HEADER_MIDDLE_CLICK;
            break;

        default:
            wxFAIL_MSG( wxS("unexpected event type") );
            return false;
    }

    wxHeaderCtrlEvent event(evtType, GetId());
    event.SetEventObject(this);
    event.SetColumn(idx);

    return GetEventHandler()->ProcessEvent(event);
}

bool wxHeaderCtrl::MSWOnNotify(int idCtrl, WXLPARAM lParam, WXLPARAM *result)
{
    NMHEADER * const nmhdr = (NMHEADER *)lParam;

    switch ( nmhdr->hdr.code )
    {
        case HDN_ITEMCLICK:
        case HDN_ITEMDBLCLICK:
            if ( SendClickEvent(nmhdr->hdr.code == HDN_ITEMDBLCLICK,
                                nmhdr->iItem,
                                nmhdr->iButton) )
                return true;
            break;

            // although we should get the notifications about the right clicks
            // via HDN_ITEM[DBL]CLICK too according to MSDN this simply doesn't
            // happen in practice on any Windows system up to 2003
        case NM_RCLICK:
        case NM_RDBLCLK:
            {
                POINT pt;
                const int col = wxMSWGetColumnClicked(&nmhdr->hdr, &pt);
                if ( col != wxNOT_FOUND )
                {
                    if ( SendClickEvent(nmhdr->hdr.code == NM_RDBLCLK, col, 1) )
                        return true;
                }
                //else: ignore clicks outside any column
            }
            break;
    }

    return wxHeaderCtrlBase::MSWOnNotify(idCtrl, lParam, result);
}
