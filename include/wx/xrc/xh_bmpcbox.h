/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xrc/xh_bmpcbox.h
// Purpose:     XML resource handler for wxBitmapComboBox
// Author:      Jaakko Salli
// Created:     Sep-10-2006
// RCS-ID:      $Id:
// Copyright:   (c) 2006 Jaakko Salli
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_XH_BMPCBOX_H_
#define _WX_XH_BMPCBOX_H_

#include "wx/xrc/xmlres.h"

#if wxUSE_BITMAPCOMBOBOX

class WXDLLIMPEXP_ADV wxBitmapComboBox;

class WXDLLIMPEXP_XRC wxBitmapComboBoxXmlHandler : public wxXmlResourceHandler
{
    DECLARE_DYNAMIC_CLASS(wxBitmapComboBoxXmlHandler)
public:
    wxBitmapComboBoxXmlHandler();
    virtual wxObject *DoCreateResource();
    virtual bool CanHandle(wxXmlNode *node);
private:
    wxBitmapComboBox*    m_combobox;
    bool                m_isInside;
};

#endif

#endif // _WX_XH_BMPCBOX_H_
