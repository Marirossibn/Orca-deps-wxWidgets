/////////////////////////////////////////////////////////////////////////////
// Name:        src/unix/fontenum.cpp
// Purpose:     wxFontEnumerator class for X11/GDK
// Author:      Vadim Zeitlin
// Modified by:
// Created:     01.10.99
// RCS-ID:      $Id$
// Copyright:   (c) Vadim Zeitlin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#ifdef __GNUG__
    #pragma implementation "fontenum.h"
#endif

#include "wx/defs.h"
#include "wx/dynarray.h"
#include "wx/string.h"
#include "wx/regex.h"
#include "wx/utils.h"

#include "wx/fontmap.h"
#include "wx/fontenum.h"
#include "wx/fontutil.h"

#ifdef __VMS__ // Xlib.h for VMS is not (yet) compatible with C++
               // The resulting warnings are switched off here
#pragma message disable nosimpint
#endif
#include <X11/Xlib.h>
#ifdef __VMS__
#pragma message enable nosimpint
#endif

// ----------------------------------------------------------------------------
// private functions
// ----------------------------------------------------------------------------

// create the list of all fonts with the given spacing and encoding
static char **CreateFontList(wxChar spacing, wxFontEncoding encoding,
                             int *nFonts);

// extract all font families from the given font list and call our
// OnFacename() for each of them
static bool ProcessFamiliesFromFontList(wxFontEnumerator *This,
                                        char **fonts,
                                        int nFonts);


// ----------------------------------------------------------------------------
// private types
// ----------------------------------------------------------------------------

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// helpers
// ----------------------------------------------------------------------------

#if !wxUSE_NANOX
static char **CreateFontList(wxChar spacing,
                             wxFontEncoding encoding,
                             int *nFonts)
{
    wxNativeEncodingInfo info;
    wxGetNativeFontEncoding(encoding, &info);

#if wxUSE_FONTMAP
    if ( !wxTestFontEncoding(info) )
    {
        // ask font mapper for a replacement
        (void)wxFontMapper::Get()->GetAltForEncoding(encoding, &info);
    }
#endif // wxUSE_FONTMAP

    wxString pattern;
    pattern.Printf(wxT("-*-*-*-*-*-*-*-*-*-*-%c-*-%s-%s"),
                   spacing,
                   info.xregistry.c_str(),
                   info.xencoding.c_str());

    // get the list of all fonts
    return XListFonts((Display *)wxGetDisplay(), pattern.mb_str(), 32767, nFonts);
}

static bool ProcessFamiliesFromFontList(wxFontEnumerator *This,
                                        char **fonts,
                                        int nFonts)
{
#if wxUSE_REGEX
    wxRegEx re(wxT("^(-[^-]*){14}$"), wxRE_NOSUB);
#endif // wxUSE_REGEX

    // extract the list of (unique) font families
    wxSortedArrayString families;
    for ( int n = 0; n < nFonts; n++ )
    {
        char *font = fonts[n];
#if wxUSE_REGEX
#if wxUSE_UNICODE
        wxString sfont( wxConvLocal.cMB2WC( font ) );
        if ( !re.Matches(sfont) )
#else
        if ( !re.Matches(font) )
#endif        
#else // !wxUSE_REGEX
        if ( !wxString(font).Matches(wxT("-*-*-*-*-*-*-*-*-*-*-*-*-*-*")) )
#endif // wxUSE_REGEX/!wxUSE_REGEX
        {
            // it's not a full font name (probably an alias)
            continue;
        }

        char *dash = strchr(font + 1, '-');
        char *family = dash + 1;
        dash = strchr(family, '-');
        *dash = '\0'; // !NULL because Matches() above succeeded
#if wxUSE_UNICODE
       wxString fam( wxConvLocal.cMB2WC( family ) );
#else        
        wxString fam(family);
#endif

        if ( families.Index(fam) == wxNOT_FOUND )
        {
            if ( !This->OnFacename(fam) )
            {
                // stop enumerating
                return FALSE;
            }

            families.Add(fam);
        }
        //else: already seen
    }

    return TRUE;
}
#endif
  // wxUSE_NANOX

// ----------------------------------------------------------------------------
// wxFontEnumerator
// ----------------------------------------------------------------------------

bool wxFontEnumerator::EnumerateFacenames(wxFontEncoding encoding,
                                          bool fixedWidthOnly)
{
#if wxUSE_NANOX
    return FALSE;
#else
    int nFonts;
    char **fonts;

    if ( fixedWidthOnly )
    {
        bool cont = TRUE;
        fonts = CreateFontList(wxT('m'), encoding, &nFonts);
        if ( fonts )
        {
            cont = ProcessFamiliesFromFontList(this, fonts, nFonts);

            XFreeFontNames(fonts);
        }

        if ( !cont )
        {
            return TRUE;
        }

        fonts = CreateFontList(wxT('c'), encoding, &nFonts);
        if ( !fonts )
        {
            return TRUE;
        }
    }
    else
    {
        fonts = CreateFontList(wxT('*'), encoding, &nFonts);

        if ( !fonts )
        {
            // it's ok if there are no fonts in given encoding - but it's not
            // ok if there are no fonts at all
            wxASSERT_MSG(encoding != wxFONTENCODING_SYSTEM,
                         wxT("No fonts at all on this system?"));

            return FALSE;
        }
    }

    (void)ProcessFamiliesFromFontList(this, fonts, nFonts);

    XFreeFontNames(fonts);
    return TRUE;
#endif
    // wxUSE_NANOX
}

bool wxFontEnumerator::EnumerateEncodings(const wxString& family)
{
#if wxUSE_NANOX
    return FALSE;
#else
    wxString pattern;
    pattern.Printf(wxT("-*-%s-*-*-*-*-*-*-*-*-*-*-*-*"),
                   family.IsEmpty() ? wxT("*") : family.c_str());

    // get the list of all fonts
    int nFonts;
    char **fonts = XListFonts((Display *)wxGetDisplay(), pattern.mb_str(),
                              32767, &nFonts);

    if ( !fonts )
    {
        // unknown family?
        return FALSE;
    }

    // extract the list of (unique) encodings
    wxSortedArrayString encodings;
    for ( int n = 0; n < nFonts; n++ )
    {
        char *font = fonts[n];
        if ( !wxString(font).Matches(wxT("-*-*-*-*-*-*-*-*-*-*-*-*-*-*")) )
        {
            // it's not a full font name (probably an alias)
            continue;
        }

        // extract the family
        char *dash = strchr(font + 1, '-');
        char *familyFont = dash + 1;
        dash = strchr(familyFont, '-');
        *dash = '\0'; // !NULL because Matches() above succeeded

        if ( !family.IsEmpty() && (family != familyFont) )
        {
            // family doesn't match
            continue;
        }

        // now extract the registry/encoding
        char *p = dash + 1; // just after the dash after family
        dash = strrchr(p, '-');

        wxString registry(dash + 1);
        *dash = '\0';

        dash = strrchr(p, '-');
        wxString encoding(dash + 1);

        encoding << wxT('-') << registry;
        if ( encodings.Index(encoding) == wxNOT_FOUND )
        {
            if ( !OnFontEncoding(familyFont, encoding) )
            {
                break;
            }

            encodings.Add(encoding);
        }
        //else: already had this one
    }

    XFreeFontNames(fonts);

    return TRUE;
#endif
    // wxUSE_NANOX
}
