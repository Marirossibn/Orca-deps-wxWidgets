///////////////////////////////////////////////////////////////////////////////
// Name:        wx/aui/toolbar.h
// Purpose:     Documentation of wxAuiToolBar and related classes.
// Created:     2011-01-17
// Copyright:   (c) 2011 wxWidgets development team
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

/**
    wxAuiToolBarStyle is part of the wxAUI class framework, used to define the appearance of a wxAuiToolBar.

    See also @ref overview_aui.

    @library{wxaui}
    @category{aui}
  */
enum wxAuiToolBarStyle
{
    /**
      Shows the text in the toolbar buttons; by default only icons are shown.
    */
    wxAUI_TB_TEXT          = 1 << 0,

    /**
      Don't show tooltips on wxAuiToolBar items.
    */
    wxAUI_TB_NO_TOOLTIPS   = 1 << 1,

    /**
      Do not auto-resize the wxAuiToolBar.
    */
    wxAUI_TB_NO_AUTORESIZE = 1 << 2,

    /**
      Shows a gripper on the wxAuiToolBar.
    */
    wxAUI_TB_GRIPPER       = 1 << 3,

    /**
      The wxAuiToolBar can contain overflow items.
    */
    wxAUI_TB_OVERFLOW      = 1 << 4,

    /**
      Using this style forces the toolbar to be vertical and be only dockable to the left or right sides of the window whereas by default it can be horizontal or vertical and be docked anywhere.
    */
    wxAUI_TB_VERTICAL      = 1 << 5,

    /**
      Shows the text and the icons alongside, not vertically stacked. This style must be used with wxAUI_TB_TEXT
    */
    wxAUI_TB_HORZ_LAYOUT   = 1 << 6,

    /**
      Analogous to wxAUI_TB_VERTICAL, but forces the toolbar to be horizontal, docking to the top or bottom of the window.
    */
    wxAUI_TB_HORIZONTAL    = 1 << 7,


    /**
      Draw a plain background (based on parent) instead of the default gradient background.

      @since 2.9.5
    */
    wxAUI_TB_PLAIN_BACKGROUND = 1 << 8,

    /**
       Shows the text alongside the icons, not vertically stacked.
    */
    wxAUI_TB_HORZ_TEXT     = (wxAUI_TB_HORZ_LAYOUT | wxAUI_TB_TEXT),

    /**
      Shows the text in the toolbar buttons; by default only icons are shown.
    */
    wxAUI_ORIENTATION_MASK = (wxAUI_TB_VERTICAL | wxAUI_TB_HORIZONTAL),

    /**
      By default only icons are shown.
    */
    wxAUI_TB_DEFAULT_STYLE = 0

};

/**
    wxAuiToolBarArtSetting

    @library{wxaui}
    @category{aui}
*/
enum wxAuiToolBarArtSetting
{
    /**
      wxAuiToolBar seperator size.
    */
    wxAUI_TBART_SEPARATOR_SIZE = 0,

    /**
      wxAuiToolBar gripper size.
    */
    wxAUI_TBART_GRIPPER_SIZE = 1,

    /**
      Overflow button size in wxAuiToolBar.
    */
    wxAUI_TBART_OVERFLOW_SIZE = 2

    /**
      Drop down button size in wxAuiToolBar.

      @since 3.1.2
    */
    wxAUI_TBART_DROPDOWN_SIZE = 3

};

/**
  wxAuiToolBarToolTextOrientation

  @library{wxaui}
  @category{aui}
  */
enum wxAuiToolBarToolTextOrientation
{
    /**
      Text in wxAuiToolBar items is left aligned, currently unused/unimplemented.
    */
    wxAUI_TBTOOL_TEXT_LEFT = 0,

    /**
      Text in wxAuiToolBar items is right aligned.
    */
    wxAUI_TBTOOL_TEXT_RIGHT = 1,

    /**
      Text in wxAuiToolBar items is top aligned, currently unused/unimplemented.
    */
    wxAUI_TBTOOL_TEXT_TOP = 2,

