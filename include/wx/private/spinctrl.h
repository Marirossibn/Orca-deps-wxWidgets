///////////////////////////////////////////////////////////////////////////////
// Name:        wx/private/spinctrl.h
// Purpose:     Private functions used in wxSpinCtrl implementation.
// Author:      Vadim Zeitlin
// Created:     2019-11-13
// Copyright:   (c) 2019 Vadim Zeitlin <vadim@wxwidgets.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_PRIVATE_SPINCTRL_H_
#define _WX_PRIVATE_SPINCTRL_H_

namespace wxPrivate
{

// This is an internal helper function currently used by all ports: return the
// string containing hexadecimal representation of the given number.
extern wxString wxSpinCtrlFormatAsHex(long val, long maxVal);

// The helper function to determine the best size for the given control.
// We can't implement this function in the wxSpinCtrlBase because MSW implementation
// of wxSpinCtrl is derived from wxSpinButton but uses the same algorithm.
extern wxSize wxSpinCtrlGetBestSize(const wxControl* spin, int minVal, int maxVal, int base);

} // namespace wxPrivate

#endif // _WX_PRIVATE_SPINCTRL_H_
