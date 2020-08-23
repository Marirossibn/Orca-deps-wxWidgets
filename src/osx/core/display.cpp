/////////////////////////////////////////////////////////////////////////////
// Name:        src/osx/core/display.cpp
// Purpose:     Mac implementation of wxDisplay class
// Author:      Ryan Norton & Brian Victor
// Modified by: Royce Mitchell III, Vadim Zeitlin
// Created:     06/21/02
// Copyright:   (c) wxWidgets team
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/private/display.h"

#ifndef WX_PRECOMP
    #include "wx/dynarray.h"
    #include "wx/log.h"
    #include "wx/string.h"
    #include "wx/gdicmn.h"
    #include "wx/nonownedwnd.h"
#endif

#include "wx/osx/private.h"

// ----------------------------------------------------------------------------
// common helpers compiled even in wxUSE_DISPLAY==0 case
// ----------------------------------------------------------------------------

// This one is defined in Objective C++ code.
extern wxRect wxOSXGetMainDisplayClientArea();
extern wxRect wxOSXGetDisplayClientArea(CGDirectDisplayID id);

namespace
{

double wxGetScaleFactor( CGDirectDisplayID ID)
{
    wxCFRef<CGDisplayModeRef> mode = CGDisplayCopyDisplayMode(ID);
    size_t width = CGDisplayModeGetWidth(mode);
    size_t pixelsw = CGDisplayModeGetPixelWidth(mode);
    return (double)pixelsw/width;
}

wxRect wxGetDisplayGeometry(CGDirectDisplayID id)
{
    CGRect theRect = CGDisplayBounds(id);
    return wxRect( (int)theRect.origin.x,
                   (int)theRect.origin.y,
                   (int)theRect.size.width,
                   (int)theRect.size.height ); //floats
}

int wxGetDisplayDepth(CGDirectDisplayID id)
{
    CGDisplayModeRef currentMode = CGDisplayCopyDisplayMode(id);
    CFStringRef encoding = CGDisplayModeCopyPixelEncoding(currentMode);

    int theDepth = 32; // some reasonable default
    if(encoding)
    {
        if(CFStringCompare(encoding, CFSTR(IO32BitDirectPixels), kCFCompareCaseInsensitive) == kCFCompareEqualTo)
            theDepth = 32;
        else if(CFStringCompare(encoding, CFSTR(IO16BitDirectPixels), kCFCompareCaseInsensitive) == kCFCompareEqualTo)
            theDepth = 16;
        else if(CFStringCompare(encoding, CFSTR(IO8BitIndexedPixels), kCFCompareCaseInsensitive) == kCFCompareEqualTo)
            theDepth = 8;

        CFRelease(encoding);
    }

    CGDisplayModeRelease(currentMode);

    return theDepth;
}

} // anonymous namespace

#if wxUSE_DISPLAY

#include "wx/scopedarray.h"

// ----------------------------------------------------------------------------
// display classes implementation
// ----------------------------------------------------------------------------

class wxDisplayImplMacOSX : public wxDisplayImpl
{
public:
    wxDisplayImplMacOSX(unsigned n, CGDirectDisplayID id)
        : wxDisplayImpl(n),
          m_id(id)
    {
    }

    virtual wxRect GetGeometry() const wxOVERRIDE;
    virtual wxRect GetClientArea() const wxOVERRIDE;
    virtual int GetDepth() const wxOVERRIDE;
    virtual double GetScaleFactor() const wxOVERRIDE;

    virtual wxArrayVideoModes GetModes(const wxVideoMode& mode) const wxOVERRIDE;
    virtual wxVideoMode GetCurrentMode() const wxOVERRIDE;
    virtual bool ChangeMode(const wxVideoMode& mode) wxOVERRIDE;

    virtual bool IsPrimary() const wxOVERRIDE;

private:
    CGDirectDisplayID m_id;

    wxDECLARE_NO_COPY_CLASS(wxDisplayImplMacOSX);
};

class wxDisplayFactoryMacOSX : public wxDisplayFactory
{
public:
    wxDisplayFactoryMacOSX() {}

    virtual wxDisplayImpl *CreateDisplay(unsigned n) wxOVERRIDE;
    virtual unsigned GetCount() wxOVERRIDE;
    virtual int GetFromPoint(const wxPoint& pt) wxOVERRIDE;
    virtual int GetFromWindow(const wxWindow *window) wxOVERRIDE;

protected:
    wxDECLARE_NO_COPY_CLASS(wxDisplayFactoryMacOSX);
};

// ============================================================================
// wxDisplayFactoryMacOSX implementation
// ============================================================================

// gets all displays that are not mirror displays