    /**
      Text in wxAuiToolBar items is bottom aligned.
    */
    wxAUI_TBTOOL_TEXT_BOTTOM = 3

};


/**
    @class wxAuiToolBarEvent

    wxAuiToolBarEvent is used for the events generated by @ref wxAuiToolBar.

    @library{wxaui}
    @category{aui}
*/
class wxAuiToolBarEvent : public wxNotifyEvent
{
public:
    wxAuiToolBarEvent(wxEventType commandType = wxEVT_NULL,
                      int winId = 0);
    wxAuiToolBarEvent(const wxAuiToolBarEvent& c);

    /**
      Returns whether the drop down menu has been clicked.
    */
    bool IsDropDownClicked() const;

    /**
      Returns the point where the user clicked with the mouse.
    */
    wxPoint GetClickPoint() const;

    /**
      Returns the wxAuiToolBarItem rectangle bounding the mouse click point.
    */
    wxRect GetItemRect() const;

    /**
      Returns the wxAuiToolBarItem identifier.
    */
    int GetToolId() const;


    void SetDropDownClicked(bool c);
    void SetClickPoint(const wxPoint& p);
    void SetItemRect(const wxRect& r);
    void SetToolId(int toolId);
};

wxEventType wxEVT_AUITOOLBAR_TOOL_DROPDOWN;
wxEventType wxEVT_AUITOOLBAR_OVERFLOW_CLICK;
wxEventType wxEVT_AUITOOLBAR_RIGHT_CLICK;
wxEventType wxEVT_AUITOOLBAR_MIDDLE_CLICK;
wxEventType wxEVT_AUITOOLBAR_BEGIN_DRAG;


/**
    @class wxAuiToolBarItem

    wxAuiToolBarItem is part of the wxAUI class framework, representing a toolbar element.

    See also @ref wxAuiToolBar and @ref overview_aui.

    It has a unique id (except for the separators which always have id = -1), the
    style (telling whether it is a normal button, separator or a control), the
    state (toggled or not, enabled or not) and short and long help strings. The
    default implementations use the short help string for the tooltip text which
    is popped up when the mouse pointer enters the tool and the long help string
    for the applications status bar (currently not implemented).

    @library{wxaui}
    @category{aui}
*/
class wxAuiToolBarItem
{
public:

    /**
       Default Constructor
    */
    wxAuiToolBarItem();

    /**
      Assigns the properties of the wxAuiToolBarItem "c" to this.
    */
    wxAuiToolBarItem(const wxAuiToolBarItem& c);

    /**
       Assigns the properties of the wxAuiToolBarItem "c" to this, returning a pointer to this.
    */
    wxAuiToolBarItem& operator=(const wxAuiToolBarItem& c);

    /**
      Assigns the properties of the wxAuiToolBarItem "c" to this.
    */
    void Assign(const wxAuiToolBarItem& c);

    /**
      Assigns a window to the toolbar item.
    */
    void SetWindow(wxWindow* w);
    /**
      Returns the wxWindow* associated to the toolbar item.
    */
    wxWindow* GetWindow();

    /**
      Sets the toolbar item identifier.
    */
    void SetId(int new_id);
    /**
      Returns the toolbar item identifier.
    */
    int GetId() const;

    /**
      Sets the wxAuiToolBarItem kind.
    */
    void SetKind(int new_kind);

    /**
      Returns the toolbar item kind.
    */
    int GetKind() const;

    /**

    */
    void SetState(int new_state);
    /**

    */
    int GetState() const;

    /**

    */
    void SetSizerItem(wxSizerItem* s);
    /**

    */
    wxSizerItem* GetSizerItem() const;

    /**

    */
    void SetLabel(const wxString& s);
    /**

    */
    const wxString& GetLabel() const;

    /**

    */
    void SetBitmap(const wxBitmap& bmp);
    /**

    */
    const wxBitmap& GetBitmap() const;

    /**

    */
    void SetDisabledBitmap(const wxBitmap& bmp);
    /**

    */
    const wxBitmap& GetDisabledBitmap() const;

