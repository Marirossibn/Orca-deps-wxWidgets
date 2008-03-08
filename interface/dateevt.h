/////////////////////////////////////////////////////////////////////////////
// Name:        dateevt.h
// Purpose:     documentation for wxDateEvent class
// Author:      wxWidgets team
// RCS-ID:      $Id$
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

/**
    @class wxDateEvent
    @wxheader{dateevt.h}
    
    This event class holds information about a date change and is used together
    with wxDatePickerCtrl. It also serves as a base class
    for wxCalendarEvent.
    
    @library{wxadv}
    @category{events}
*/
class wxDateEvent : public wxCommandEvent
{
public:
    /**
        Returns the date.
    */
    const wxDateTime GetDate();

    /**
        Sets the date carried by the event, normally only used by the library
        internally.
    */
    void SetDate(const wxDateTime& date);
};
