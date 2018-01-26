///////////////////////////////////////////////////////////////////////////////
// Name:        src/gtk/textentry.cpp
// Purpose:     wxTextEntry implementation for wxGTK
// Author:      Vadim Zeitlin
// Created:     2007-09-24
// Copyright:   (c) 2007 Vadim Zeitlin <vadim@wxwindows.org>
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

#if wxUSE_TEXTCTRL || wxUSE_COMBOBOX

#ifndef WX_PRECOMP
    #include "wx/event.h"
    #include "wx/textentry.h"
    #include "wx/textctrl.h"
    #include "wx/textcompleter.h"
    #include "wx/window.h"
#endif //WX_PRECOMP

#include <gtk/gtk.h>
#include "wx/gtk/private.h"
#include "wx/gtk/private/gtk2-compat.h"
#include "wx/gtk/private/object.h"
#include "wx/gtk/private/string.h"

//-----------------------------------------------------------------------------
//  helper function to get the length of the text
//-----------------------------------------------------------------------------

static unsigned int GetEntryTextLength(GtkEntry* entry)
{
#if GTK_CHECK_VERSION(2, 14, 0)
    if ( wx_is_at_least_gtk2(14) )
    {
        return gtk_entry_get_text_length(entry);
    }
#endif // GTK+ 2.14+

    return strlen(gtk_entry_get_text(entry));
}

// ============================================================================
// signal handlers implementation
// ============================================================================

// "insert_text" handler for GtkEntry
extern "C" {
static void
wx_gtk_insert_text_callback(GtkEditable *editable,
                            const gchar * new_text,
                            gint new_text_length,
                            gint * position,
                            wxTextEntry *text)
{
    GtkEntry *entry = GTK_ENTRY (editable);

#if GTK_CHECK_VERSION(3,0,0) || defined(GSEAL_ENABLE)
    const int text_max_length = gtk_entry_buffer_get_max_length(gtk_entry_get_buffer(entry));
#else
    const int text_max_length = entry->text_max_length;
#endif

    bool handled = false;

    // check that we don't overflow the max length limit if we have it
    if ( text_max_length )
    {
        const int text_length = GetEntryTextLength(entry);

        // We can't use new_text_length as it is in bytes while we want to count
        // characters (in first approximation, anyhow...).
        if ( text_length + g_utf8_strlen(new_text, -1) > text_max_length )
        {
            // Prevent the new text from being inserted.
            handled = true;

            // Currently we don't insert anything at all, but it would be better to
            // insert as many characters as would fit into the text control and
            // only discard the rest.

            // Notify the user code about overflow.
            text->SendMaxLenEvent();
        }
    }

    // Check if we have to convert all input to upper-case
    if ( !handled && text->GTKIsUpperCase() )
    {
        const wxGtkString upper(g_utf8_strup(new_text, new_text_length));

        // Use the converted text to generate events
        if ( !text->GTKEntryOnInsertText(upper) )
        {
            // Event not handled, so do insert the text: we have to do it
            // ourselves to use the upper-case version of it

            // Prevent recursive call to this handler again
            g_signal_handlers_block_by_func
            (
                editable,
                (gpointer)wx_gtk_insert_text_callback,
                text
            );

            gtk_editable_insert_text(editable, upper, strlen(upper), position);

            g_signal_handlers_unblock_by_func
            (
                editable,
                (gpointer)wx_gtk_insert_text_callback,
                text
            );
        }

        // Don't call the default handler in any case, either the event was
        // handled in the user code or we've already inserted the text.
        handled = true;
    }

    if ( !handled && text->GTKEntryOnInsertText(new_text) )
    {
        // If we already handled the new text insertion, don't do it again.
        handled = true;
    }

    if ( handled )
    {
        // We must update the position to point after the newly inserted text,
        // as expected by GTK+.
        *position = text->GetInsertionPoint();

        g_signal_stop_emission_by_name (editable, "insert_text");
    }
}
}

//-----------------------------------------------------------------------------
//  clipboard events: "copy-clipboard", "cut-clipboard", "paste-clipboard"
//-----------------------------------------------------------------------------