    /**

    */
    void SetHoverBitmap(const wxBitmap& bmp);
    /**

    */
    const wxBitmap& GetHoverBitmap() const;

    /**

    */
    void SetShortHelp(const wxString& s);
    /**

    */
    const wxString& GetShortHelp() const;

    /**

    */
    void SetLongHelp(const wxString& s);
    /**

    */
    const wxString& GetLongHelp() const;

    /**

    */
    void SetMinSize(const wxSize& s);
    /**

    */
    const wxSize& GetMinSize() const;

    /**

    */
    void SetSpacerPixels(int s);
    /**

    */
    int GetSpacerPixels() const;

    /**

    */
    void SetProportion(int p);
    /**

    */
    int GetProportion() const;

    /**

    */
    void SetActive(bool b);
    /**

    */
    bool IsActive() const;

    /**
        Set whether this tool has a drop down button.

        This is only valid for wxITEM_NORMAL tools.
    */
    void SetHasDropDown(bool b);
    /**
        Returns whether the toolbar item has an associated drop down button.
    */
    bool HasDropDown() const;

    /**

    */
    void SetSticky(bool b);
    /**

    */
    bool IsSticky() const;

    /**

    */
    void SetUserData(long l);
    /**

    */
    long GetUserData() const;

    /**

    */
    void SetAlignment(int l);
    /**

    */
    int GetAlignment() const;
};

/**
    @class wxAuiToolBarArt

    wxAuiToolBarArt is part of the wxAUI class framework.
    See also @ref wxAuiToolBar and @ref overview_aui.

    @library{wxaui}
    @category{aui}
*/
class wxAuiToolBarArt
{
public:
    wxAuiToolBarArt();

    virtual wxAuiToolBarArt* Clone() = 0;
    virtual void SetFlags(unsigned int flags) = 0;
    virtual unsigned int GetFlags() = 0;
    virtual void SetFont(const wxFont& font) = 0;
    virtual wxFont GetFont() = 0;
    virtual void SetTextOrientation(int orientation) = 0;
    virtual int GetTextOrientation() = 0;

    virtual void DrawBackground(
                         wxDC& dc,
                         wxWindow* wnd,
                         const wxRect& rect) = 0;

    virtual void DrawPlainBackground(
                                  wxDC& dc,
                                  wxWindow* wnd,
                                  const wxRect& rect) = 0;

    virtual void DrawLabel(
                         wxDC& dc,
                         wxWindow* wnd,
                         const wxAuiToolBarItem& item,
                         const wxRect& rect) = 0;

    virtual void DrawButton(
                         wxDC& dc,
                         wxWindow* wnd,
                         const wxAuiToolBarItem& item,
                         const wxRect& rect) = 0;

    virtual void DrawDropDownButton(
                         wxDC& dc,
                         wxWindow* wnd,
                         const wxAuiToolBarItem& item,
                         const wxRect& rect) = 0;

    virtual void DrawControlLabel(
                         wxDC& dc,
                         wxWindow* wnd,
                         const wxAuiToolBarItem& item,
                         const wxRect& rect) = 0;

    virtual void DrawSeparator(
                         wxDC& dc,
                         wxWindow* wnd,
                         const wxRect& rect) = 0;

    virtual void DrawGripper(
                         wxDC& dc,
                         wxWindow* wnd,
                         const wxRect& rect) = 0;

    virtual void DrawOverflowButton(
                         wxDC& dc,
                         wxWindow* wnd,
                         const wxRect& rect,
                         int state) = 0;

    virtual wxSize GetLabelSize(
                         wxDC& dc,
                         wxWindow* wnd,
                         const wxAuiToolBarItem& item) = 0;

    virtual wxSize GetToolSize(
                         wxDC& dc,
                         wxWindow* wnd,
                         const wxAuiToolBarItem& item) = 0;

    virtual int GetElementSize(int element_id) = 0;
    virtual void SetElementSize(int element_id, int size) = 0;

    virtual int ShowDropDown(
                         wxWindow* wnd,
                         const wxAuiToolBarItemArray& items) = 0;
};


