/////////////////////////////////////////////////////////////////////////////
// Name:        combobox.cpp
// Purpose:     wxComboBox class
// Author:      Julian Smart
// Modified by:
// Created:     17/09/98
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "combobox.h"
#endif

#include "wx/setup.h"

#if wxUSE_COMBOBOX

#include "wx/combobox.h"

#ifdef __VMS__
#pragma message disable nosimpint
#endif
#include <Xm/Xm.h>
#ifdef __VMS__
#pragma message enable nosimpint
#endif

// use the old, GPL'd combobox
#if (XmVersion < 2000)

#include "xmcombo/xmcombo.h"

#include "wx/motif/private.h"

void  wxComboBoxCallback (Widget w, XtPointer clientData,
                          XmComboBoxSelectionCallbackStruct * cbs);

IMPLEMENT_DYNAMIC_CLASS(wxComboBox, wxControl)

bool wxComboBox::Create(wxWindow *parent, wxWindowID id,
                        const wxString& value,
                        const wxPoint& pos,
                        const wxSize& size,
                        int n, const wxString choices[],
                        long style,
                        const wxValidator& validator,
                        const wxString& name)
{
    if( !CreateControl( parent, id, pos, size, style, validator, name ) )
        return FALSE;

    m_noStrings = n;

    Widget parentWidget = (Widget) parent->GetClientWidget();

    Widget buttonWidget = XtVaCreateManagedWidget(name.c_str(),
        xmComboBoxWidgetClass, parentWidget,
        XmNmarginHeight, 0,
        XmNmarginWidth, 0,
        XmNshowLabel, False,
        XmNeditable, ((style & wxCB_READONLY) != wxCB_READONLY),
        XmNsorted, ((style & wxCB_SORT) == wxCB_SORT),
        XmNstaticList, ((style & wxCB_SIMPLE) == wxCB_SIMPLE),
        NULL);

    int i;
    for (i = 0; i < n; i++)
    {
        wxXmString str( choices[i] );
        XmComboBoxAddItem(buttonWidget, str(), 0);
        m_stringList.Add(choices[i]);
    }

    m_mainWidget = (Widget) buttonWidget;

    XtManageChild (buttonWidget);

    SetValue(value);

    ChangeFont(FALSE);

    XtAddCallback (buttonWidget, XmNselectionCallback, (XtCallbackProc) wxComboBoxCallback,
        (XtPointer) this);
    XtAddCallback (buttonWidget, XmNvalueChangedCallback, (XtCallbackProc) wxComboBoxCallback,
        (XtPointer) this);

    AttachWidget (parent, m_mainWidget, (WXWidget) NULL, pos.x, pos.y, size.x, size.y);

    ChangeBackgroundColour();

    return TRUE;
}

wxComboBox::~wxComboBox()
{
    DetachWidget((Widget) m_mainWidget); // Removes event handlers
    XtDestroyWidget((Widget) m_mainWidget);
    m_mainWidget = (WXWidget) 0;
    if ( HasClientObjectData() )
        m_clientDataDict.DestroyData();
}

void wxComboBox::DoSetSize(int x, int y, int width, int height, int sizeFlags)
{
    // Necessary so it doesn't call wxChoice::SetSize
    wxWindow::DoSetSize(x, y, width, DoGetBestSize().y, sizeFlags);
}

wxString wxComboBox::GetValue() const
{
    char *s = XmComboBoxGetString ((Widget) m_mainWidget);
    if (s)
    {
        wxString str(s);
        XtFree (s);
        return str;
    }
    else
        return wxEmptyString;
}

void wxComboBox::SetValue(const wxString& value)
{
    m_inSetValue = TRUE;
    if( !value.empty() )
        XmComboBoxSetString( (Widget)m_mainWidget,
                             wxConstCast(value.c_str(), char) );
    m_inSetValue = FALSE;
}

void wxComboBox::SetString(int n, const wxString& s)
{
    wxFAIL_MSG( wxT("wxComboBox::SetString only implemented for Motif 2.0") );
}

int wxComboBox::DoAppend(const wxString& item)
{
    wxXmString str( item.c_str() );
    XmComboBoxAddItem((Widget) m_mainWidget, str(), 0);
    m_stringList.Add(item);
    m_noStrings ++;

    return GetCount() - 1;
}

void wxComboBox::Delete(int n)
{
    XmComboBoxDeletePos((Widget) m_mainWidget, n+1);
    wxStringList::Node *node = m_stringList.Item(n);
    if (node)
    {
        delete[] node->GetData();
        delete node;
    }
    m_clientDataDict.Delete(n, HasClientObjectData());
    m_noStrings--;
}

void wxComboBox::Clear()
{
    XmComboBoxDeleteAllItems((Widget) m_mainWidget);
    m_stringList.Clear();

    if ( HasClientObjectData() )
        m_clientDataDict.DestroyData();
    m_noStrings = 0;
}

void wxComboBox::SetSelection (int n)
{
    XmComboBoxSelectPos((Widget) m_mainWidget, n+1, False);
}