// common part of the event handlers below
static void
DoHandleClipboardCallback( GtkWidget *widget,
                           wxWindow *win,
                           wxEventType eventType,
                           const gchar* signal_name)
{
    wxClipboardTextEvent event( eventType, win->GetId() );
    event.SetEventObject( win );
    if ( win->HandleWindowEvent( event ) )
    {
        // don't let the default processing to take place if we did something
        // ourselves in the event handler
        g_signal_stop_emission_by_name (widget, signal_name);
    }
}

extern "C"
{

static void
wx_gtk_copy_clipboard_callback( GtkWidget *widget, wxWindow *win )
{
    DoHandleClipboardCallback(
        widget, win, wxEVT_TEXT_COPY, "copy-clipboard" );
}

static void
wx_gtk_cut_clipboard_callback( GtkWidget *widget, wxWindow *win )
{
    DoHandleClipboardCallback(
        widget, win, wxEVT_TEXT_CUT, "cut-clipboard" );
}

static void
wx_gtk_paste_clipboard_callback( GtkWidget *widget, wxWindow *win )
{
    DoHandleClipboardCallback(
        widget, win, wxEVT_TEXT_PASTE, "paste-clipboard" );
}

} // extern "C"

// This class gathers the all auto-complete-related stuff we use. It is
// allocated on demand by wxTextEntry when AutoComplete() is called.
//
// GTK already has completion functionality support for a GtkEntry via
// GtkEntryCompletion. This class simply forwards to GtkListStore
// in case we used ChangeStrings() overload, or to wxTextCompleter
// associated with it if ChangeCustomCompleter() was called.
class wxTextAutoCompleteData
{
public:
    // Factory function, may return NULL if entry is invalid.
    static wxTextAutoCompleteData* New(wxTextEntry *entry)
    {
        if ( !GTK_IS_ENTRY(entry->GetEntry()) )
        {
            // This is probably a multiline wxTextCtrl which doesn't have any
            // associated GtkEntry.
            return NULL;
        }

        return new wxTextAutoCompleteData(entry);
    }

    ~wxTextAutoCompleteData()
    {
        delete m_completer;
    }

    void ChangeStrings(const wxArrayString& strings)
    {
        wxDELETE( m_completer );

        DoUpdateCompleterType();

        DoEnableCompletion();

        wxGtkObject<GtkListStore> store(gtk_list_store_new (1, G_TYPE_STRING));

        for ( wxArrayString::const_iterator i = strings.begin();
              i != strings.end();
              ++i )
        {
            AppendToStore(store, *i);
        }

        UseModel(store);
    }

    // Takes ownership of the pointer if it is non-NULL.
    bool ChangeCustomCompleter(wxTextCompleter *completer)
    {
        delete m_completer;
        m_completer = completer;

        if ( m_completer )
        {
            wxTextCompleterFixed* fixedCompl =
                dynamic_cast<wxTextCompleterFixed*>(m_completer);

            if ( fixedCompl )
            {
                wxArrayString completions;
                fixedCompl->GetCompletions(wxEmptyString, completions);

                ChangeStrings(completions);

                wxDELETE(m_completer);
                return true;
            }

            DoEnableCompletion();

            DoUpdateCompletionModel();
        }
        else
        {
            DisableCompletion();
        }

        DoUpdateCompleterType();

        return true;
    }

    void DisableCompletion()
    {
        gtk_entry_set_completion (GetGtkEntry(), NULL);

        wxDELETE(m_completer);
        DoUpdateCompleterType();
    }

private:
    // Ctor is private, use New() to create objects of this type.
    explicit wxTextAutoCompleteData(wxTextEntry *entry)
        : m_entry(entry)
    {
        m_completer = NULL;

        m_isDynamicCompleter = false;

        m_newCompletionsNeeded = m_entry->IsEmpty();
    }

    // Helper function for appending a string to GtkListStore.
    void AppendToStore(GtkListStore* store, const wxString& s)
    {
        GtkTreeIter iter;
        gtk_list_store_append (store, &iter);
        gtk_list_store_set (store, &iter, 0, (const gchar *)s.utf8_str(), -1);
    }