static CGDisplayErr wxOSXGetDisplayList(CGDisplayCount maxDisplays,
                                   CGDirectDisplayID *displays,
                                   CGDisplayCount *displayCount)
{
    CGDisplayErr error = kCGErrorSuccess;
    CGDisplayCount onlineCount;

    error = CGGetOnlineDisplayList(0,NULL,&onlineCount);
    if ( error == kCGErrorSuccess )
    {
        *displayCount = 0;
        if ( onlineCount > 0 )
        {
            CGDirectDisplayID *onlineDisplays = new CGDirectDisplayID[onlineCount];
            error = CGGetOnlineDisplayList(onlineCount,onlineDisplays,&onlineCount);
            if ( error == kCGErrorSuccess )
            {
                for ( CGDisplayCount i = 0; i < onlineCount; ++i )
                {
                    if ( CGDisplayMirrorsDisplay(onlineDisplays[i]) != kCGNullDirectDisplay )
                        continue;

                    if ( displays == NULL )
                        *displayCount += 1;
                    else
                    {
                        if ( *displayCount < maxDisplays )
                        {
                            displays[*displayCount] = onlineDisplays[i];
                            *displayCount += 1;
                        }
                    }
                }
            }
            delete[] onlineDisplays;
        }

    }
    return error;
}

static int wxOSXGetDisplayFromID( CGDirectDisplayID theID )
{
    int nWhich = wxNOT_FOUND;
    CGDisplayCount theCount;
    CGDisplayErr err = wxOSXGetDisplayList(0, NULL, &theCount);

    if (err == CGDisplayNoErr && theCount > 0 )
    {
        CGDirectDisplayID* theIDs = new CGDirectDisplayID[theCount];
        err = wxOSXGetDisplayList(theCount, theIDs, &theCount);
        wxASSERT(err == CGDisplayNoErr);

        for (nWhich = 0; nWhich < (int) theCount; ++nWhich)
        {
            if (theIDs[nWhich] == theID)
                break;
        }

        delete [] theIDs;

        if (nWhich == (int) theCount)
        {
            wxFAIL_MSG(wxT("Failed to find display in display list"));
            nWhich = wxNOT_FOUND;
        }
    }

    return nWhich;
}

unsigned wxDisplayFactoryMacOSX::GetCount()
{
    CGDisplayCount count;
    CGDisplayErr err = wxOSXGetDisplayList(0, NULL, &count);

    wxCHECK_MSG( err == CGDisplayNoErr, 0, "wxOSXGetDisplayList() failed" );

    return count;
}

int wxDisplayFactoryMacOSX::GetFromPoint(const wxPoint& p)
{
    CGPoint thePoint = {(float)p.x, (float)p.y};
    CGDirectDisplayID theID;
    CGDisplayCount theCount;
    CGDisplayErr err = CGGetDisplaysWithPoint(thePoint, 1, &theID, &theCount);
    wxASSERT(err == CGDisplayNoErr);

    if (theCount)
        return wxOSXGetDisplayFromID(theID);

    return wxNOT_FOUND;
}

int wxDisplayFactoryMacOSX::GetFromWindow(const wxWindow *window)
{
    wxCHECK_MSG( window, wxNOT_FOUND, "window can't be NULL" );

    wxNonOwnedWindow* const tlw = window->MacGetTopLevelWindow();
    int x,y,w,h;

    tlw->GetPosition(&x, &y);
    tlw->GetSize(&w, &h);

    CGRect r = CGRectMake(x, y, w, h);
    CGDisplayCount theCount;
    CGDisplayErr err = CGGetDisplaysWithRect(r, 0, NULL, &theCount);
    wxASSERT(err == CGDisplayNoErr);

    wxScopedArray<CGDirectDisplayID> theIDs(theCount);
    err = CGGetDisplaysWithRect(r, theCount, theIDs.get(), &theCount);
    wxASSERT(err == CGDisplayNoErr);

    const double scaleWindow = tlw->GetContentScaleFactor();
    for ( int i = 0 ; i < theCount; ++i )
    {
        // find a screen intersecting having the same contentScale as the window itself
        double scale = wxGetScaleFactor(theIDs[i]);
        if ( fabs(scale - scaleWindow) < 0.01 )
        {
            return wxOSXGetDisplayFromID(theIDs[i]);
        }
    }

    return wxNOT_FOUND;
}

wxDisplayImpl *wxDisplayFactoryMacOSX::CreateDisplay(unsigned n)
{
    CGDisplayCount theCount = GetCount();
    wxScopedArray<CGDirectDisplayID> theIDs(theCount);

    CGDisplayErr err = wxOSXGetDisplayList(theCount, theIDs.get(), &theCount);
    wxCHECK_MSG( err == CGDisplayNoErr, NULL, "wxOSXGetDisplayList() failed" );

    wxCHECK_MSG( n < theCount, NULL, wxS("Invalid display index") );

    return new wxDisplayImplMacOSX(n, theIDs[n]);
}

// ============================================================================
// wxDisplayImplMacOSX implementation
// ============================================================================

bool wxDisplayImplMacOSX::IsPrimary() const
{
    return CGDisplayIsMain(m_id);
}

wxRect wxDisplayImplMacOSX::GetGeometry() const
{
    return wxGetDisplayGeometry(m_id);
}

wxRect wxDisplayImplMacOSX::GetClientArea() const
{
    return wxOSXGetDisplayClientArea(m_id);
}

int wxDisplayImplMacOSX::GetDepth() const
{
    return wxGetDisplayDepth(m_id);
}

double wxDisplayImplMacOSX::GetScaleFactor() const
{
    return wxGetScaleFactor(m_id);
}

