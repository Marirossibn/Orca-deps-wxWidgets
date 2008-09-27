/////////////////////////////////////////////////////////////////////////////
// Name:        xrc/xmlres.h
// Purpose:     interface of wxXmlResource
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxXmlResource

    This is the main class for interacting with the XML-based resource system.

    The class holds XML resources from one or more .xml files, binary files or zip
    archive files.

    @see @ref overview_xrc, @ref xrc_format

    @library{wxxrc}
    @category{xrc}
*/
class wxXmlResource : public wxObject
{
public:
    //@{
    /**
        Constructor.

        @param flags
            wxXRC_USE_LOCALE: translatable strings will be translated via _().
            wxXRC_NO_SUBCLASSING: subclass property of object nodes will be ignored
            (useful for previews in XRC editors). wxXRC_NO_RELOADING will prevent the
            XRC files from being reloaded from disk in case they have been modified
        there
            since being last loaded (may slightly speed up loading them).
        @param domain
            The name of the gettext catalog to search for
              translatable strings.  By default all loaded catalogs will be
              searched.  This provides a way to allow the strings to only come
              from a specific catalog.
    */
    wxXmlResource(const wxString& filemask,
                  int flags = wxXRC_USE_LOCALE,
                  const wxString domain = wxEmptyString);
    wxXmlResource(int flags = wxXRC_USE_LOCALE,
                  const wxString domain = wxEmptyString);
    //@}

    /**
        Destructor.
    */
    virtual ~wxXmlResource();

    /**
        Initializes only a specific handler (or custom handler). Convention says
        that the handler name is equal to the control's name plus 'XmlHandler', for
        example
        wxTextCtrlXmlHandler, wxHtmlWindowXmlHandler. The XML resource compiler
        (wxxrc) can create include file that contains initialization code for
        all controls used within the resource. Note that this handler should be
        allocated on the heap, since it will be delete by
        ClearHandlers() later.
    */
    void AddHandler(wxXmlResourceHandler* handler);

    /**
        Attaches an unknown control to the given panel/window/dialog.
        Unknown controls are used in conjunction with object class="unknown".
    */
    bool AttachUnknownControl(const wxString& name,
                              wxWindow* control,
                              wxWindow* parent = NULL);

    /**
        Removes all handlers and deletes them (this means that any handlers added using
        AddHandler() must be allocated on the heap).
    */
    void ClearHandlers();

    /**
        Compares the XRC version to the argument. Returns -1 if the XRC version
        is less than the argument, +1 if greater, and 0 if they are equal.
    */
    int CompareVersion(int major, int minor, int release,
                       int revision) const;

    /**
        Gets the global resources object or creates one if none exists.
    */
    static wxXmlResource* Get();

    /**
        Returns the domain (message catalog) that will be used to load
        translatable strings in the XRC.
    */
    wxChar* GetDomain();

    /**
        Returns flags, which may be a bitlist of wxXRC_USE_LOCALE and
        wxXRC_NO_SUBCLASSING.
    */
    int GetFlags() const;

    /**
        Returns version information (a.b.c.d = d+ 256*c + 256@c 2*b + 256@c 3*a).
    */
    long GetVersion() const;

    /**
        Returns a numeric ID that is equivalent to the string ID used in an XML
        resource. If an unknown @a str_id is requested (i.e. other than wxID_XXX
        or integer), a new record is created which associates the given string with
        a number. If @a value_if_not_found is @c wxID_NONE, the number is obtained via
        wxNewId(). Otherwise @a value_if_not_found is used.
        Macro @c XRCID(name) is provided for convenient use in event tables.
    */
#define int GetXRCID(const wxString& str_id, int value_if_not_found = -2)     /* implementation is private */

    /**
        Initializes handlers for all supported controls/windows. This will
        make the executable quite big because it forces linking against
        most of the wxWidgets library.
    */
    void InitAllHandlers();

    /**
        Loads resources from XML files that match given filemask.
        This method understands VFS (see filesys.h).
    */
    bool Load(const wxString& filemask);

    /**
        Loads a bitmap resource from a file.
    */
    wxBitmap LoadBitmap(const wxString& name);

    //@{
    /**
        Loads a dialog. @a dlg points to parent window (if any).
        This form is used to finish creation of an already existing instance (the main
        reason
        for this is that you may want to use derived class with a new event table).
        Example:
    */
    wxDialog* LoadDialog(wxWindow* parent, const wxString& name);
    bool LoadDialog(wxDialog* dlg, wxWindow* parent,
                    const wxString& name);
    //@}

    /**
        Loads a frame.
    */
    bool LoadFrame(wxFrame* frame, wxWindow* parent,
                   const wxString& name);

    /**
        Loads an icon resource from a file.
    */
    wxIcon LoadIcon(const wxString& name);

    /**
        Loads menu from resource. Returns @NULL on failure.
    */
    wxMenu* LoadMenu(const wxString& name);

    //@{
    /**
        Loads a menubar from resource. Returns @NULL on failure.
    */
    wxMenuBar* LoadMenuBar(wxWindow* parent, const wxString& name);
    wxMenuBar* LoadMenuBar(const wxString& name);
    //@}

    //@{
    /**
        Load an object from the resource specifying both the resource name and the
        class name.
        The first overload lets you load nonstandard container windows and returns @c
        @NULL
        on failure. The second one lets you finish the creation of an existing
        instance and returns @false on failure.
    */
    wxObject* LoadObject(wxWindow* parent, const wxString& name,
                         const wxString& classname);
    bool LoadObject(wxObject* instance, wxWindow* parent,
                    const wxString& name,
                    const wxString& classname);
    //@}

