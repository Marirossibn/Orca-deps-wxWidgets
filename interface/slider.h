/////////////////////////////////////////////////////////////////////////////
// Name:        slider.h
// Purpose:     documentation for wxSlider class
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxSlider
    @wxheader{slider.h}

    A slider is a control with a handle which can be pulled
    back and forth to change the value.

    On Windows, the track bar control is used.

    Slider events are handled in the same way as a scrollbar.

    @beginStyleTable
    @style{wxSL_HORIZONTAL}:
           Displays the slider horizontally (this is the default).
    @style{wxSL_VERTICAL}:
           Displays the slider vertically.
    @style{wxSL_AUTOTICKS}:
           Displays tick marks.
    @style{wxSL_LABELS}:
           Displays minimum, maximum and value labels.
    @style{wxSL_LEFT}:
           Displays ticks on the left and forces the slider to be vertical.
    @style{wxSL_RIGHT}:
           Displays ticks on the right and forces the slider to be vertical.
    @style{wxSL_TOP}:
           Displays ticks on the top.
    @style{wxSL_BOTTOM}:
           Displays ticks on the bottom (this is the default).
    @style{wxSL_SELRANGE}:
           Allows the user to select a range on the slider. Windows only.
    @style{wxSL_INVERSE}:
           Inverses the mininum and maximum endpoints on the slider. Not
           compatible with wxSL_SELRANGE.
    @endStyleTable

    @library{wxcore}
    @category{ctrl}
    @appearance{slider.png}

    @seealso
    @ref overview_eventhandlingoverview "Event handling overview", wxScrollBar
*/
class wxSlider : public wxControl
{
public:
    //@{
    /**
        Constructor, creating and showing a slider.
        
        @param parent
        Parent window. Must not be @NULL.
        
        @param id
        Window identifier. The value wxID_ANY indicates a default value.
        
        @param value
        Initial position for the slider.
        
        @param minValue
        Minimum slider position.
        
        @param maxValue
        Maximum slider position.
        
        @param size
        Window size. If wxDefaultSize is specified then a default size is
        chosen.
        
        @param style
        Window style. See wxSlider.
        
        @param validator
        Window validator.
        
        @param name
        Window name.
        
        @sa Create(), wxValidator
    */
    wxSlider();
    wxSlider(wxWindow* parent, wxWindowID id, int value,
             int minValue, int maxValue,
             const wxPoint& point = wxDefaultPosition,
             const wxSize& size = wxDefaultSize,
             long style = wxSL_HORIZONTAL,
             const wxValidator& validator = wxDefaultValidator,
             const wxString& name = "slider");
    //@}

    /**
        Destructor, destroying the slider.
    */
    ~wxSlider();

    /**
        Clears the selection, for a slider with the @b wxSL_SELRANGE style.
        
        @remarks Windows 95 only.
    */
    void ClearSel();

    /**
        Clears the ticks.
        
        @remarks Windows 95 only.
    */
    void ClearTicks();

    /**
        Used for two-step slider construction. See wxSlider()
        for further details.
    */
    bool Create(wxWindow* parent, wxWindowID id, int value,
                int minValue, int maxValue,
                const wxPoint& point = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxSL_HORIZONTAL,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = "slider");

    /**
        Returns the line size.
        
        @sa SetLineSize()
    */
    int GetLineSize();

    /**
        Gets the maximum slider value.
        
        @sa GetMin(), SetRange()
    */
    int GetMax();

    /**
        Gets the minimum slider value.
        
        @sa GetMin(), SetRange()
    */
    int GetMin();

    /**
        Returns the page size.
        
        @sa SetPageSize()
    */
    int GetPageSize();

    /**
        Returns the selection end point.
        
        @remarks Windows 95 only.
        
        @sa GetSelStart(), SetSelection()
    */
    int GetSelEnd();

    /**
        Returns the selection start point.
        
        @remarks Windows 95 only.
        
        @sa GetSelEnd(), SetSelection()
    */
    int GetSelStart();

    /**
        Returns the thumb length.
        
        @remarks Windows 95 only.
        
        @sa SetThumbLength()
    */
    int GetThumbLength();

    /**
        Returns the tick frequency.
        
        @remarks Windows 95 only.
        
        @sa SetTickFreq()
    */
    int GetTickFreq();

    /**
        Gets the current slider value.
        
        @sa GetMin(), GetMax(), SetValue()
    */
    int GetValue();

    /**
        Sets the line size for the slider.
        
        @param lineSize
        The number of steps the slider moves when the user moves it up or down a line.
        
        @sa GetLineSize()
    */
    void SetLineSize(int lineSize);

    /**
        Sets the page size for the slider.
        
        @param pageSize
        The number of steps the slider moves when the user pages up or down.
        
        @sa GetPageSize()
    */
    void SetPageSize(int pageSize);

    /**
        Sets the minimum and maximum slider values.
        
        @sa GetMin(), GetMax()
    */
    void SetRange(int minValue, int maxValue);

    /**
        Sets the selection.
        
        @param startPos
        The selection start position.
        
        @param endPos
        The selection end position.
        
        @remarks Windows 95 only.
        
        @sa GetSelStart(), GetSelEnd()
    */
    void SetSelection(int startPos, int endPos);

    /**
        Sets the slider thumb length.
        
        @param len
        The thumb length.
        
        @remarks Windows 95 only.
        
        @sa GetThumbLength()
    */
    void SetThumbLength(int len);

    /**
        Sets a tick position.
        
        @param tickPos
        The tick position.
        
        @remarks Windows 95 only.
        
        @sa SetTickFreq()
    */
    void SetTick(int tickPos);

    /**
        Sets the tick mark frequency and position.
        
        @param n
        Frequency. For example, if the frequency is set to two, a tick mark is
        displayed for
        every other increment in the slider's range.
        
        @param pos
        Position. Must be greater than zero. TODO: what is this for?
        
        @remarks Windows 95 only.
        
        @sa GetTickFreq()
    */
    void SetTickFreq(int n, int pos);

    /**
        Sets the slider position.
        
        @param value
        The slider position.
    */
    void SetValue(int value);
};
