/////////////////////////////////////////////////////////////////////////////
// Name:        xh_sizer.h
// Purpose:     XML resource handler for wxBoxSizer
// Author:      Vaclav Slavik
// Created:     2000/04/24
// RCS-ID:      $Id$
// Copyright:   (c) 2000 Vaclav Slavik
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_XH_SIZER_H_
#define _WX_XH_SIZER_H_

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "xh_sizer.h"
#endif

#include "wx/xrc/xmlres.h"


class WXDLLEXPORT wxSizer;

class WXDLLIMPEXP_XRC wxSizerXmlHandler : public wxXmlResourceHandler
{
DECLARE_DYNAMIC_CLASS(wxSizerXmlHandler)
public:
    wxSizerXmlHandler();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);

private:
    bool m_isInside;
    wxSizer *m_parentSizer;

    bool IsSizerNode(wxXmlNode *node);
};


#endif // _WX_XH_BOXSIZER_H_