    void DoEnableCompletion()
    {
        if ( !gtk_entry_get_completion (GetGtkEntry()) )
        {
            GtkEntryCompletion * const completion = gtk_entry_completion_new();

            gtk_entry_completion_set_text_column (completion, 0);
            gtk_entry_set_completion (GetGtkEntry(), completion);
        }
    }

    // for a given prefix, if DoUpdateCompletionModel() succeeds,
    // we won't do any further update of the model as long as we
    // do not clear the textentry. but then we have to start over again.

    void OnEntryChanged( wxCommandEvent& event )
    {
        if ( event.GetString().empty() )
        {
            m_newCompletionsNeeded = true;
        }
        else
        {
            if ( m_newCompletionsNeeded )
                DoUpdateCompletionModel();
        }

        event.Skip();
    }

    void DoUpdateCompleterType()
    {
        const bool isDynamic = (m_completer != NULL);

        if ( m_isDynamicCompleter == isDynamic )
            // we already connected/disconnected to/from
            // the event handler.
            return;

        m_isDynamicCompleter = isDynamic;

        wxWindow * const win = m_entry->GetEditableWindow();

        // Disconnect from the event handler if we request
        // a non-dynamic behaviour of our completion methods
        // (e.g. completions are supplied via
        //       ChangeStrings() or wxTextCompleterFixed )
        // Connect otherwise.
        //
        // The event handler role is to request from the m_completer
        // to generate dynamically new completions (e.g from database)
        // if a certain condition is met (e.g. textentry is cleared
        // and/or typed in text length >= *MIN_PREFIX_LENGTH* )
        //

        if ( !m_completer )
        {
            win->Unbind(wxEVT_TEXT, &wxTextAutoCompleteData::OnEntryChanged, this);
        }
        else
        {
            win->Bind(wxEVT_TEXT, &wxTextAutoCompleteData::OnEntryChanged, this);
        }
    }

    // Really change the completion model (which may be NULL).
    void UseModel(GtkListStore* store)
    {
        GtkEntryCompletion* const c = gtk_entry_get_completion (GetGtkEntry());
        gtk_entry_completion_set_model (c, GTK_TREE_MODEL(store));
        gtk_entry_completion_complete (c);
    }

    // Recreate the model to contain all completions for the current prefix.
    //
    // This should only be called when using a custom completer.
    void DoUpdateCompletionModel()
    {
        wxASSERT_MSG( m_completer, "m_completer should not be null." );

        const wxString prefix = m_entry->GetValue();

        if ( m_completer->Start(prefix) )
        {
            wxGtkObject<GtkListStore> store(gtk_list_store_new (1, G_TYPE_STRING));

            for (;;)
            {
                const wxString s = m_completer->GetNext();
                if ( s.empty() )
                    break;

                AppendToStore(store, s);
            }

            UseModel(store);

            m_newCompletionsNeeded = false;
        }
        else
        {
            UseModel(NULL);
        }
    }


    GtkEntry* GetGtkEntry() const { return m_entry->GetEntry(); }


    // The text entry we're associated with.
    wxTextEntry * const m_entry;

    // Custom completer or NULL if none.
    wxTextCompleter *m_completer;

    // helps to decide if we should connect/disconnect
    // to/from the event handler.
    bool m_isDynamicCompleter;

    // Each time we enter a new prefix, GtkEntryCompletion needs to be fed with
    // new completions. And this flag lets us try to DoUpdateCompletionModel()
    // and if it succeeds, it'll set the flag to false and OnEntryChanged()
    // will not try to call it again unless we entered a new prefix.
    bool m_newCompletionsNeeded;

    wxDECLARE_NO_COPY_CLASS(wxTextAutoCompleteData);
};


// ============================================================================
// wxTextEntry implementation
// ============================================================================

// ----------------------------------------------------------------------------
// initialization and destruction
// ----------------------------------------------------------------------------

wxTextEntry::wxTextEntry()
{
    m_autoCompleteData = NULL;
    m_isUpperCase = false;
}

wxTextEntry::~wxTextEntry()
{
    delete m_autoCompleteData;
}

