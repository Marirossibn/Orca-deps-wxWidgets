/////////////////////////////////////////////////////////////////////////////
// Name:        wx/msw/dcscreen.h
// Purpose:     wxScreenDC class
// Author:      Julian Smart
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_MSW_DCSCREEN_H_
#define _WX_MSW_DCSCREEN_H_

#include "wx/dcscreen.h"
#include "wx/msw/dc.h"

class WXDLLEXPORT wxScreenDCImpl : public wxMSWDCImpl
{
public:
    // Create a DC representing the whole screen
    wxScreenDCImpl( wxScreenDC *owner );

protected:
    virtual void DoGetSize(int *w, int *h) const
    {
        GetDeviceSize(w, h);
    }

private:
    DECLARE_CLASS(wxScreenDCImpl)
    DECLARE_NO_COPY_CLASS(wxScreenDCImpl)
};

#endif // _WX_MSW_DCSCREEN_H_

