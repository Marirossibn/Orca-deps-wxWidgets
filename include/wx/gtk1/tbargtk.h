/////////////////////////////////////////////////////////////////////////////
// Name:        tbargtk.h
// Purpose:     GTK toolbar
// Author:      Robert Roebling
// Modified by:
// Created:     01/02/97
// RCS-ID:      $Id$
// Copyright:   (c) Robert Roebling
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __TBARGTKH__
#define __TBARGTKH__

#ifdef __GNUG__
#pragma interface
#endif

#include "wx/defs.h"
#include "wx/control.h"

//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------

class wxToolBarTool;
class wxToolBarGTK;

//-----------------------------------------------------------------------------
// constants
//-----------------------------------------------------------------------------

#define wxTOOL_STYLE_BUTTON          1
#define wxTOOL_STYLE_SEPARATOR       2

//-----------------------------------------------------------------------------
// global data
//-----------------------------------------------------------------------------

extern const char *wxToolBarNameStr;

//-----------------------------------------------------------------------------
// wxToolBarTool
//-----------------------------------------------------------------------------

class wxToolBarTool: public wxObject
{
  DECLARE_DYNAMIC_CLASS(wxToolBarTool)
  
  public:

    wxToolBarTool(void) {}; 
    wxToolBarTool( wxToolBarGTK *owner, const int theIndex = 0, 
      const wxBitmap& bitmap1 = wxNullBitmap, const wxBitmap& bitmap2 = wxNullBitmap, 
      const bool toggle = FALSE, wxObject *clientData = NULL, 
      const wxString& shortHelpString = "", const wxString& longHelpString = "");
   ~wxToolBarTool(void);

  public:
  
    int                   m_toolStyle;
    wxObject             *m_clientData;
    int                   m_index;
    bool                  m_toggleState;
    bool                  m_isToggle;
    bool                  m_deleteSecondBitmap;
    bool                  m_enabled;
    wxBitmap              m_bitmap1;
    wxBitmap              m_bitmap2;
    bool                  m_isMenuCommand;
    wxString              m_shortHelpString;
    wxString              m_longHelpString;
    wxToolBarGTK         *m_owner;
};

//-----------------------------------------------------------------------------
// wxToolBarGTK
//-----------------------------------------------------------------------------

class wxToolBarGTK: public wxControl
{
  DECLARE_DYNAMIC_CLASS(wxToolBarGTK)
  
  public:

    wxToolBarGTK(void);
    wxToolBarGTK( wxWindow *parent, const wxWindowID id, 
      const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
      const long style = 0, const wxString& name = wxToolBarNameStr );
   ~wxToolBarGTK(void);

   bool Create( wxWindow *parent, const wxWindowID id, 
     const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
     const long style = 0, const wxString& name = wxToolBarNameStr);

    // Only allow toggle if returns TRUE. Call when left button up.
    virtual bool OnLeftClick(int toolIndex, bool toggleDown);

    // Call when right button down.
    virtual void OnRightClick(int toolIndex, float x, float y);

    // Called when the mouse cursor enters a tool bitmap.
    // Argument is -1 if mouse is exiting the toolbar.
    virtual void OnMouseEnter(int toolIndex);

    // If pushedBitmap is NULL, a reversed version of bitmap is
    // created and used as the pushed/toggled image.
    // If toggle is TRUE, the button toggles between the two states.
    virtual wxToolBarTool *AddTool( const int toolIndex, const wxBitmap& bitmap, 
      const wxBitmap& pushedBitmap = wxNullBitmap, const bool toggle = FALSE,
      const float xPos = -1, const float yPos = -1, wxObject *clientData = NULL,
      const wxString& helpString1 = "", const wxString& helpString2 = "");
    virtual void AddSeparator(void);
    virtual void ClearTools(void);

    virtual void EnableTool(const int toolIndex, const bool enable);
    virtual void ToggleTool(const int toolIndex, const bool toggle); // toggle is TRUE if toggled on
    virtual void SetToggle(const int toolIndex, const bool toggle); // Set this to be togglable (or not)
    virtual wxObject *GetToolClientData(const int index) const;

    virtual bool GetToolState(const int toolIndex) const;
    virtual bool GetToolEnabled(const int toolIndex) const;

    virtual void SetMargins(const int x, const int y);
    void SetMargins(const wxSize& size) { SetMargins(size.x, size.y); };
    virtual void SetToolPacking(const int packing);
    virtual void SetToolSeparation(const int separation);
  
  public:
  
    GtkToolbar   *m_toolbar;
    wxList        m_tools;
  
  DECLARE_EVENT_TABLE()
};

#endif
    // __TBARGTKH__