// ----------------------------------------------------------------------------
// text operations
// ----------------------------------------------------------------------------

void wxTextEntry::WriteText(const wxString& value)
{
    GtkEditable * const edit = GetEditable();

    // remove the selection if there is one and suppress the text change event
    // generated by this: we only want to generate one event for this change,
    // not two
    {
        EventsSuppressor noevents(this);
        gtk_editable_delete_selection(edit);
    }

    // insert new text at the cursor position
    gint len = gtk_editable_get_position(edit);
    gtk_editable_insert_text
    (
        edit,
        wxGTK_CONV_FONT(value, GetEditableWindow()->GetFont()),
        -1,     // text: length: compute it using strlen()
        &len    // will be updated to position after the text end
    );

    // and move cursor to the end of new text
    gtk_editable_set_position(edit, len);
}

void wxTextEntry::DoSetValue(const wxString& value, int flags)
{
    if (value != DoGetValue())
    {
        // Use Remove() rather than SelectAll() to avoid unnecessary clipboard
        // operations, and prevent triggering an apparent bug in GTK which
        // causes the subsequent WriteText() to append rather than overwrite.
        {
            EventsSuppressor noevents(this);
            Remove(0, -1);
        }
        EventsSuppressor noeventsIf(this, !(flags & SetValue_SendEvent));
        WriteText(value);
    }
    else if (flags & SetValue_SendEvent)
        SendTextUpdatedEvent(GetEditableWindow());

    SetInsertionPoint(0);
}

wxString wxTextEntry::DoGetValue() const
{
    const wxGtkString value(gtk_editable_get_chars(GetEditable(), 0, -1));

    return wxGTK_CONV_BACK_FONT(value,
            const_cast<wxTextEntry *>(this)->GetEditableWindow()->GetFont());
}

void wxTextEntry::Remove(long from, long to)
{
    gtk_editable_delete_text(GetEditable(), from, to);
}

// static
unsigned int wxTextEntry::GTKGetEntryTextLength(GtkEntry* entry)
{
    return GetEntryTextLength(entry);
}

// ----------------------------------------------------------------------------
// clipboard operations
// ----------------------------------------------------------------------------

void wxTextEntry::GTKConnectClipboardSignals(GtkWidget* entry)
{
    g_signal_connect(entry, "copy-clipboard",
                     G_CALLBACK (wx_gtk_copy_clipboard_callback),
                     GetEditableWindow());
    g_signal_connect(entry, "cut-clipboard",
                     G_CALLBACK (wx_gtk_cut_clipboard_callback),
                     GetEditableWindow());
    g_signal_connect(entry, "paste-clipboard",
                     G_CALLBACK (wx_gtk_paste_clipboard_callback),
                     GetEditableWindow());
}

void wxTextEntry::Copy()
{
    gtk_editable_copy_clipboard(GetEditable());
}

void wxTextEntry::Cut()
{
    gtk_editable_cut_clipboard(GetEditable());
}

void wxTextEntry::Paste()
{
    gtk_editable_paste_clipboard(GetEditable());
}

// ----------------------------------------------------------------------------
// undo/redo
// ----------------------------------------------------------------------------

void wxTextEntry::Undo()
{
    // TODO: not implemented
}

void wxTextEntry::Redo()
{
    // TODO: not implemented
}

bool wxTextEntry::CanUndo() const
{
    return false;
}

bool wxTextEntry::CanRedo() const
{
    return false;
}

// ----------------------------------------------------------------------------
// insertion point
// ----------------------------------------------------------------------------

void wxTextEntry::SetInsertionPoint(long pos)
{
    gtk_editable_set_position(GetEditable(), pos);
}

long wxTextEntry::GetInsertionPoint() const
{
    return gtk_editable_get_position(GetEditable());
}

long wxTextEntry::GetLastPosition() const
{
    // this can't be implemented for arbitrary GtkEditable so only do it for
    // GtkEntries
    long pos = -1;
    GtkEntry* entry = (GtkEntry*)GetEditable();
    if (GTK_IS_ENTRY(entry))
        pos = GetEntryTextLength(entry);

    return pos;
}