static int wxOSXCGDisplayModeGetBitsPerPixel( CGDisplayModeRef theValue )
{
    wxCFRef<CFStringRef> pixelEncoding( CGDisplayModeCopyPixelEncoding(theValue) );
    int depth = 0;
    if ( CFStringCompare( pixelEncoding, CFSTR(IO32BitDirectPixels) , kCFCompareCaseInsensitive) == kCFCompareEqualTo )
        depth = 32;
    else if ( CFStringCompare( pixelEncoding, CFSTR(IO16BitDirectPixels) , kCFCompareCaseInsensitive) == kCFCompareEqualTo )
        depth = 16;
    else if ( CFStringCompare( pixelEncoding, CFSTR(IO8BitIndexedPixels) , kCFCompareCaseInsensitive) == kCFCompareEqualTo )
        depth = 8;
    
    return depth;
}

wxArrayVideoModes wxDisplayImplMacOSX::GetModes(const wxVideoMode& mode) const
{
    wxArrayVideoModes resultModes;

    wxCFRef<CFArrayRef> theArray(CGDisplayCopyAllDisplayModes( m_id ,NULL ) );
    
    for (CFIndex i = 0; i < CFArrayGetCount(theArray); ++i)
    {
        CGDisplayModeRef theValue = static_cast<CGDisplayModeRef>(const_cast<void*>(CFArrayGetValueAtIndex(theArray, i)));
        
        wxVideoMode theMode(
                            CGDisplayModeGetWidth(theValue),
                            CGDisplayModeGetHeight(theValue),
                            wxOSXCGDisplayModeGetBitsPerPixel(theValue),
                            CGDisplayModeGetRefreshRate(theValue));
        
        if (theMode.Matches( mode ))
            resultModes.Add( theMode );
    }

    return resultModes;
}

wxVideoMode wxDisplayImplMacOSX::GetCurrentMode() const
{
    wxCFRef<CGDisplayModeRef> theValue( CGDisplayCopyDisplayMode( m_id ) );
    
    return wxVideoMode(
                       CGDisplayModeGetWidth(theValue),
                       CGDisplayModeGetHeight(theValue),
                       wxOSXCGDisplayModeGetBitsPerPixel(theValue),
                       CGDisplayModeGetRefreshRate(theValue));
}

bool wxDisplayImplMacOSX::ChangeMode( const wxVideoMode& mode )
{
#ifndef __WXOSX_IPHONE__
    if (mode == wxDefaultVideoMode)
    {
        CGRestorePermanentDisplayConfiguration();
        return true;
    }
#endif

    wxCHECK_MSG( mode.GetWidth() && mode.GetHeight(), false,
                wxT("at least the width and height must be specified") );
    
    bool bOK = false;
    wxCFRef<CFArrayRef> theArray(CGDisplayCopyAllDisplayModes( m_id ,NULL ) );
    
    for (CFIndex i = 0; i < CFArrayGetCount(theArray); ++i)
    {
        CGDisplayModeRef theValue = static_cast<CGDisplayModeRef>(const_cast<void*>(CFArrayGetValueAtIndex(theArray, i)));
        
        wxVideoMode theMode(
                            CGDisplayModeGetWidth(theValue),
                            CGDisplayModeGetHeight(theValue),
                            wxOSXCGDisplayModeGetBitsPerPixel(theValue),
                            CGDisplayModeGetRefreshRate(theValue));
        
        if ( theMode.GetWidth() == mode.GetWidth() && theMode.GetHeight() == mode.GetHeight() &&
            ( mode.GetDepth() == 0 || theMode.GetDepth() == mode.GetDepth() ) &&
            ( mode.GetRefresh() == 0 || theMode.GetRefresh() == mode.GetRefresh() ) )
        {
            CGDisplaySetDisplayMode( m_id, theValue , NULL );
            bOK = true;
            break;
        }
    }
    
    return bOK;
}

// ============================================================================
// wxDisplay::CreateFactory()
// ============================================================================

/* static */ wxDisplayFactory *wxDisplay::CreateFactory()
{
    return new wxDisplayFactoryMacOSX;
}

#else // !wxUSE_DISPLAY

class wxDisplayImplSingleMacOSX : public wxDisplayImplSingle
{
public:
    virtual wxRect GetGeometry() const wxOVERRIDE
    {
        return wxGetDisplayGeometry(CGMainDisplayID());
    }

    virtual wxRect GetClientArea() const wxOVERRIDE
    {
        return wxOSXGetMainDisplayClientArea();
    }

    virtual int GetDepth() const wxOVERRIDE
    {
        return wxGetDisplayDepth(CGMainDisplayID());
    }
};

class wxDisplayFactorySingleMacOSX : public wxDisplayFactorySingle
{
protected:
    virtual wxDisplayImpl *CreateSingleDisplay() wxOVERRIDE
    {
        return new wxDisplayImplSingleMacOSX;
    }
};

/* static */ wxDisplayFactory *wxDisplay::CreateFactory()
{
    return new wxDisplayFactorySingleMacOSX;
}

#endif // wxUSE_DISPLAY