/**
    @class wxAuiDefaultToolBarArt

    wxAuiDefaultToolBarArt is part of the wxAUI class framework.
    See also @ref wxAuiToolBarArt , @ref wxAuiToolBar and @ref overview_aui.

    @library{wxaui}
    @category{aui}
*/
class wxAuiDefaultToolBarArt : public wxAuiToolBarArt
{
public:
    wxAuiDefaultToolBarArt();
    virtual ~wxAuiDefaultToolBarArt();

    virtual wxAuiToolBarArt* Clone();
    virtual void SetFlags(unsigned int flags);
    virtual unsigned int GetFlags();
    virtual void SetFont(const wxFont& font);
    virtual wxFont GetFont();
    virtual void SetTextOrientation(int orientation);
    virtual int GetTextOrientation();

    virtual void DrawBackground(
                wxDC& dc,
                wxWindow* wnd,
                const wxRect& rect);

    virtual void DrawPlainBackground(wxDC& dc,
                                  wxWindow* wnd,
                                  const wxRect& rect);

    virtual void DrawLabel(
                wxDC& dc,
                wxWindow* wnd,
                const wxAuiToolBarItem& item,
                const wxRect& rect);

    virtual void DrawButton(
                wxDC& dc,
                wxWindow* wnd,
                const wxAuiToolBarItem& item,
                const wxRect& rect);

    virtual void DrawDropDownButton(
                wxDC& dc,
                wxWindow* wnd,
                const wxAuiToolBarItem& item,
                const wxRect& rect);

    virtual void DrawControlLabel(
                wxDC& dc,
                wxWindow* wnd,
                const wxAuiToolBarItem& item,
                const wxRect& rect);

    virtual void DrawSeparator(
                wxDC& dc,
                wxWindow* wnd,
                const wxRect& rect);

    virtual void DrawGripper(
                wxDC& dc,
                wxWindow* wnd,
                const wxRect& rect);

    virtual void DrawOverflowButton(
                wxDC& dc,
                wxWindow* wnd,
                const wxRect& rect,
                int state);

    virtual wxSize GetLabelSize(
                wxDC& dc,
                wxWindow* wnd,
                const wxAuiToolBarItem& item);

    virtual wxSize GetToolSize(
                wxDC& dc,
                wxWindow* wnd,
                const wxAuiToolBarItem& item);

    virtual int GetElementSize(int element);
    virtual void SetElementSize(int element_id, int size);

    virtual int ShowDropDown(wxWindow* wnd,
                             const wxAuiToolBarItemArray& items);
};



/**
    @class wxAuiToolBar

    wxAuiToolBar is a dockable toolbar, part of the wxAUI class framework.
    See also @ref overview_aui.

    The appearance of this class is configurable and can be changed by calling
    wxAuiToolBar::SetArtProvider(). By default, native art provider is used if
    available (currently only in wxMSW) and wxAuiGenericToolBarArt otherwise.

    @beginStyleTable
    @style{wxAUI_TB_TEXT}
    @style{wxAUI_TB_NO_TOOLTIPS}
    @style{wxAUI_TB_NO_AUTORESIZE}
    @style{wxAUI_TB_GRIPPER}
    @style{wxAUI_TB_OVERFLOW}
    @style{wxAUI_TB_VERTICAL}
        using this style forces the toolbar to be vertical and
        be only dockable to the left or right sides of the window
        whereas by default it can be horizontal or vertical and
        be docked anywhere
    @style{wxAUI_TB_HORZ_LAYOUT}
    @style{wxAUI_TB_HORIZONTAL}
        analogous to wxAUI_TB_VERTICAL, but forces the toolbar
        to be horizontal
    @style{wxAUI_TB_PLAIN_BACKGROUND}
        Draw a plain background (based on parent) instead of the
        default gradient background.
    @style{wxAUI_TB_HORZ_TEXT}
        Equivalent to wxAUI_TB_HORZ_LAYOUT | wxAUI_TB_TEXT
    @style{wxAUI_TB_DEFAULT_STYLE}
        The default is to have no styles
    @endStyleTable

    @beginEventEmissionTable{wxAuiToolBarEvent}
    @event{EVT_AUITOOLBAR_TOOL_DROPDOWN(id, func)}
        Process a wxEVT_AUITOOLBAR_TOOL_DROPDOWN event
    @event{EVT_AUITOOLBAR_OVERFLOW_CLICK(id, func)}
        Process a wxEVT_AUITOOLBAR_OVERFLOW_CLICK event
    @event{EVT_AUITOOLBAR_RIGHT_CLICK(id, func)}
        Process a wxEVT_AUITOOLBAR_RIGHT_CLICK event
    @event{EVT_AUITOOLBAR_MIDDLE_CLICK(id, func)}
        Process a wxEVT_AUITOOLBAR_MIDDLE_CLICK event
    @event{EVT_AUITOOLBAR_BEGIN_DRAG(id, func)}
        Process a wxEVT_AUITOOLBAR_BEGIN_DRAG event
    @endEventTable

    @library{wxaui}
    @category{aui}
*/
class wxAuiToolBar : public wxControl
{
public:
    /**
        Default constructor, use Create() later.

        @since 2.9.5
     */
    wxAuiToolBar();