// ----------------------------------------------------------------------------
// selection
// ----------------------------------------------------------------------------

void wxTextEntry::SetSelection(long from, long to)
{
    // in wx convention, (-1, -1) means the entire range but GTK+ translates -1
    // (or any negative number for that matter) into last position so we need
    // to translate manually
    if ( from == -1 && to == -1 )
        from = 0;

    // for compatibility with MSW, exchange from and to parameters so that the
    // insertion point is set to the start of the selection and not its end as
    // GTK+ does by default
    gtk_editable_select_region(GetEditable(), to, from);

#ifndef __WXGTK3__
    // avoid reported problem with RHEL 5 GTK+ 2.10 where selection is reset by
    // a clipboard callback, see #13277
    if (!wx_is_at_least_gtk2(12))
    {
        GtkEntry* entry = GTK_ENTRY(GetEditable());
        if (to < 0)
            to = entry->text_length;
        entry->selection_bound = to;
    }
#endif
}

void wxTextEntry::GetSelection(long *from, long *to) const
{
    gint start, end;
    if ( gtk_editable_get_selection_bounds(GetEditable(), &start, &end) )
    {
        // the output must always be in order, although in GTK+ it isn't
        if ( start > end )
        {
            gint tmp = start;
            start = end;
            end = tmp;
        }
    }
    else // no selection
    {
        // for compatibility with MSW return the empty selection at cursor
        start =
        end = GetInsertionPoint();
    }

    if ( from )
        *from = start;

    if ( to )
        *to = end;
}

// ----------------------------------------------------------------------------
// auto completion
// ----------------------------------------------------------------------------

wxTextAutoCompleteData *wxTextEntry::GetOrCreateCompleter()
{
    if ( !m_autoCompleteData )
    {
        m_autoCompleteData = wxTextAutoCompleteData::New(this);
    }

    return m_autoCompleteData;
}

bool wxTextEntry::DoAutoCompleteStrings(const wxArrayString& choices)
{
    wxTextAutoCompleteData * const ac = GetOrCreateCompleter();
    if ( !ac )
        return false;

    ac->ChangeStrings(choices);

    return true;
}

bool wxTextEntry::DoAutoCompleteCustom(wxTextCompleter *completer)
{
    // First deal with the case when we just want to disable auto-completion.
    if ( !completer )
    {
        if ( m_autoCompleteData )
            m_autoCompleteData->DisableCompletion();
        //else: Nothing to do, we hadn't used auto-completion even before.
    }
    else // Have a valid completer.
    {
        wxTextAutoCompleteData * const ac = GetOrCreateCompleter();
        if ( !ac )
        {
            // Delete the custom completer for consistency with the case when
            // we succeed to avoid memory leaks in user code.
            delete completer;
            return false;
        }

        // This gives ownership of the custom completer to m_autoCompleteData.
        if ( !ac->ChangeCustomCompleter(completer) )
            return false;
    }

    return true;
}
// ----------------------------------------------------------------------------
// editable status
// ----------------------------------------------------------------------------

bool wxTextEntry::IsEditable() const
{
    return gtk_editable_get_editable(GetEditable()) != 0;
}

void wxTextEntry::SetEditable(bool editable)
{
    gtk_editable_set_editable(GetEditable(), editable);
}

// ----------------------------------------------------------------------------
// input restrictions
// ----------------------------------------------------------------------------

void wxTextEntry::SetMaxLength(unsigned long len)
{
    GtkEntry* const entry = (GtkEntry*)GetEditable();
    if (!GTK_IS_ENTRY(entry))
        return;

    gtk_entry_set_max_length(entry, len);
}

void wxTextEntry::SendMaxLenEvent()
{
    // remember that the next changed signal is to be ignored to avoid
    // generating a dummy wxEVT_TEXT event
    //IgnoreNextTextUpdate();

    wxWindow * const win = GetEditableWindow();
    wxCommandEvent event(wxEVT_TEXT_MAXLEN, win->GetId());
    event.SetEventObject(win);
    event.SetString(GetValue());
    win->HandleWindowEvent(event);
}

