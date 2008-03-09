/////////////////////////////////////////////////////////////////////////////
// Name:        region.h
// Purpose:     documentation for wxRegionIterator class
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxRegionIterator
    @wxheader{region.h}

    This class is used to iterate through the rectangles in a region,
    typically when examining the damaged regions of a window within an OnPaint call.

    To use it, construct an iterator object on the stack and loop through the
    regions, testing the object and incrementing the iterator at the end of the
    loop.

    See wxPaintEvent for an example of use.

    @library{wxcore}
    @category{FIXME}

    @seealso
    wxPaintEvent
*/
class wxRegionIterator : public wxObject
{
public:
    //@{
    /**
        Creates an iterator object given a region.
    */
    wxRegionIterator();
    wxRegionIterator(const wxRegion& region);
    //@}

    /**
        An alias for GetHeight.
    */
    wxCoord GetH();

    /**
        Returns the height value for the current region.
    */
    wxCoord GetHeight();

    /**
        Returns the current rectangle.
    */
    wxRect GetRect();

    /**
        An alias for GetWidth.
    */
    wxCoord GetW();

    /**
        Returns the width value for the current region.
    */
    wxCoord GetWidth();

    /**
        Returns the x value for the current region.
    */
    wxCoord GetX();

    /**
        Returns the y value for the current region.
    */
    wxCoord GetY();

    /**
        Returns @true if there are still some rectangles; otherwise returns @false.
    */
    bool HaveRects();

    //@{
    /**
        Resets the iterator to the given region.
    */
    void Reset();
    void Reset(const wxRegion& region);
    //@}

    /**
        Increment operator. Increments the iterator to the next region.
    */
    void operator ++();

    /**
        Returns @true if there are still some rectangles; otherwise returns @false.
        You can use this to test the iterator object as if it were of type bool.
    */
    operator bool();
};


/**
    @class wxRegion
    @wxheader{region.h}

    A wxRegion represents a simple or complex region on a device context or window.

    This class uses @ref overview_trefcount "reference counting and copy-on-write"
    internally so that assignments between two instances of this class are very
    cheap. You can therefore use actual objects instead of pointers without
    efficiency problems. If an instance of this class is changed it will create
    its own data internally so that other instances, which previously shared the
    data using the reference counting, are not affected.

    @library{wxcore}
    @category{data}

    @seealso
    wxRegionIterator
*/
class wxRegion : public wxGDIObject
{
public:
    //@{
    /**
        Constructs a region using the non-transparent pixels of a bitmap.  See
        Union() for more details.
    */
    wxRegion();
    wxRegion(wxCoord x, wxCoord y, wxCoord width, wxCoord height);
    wxRegion(const wxPoint& topLeft, const wxPoint& bottomRight);
    wxRegion(const wxRect& rect);
    wxRegion(const wxRegion& region);
    wxRegion(size_t n, const wxPoint points,
             int fillStyle = wxWINDING_RULE);
    wxRegion(const wxBitmap& bmp);
    wxRegion(const wxBitmap& bmp, const wxColour& transColour,
             int tolerance = 0);
    //@}

    /**
        Destructor.
        See @ref overview_refcountdestruct "reference-counted object destruction" for
        more info.
    */
    ~wxRegion();

    /**
        Clears the current region.
    */
    void Clear();

    //@{
    /**
        Returns a value indicating whether the given rectangle is contained within the
        region.
        
        @returns The return value is one of wxOutRegion, wxPartRegion and
                 wxInRegion.
    */
    wxRegionContain Contains(long& x, long& y);
    wxRegionContain Contains(const wxPoint& pt);
    wxRegionContain Contains(long& x, long& y,
                             long& width,
                             long& height);
    wxRegionContain Contains(const wxRect& rect);
    //@}

    /**
        Convert the region to a black and white bitmap with the white pixels
        being inside the region.
    */
    wxBitmap ConvertToBitmap();

    //@{
    /**
        Returns the outer bounds of the region.
    */
    void GetBox(wxCoord& x, wxCoord& y, wxCoord& width,
                wxCoord& height);
    wxRect GetBox();
    //@}

    //@{
    /**
        Finds the intersection of this region and another region.
        
        @returns @true if successful, @false otherwise.
        
        @remarks Creates the intersection of the two regions, that is, the parts
                 which are in both regions. The result is stored in this
                 region.
    */
    bool Intersect(wxCoord x, wxCoord y, wxCoord width,
                   wxCoord height);
    bool Intersect(const wxRect& rect);
    bool Intersect(const wxRegion& region);
    //@}

    /**
        Returns @true if the region is empty, @false otherwise.
    */
    bool IsEmpty();

    /**
        Returns @true if the region is equal to, i.e. covers the same area as,
        another one. Note that if both this region and @a region are invalid, they
        are considered to be equal.
    */
    bool IsEqual(const wxRegion& region);

    //@{
    /**
        Moves the region by the specified offsets in horizontal and vertical
        directions.
        
        @returns @true if successful, @false otherwise (the region is unchanged
                 then).
    */
    bool Offset(wxCoord x, wxCoord y);
    bool Offset(const wxPoint& pt);
    //@}

    //@{
    /**
        Subtracts a region from this region.
        
        @returns @true if successful, @false otherwise.
        
        @remarks This operation combines the parts of 'this' region that are not
                 part of the second region. The result is stored in this
                 region.
    */
    bool Subtract(const wxRect& rect);
    bool Subtract(const wxRegion& region);
    //@}

    //@{
    /**
        Finds the union of this region and the non-transparent pixels of a
        bitmap. Colour to be treated as transparent is specified in the
        @a transColour argument, along with an
        optional colour tolerance value.
        
        @returns @true if successful, @false otherwise.
        
        @remarks This operation creates a region that combines all of this region
                 and the second region. The result is stored in this
                 region.
    */
    bool Union(wxCoord x, wxCoord y, wxCoord width, wxCoord height);
    bool Union(const wxRect& rect);
    bool Union(const wxRegion& region);
    bool Union(const wxBitmap& bmp);
    bool Union(const wxBitmap& bmp, const wxColour& transColour,
               int tolerance = 0);
    //@}

    //@{
    /**
        Finds the Xor of this region and another region.
        
        @returns @true if successful, @false otherwise.
        
        @remarks This operation creates a region that combines all of this region
                 and the second region, except for any overlapping
                 areas. The result is stored in this region.
    */
    bool Xor(wxCoord x, wxCoord y, wxCoord width, wxCoord height);
    bool Xor(const wxRect& rect);
    bool Xor(const wxRegion& region);
    //@}

    /**
        Assignment operator, using @ref overview_trefcount "reference counting".
    */
    void operator =(const wxRegion& region);
};