    /**
        Constructor creating and initializing the object.
     */
    wxAuiToolBar(wxWindow* parent,
                 wxWindowID id = wxID_ANY,
                 const wxPoint& position = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = wxAUI_TB_DEFAULT_STYLE);

    /**
        Really create wxAuiToolBar created using default constructor.

        @since 2.9.5
     */
    bool Create(wxWindow* parent,
                wxWindowID id = wxID_ANY,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxAUI_TB_DEFAULT_STYLE);
    virtual ~wxAuiToolBar();

    void SetWindowStyleFlag(long style);
    long GetWindowStyleFlag() const;

    void SetArtProvider(wxAuiToolBarArt* art);
    wxAuiToolBarArt* GetArtProvider() const;

    bool SetFont(const wxFont& font);


    wxAuiToolBarItem* AddTool(int tool_id,
                 const wxString& label,
                 const wxBitmap& bitmap,
                 const wxString& short_help_string = wxEmptyString,
                 wxItemKind kind = wxITEM_NORMAL);

    wxAuiToolBarItem* AddTool(int tool_id,
                 const wxString& label,
                 const wxBitmap& bitmap,
                 const wxBitmap& disabled_bitmap,
                 wxItemKind kind,
                 const wxString& short_help_string,
                 const wxString& long_help_string,
                 wxObject* client_data);

    wxAuiToolBarItem* AddTool(int tool_id,
                 const wxBitmap& bitmap,
                 const wxBitmap& disabled_bitmap,
                 bool toggle = false,
                 wxObject* client_data = NULL,
                 const wxString& short_help_string = wxEmptyString,
                 const wxString& long_help_string = wxEmptyString);

    wxAuiToolBarItem* AddLabel(int tool_id,
                  const wxString& label = wxEmptyString,
                  const int width = -1);
    wxAuiToolBarItem* AddControl(wxControl* control,
                    const wxString& label = wxEmptyString);
    wxAuiToolBarItem* AddSeparator();
    wxAuiToolBarItem* AddSpacer(int pixels);
    wxAuiToolBarItem* AddStretchSpacer(int proportion = 1);

    bool Realize();

    wxControl* FindControl(int window_id);
    wxAuiToolBarItem* FindToolByPosition(wxCoord x, wxCoord y) const;
    wxAuiToolBarItem* FindToolByIndex(int idx) const;
    wxAuiToolBarItem* FindTool(int tool_id) const;

    void ClearTools();
    void Clear();

    /**
        Destroys the tool with the given ID and its associated window, if any.

        @param tool_id ID of a previously added tool.
        @return @true if the tool was destroyed or @false otherwise, e.g. if
            the tool with the given ID was not found.

        @since 3.1.4
     */
    bool DestroyTool(int tool_id);