void wxTextEntry::ForceUpper()
{
    if ( !m_isUpperCase )
    {
        ConvertToUpperCase();

        m_isUpperCase = true;
    }
}

// ----------------------------------------------------------------------------
// IM handling
// ----------------------------------------------------------------------------

int wxTextEntry::GTKIMFilterKeypress(GdkEventKey* event) const
{
    int result = false;
#if GTK_CHECK_VERSION(2, 22, 0)
    if (wx_is_at_least_gtk2(22))
    {
        result = gtk_entry_im_context_filter_keypress(GetEntry(), event);
    }
#else // GTK+ < 2.22
    wxUnusedVar(event);
#endif // GTK+ 2.22+

    return result;
}

void wxTextEntry::GTKConnectInsertTextSignal(GtkEntry* entry)
{
    g_signal_connect(entry, "insert_text",
                     G_CALLBACK(wx_gtk_insert_text_callback), this);
}

bool wxTextEntry::GTKEntryOnInsertText(const char* text)
{
    return GetEditableWindow()->GTKOnInsertText(text);
}

// ----------------------------------------------------------------------------
// margins support
// ----------------------------------------------------------------------------

bool wxTextEntry::DoSetMargins(const wxPoint& margins)
{
#if GTK_CHECK_VERSION(2,10,0) && !defined(__WXGTK4__)
    GtkEntry* entry = GetEntry();

    if ( !entry )
        return false;
    if ( !wx_is_at_least_gtk2(10) )
        return false;

    wxGCC_WARNING_SUPPRESS(deprecated-declarations)
    const GtkBorder* oldBorder = gtk_entry_get_inner_border(entry);
    GtkBorder newBorder;

    if ( oldBorder )
        newBorder = *oldBorder;
    else
    {
        // Use some reasonable defaults for initial margins
        newBorder.left = 2;
        newBorder.right = 2;

        // These numbers seem to let the text remain vertically centered
        // in common use scenarios when margins.y == -1.
        newBorder.top = 3;
        newBorder.bottom = 3;
    }

    if ( margins.x != -1 )
        newBorder.left = margins.x;

    if ( margins.y != -1 )
        newBorder.top = margins.y;

    gtk_entry_set_inner_border(entry, &newBorder);
    wxGCC_WARNING_RESTORE()

    return true;
#else
    wxUnusedVar(margins);
    return false;
#endif
}

wxPoint wxTextEntry::DoGetMargins() const
{
    wxPoint point(-1, -1);
#if GTK_CHECK_VERSION(2,10,0) && !defined(__WXGTK4__)
    GtkEntry* entry = GetEntry();
    if (entry)
    {
        if (wx_is_at_least_gtk2(10))
        {
            wxGCC_WARNING_SUPPRESS(deprecated-declarations)
            const GtkBorder* border = gtk_entry_get_inner_border(entry);
            wxGCC_WARNING_RESTORE()
            if (border)
            {
                point.x = border->left;
                point.y = border->top;
            }
        }
    }
#endif
    return point;
}

#ifdef __WXGTK3__
bool wxTextEntry::SetHint(const wxString& hint)
{
#if GTK_CHECK_VERSION(3,2,0)
    GtkEntry *entry = GetEntry();
    if (entry && gtk_check_version(3,2,0) == NULL)
    {
        gtk_entry_set_placeholder_text
        (
            entry,
            wxGTK_CONV_FONT(hint, GetEditableWindow()->GetFont())
        );
        return true;
    }
#endif
    return wxTextEntryBase::SetHint(hint);
}

wxString wxTextEntry::GetHint() const
{
#if GTK_CHECK_VERSION(3,2,0)
    GtkEntry *entry = GetEntry();
    if (entry && gtk_check_version(3,2,0) == NULL)
    {
        return wxGTK_CONV_BACK_FONT
               (
                gtk_entry_get_placeholder_text(entry),
                const_cast<wxTextEntry *>(this)->GetEditableWindow()->GetFont()
               );
    }
#endif
    return wxTextEntryBase::GetHint();
}
#endif // __WXGTK3__

#endif // wxUSE_TEXTCTRL || wxUSE_COMBOBOX