int wxComboBox::GetSelection (void) const
{
    int sel = XmComboBoxGetSelectedPos((Widget) m_mainWidget);
    if (sel == 0)
        return -1;
    else
        return sel - 1;
}

wxString wxComboBox::GetString(int n) const
{
    wxStringList::Node *node = m_stringList.Item(n);
    if (node)
        return wxString(node->GetData ());
    else
        return wxEmptyString;
}

int wxComboBox::FindString(const wxString& s) const
{
    int *pos_list = NULL;
    int count = 0;
    wxXmString text( s );
    bool found = (XmComboBoxGetMatchPos((Widget) m_mainWidget,
        text(), &pos_list, &count) != 0);

    if (found && count > 0)
    {
        int pos = pos_list[0] - 1;
        free(pos_list);
        return pos;
    }

    return -1;
}

// Clipboard operations
void wxComboBox::Copy()
{
    XmComboBoxCopy((Widget) m_mainWidget, CurrentTime);
}

void wxComboBox::Cut()
{
    XmComboBoxCut((Widget) m_mainWidget, CurrentTime);
}

void wxComboBox::Paste()
{
    XmComboBoxPaste((Widget) m_mainWidget);
}

void wxComboBox::SetEditable(bool WXUNUSED(editable))
{
    // TODO
}

void wxComboBox::SetInsertionPoint(long pos)
{
    XmComboBoxSetInsertionPosition ((Widget) m_mainWidget, (XmTextPosition) pos);
}

void wxComboBox::SetInsertionPointEnd()
{
    XmTextPosition pos = XmComboBoxGetLastPosition ((Widget) m_mainWidget);
    XmComboBoxSetInsertionPosition ((Widget) m_mainWidget, (XmTextPosition) (pos + 1));
}

long wxComboBox::GetInsertionPoint() const
{
    return (long) XmComboBoxGetInsertionPosition ((Widget) m_mainWidget);
}

long wxComboBox::GetLastPosition() const
{
    return (long) XmComboBoxGetLastPosition ((Widget) m_mainWidget);
}

void wxComboBox::Replace(long from, long to, const wxString& value)
{
    XmComboBoxReplace ((Widget) m_mainWidget, (XmTextPosition) from,
                       (XmTextPosition) to,
                       wxConstCast(value.c_str(), char));
}

void wxComboBox::Remove(long from, long to)
{
    XmComboBoxSetSelection ((Widget) m_mainWidget, (XmTextPosition) from, (XmTextPosition) to,
                      (Time) 0);
    XmComboBoxRemove ((Widget) m_mainWidget);
}

void wxComboBox::SetSelection(long from, long to)
{
    XmComboBoxSetSelection ((Widget) m_mainWidget, (XmTextPosition) from, (XmTextPosition) to,
                      (Time) 0);
}

void  wxComboBoxCallback (Widget WXUNUSED(w), XtPointer clientData,
                          XmComboBoxSelectionCallbackStruct * cbs)
{
    wxComboBox *item = (wxComboBox *) clientData;

    switch (cbs->reason)
    {
    case XmCR_SINGLE_SELECT:
    case XmCR_BROWSE_SELECT:
        {
            wxCommandEvent event (wxEVT_COMMAND_COMBOBOX_SELECTED,
                                  item->GetId());
            event.m_commandInt = cbs->index - 1;
            event.m_commandString = item->GetString (event.m_commandInt);
            if ( item->HasClientObjectData() )
                event.SetClientObject( item->GetClientObject(cbs->index - 1) );
            else if ( item->HasClientUntypedData() )
                event.SetClientData( item->GetClientData(cbs->index - 1) );
            event.m_extraLong = TRUE;
            event.SetEventObject(item);
            item->ProcessCommand (event);
            break;
        }
    case XmCR_VALUE_CHANGED:
        {
            wxCommandEvent event (wxEVT_COMMAND_TEXT_UPDATED, item->GetId());
            event.m_commandInt = -1;
            event.m_commandString = item->GetValue();
            event.m_extraLong = TRUE;
            event.SetEventObject(item);
            item->ProcessCommand (event);
            break;
        }
    default:
        break;
    }
}

void wxComboBox::ChangeFont(bool keepOriginalSize)
{
    // Don't use the base class wxChoice's ChangeFont
    wxWindow::ChangeFont(keepOriginalSize);
}

void wxComboBox::ChangeBackgroundColour()
{
    wxWindow::ChangeBackgroundColour();
}

void wxComboBox::ChangeForegroundColour()
{
    wxWindow::ChangeForegroundColour();
}

wxSize wxComboBox::DoGetBestSize() const
{
    if( (GetWindowStyle() & wxCB_DROPDOWN) == wxCB_DROPDOWN ||
        (GetWindowStyle() & wxCB_READONLY) == wxCB_READONLY )
    {
        wxSize items = GetItemsSize();
        // FIXME arbitrary constants
        return wxSize( ( items.x ? items.x + 50 : 120 ),
                         items.y + 10 );
    }
    else
        return wxWindow::DoGetBestSize();
}

#endif // XmVersion < 2000

#endif // wxUSE_COMBOBOX