    //@{
    /**
        Loads a panel. @a panel points to parent window (if any). This form
        is used to finish creation of an already existing instance.
    */
    wxPanel* LoadPanel(wxWindow* parent, const wxString& name);
    bool LoadPanel(wxPanel* panel, wxWindow* parent,
                   const wxString& name);
    //@}

    /**
        Loads a toolbar.
    */
    wxToolBar* LoadToolBar(wxWindow* parent, const wxString& name);

    /**
        Sets the global resources object and returns a pointer to the previous one (may
        be @NULL).
    */
    static wxXmlResource* Set(wxXmlResource* res);

    /**
        Sets the domain (message catalog) that will be used to load
        translatable strings in the XRC.
    */
    wxChar* SetDomain(const wxChar* domain);

    /**
        Sets flags (bitlist of wxXRC_USE_LOCALE and wxXRC_NO_SUBCLASSING).
    */
    void SetFlags(int flags);

    /**
        This function unloads a resource previously loaded by
        Load().
        Returns @true if the resource was successfully unloaded and @false if it
        hasn't
        been found in the list of loaded resources.
    */
    bool Unload(const wxString& filename);
};



/**
    @class wxXmlResourceHandler

    wxXmlResourceHandler is an abstract base class for resource handlers
    capable of creating a control from an XML node.

    See @ref overview_xrc for details.

    @library{wxxrc}
    @category{xrc}
*/
class wxXmlResourceHandler : public wxObject
{
public:
    /**
        Default constructor.
    */
    wxXmlResourceHandler();

    /**
        Destructor.
    */
    virtual ~wxXmlResourceHandler();

    /**
        Add a style flag (e.g. wxMB_DOCKABLE) to the list of flags
        understood by this handler.
    */
    void AddStyle(const wxString& name, int value);

    /**
        Add styles common to all wxWindow-derived classes.
    */
    void AddWindowStyles();

    /**
        Returns @true if it understands this node and can create
        a resource from it, @false otherwise.
    */
    bool CanHandle(wxXmlNode* node);

    /**
        Creates children.
    */
    void CreateChildren(wxObject* parent, bool this_hnd_only = false);

    /**
        Helper function.
    */
    void CreateChildrenPrivately(wxObject* parent,
                                 wxXmlNode* rootnode = NULL);

    /**
        Creates a resource from a node.
    */
    wxObject* CreateResFromNode(wxXmlNode* node, wxObject* parent,
                                wxObject* instance = NULL);

    /**
        Creates an object (menu, dialog, control, ...) from an XML node.
        Should check for validity. @a parent is a higher-level object
        (usually window, dialog or panel) that is often necessary to create the resource.

        If @b instance is non-@NULL it should not create a new instance via 'new' but
        should rather use this one, and call its Create method.
    */
    wxObject* CreateResource(wxXmlNode* node, wxObject* parent,
                             wxObject* instance);

    /**
        Called from CreateResource after variables
        were filled.
    */
    wxObject* DoCreateResource();

    /**
        )
        Creates a animation() from the filename specified in @e param.
    */
    wxAnimation GetAnimation();

    /**
        , @b wxSize@e size = wxDefaultSize)
        Gets a bitmap.
    */
    wxBitmap GetBitmap();

    /**
        Gets a bool flag (1, t, yes, on, true are @true, everything else is @false).
    */
    bool GetBool(const wxString& param, bool defaultv = false);

    /**
        Gets colour in HTML syntax (\#RRGGBB).
    */
    wxColour GetColour(const wxString& param,
                   const wxColour& default = wxNullColour);

    /**
        Returns the current file system.
    */
    wxFileSystem GetCurFileSystem();

    /**
        Gets a dimension (may be in dialog units).
    */
    wxCoord GetDimension(const wxString& param, wxCoord defaultv = 0);

    /**
        )
        Gets a font.
    */
    wxFont GetFont();

    /**
        Returns the XRCID.
    */
    int GetID();

    /**
        , @b wxSize@e size = wxDefaultSize)
        Returns an icon.
    */
    wxIcon GetIcon();

    /**
        Gets the integer value from the parameter.
    */
    long GetLong(const wxString& param, long defaultv = 0);

    /**
        Returns the resource name.
    */
    wxString GetName();

    /**
        Gets node content from wxXML_ENTITY_NODE.
    */
    wxString GetNodeContent(wxXmlNode* node);

    /**
        Finds the node or returns @NULL.
    */
    wxXmlNode* GetParamNode(const wxString& param);

    /**
        Finds the parameter value or returns the empty string.
    */
    wxString GetParamValue(const wxString& param);

    /**
        )
        Gets the position (may be in dialog units).
    */
    wxPoint GetPosition();

    /**
        )
        Gets the size (may be in dialog units).
    */
    wxSize GetSize();

    /**
        , @b int@e defaults = 0)
        Gets style flags from text in form "flag | flag2| flag3 |..."
        Only understands flags added with AddStyle.
    */
    int GetStyle();

    /**
        Gets text from param and does some conversions:
        - replaces \\n, \\r, \\t by respective characters (according to C syntax)
        - replaces @c $ by @c  and @c $$ by @c $ (needed for @c _File to @c File
          translation because of XML syntax)
        - calls wxGetTranslations (unless disabled in wxXmlResource)
    */
    wxString GetText(const wxString& param);

    /**
        Check to see if a parameter exists.
    */
    bool HasParam(const wxString& param);

    /**
        Convenience function. Returns @true if the node has a property class equal to
        classname,
        e.g. object class="wxDialog".
    */
    bool IsOfClass(wxXmlNode* node, const wxString& classname);

    /**
        Sets the parent resource.
    */
    void SetParentResource(wxXmlResource* res);

    /**
        Sets common window options.
    */
    void SetupWindow(wxWindow* wnd);
};