    /**
        Destroys the tool at the given position and its associated window, if
        any.

        @param idx The index, or position, of a previously added tool.
        @return @true if the tool was destroyed or @false otherwise, e.g. if
            the provided index is out of range.
     */
    bool DestroyToolByIndex(int idx);

    /**
        Removes the tool with the given ID from the toolbar.

        Note that if this tool was added by AddControl(), the associated
        control is @e not deleted and must either be reused (e.g. by
        reparenting it under a different window) or destroyed by caller.
        If this behaviour is unwanted, prefer using DestroyTool() instead.

        @param tool_id ID of a previously added tool.
        @return @true if the tool was removed or @false otherwise, e.g. if the
            tool with the given ID was not found.
     */
    bool DeleteTool(int tool_id);

    /**
        Removes the tool at the given position from the toolbar.

        Note that if this tool was added by AddControl(), the associated
        control is @e not deleted and must either be reused (e.g. by
        reparenting it under a different window) or destroyed by caller.
        If this behaviour is unwanted, prefer using DestroyToolByIndex()
        instead.

        @param idx The index, or position, of a previously added tool.
        @return @true if the tool was removed or @false otherwise, e.g. if the
            provided index is out of range.
     */
    bool DeleteByIndex(int idx);

    size_t GetToolCount() const;
    int GetToolPos(int tool_id) const;
    int GetToolIndex(int tool_id) const;
    bool GetToolFits(int tool_id) const;
    wxRect GetToolRect(int tool_id) const;
    bool GetToolFitsByIndex(int tool_id) const;
    bool GetToolBarFits() const;

    void SetMargins(const wxSize& size);
    void SetMargins(int x, int y);
    void SetMargins(int left, int right, int top, int bottom);

    void SetToolBitmapSize(const wxSize& size);
    wxSize GetToolBitmapSize() const;

    bool GetOverflowVisible() const;
    void SetOverflowVisible(bool visible);

    bool GetGripperVisible() const;
    void SetGripperVisible(bool visible);

    void ToggleTool(int tool_id, bool state);
    bool GetToolToggled(int tool_id) const;

    void EnableTool(int tool_id, bool state);
    bool GetToolEnabled(int tool_id) const;

    /**
        Set whether the specified toolbar item has a drop down button.

        This is only valid for wxITEM_NORMAL tools.

        @see wxAuiToolBarItem::SetHasDropDown()
    */
    void SetToolDropDown(int tool_id, bool dropdown);

    /**
        Returns whether the specified toolbar item has an associated drop down
        button.

        @see wxAuiToolBarItem::HasDropDown()
    */
    bool GetToolDropDown(int tool_id) const;

    void SetToolBorderPadding(int padding);
    int  GetToolBorderPadding() const;

    void SetToolTextOrientation(int orientation);
    int  GetToolTextOrientation() const;

    void SetToolPacking(int packing);
    int  GetToolPacking() const;

    void SetToolProportion(int tool_id, int proportion);
    int  GetToolProportion(int tool_id) const;

    void SetToolSeparation(int separation);
    int GetToolSeparation() const;

    void SetToolSticky(int tool_id, bool sticky);
    bool GetToolSticky(int tool_id) const;

    wxString GetToolLabel(int tool_id) const;
    void SetToolLabel(int tool_id, const wxString& label);

    wxBitmap GetToolBitmap(int tool_id) const;
    void SetToolBitmap(int tool_id, const wxBitmap& bitmap);

    wxString GetToolShortHelp(int tool_id) const;
    void SetToolShortHelp(int tool_id, const wxString& help_string);

    wxString GetToolLongHelp(int tool_id) const;
    void SetToolLongHelp(int tool_id, const wxString& help_string);

    void SetCustomOverflowItems(const wxAuiToolBarItemArray& prepend,
                                const wxAuiToolBarItemArray& append);

    /** get size of hint rectangle for a particular dock location */
    wxSize GetHintSize(int dock_direction) const;
    bool IsPaneValid(const wxAuiPaneInfo& pane) const;
};

