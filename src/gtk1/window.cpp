/////////////////////////////////////////////////////////////////////////////
// Name:        window.cpp
// Purpose:
// Author:      Robert Roebling
// Created:     01/02/97
// Id:
// Copyright:   (c) 1998 Robert Roebling, Julian Smart and Markus Holzem
// Licence:   	wxWindows licence
/////////////////////////////////////////////////////////////////////////////


#ifdef __GNUG__
#pragma implementation "window.h"
#endif

#include "wx/defs.h"
#include "wx/window.h"
#include "wx/dc.h"
#include "wx/frame.h"
#include "wx/app.h"
#include "wx/layout.h"
#include "wx/utils.h"
#include "wx/dialog.h"
#include "wx/msgdlg.h"
#include "wx/dcclient.h"
#include "wx/dnd.h"
#include "wx/mdi.h"
#include "wx/notebook.h"
#include "gdk/gdkkeysyms.h"
#include <math.h>
#include "wx/gtk/win_gtk.h"
#include "gdk/gdkprivate.h"

//-----------------------------------------------------------------------------
// data
//-----------------------------------------------------------------------------

extern wxList wxPendingDelete;
extern wxList wxTopLevelWindows;
extern bool   g_blockEventsOnDrag;

//-----------------------------------------------------------------------------
// GTK callbacks for wxWindows event system
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// expose (of m_wxwindow, not of m_widget)

void gtk_window_expose_callback( GtkWidget *WXUNUSED(widget), GdkEventExpose *gdk_event, wxWindow *win )
{ 
  if (!win->HasVMT()) return;
  if (g_blockEventsOnDrag) return;
  
/*
  printf( "OnExpose from " );
  if (win->GetClassInfo() && win->GetClassInfo()->GetClassName())
    printf( win->GetClassInfo()->GetClassName() );
  printf( ".\n" );
  
  printf( "x: %d \n", gdk_event->area.x );
  printf( "y: %d \n", gdk_event->area.y );
  printf( "w: %d \n", gdk_event->area.width );
  printf( "h: %d \n", gdk_event->area.height );
*/

  win->m_updateRegion.Union( gdk_event->area.x,
                             gdk_event->area.y,
			     gdk_event->area.width,
			     gdk_event->area.height );
			     
  if (gdk_event->count > 0) return;

  wxPaintEvent event( win->GetId() );
  event.SetEventObject( win );
  win->ProcessEvent( event );
  
  win->m_updateRegion.Clear();
};

//-----------------------------------------------------------------------------
// draw (of m_wxwindow, not of m_widget)

void gtk_window_draw_callback( GtkWidget *WXUNUSED(widget), GdkRectangle *rect, wxWindow *win )
{ 
  if (!win->HasVMT()) return;
  if (g_blockEventsOnDrag) return;
  
/*
  printf( "OnDraw from " );
  if (win->GetClassInfo() && win->GetClassInfo()->GetClassName())
    printf( win->GetClassInfo()->GetClassName() );
  printf( ".\n" );
  
  printf( "x: %d \n", rect->x );
  printf( "y: %d \n", rect->y );
  printf( "w: %d \n", rect->width );
  printf( "h: %d \n", rect->height );
*/

  win->m_updateRegion.Union( rect->x, rect->y, rect->width, rect->height );
			     
  wxPaintEvent event( win->GetId() );
  event.SetEventObject( win );
  win->ProcessEvent( event );
  
  win->m_updateRegion.Clear();
};

//-----------------------------------------------------------------------------
// size 
//      I don't any longer intercept GTK's internal resize events (except frames)

/*
void gtk_window_size_callback( GtkWidget *WXUNUSED(widget), GtkAllocation* alloc, wxWindow *win )
{ 
  if (!win->HasVMT()) return;
  if (g_blockEventsOnDrag) return;
  
  return;

  if ((win->m_x == alloc->x) &&
      (win->m_y == alloc->y) &&
      (win->m_width == alloc->width) &&
      (win->m_height == alloc->height))
  {
    return;
  };
  
  printf( "OnResize from " );
  if (win->GetClassInfo() && win->GetClassInfo()->GetClassName())
    printf( win->GetClassInfo()->GetClassName() );
  printf( " .\n" );
    
  printf( "  Old: X: %d  Y: %d ", win->m_x, win->m_y );
  printf( "  W: %d  H: %d ", win->m_width, win->m_height );
  printf( " .\n" );
    
  printf( "  New: X: %d  Y: %d ", alloc->x, alloc->y );
  printf( "  W: %d  H: %d ", alloc->width, alloc->height );
  printf( " .\n" );
  
  wxSizeEvent event( wxSize( alloc->width, alloc->height), win->GetId() );
  event.SetEventObject( win );
  win->ProcessEvent( event );
};
*/

//-----------------------------------------------------------------------------
// key_press

gint gtk_window_key_press_callback( GtkWidget *WXUNUSED(widget), GdkEventKey *gdk_event, wxWindow *win )
{ 
  if (!win->HasVMT()) return FALSE;
  if (g_blockEventsOnDrag) return FALSE;

/*
  printf( "OnKeyPress from " );
  if (win->GetClassInfo() && win->GetClassInfo()->GetClassName())
    printf( win->GetClassInfo()->GetClassName() );
  printf( ".\n" );
*/

  long key_code = 0;
  switch (gdk_event->keyval)
  {
    case GDK_BackSpace:   key_code = WXK_BACK;     	break;
    case GDK_Tab:         key_code = WXK_TAB;     	break;
    case GDK_Linefeed:    key_code = WXK_RETURN;  	break;
    case GDK_Clear:	  key_code = WXK_CLEAR;   	break;
    case GDK_Return:	  key_code = WXK_RETURN;  	break;
    case GDK_Pause:	  key_code = WXK_PAUSE;   	break;
    case GDK_Scroll_Lock: key_code = WXK_SCROLL; 	break;
    case GDK_Escape:	  key_code = WXK_ESCAPE;  	break;
    case GDK_Delete:	  key_code = WXK_DELETE;  	break;
    case GDK_Home: 	  key_code = WXK_HOME;    	break;
    case GDK_Left:	  key_code = WXK_LEFT;    	break;
    case GDK_Up:	  key_code = WXK_UP;      	break;
    case GDK_Right:	  key_code = WXK_RIGHT;   	break;
    case GDK_Down:	  key_code = WXK_DOWN;    	break;
    case GDK_Prior:	  key_code = WXK_PRIOR;   	break;
//  case GDK_Page_Up:	  key_code = WXK_PAGEUP;  	break;
    case GDK_Next:	  key_code = WXK_NEXT;    	break;
//  case GDK_Page_Down:   key_code = WXK_PAGEDOWN; 	break;
    case GDK_End:	  key_code = WXK_END;     	break;
    case GDK_Begin:	  key_code = WXK_HOME;    	break;
    case GDK_Select:	  key_code = WXK_SELECT;  	break;
    case GDK_Print:	  key_code = WXK_PRINT;   	break;
    case GDK_Execute:	  key_code = WXK_EXECUTE; 	break;
    case GDK_Insert:	  key_code = WXK_INSERT;  	break;
    case GDK_Num_Lock:	  key_code = WXK_NUMLOCK; 	break;
    case GDK_KP_Tab:	  key_code = WXK_TAB;     	break;
    case GDK_KP_Enter:	  key_code = WXK_RETURN;  	break;
    case GDK_KP_Home:	  key_code = WXK_HOME;    	break;
    case GDK_KP_Left:	  key_code = WXK_LEFT;    	break;
    case GDK_KP_Up:	  key_code = WXK_UP;      	break;
    case GDK_KP_Right: 	  key_code = WXK_RIGHT;   	break;
    case GDK_KP_Down:	  key_code = WXK_DOWN;    	break;
    case GDK_KP_Prior:	  key_code = WXK_PRIOR;   	break;
//  case GDK_KP_Page_Up:  key_code = WXK_PAGEUP; 	break;
    case GDK_KP_Next:	  key_code = WXK_NEXT;    	break;
//  case GDK_KP_Page_Down: key_code = WXK_PAGEDOWN; 	break;
    case GDK_KP_End:	  key_code = WXK_END;     	break;
    case GDK_KP_Begin:	  key_code = WXK_HOME;    	break;
    case GDK_KP_Insert:	  key_code = WXK_INSERT;  	break;
    case GDK_KP_Delete:	  key_code = WXK_DELETE;  	break;
    case GDK_KP_Multiply: key_code = WXK_MULTIPLY; 	break;
    case GDK_KP_Add:	  key_code = WXK_ADD;     	break;
    case GDK_KP_Separator: key_code = WXK_SEPARATOR; 	break;
    case GDK_KP_Subtract: key_code = WXK_SUBTRACT; 	break;
    case GDK_KP_Decimal:  key_code = WXK_DECIMAL; 	break;
    case GDK_KP_Divide:	  key_code = WXK_DIVIDE;  	break;
    case GDK_KP_0:	  key_code = WXK_NUMPAD0; 	break;
    case GDK_KP_1:	  key_code = WXK_NUMPAD1; 	break;
    case GDK_KP_2:	  key_code = WXK_NUMPAD2; 	break;
    case GDK_KP_3:	  key_code = WXK_NUMPAD3; 	break;
    case GDK_KP_4:	  key_code = WXK_NUMPAD4; 	break;
    case GDK_KP_5:	  key_code = WXK_NUMPAD5; 	break;
    case GDK_KP_6:	  key_code = WXK_NUMPAD6; 	break;
    case GDK_KP_7:	  key_code = WXK_NUMPAD7; 	break;
    case GDK_KP_8:	  key_code = WXK_NUMPAD7; 	break;
    case GDK_KP_9:	  key_code = WXK_NUMPAD9; 	break;
    case GDK_F1:	  key_code = WXK_F1;		break;
    case GDK_F2:	  key_code = WXK_F2;		break;
    case GDK_F3:	  key_code = WXK_F3;		break;
    case GDK_F4:	  key_code = WXK_F4;		break;
    case GDK_F5:	  key_code = WXK_F5;		break;
    case GDK_F6:	  key_code = WXK_F6;		break;
    case GDK_F7:	  key_code = WXK_F7;		break;
    case GDK_F8:	  key_code = WXK_F8;		break;
    case GDK_F9:	  key_code = WXK_F9;		break;
    case GDK_F10:	  key_code = WXK_F10;		break;
    case GDK_F11:	  key_code = WXK_F11;		break;
    case GDK_F12:	  key_code = WXK_F12;		break;
    default:
    {
      if ((gdk_event->keyval >= 0x20) && (gdk_event->keyval <= 0xFF)) 
        key_code = gdk_event->keyval;
    };
  };

  if (!key_code) return FALSE;
  
  wxKeyEvent event( wxEVT_CHAR );
  event.m_shiftDown = (gdk_event->state & GDK_SHIFT_MASK);
  event.m_controlDown = (gdk_event->state & GDK_CONTROL_MASK);
  event.m_altDown = (gdk_event->state & GDK_MOD1_MASK);
  event.m_metaDown = (gdk_event->state & GDK_MOD2_MASK);
  event.m_keyCode = key_code;
  event.m_x = 0;
  event.m_y = 0;
  event.SetEventObject( win );
  return win->ProcessEvent( event );
};

//-----------------------------------------------------------------------------
// button_press

gint gtk_window_button_press_callback( GtkWidget *widget, GdkEventButton *gdk_event, wxWindow *win )
{ 
  if (widget->window != gdk_event->window) return FALSE;
  if (g_blockEventsOnDrag) return FALSE;

  if (win->m_wxwindow)
  {
    if (GTK_WIDGET_CAN_FOCUS(win->m_wxwindow) && !GTK_WIDGET_HAS_FOCUS (win->m_wxwindow) )
    {
      gtk_widget_grab_focus (win->m_wxwindow);
      
/*
      printf( "GrabFocus from " );
      if (win->GetClassInfo() && win->GetClassInfo()->GetClassName())
        printf( win->GetClassInfo()->GetClassName() );
      printf( ".\n" );
*/
      
    };
  };
    
  if (!win->HasVMT()) return FALSE;
    
/*
  printf( "OnButtonPress from " );
  if (win->GetClassInfo() && win->GetClassInfo()->GetClassName())
    printf( win->GetClassInfo()->GetClassName() );
  printf( ".\n" );
*/
  
  wxEventType event_type = wxEVT_LEFT_DOWN;
  
  if (gdk_event->button == 1)
  {
    switch (gdk_event->type)
    {
      case GDK_BUTTON_PRESS: event_type = wxEVT_LEFT_DOWN; break;
      case GDK_2BUTTON_PRESS: event_type = wxEVT_LEFT_DCLICK; break;
      default:  break;
    };
  }
  else if (gdk_event->button == 2)
  {
    switch (gdk_event->type)
    {
      case GDK_BUTTON_PRESS: event_type = wxEVT_MIDDLE_DOWN; break;
      case GDK_2BUTTON_PRESS: event_type = wxEVT_MIDDLE_DCLICK; break;
      default:  break;
    };
  }
  else if (gdk_event->button == 3)
  {
    switch (gdk_event->type)
    {
      case GDK_BUTTON_PRESS: event_type = wxEVT_RIGHT_DOWN; break;
      case GDK_2BUTTON_PRESS: event_type = wxEVT_RIGHT_DCLICK; break;
      default:  break;
    };
  };
  
  wxMouseEvent event( event_type );
  event.m_shiftDown = (gdk_event->state & GDK_SHIFT_MASK);
  event.m_controlDown = (gdk_event->state & GDK_CONTROL_MASK);
  event.m_altDown = (gdk_event->state & GDK_MOD1_MASK);
  event.m_metaDown = (gdk_event->state & GDK_MOD2_MASK);
  event.m_leftDown = (gdk_event->state & GDK_BUTTON1_MASK);
  event.m_middleDown = (gdk_event->state & GDK_BUTTON2_MASK);
  event.m_rightDown = (gdk_event->state & GDK_BUTTON3_MASK);
  
  event.m_x = (long)gdk_event->x;
  event.m_y = (long)gdk_event->y;
  event.SetEventObject( win );
  
  win->ProcessEvent( event );
  
  return TRUE;
};

//-----------------------------------------------------------------------------
// button_release

gint gtk_window_button_release_callback( GtkWidget *widget, GdkEventButton *gdk_event, wxWindow *win )
{ 
  if (widget->window != gdk_event->window) return TRUE;

  if (g_blockEventsOnDrag) return FALSE;

  if (!win->HasVMT()) return FALSE;
 
/*
  printf( "OnButtonRelease from " );
  if (win->GetClassInfo() && win->GetClassInfo()->GetClassName())
    printf( win->GetClassInfo()->GetClassName() );
  printf( ".\n" );
*/
  
  wxEventType event_type = wxEVT_NULL;
  
  switch (gdk_event->button)
  {
    case 1: event_type = wxEVT_LEFT_UP; break;
    case 2: event_type = wxEVT_MIDDLE_UP; break;
    case 3: event_type = wxEVT_RIGHT_UP; break;
  };

  wxMouseEvent event( event_type );
  event.m_shiftDown = (gdk_event->state & GDK_SHIFT_MASK);
  event.m_controlDown = (gdk_event->state & GDK_CONTROL_MASK);
  event.m_altDown = (gdk_event->state & GDK_MOD1_MASK);
  event.m_metaDown = (gdk_event->state & GDK_MOD2_MASK);
  event.m_leftDown = (gdk_event->state & GDK_BUTTON1_MASK);
  event.m_middleDown = (gdk_event->state & GDK_BUTTON2_MASK);
  event.m_rightDown = (gdk_event->state & GDK_BUTTON3_MASK);
  event.m_x = (long)gdk_event->x;
  event.m_y = (long)gdk_event->y;
  event.SetEventObject( win );
  
  return win->ProcessEvent( event );
};

//-----------------------------------------------------------------------------
// motion_notify

gint gtk_window_motion_notify_callback( GtkWidget *widget, GdkEventMotion *gdk_event, wxWindow *win )
{ 
  if (widget->window != gdk_event->window) return TRUE;

  if (g_blockEventsOnDrag) return FALSE;

  if (!win->HasVMT()) return FALSE;
  
/*
  printf( "OnMotion from " );
  if (win->GetClassInfo() && win->GetClassInfo()->GetClassName())
    printf( win->GetClassInfo()->GetClassName() );
  printf( ".\n" );
*/
  
  wxMouseEvent event( wxEVT_MOTION );
  event.m_shiftDown = (gdk_event->state & GDK_SHIFT_MASK);
  event.m_controlDown = (gdk_event->state & GDK_CONTROL_MASK);
  event.m_altDown = (gdk_event->state & GDK_MOD1_MASK);
  event.m_metaDown = (gdk_event->state & GDK_MOD2_MASK);
  event.m_leftDown = (gdk_event->state & GDK_BUTTON1_MASK);
  event.m_middleDown = (gdk_event->state & GDK_BUTTON2_MASK);
  event.m_rightDown = (gdk_event->state & GDK_BUTTON3_MASK);
  
  event.m_x = (long)gdk_event->x;
  event.m_y = (long)gdk_event->y;
  event.SetEventObject( win );
  
  win->ProcessEvent( event );
  
  return FALSE;
};

//-----------------------------------------------------------------------------
// focus_in

void gtk_window_focus_in_callback( GtkWidget *WXUNUSED(widget), GdkEvent *WXUNUSED(event), wxWindow *win )
{
  if (g_blockEventsOnDrag) return;
  if (win->m_wxwindow)
  {
    if (GTK_WIDGET_CAN_FOCUS(win->m_wxwindow))
    {
      GTK_WIDGET_SET_FLAGS (win->m_wxwindow, GTK_HAS_FOCUS);
/*      
      printf( "SetFocus flag from " );
      if (win->GetClassInfo() && win->GetClassInfo()->GetClassName())
        printf( win->GetClassInfo()->GetClassName() );
      printf( ".\n" );
*/
    };
  };
  
  if (!win->HasVMT()) return;
  
/*
  printf( "OnSetFocus from " );
  if (win->GetClassInfo() && win->GetClassInfo()->GetClassName())
    printf( win->GetClassInfo()->GetClassName() );
  printf( "   " );
  printf( WXSTRINGCAST win->GetLabel() );
  printf( ".\n" );
*/
  
  wxFocusEvent event( wxEVT_SET_FOCUS, win->GetId() );
  event.SetEventObject( win );
  win->ProcessEvent( event );
};

//-----------------------------------------------------------------------------
// focus out

void gtk_window_focus_out_callback( GtkWidget *WXUNUSED(widget), GdkEvent *WXUNUSED(event), wxWindow *win )
{
  if (g_blockEventsOnDrag) return;
  if (win->m_wxwindow)
  {
    if (GTK_WIDGET_CAN_FOCUS(win->m_wxwindow))
      GTK_WIDGET_UNSET_FLAGS (win->m_wxwindow, GTK_HAS_FOCUS);
  };
  
  if (!win->HasVMT()) return;
  
/*
  printf( "OnKillFocus from " );
  if (win->GetClassInfo() && win->GetClassInfo()->GetClassName())
    printf( win->GetClassInfo()->GetClassName() );
  printf( ".\n" );
*/
  
  wxFocusEvent event( wxEVT_KILL_FOCUS, win->GetId() );
  event.SetEventObject( win );
  win->ProcessEvent( event );
};

//-----------------------------------------------------------------------------
// vertical scroll

void gtk_window_vscroll_callback( GtkWidget *WXUNUSED(widget), wxWindow *win )
{
  if (g_blockEventsOnDrag) return;

/*
  printf( "OnVScroll from " );
  if (win->GetClassInfo() && win->GetClassInfo()->GetClassName())
    printf( win->GetClassInfo()->GetClassName() );
  printf( ".\n" );
*/
  
  if (!win->HasVMT()) return;
  
  float diff = win->m_vAdjust->value - win->m_oldVerticalPos;
  if (fabs(diff) < 0.2) return;
  
/*
  int i = (int)(win->m_oldVerticalPos+0.5);
  printf( "Old value: %d.\n", i );
  i = (int)(win->m_vAdjust->value+0.5);
  printf( "Sending new value: %d.\n", i );
*/

  wxEventType command = wxEVT_NULL;
  
  float line_step = win->m_vAdjust->step_increment;
  float page_step = win->m_vAdjust->page_increment;
  
  if (fabs(diff-line_step) < 0.2) command = wxEVT_SCROLL_LINEDOWN;
  else if (fabs(diff+line_step) < 0.2) command = wxEVT_SCROLL_LINEUP;
  else if (fabs(diff-page_step) < 0.2) command = wxEVT_SCROLL_PAGEDOWN;
  else if (fabs(diff+page_step) < 0.2) command = wxEVT_SCROLL_PAGEUP;
  else command = wxEVT_SCROLL_THUMBTRACK;
      
  int value = (int)(win->m_vAdjust->value+0.5);

  wxScrollEvent event( command, win->GetId(), value, wxVERTICAL );
  event.SetEventObject( win );
  win->ProcessEvent( event );
};

//-----------------------------------------------------------------------------
// horizontal scroll

void gtk_window_hscroll_callback( GtkWidget *WXUNUSED(widget), wxWindow *win )
{ 
  if (g_blockEventsOnDrag) return;
  
/*
  printf( "OnHScroll from " );
  if (win->GetClassInfo() && win->GetClassInfo()->GetClassName())
    printf( win->GetClassInfo()->GetClassName() );
  printf( ".\n" );
*/
  
  if (!win->HasVMT()) return;
    
  float diff = win->m_hAdjust->value - win->m_oldHorizontalPos;
  if (fabs(diff) < 0.2) return;
  
/*
  int i = (int)(win->m_oldHorizontalPos+0.5);
  printf( "Old value: %d.\n", i );
  i = (int)(win->m_hAdjust->value+0.5);
  printf( "Sending new value: %d.\n", i );
*/
      
  wxEventType command = wxEVT_NULL;
  
  float line_step = win->m_hAdjust->step_increment;
  float page_step = win->m_hAdjust->page_increment;
  
  if (fabs(diff-line_step) < 0.2) command = wxEVT_SCROLL_LINEDOWN;
  else if (fabs(diff+line_step) < 0.2) command = wxEVT_SCROLL_LINEUP;
  else if (fabs(diff-page_step) < 0.2) command = wxEVT_SCROLL_PAGEDOWN;
  else if (fabs(diff+page_step) < 0.2) command = wxEVT_SCROLL_PAGEUP;
  else command = wxEVT_SCROLL_THUMBTRACK;

  int value = (int)(win->m_hAdjust->value+0.5);
      
  wxScrollEvent event( command, win->GetId(), value, wxHORIZONTAL );
  event.SetEventObject( win );
  win->ProcessEvent( event );
};

//-----------------------------------------------------------------------------
// vertical scroll change

void gtk_window_vscroll_change_callback( GtkWidget *WXUNUSED(widget), wxWindow *win )
{
  if (g_blockEventsOnDrag) return;

/*
  printf( "OnVScroll change from " );
  if (win->GetClassInfo() && win->GetClassInfo()->GetClassName())
    printf( win->GetClassInfo()->GetClassName() );
  printf( ".\n" );
*/
  
  if (!win->HasVMT()) return;
  
  wxEventType command = wxEVT_SCROLL_THUMBTRACK;
  int value = (int)(win->m_vAdjust->value+0.5);

  wxScrollEvent event( command, win->GetId(), value, wxVERTICAL );
  event.SetEventObject( win );
  win->ProcessEvent( event );
};

//-----------------------------------------------------------------------------
// horizontal scroll change

void gtk_window_hscroll_change_callback( GtkWidget *WXUNUSED(widget), wxWindow *win )
{ 
  if (g_blockEventsOnDrag) return;
  
/*
  printf( "OnHScroll change from " );
  if (win->GetClassInfo() && win->GetClassInfo()->GetClassName())
    printf( win->GetClassInfo()->GetClassName() );
  printf( ".\n" );
*/
  
  if (!win->HasVMT()) return;
    
  wxEventType command = wxEVT_SCROLL_THUMBTRACK;
  int value = (int)(win->m_hAdjust->value+0.5);
      
  wxScrollEvent event( command, win->GetId(), value, wxHORIZONTAL );
  event.SetEventObject( win );
  win->ProcessEvent( event );
};

//-----------------------------------------------------------------------------
// drop

void gtk_window_drop_callback( GtkWidget *widget, GdkEvent *event, wxWindow *win )
{
  printf( "OnDrop.\n" );

  if (win->GetDropTarget())
  {
    int x = 0;
    int y = 0;
    gdk_window_get_pointer( widget->window, &x, &y, NULL );
    win->GetDropTarget()->Drop( event, x, y );
  };
  
/*
  g_free (event->dropdataavailable.data);
  g_free (event->dropdataavailable.data_type);
*/
}

//-----------------------------------------------------------------------------
// destroy

bool gtk_window_destroy_callback( GtkWidget *WXUNUSED(widget), GdkEvent *WXUNUSED(event), wxWindow *win )
{ 
  printf( "OnDestroy from " );
  if (win->GetClassInfo() && win->GetClassInfo()->GetClassName())
    printf( win->GetClassInfo()->GetClassName() );
  printf( ".\n" );
  printf( "Goodbye.\n" );
  printf( "     Robert Roebling.\n" );
  
  return FALSE;
};

//-----------------------------------------------------------------------------
// enter

bool gtk_window_enter_callback( GtkWidget *widget, GdkEvent *WXUNUSED(event), wxWindow *win )
{
  if (g_blockEventsOnDrag) return FALSE;
  
  if (widget->window)
    gdk_window_set_cursor( widget->window, win->m_cursor->GetCursor() );
    
  return TRUE;
};
    
//-----------------------------------------------------------------------------
// leave

bool gtk_window_leave_callback( GtkWidget *widget, GdkEvent *WXUNUSED(event), wxWindow *WXUNUSED(win) )
{
  if (g_blockEventsOnDrag) return FALSE;
  
  if (widget->window)
    gdk_window_set_cursor( widget->window, wxSTANDARD_CURSOR->GetCursor() );
    
  return TRUE;
};
    
//-----------------------------------------------------------------------------
// wxWindow implementation
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxWindow,wxEvtHandler)

BEGIN_EVENT_TABLE(wxWindow, wxEvtHandler)
//  EVT_CHAR(wxWindow::OnChar)
  EVT_SIZE(wxWindow::OnSize)
//  EVT_ERASE_BACKGROUND(wxWindow::OnEraseBackground)
  EVT_SYS_COLOUR_CHANGED(wxWindow::OnSysColourChanged)
  EVT_INIT_DIALOG(wxWindow::OnInitDialog)
//  EVT_IDLE(wxWindow::OnIdle)
END_EVENT_TABLE()

wxWindow::wxWindow()
{
  m_widget = NULL;
  m_wxwindow = NULL;
  m_parent = NULL;
  m_children.DeleteContents( FALSE );
  m_x = 0;
  m_y = 0;
  m_width = 0;
  m_height = 0;
  m_retCode = 0;
  m_eventHandler = this;
  m_windowValidator = NULL;
  m_windowId = -1;
  m_cursor = new wxCursor( wxCURSOR_ARROW );
  m_font = *wxSWISS_FONT;
  m_windowStyle = 0;
  m_windowName = "noname";
  m_constraints = NULL;
  m_constraintsInvolvedIn = NULL;
  m_windowSizer = NULL;
  m_sizerParent = NULL;
  m_autoLayout = FALSE;
  m_sizeSet = FALSE;
  m_hasVMT = FALSE;
  m_needParent = TRUE;
  m_hasScrolling = FALSE;
  m_hAdjust = NULL;
  m_vAdjust = NULL;
  m_oldHorizontalPos = 0.0;
  m_oldVerticalPos = 0.0;
  m_isShown = FALSE;
  m_isEnabled = TRUE;
  m_drawingOffsetX = 0;
  m_drawingOffsetY = 0;
  m_pDropTarget = NULL;
};

wxWindow::wxWindow( wxWindow *parent, wxWindowID id,
      const wxPoint &pos, const wxSize &size,
      long style, const wxString &name )
{
  Create( parent, id, pos, size, style, name );
};

bool wxWindow::Create( wxWindow *parent, wxWindowID id,
      const wxPoint &pos, const wxSize &size,
      long style, const wxString &name )
{
  m_isShown = FALSE;
  m_isEnabled = TRUE;
  m_needParent = TRUE;
  
  PreCreation( parent, id, pos, size, style, name );
  
  m_widget = gtk_scrolled_window_new( NULL, NULL );
  m_hasScrolling = TRUE;
  
  GtkScrolledWindow *s_window;
  s_window = GTK_SCROLLED_WINDOW(m_widget);
  
  GtkScrolledWindowClass *scroll_class = GTK_SCROLLED_WINDOW_CLASS( GTK_OBJECT(m_widget)->klass );
  scroll_class->scrollbar_spacing = 0;
  
  gtk_scrolled_window_set_policy( s_window, GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC );
    
  m_oldHorizontalPos = 0.0;
  m_oldVerticalPos = 0.0;
 
  m_hAdjust = gtk_range_get_adjustment( GTK_RANGE(s_window->hscrollbar) );
  m_vAdjust = gtk_range_get_adjustment( GTK_RANGE(s_window->vscrollbar) );
  
  gtk_signal_connect (GTK_OBJECT (m_hAdjust), "value_changed",
		      (GtkSignalFunc) gtk_window_hscroll_callback, (gpointer) this );
  gtk_signal_connect (GTK_OBJECT (m_vAdjust), "value_changed",
		      (GtkSignalFunc) gtk_window_vscroll_callback, (gpointer) this );
		      
  gtk_signal_connect (GTK_OBJECT (m_hAdjust), "changed",
		      (GtkSignalFunc) gtk_window_hscroll_change_callback, (gpointer) this );
  gtk_signal_connect (GTK_OBJECT (m_vAdjust), "changed",
		      (GtkSignalFunc) gtk_window_vscroll_change_callback, (gpointer) this );
  
  GtkViewport *viewport;
  viewport = GTK_VIEWPORT(s_window->viewport);
  
  if (m_windowStyle & wxRAISED_BORDER)
  {
    gtk_viewport_set_shadow_type( viewport, GTK_SHADOW_OUT );
  }
  else if (m_windowStyle & wxSUNKEN_BORDER)
  {
    gtk_viewport_set_shadow_type( viewport, GTK_SHADOW_IN );
  }
  else
  {
    gtk_viewport_set_shadow_type( viewport, GTK_SHADOW_NONE );
  };
    
  m_wxwindow = gtk_myfixed_new();
  
  if (m_windowStyle & wxTAB_TRAVERSAL == wxTAB_TRAVERSAL)
    GTK_WIDGET_UNSET_FLAGS( m_wxwindow, GTK_CAN_FOCUS );
  else
    GTK_WIDGET_SET_FLAGS( m_wxwindow, GTK_CAN_FOCUS );

  gtk_container_add( GTK_CONTAINER(m_widget), m_wxwindow );

  // shut the viewport up
  gtk_viewport_set_hadjustment( viewport, (GtkAdjustment*) gtk_adjustment_new( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0) );
  gtk_viewport_set_vadjustment( viewport, (GtkAdjustment*) gtk_adjustment_new( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0) );
  
  // I _really_ don't want scrollbars in the beginning    
  m_vAdjust->lower = 0.0;
  m_vAdjust->upper = 1.0;
  m_vAdjust->value = 0.0;
  m_vAdjust->step_increment = 1.0;
  m_vAdjust->page_increment = 1.0;
  m_vAdjust->page_size = 5.0;
  gtk_signal_emit_by_name( GTK_OBJECT(m_vAdjust), "changed" );
  m_hAdjust->lower = 0.0;
  m_hAdjust->upper = 1.0;
  m_hAdjust->value = 0.0;
  m_hAdjust->step_increment = 1.0;
  m_hAdjust->page_increment = 1.0;
  m_hAdjust->page_size = 5.0;
  gtk_signal_emit_by_name( GTK_OBJECT(m_hAdjust), "changed" );
  
  gtk_widget_show( m_wxwindow );
  
  PostCreation();
  
  Show( TRUE );

  return TRUE;  
};

wxWindow::~wxWindow(void)
{
  m_hasVMT = FALSE;
  
  if (m_pDropTarget) delete m_pDropTarget;
  
  if (m_parent) m_parent->RemoveChild( this );
  if (m_widget) Show( FALSE );

  DestroyChildren();
  
  if (m_wxwindow) gtk_widget_destroy( m_wxwindow );
    
  if (m_widget) gtk_widget_destroy( m_widget );
  
//  delete m_cursor;

  DeleteRelatedConstraints();
  if (m_constraints)
  {
    // This removes any dangling pointers to this window
    // in other windows' constraintsInvolvedIn lists.
    UnsetConstraints(m_constraints);
    delete m_constraints;
    m_constraints = NULL;
  }
  if (m_windowSizer)
  {
    delete m_windowSizer;
    m_windowSizer = NULL;
  }
  // If this is a child of a sizer, remove self from parent
  if (m_sizerParent)
    m_sizerParent->RemoveChild((wxWindow *)this);

  // Just in case the window has been Closed, but
  // we're then deleting immediately: don't leave
  // dangling pointers.
  wxPendingDelete.DeleteObject(this);

  // Just in case we've loaded a top-level window via
  // wxWindow::LoadNativeDialog but we weren't a dialog
  // class
  wxTopLevelWindows.DeleteObject(this);
    
};

void wxWindow::PreCreation( wxWindow *parent, wxWindowID id,
      const wxPoint &pos, const wxSize &size,
      long style, const wxString &name )
{
  if (m_needParent && (parent == NULL))
    wxFatalError( "Need complete parent.", name );

  m_widget = NULL;
  m_hasVMT = FALSE;
  m_parent = parent;
  m_children.DeleteContents( FALSE );
  m_x = (int)pos.x;
  m_y = (int)pos.y;
  m_width = size.x;
  if (m_width == -1) m_width = 20;
  m_height = size.y;
  if (m_height == -1) m_height = 20;
  m_retCode = 0;
  m_eventHandler = this;
  m_windowValidator = NULL;
  m_windowId = id;
  m_sizeSet = FALSE;
  m_cursor = new wxCursor( wxCURSOR_ARROW );
  m_font = *wxSWISS_FONT;
  m_backgroundColour = wxWHITE;
  m_foregroundColour = wxBLACK;
  m_windowStyle = style;
  m_windowName = name;
  m_constraints = NULL;
  m_constraintsInvolvedIn = NULL;
  m_windowSizer = NULL;
  m_sizerParent = NULL;
  m_autoLayout = FALSE;
  m_drawingOffsetX = 0;
  m_drawingOffsetY = 0;
  m_pDropTarget = NULL;
}

void wxWindow::PostCreation(void)
{
  if (m_parent) m_parent->AddChild( this );
  
//  GtkStyle *style = m_widget->style;
//  style->font = m_font.GetInternalFont( 1.0 );          // destroy old font ?
  
  GtkWidget *connect_widget = m_widget;
  if (m_wxwindow) connect_widget = m_wxwindow;
 
  gtk_object_set_data (GTK_OBJECT (connect_widget), "MyWxWindow", (gpointer)this );
  
  if (m_wxwindow)
  {
    gtk_signal_connect( GTK_OBJECT(m_wxwindow), "expose_event", 
      GTK_SIGNAL_FUNC(gtk_window_expose_callback), (gpointer)this );
      
    gtk_signal_connect( GTK_OBJECT(m_wxwindow), "draw", 
      GTK_SIGNAL_FUNC(gtk_window_draw_callback), (gpointer)this );
  };
  
/*
  gtk_signal_connect( GTK_OBJECT(m_widget), "size_allocate", 
    GTK_SIGNAL_FUNC(gtk_window_size_callback), (gpointer)this );
*/

  gtk_signal_connect( GTK_OBJECT(connect_widget), "key_press_event",
    GTK_SIGNAL_FUNC(gtk_window_key_press_callback), (gpointer)this );

  gtk_signal_connect( GTK_OBJECT(connect_widget), "button_press_event",
    GTK_SIGNAL_FUNC(gtk_window_button_press_callback), (gpointer)this );
    
  gtk_signal_connect( GTK_OBJECT(connect_widget), "button_release_event",
    GTK_SIGNAL_FUNC(gtk_window_button_release_callback), (gpointer)this );
    
  gtk_signal_connect( GTK_OBJECT(connect_widget), "motion_notify_event",
    GTK_SIGNAL_FUNC(gtk_window_motion_notify_callback), (gpointer)this );
    
  gtk_signal_connect( GTK_OBJECT(connect_widget), "focus_in_event", 
    GTK_SIGNAL_FUNC(gtk_window_focus_in_callback), (gpointer)this );

  gtk_signal_connect( GTK_OBJECT(connect_widget), "focus_out_event", 
    GTK_SIGNAL_FUNC(gtk_window_focus_out_callback), (gpointer)this );

  gtk_signal_connect( GTK_OBJECT(connect_widget), "drop_data_available_event",
    GTK_SIGNAL_FUNC(gtk_window_drop_callback), (gpointer)this );
      
  // Only for cursor handling
    
  gtk_signal_connect( GTK_OBJECT(m_widget), "enter_notify_event", 
    GTK_SIGNAL_FUNC(gtk_window_enter_callback), (gpointer)this );
    
  gtk_signal_connect( GTK_OBJECT(m_widget), "leave_notify_event", 
    GTK_SIGNAL_FUNC(gtk_window_leave_callback), (gpointer)this );
    
  if (m_wxwindow)
  {
    gtk_signal_connect( GTK_OBJECT(m_wxwindow), "enter_notify_event", 
      GTK_SIGNAL_FUNC(gtk_window_enter_callback), (gpointer)this );
      
    gtk_signal_connect( GTK_OBJECT(m_wxwindow), "leave_notify_event", 
      GTK_SIGNAL_FUNC(gtk_window_leave_callback), (gpointer)this );
  };
  
/*
  // Does destroy ever get called ?

  gtk_signal_connect( GTK_OBJECT(m_widget), "destroy_event", 
    GTK_SIGNAL_FUNC(gtk_window_destroy_callback), (gpointer)this );
    
  if (m_wxwindow)
  {
    gtk_signal_connect( GTK_OBJECT(m_wxwindow), "destroy_event", 
      GTK_SIGNAL_FUNC(gtk_window_destroy_callback), (gpointer)this );
  };
*/
  
  if (m_widget && m_parent) gtk_widget_realize( m_widget );
  if (m_wxwindow) gtk_widget_realize( m_wxwindow );
  
  SetCursor( wxSTANDARD_CURSOR );
  
  m_hasVMT = TRUE;
};

bool wxWindow::HasVMT(void)
{
  return m_hasVMT;
};

bool wxWindow::Close( bool force )
{
  wxCloseEvent event(wxEVT_CLOSE_WINDOW, m_windowId);
  event.SetEventObject(this);
  event.SetForce(force);

  return GetEventHandler()->ProcessEvent(event);
};

bool wxWindow::Destroy(void)
{
  m_hasVMT = FALSE;
  delete this;
  return TRUE;
};

bool wxWindow::DestroyChildren(void)
{
  if (GetChildren()) 
  {
    wxNode *node;
    while ((node = GetChildren()->First()) != (wxNode *)NULL) 
    {
      wxWindow *child;
      if ((child = (wxWindow *)node->Data()) != (wxWindow *)NULL) 
      {
        delete child;
	if (GetChildren()->Member(child)) delete node;
      };
    };
  };
  return TRUE;
};

void wxWindow::PrepareDC( wxDC &WXUNUSED(dc) )
{
  // are we to set fonts here ?
};

void wxWindow::ImplementSetSize(void)
{ 
  gtk_widget_set_usize( m_widget, m_width, m_height );
};

void wxWindow::ImplementSetPosition(void)
{
  if ((m_parent) && (m_parent->m_wxwindow))
    gtk_myfixed_move( GTK_MYFIXED(m_parent->m_wxwindow), m_widget, m_x, m_y );
  else
    gtk_widget_set_uposition( m_widget, m_x, m_y );
};

void wxWindow::SetSize( int x, int y, int width, int height, int sizeFlags )
{
  int newX = x;
  int newY = y;
  int newW = width;
  int newH = height;
  
  if ((sizeFlags & wxSIZE_USE_EXISTING) == wxSIZE_USE_EXISTING)
  {
    if (newX == -1) newX = m_x;
    if (newY == -1) newY = m_y;
    if (newW == -1) newW = m_width;
    if (newH == -1) newH = m_height;
  };
  
  if ((sizeFlags & wxSIZE_AUTO_WIDTH) == wxSIZE_AUTO_WIDTH)
  {
    if (newW == -1) newW = 80;
  };
  
  if ((sizeFlags & wxSIZE_AUTO_HEIGHT) == wxSIZE_AUTO_HEIGHT)
  {
    if (newH == -1) newH = 26;
  };
  
  if ((m_x != newX) || (m_y != newY) || (!m_sizeSet))
  {
    m_x = newX;
    m_y = newY;
    ImplementSetPosition();
  };
  if ((m_width != newW) || (m_height != newH) || (!m_sizeSet))
  {
    m_width = newW;
    m_height = newH;
    ImplementSetSize();
  };
  m_sizeSet = TRUE;
  
  wxSizeEvent event( wxSize(m_width,m_height), GetId() );
  event.SetEventObject( this );
  ProcessEvent( event );
};

void wxWindow::SetSize( int width, int height )
{
  SetSize( -1, -1, width, height, wxSIZE_USE_EXISTING );
};

void wxWindow::Move( int x, int y )
{
  SetSize( x, y, -1, -1, wxSIZE_USE_EXISTING );
};

void wxWindow::GetSize( int *width, int *height ) const
{
  (*width) = m_width;
  (*height) = m_height;
};

void wxWindow::SetClientSize( int width, int height )
{
  if (!m_wxwindow)
  {
    SetSize( width, height );
  }
  else
  {
    int dw = 0;
    int dh = 0;
    
    if (!m_hasScrolling)
    {
/*
      do we have sunken dialogs ?
      
      GtkStyleClass *window_class = m_wxwindow->style->klass;
    
      dw += 2 * window_class->xthickness;
      dh += 2 * window_class->ythickness;
*/
    }
    else
    {
      GtkScrolledWindow *scroll_window = GTK_SCROLLED_WINDOW(m_widget);
      GtkScrolledWindowClass *scroll_class = GTK_SCROLLED_WINDOW_CLASS( GTK_OBJECT(m_widget)->klass );
    
      GtkWidget *viewport = scroll_window->viewport;
      GtkStyleClass *viewport_class = viewport->style->klass;
    
      GtkWidget *hscrollbar = scroll_window->hscrollbar;
      GtkWidget *vscrollbar = scroll_window->vscrollbar;
    
      if ((m_windowStyle & wxRAISED_BORDER) ||
          (m_windowStyle & wxSUNKEN_BORDER)
	 )
      {
        dw += 2 * viewport_class->xthickness;
        dh += 2 * viewport_class->ythickness;
      };
    
      if (GTK_WIDGET_VISIBLE(vscrollbar))
      {
        dw += vscrollbar->allocation.width;
        dw += scroll_class->scrollbar_spacing;
      };
    
      if (GTK_WIDGET_VISIBLE(hscrollbar))
      {
        dh += hscrollbar->allocation.height;
        dw += scroll_class->scrollbar_spacing;
      };
    };
    
    SetSize( width+dw, height+dh );
  };
};

void wxWindow::GetClientSize( int *width, int *height ) const
{
  if (!m_wxwindow)
  {
    if (width) (*width) = m_width;
    if (height) (*height) = m_height;
  }
  else
  {
    int dw = 0;
    int dh = 0;
    
    if (!m_hasScrolling)
    {
/*
      do we have sunken dialogs ?
      
      GtkStyleClass *window_class = m_wxwindow->style->klass;
    
      dw += 2 * window_class->xthickness;
      dh += 2 * window_class->ythickness;
*/
    }
    else
    {
      GtkScrolledWindow *scroll_window = GTK_SCROLLED_WINDOW(m_widget);
      GtkScrolledWindowClass *scroll_class = GTK_SCROLLED_WINDOW_CLASS( GTK_OBJECT(m_widget)->klass );
    
      GtkWidget *viewport = scroll_window->viewport;
      GtkStyleClass *viewport_class = viewport->style->klass;
    
      GtkWidget *hscrollbar = scroll_window->hscrollbar;
      GtkWidget *vscrollbar = scroll_window->vscrollbar;
    
      if ((m_windowStyle & wxRAISED_BORDER) ||
          (m_windowStyle & wxSUNKEN_BORDER)
	 )
      {
        dw += 2 * viewport_class->xthickness;
        dh += 2 * viewport_class->ythickness;
      };
    
      if (GTK_WIDGET_VISIBLE(vscrollbar))
      {
//        dw += vscrollbar->allocation.width;
        dw += 15;                               // range.slider_width = 11 + 2*2pts edge
        dw += scroll_class->scrollbar_spacing;
      };
    
      if (GTK_WIDGET_VISIBLE(hscrollbar))
      {
//        dh += hscrollbar->allocation.height;
        dh += 15;
        dh += scroll_class->scrollbar_spacing;
      };
    };
    
    if (width) (*width) = m_width - dw;
    if (height) (*height) = m_height - dh;
  };
};

void wxWindow::GetPosition( int *x, int *y ) const
{
  if (x) (*x) = m_x;
  if (y) (*y) = m_y;
};

void wxWindow::ClientToScreen( int *x, int *y )
{
  // Does this look simple ?

  GdkWindow *source = NULL;
  if (m_wxwindow)
    source = m_wxwindow->window;
  else
    source = m_widget->window;
    
  int org_x = 0;
  int org_y = 0;
  gdk_window_get_origin( source, &org_x, &org_y );

  if (!m_wxwindow)
  {  
    if (GTK_WIDGET_NO_WINDOW (m_widget))
    {
      org_x += m_widget->allocation.x;
      org_y += m_widget->allocation.y;
    };
  };
  
  if (x) *x += org_x;  
  if (y) *y += org_y;  
};

void wxWindow::ScreenToClient( int *x, int *y )
{
  GdkWindow *source = NULL;
  if (m_wxwindow)
    source = m_wxwindow->window;
  else
    source = m_widget->window;
    
  int org_x = 0;
  int org_y = 0;
  gdk_window_get_origin( source, &org_x, &org_y );

  if (!m_wxwindow)
  {  
    if (GTK_WIDGET_NO_WINDOW (m_widget))
    {
      org_x += m_widget->allocation.x;
      org_y += m_widget->allocation.y;
    };
  };
  
  if (x) *x -= org_x;  
  if (y) *y -= org_y;  
};

void wxWindow::Centre( int direction )
{
  int x = 0;
  int y = 0;
  GetPosition( &x, &y );
  if (this->IsKindOf(CLASSINFO(wxDialog)) || this->IsKindOf(CLASSINFO(wxFrame)))
  {
    if (direction & wxHORIZONTAL == wxHORIZONTAL) x = (gdk_screen_width () - m_width) / 2;
    if (direction & wxVERTICAL == wxVERTICAL) y = (gdk_screen_height () - m_height) / 2;
    gtk_widget_set_uposition( m_widget, x, y );
  }
  else
  {
    if (m_parent)
    {
      int p_w = 0;
      int p_h = 0;
      m_parent->GetSize( &p_w, &p_h );
      if (direction & wxHORIZONTAL == wxHORIZONTAL) x = (p_w - m_width) / 2;
      if (direction & wxVERTICAL == wxVERTICAL) y = (p_h - m_height) / 2;
      gtk_widget_set_uposition( m_widget, x, y );
    };
  }
};

void wxWindow::Fit(void)
{
	int maxX = 0;
	int maxY = 0;
	wxNode *node = GetChildren()->First();
	while ( node )
	{
		wxWindow *win = (wxWindow *)node->Data();
		int wx, wy, ww, wh;
		win->GetPosition(&wx, &wy);
		win->GetSize(&ww, &wh);
		if ( wx + ww > maxX )
			maxX = wx + ww;
		if ( wy + wh > maxY )
			maxY = wy + wh;

		node = node->Next();
	}
	SetClientSize(maxX + 5, maxY + 5);
};

void wxWindow::OnSize( wxSizeEvent &WXUNUSED(event) )
{
  if (GetAutoLayout()) Layout();
};

bool wxWindow::Show( bool show )
{
  if (show)
    gtk_widget_show( m_widget );
  else
    gtk_widget_hide( m_widget );
  m_isShown = show;  
  return TRUE;
};

void wxWindow::Enable( bool enable )
{
  m_isEnabled = enable;
  gtk_widget_set_sensitive( m_widget, enable );
  if (m_wxwindow) gtk_widget_set_sensitive( m_wxwindow, enable );
};

void wxWindow::MakeModal( bool modal )
{
  return;
  // Disable all other windows
  if (this->IsKindOf(CLASSINFO(wxDialog)) || this->IsKindOf(CLASSINFO(wxFrame)))
  {
    wxNode *node = wxTopLevelWindows.First();
    while (node)
    {
      wxWindow *win = (wxWindow *)node->Data();
      if (win != this)
        win->Enable(!modal);

      node = node->Next();
    }
  }
}

void wxWindow::SetFocus(void)
{
  GtkWidget *connect_widget = m_widget;
  if (m_wxwindow) connect_widget = m_wxwindow;
  if (connect_widget)
  {
    if (GTK_WIDGET_CAN_FOCUS(connect_widget) && !GTK_WIDGET_HAS_FOCUS (connect_widget) )
    {
      gtk_widget_grab_focus (connect_widget);
    };
  };
};

bool wxWindow::OnClose(void)
{
  printf( "OnClose event.\n" );
  return TRUE;
};

void wxWindow::AddChild( wxWindow *child )
{
  // Addchild is (often) called before the program
  // has left the parents constructor so that no
  // virtual tables work yet. The approach below
  // practically imitates virtual tables, i.e. it
  // implements a different AddChild() behaviour
  // for wxFrame, wxDialog, wxWindow and 
  // wxMDIParentFrame.

  if (IsKindOf(CLASSINFO(wxMDIParentFrame)))
  {
    if (child->IsKindOf(CLASSINFO(wxMDIChildFrame)))
    {
      wxMDIClientWindow *client = ((wxMDIParentFrame*)this)->GetClientWindow();
      if (client)
      { 
        client->AddChild( child );
        return;
      };
    };
  };
  
  if (IsKindOf(CLASSINFO(wxNotebook)))
  {
    wxNotebook *tab = (wxNotebook*)this;
    tab->AddChild( child );
    return;
  };
  
  m_children.Append( child );
  if (child->IsKindOf(CLASSINFO(wxFrame)) || child->IsKindOf(CLASSINFO(wxDialog)))
  {
    if ((child->m_x != -1) && (child->m_y != -1))
      gtk_widget_set_uposition( child->m_widget, child->m_x, child->m_y );
  }
  else
  {
    if (m_wxwindow)
      gtk_myfixed_put( GTK_MYFIXED(m_wxwindow), child->m_widget, child->m_x, child->m_y );
  };
  gtk_widget_set_usize( child->m_widget, child->m_width, child->m_height );
};

wxList *wxWindow::GetChildren(void)
{
  return (&m_children);
};

void wxWindow::RemoveChild( wxWindow *child )
{
  if (GetChildren())
 GetChildren()->DeleteObject( child );
  child->m_parent = NULL;
};

void wxWindow::SetReturnCode( int retCode )
{
  m_retCode = retCode;
};

int wxWindow::GetReturnCode(void)
{
  return m_retCode;
};

wxWindow *wxWindow::GetParent(void)
{
  return m_parent;
};

wxEvtHandler *wxWindow::GetEventHandler(void)
{
  return m_eventHandler;
};

void wxWindow::SetEventhandler( wxEvtHandler *handler )
{
  m_eventHandler = handler;
};

wxValidator *wxWindow::GetValidator(void)
{
  return m_windowValidator;
};

void wxWindow::SetValidator( wxValidator *validator )
{
  m_windowValidator = validator;
};

bool wxWindow::IsBeingDeleted(void)
{
  return FALSE;
};

void wxWindow::SetId( wxWindowID id )
{
  m_windowId = id;
};

wxWindowID wxWindow::GetId(void)
{
  return m_windowId;
};

void wxWindow::SetCursor( const wxCursor &cursor )
{
  if (*m_cursor == cursor) return;
  (*m_cursor) = cursor;
  if (m_widget->window)
    gdk_window_set_cursor( m_widget->window, m_cursor->GetCursor() );
  if (m_wxwindow && m_wxwindow->window)
    gdk_window_set_cursor( m_wxwindow->window, m_cursor->GetCursor() );
};

void wxWindow::Refresh( bool eraseBackground, const wxRect *rect )
{
  if (eraseBackground && m_wxwindow && m_wxwindow->window)
  {
    if (rect)
      gdk_window_clear_area( m_wxwindow->window, 
        rect->x, 
	rect->y, 
	rect->width, 
	rect->height );
    else
      Clear();
  };
  if (!rect)
  {
    if (m_wxwindow)
    {
      wxClientDC dc(this);
      PrepareDC(dc);
      long x = 0;
      long y = 0;
      dc.GetInternalDeviceOrigin( &x, &y );
      
      int w = 0;
      int h = 0;
      GetClientSize( &w, &h );
      
      GdkRectangle gdk_rect;
      gdk_rect.x = x;
      gdk_rect.y = y;
      gdk_rect.width = w;
      gdk_rect.height = h;
      gtk_widget_draw( m_wxwindow, &gdk_rect );
    };
  }
  else
  {
    GdkRectangle gdk_rect;
    gdk_rect.x = rect->x;
    gdk_rect.y = rect->y;
    gdk_rect.width = rect->width;
    gdk_rect.height = rect->height;
    if (m_wxwindow)
      gtk_widget_draw( m_wxwindow, &gdk_rect );
    else
      gtk_widget_draw( m_widget, &gdk_rect );
  };
};

bool wxWindow::IsExposed( long x, long y )
{
  return (m_updateRegion.Contains( x, y ) != wxOutRegion );
};

bool wxWindow::IsExposed( long x, long y, long width, long height )
{
  return (m_updateRegion.Contains( x, y, width, height ) != wxOutRegion );
};

void wxWindow::Clear(void)
{
  if (m_wxwindow && m_wxwindow->window) gdk_window_clear( m_wxwindow->window );
};

wxColour wxWindow::GetBackgroundColour(void) const
{
  return m_backgroundColour;
};

void wxWindow::SetBackgroundColour( const wxColour &colour )
{
  m_backgroundColour = colour;
  if (m_wxwindow)
  {
    m_backgroundColour.CalcPixel( m_wxwindow->style->colormap );
    gdk_window_set_background( m_wxwindow->window, m_backgroundColour.GetColor() );
    gdk_window_clear( m_wxwindow->window );
  };
  // do something ?
};

bool wxWindow::Validate(void)
{
  wxNode *node = GetChildren()->First();
  while (node)
  {
    wxWindow *child = (wxWindow *)node->Data();
    if (child->GetValidator() && /* child->GetValidator()->Ok() && */ !child->GetValidator()->Validate(this)) 
      { return FALSE; }
    node = node->Next();
  };
  return TRUE;
};

bool wxWindow::TransferDataToWindow(void)
{
  wxNode *node = GetChildren()->First();
  while (node)
  {
    wxWindow *child = (wxWindow *)node->Data();
    if (child->GetValidator() && /* child->GetValidator()->Ok() && */
	!child->GetValidator()->TransferToWindow() )
    {
      wxMessageBox( "Application Error", "Could not transfer data to window", wxOK|wxICON_EXCLAMATION );
      return FALSE;
    };
    node = node->Next();
  };
  return TRUE;
};

bool wxWindow::TransferDataFromWindow(void)
{
  wxNode *node = GetChildren()->First();
  while (node)
  {
    wxWindow *child = (wxWindow *)node->Data();
    if ( child->GetValidator() && /* child->GetValidator()->Ok() && */ !child->GetValidator()->TransferFromWindow() )
      { return FALSE; }
   node = node->Next();
  }
  return TRUE;
};

void wxWindow::OnInitDialog( wxInitDialogEvent &WXUNUSED(event) )
{
  TransferDataToWindow();
};

void wxWindow::InitDialog(void)
{
  wxInitDialogEvent event(GetId());
  event.SetEventObject( this );
  GetEventHandler()->ProcessEvent(event);
};

void wxWindow::SetDropTarget( wxDropTarget *dropTarget )
{
  GtkWidget *connect_widget = m_widget;
  if (m_wxwindow) connect_widget = m_wxwindow;
  if (m_pDropTarget)
  {
    m_pDropTarget->UnregisterWidget( connect_widget );
    delete m_pDropTarget;
  };
  m_pDropTarget = dropTarget;
  if (m_pDropTarget)
  {
    m_pDropTarget->RegisterWidget( connect_widget );
  };
};

wxDropTarget *wxWindow::GetDropTarget() const
{
  return m_pDropTarget;
};

void wxWindow::SetFont( const wxFont &font )
{
  m_font = font;
/*
  create new style
  copy old style values to new one
  set font in new style
  -> takes to many resources
  
  GtkStyle *style = gtk_style_new();
  ...
*/
};

wxFont *wxWindow::GetFont(void)
{
  return &m_font;
};

void wxWindow::SetWindowStyleFlag( long flag )
{
  m_windowStyle = flag;
};

long wxWindow::GetWindowStyleFlag(void) const
{
  return m_windowStyle;
};

void wxWindow::CaptureMouse(void)
{
  GtkWidget *connect_widget = m_widget;
  if (m_wxwindow) connect_widget = m_wxwindow;
  gtk_grab_add( connect_widget );
  gdk_pointer_grab ( connect_widget->window, FALSE,
                    (GdkEventMask)
		    (GDK_BUTTON_PRESS_MASK | 
		    GDK_BUTTON_RELEASE_MASK |
		    GDK_POINTER_MOTION_MASK), 
		    NULL, NULL, GDK_CURRENT_TIME );
};

void wxWindow::ReleaseMouse(void)
{
  GtkWidget *connect_widget = m_widget;
  if (m_wxwindow) connect_widget = m_wxwindow;
  gtk_grab_remove( connect_widget );
  gdk_pointer_ungrab ( GDK_CURRENT_TIME );
};

void wxWindow::SetTitle( const wxString &WXUNUSED(title) )
{
};

wxString wxWindow::GetTitle(void) const
{
  return (wxString&)m_windowName;
};

wxString wxWindow::GetLabel(void) const
{
  return GetTitle();
};

void wxWindow::SetName( const wxString &name )
{
  m_windowName = name;
};

wxString wxWindow::GetName(void) const
{
  return (wxString&)m_windowName;
};

bool wxWindow::IsShown(void) const
{
  return m_isShown;
};

bool wxWindow::IsRetained(void)
{
  return FALSE;
};

wxWindow *wxWindow::FindWindow( long id )
{
  if (id == m_windowId) return this;
  wxNode *node = m_children.First();
  while (node)
  {
    wxWindow *child = (wxWindow*)node->Data();
    wxWindow *res = child->FindWindow( id );
    if (res) return res;
    node = node->Next();
  };
  return NULL;
};

wxWindow *wxWindow::FindWindow( const wxString& name )
{
  if (name == m_windowName) return this;
  wxNode *node = m_children.First();
  while (node)
  {
    wxWindow *child = (wxWindow*)node->Data();
    wxWindow *res = child->FindWindow( name );
    if (res) return res;
    node = node->Next();
  };
  return NULL;
};

void wxWindow::SetScrollbar( int orient, int pos, int thumbVisible,
      int range, bool WXUNUSED(refresh) )
{
  if (!m_wxwindow) return;

  if (orient == wxHORIZONTAL)
  {
    float fpos = (float)pos;
    m_oldHorizontalPos = fpos;
    float frange = (float)range;
    float fthumb = (float)thumbVisible;
    
    if ((fabs(fpos-m_hAdjust->value) < 0.2) &&
        (fabs(frange-m_hAdjust->upper) < 0.2) &&
	(fabs(fthumb-m_hAdjust->page_size) < 0.2))
      return;
      
    m_hAdjust->lower = 0.0;
    m_hAdjust->upper = frange;
    m_hAdjust->value = fpos;
    m_hAdjust->step_increment = 1.0;
    m_hAdjust->page_increment = (float)(wxMax(fthumb-2,0));
    m_hAdjust->page_size = fthumb;
  }
  else
  {
    float fpos = (float)pos;
    m_oldVerticalPos = fpos;
    float frange = (float)range;
    float fthumb = (float)thumbVisible;
    
    if ((fabs(fpos-m_vAdjust->value) < 0.2) &&
        (fabs(frange-m_vAdjust->upper) < 0.2) &&
	(fabs(fthumb-m_vAdjust->page_size) < 0.2))
      return;
      
    m_vAdjust->lower = 0.0;
    m_vAdjust->upper = frange;
    m_vAdjust->value = fpos;
    m_vAdjust->step_increment = 1.0;
    m_vAdjust->page_increment = (float)(wxMax(fthumb-2,0));
    m_vAdjust->page_size = fthumb;
  };
 
  if (m_wxwindow->window)
  {  
    if (orient == wxHORIZONTAL)
      gtk_signal_emit_by_name( GTK_OBJECT(m_hAdjust), "changed" );
    else  
      gtk_signal_emit_by_name( GTK_OBJECT(m_vAdjust), "changed" );
      
//    gtk_widget_set_usize( m_widget, m_width, m_height );
  };
};

void wxWindow::SetScrollPos( int orient, int pos, bool WXUNUSED(refresh) )
{
  if (!m_wxwindow) return;
  
  if (orient == wxHORIZONTAL)
  {
    float fpos = (float)pos;
    m_oldHorizontalPos = fpos;

    if (fabs(fpos-m_hAdjust->value) < 0.2) return;
    m_hAdjust->value = fpos;
  }
  else
  {
    float fpos = (float)pos;
    m_oldVerticalPos = fpos;
    if (fabs(fpos-m_vAdjust->value) < 0.2) return;
    m_vAdjust->value = fpos;
  };
  
  if (m_wxwindow->window)
  {  
    if (orient == wxHORIZONTAL)
      gtk_signal_emit_by_name( GTK_OBJECT(m_hAdjust), "value_changed" );
    else  
      gtk_signal_emit_by_name( GTK_OBJECT(m_vAdjust), "value_changed" );
  };
};

int wxWindow::GetScrollThumb( int orient ) const
{
  if (!m_wxwindow) return 0;

  if (orient == wxHORIZONTAL)
    return (int)(m_hAdjust->page_size+0.5);
  else
    return (int)(m_vAdjust->page_size+0.5);
};

int wxWindow::GetScrollPos( int orient ) const
{
  if (!m_wxwindow) return 0;

  if (orient == wxHORIZONTAL)
    return (int)(m_hAdjust->value+0.5);
  else
    return (int)(m_vAdjust->value+0.5);
};

int wxWindow::GetScrollRange( int orient ) const
{
  if (!m_wxwindow) return 0;

  if (orient == wxHORIZONTAL)
    return (int)(m_hAdjust->upper+0.5);
  else
    return (int)(m_vAdjust->upper+0.5);
};

void wxWindow::ScrollWindow( int dx, int dy, const wxRect* WXUNUSED(rect) )
{
  if (!m_wxwindow) return;
  
  m_drawingOffsetX += dx;
  m_drawingOffsetY += dy;
  
//  printf( "X: %d  Y: %d  \n", (int)m_drawingOffsetX, (int)m_drawingOffsetY );
  
  gtk_myfixed_set_offset( GTK_MYFIXED(m_wxwindow), m_drawingOffsetX, m_drawingOffsetY );
  
/*
    The code here is very nifty, but it doesn't work with
    overlapping windows...

    int cw = 0;
    int ch = 0;
    GetClientSize( &cw, &ch );
    
    int w = cw - abs(dx);
    int h = ch - abs(dy);
    if ((h < 0) || (w < 0))
    {
      Refresh();
      return;
    };
    int s_x = 0;
    int s_y = 0;
    if (dx < 0) s_x = -dx;
    if (dy < 0) s_y = -dy;
    int d_x = 0;
    int d_y = 0;
    if (dx > 0) d_x = dx;
    if (dy > 0) d_y = dy;
    gdk_window_copy_area( m_wxwindow->window, m_wxwindow->style->fg_gc[0], d_x, d_y,
      m_wxwindow->window, s_x, s_y, w, h );
      
    wxRect rect;
    if (dx < 0) rect.x = cw+dx; else rect.x = 0;
    if (dy < 0) rect.y = ch+dy; else rect.y = 0;
    if (dy != 0) rect.width = cw; else rect.width = abs(dx);
    if (dx != 0) rect.height = ch; else rect.height = abs(dy);
  
    Refresh( TRUE, &rect );
*/
};

void wxWindow::GetDrawingOffset( long *x, long *y )
{
  if (x) *x = m_drawingOffsetX;
  if (y) *y = m_drawingOffsetY;
};

//-------------------------------------------------------------------------------------
//          Layout
//-------------------------------------------------------------------------------------

wxLayoutConstraints *wxWindow::GetConstraints(void) const
{
  return m_constraints;
};

void wxWindow::SetConstraints( wxLayoutConstraints *constraints )
{
  if (m_constraints)
  {
    UnsetConstraints(m_constraints);
    delete m_constraints;
  }
  m_constraints = constraints;
  if (m_constraints)
  {
    // Make sure other windows know they're part of a 'meaningful relationship'
    if (m_constraints->left.GetOtherWindow() && (m_constraints->left.GetOtherWindow() != this))
      m_constraints->left.GetOtherWindow()->AddConstraintReference((wxWindow *)this);
    if (m_constraints->top.GetOtherWindow() && (m_constraints->top.GetOtherWindow() != this))
      m_constraints->top.GetOtherWindow()->AddConstraintReference((wxWindow *)this);
    if (m_constraints->right.GetOtherWindow() && (m_constraints->right.GetOtherWindow() != this))
      m_constraints->right.GetOtherWindow()->AddConstraintReference((wxWindow *)this);
    if (m_constraints->bottom.GetOtherWindow() && (m_constraints->bottom.GetOtherWindow() != this))
      m_constraints->bottom.GetOtherWindow()->AddConstraintReference((wxWindow *)this);
    if (m_constraints->width.GetOtherWindow() && (m_constraints->width.GetOtherWindow() != this))
      m_constraints->width.GetOtherWindow()->AddConstraintReference((wxWindow *)this);
    if (m_constraints->height.GetOtherWindow() && (m_constraints->height.GetOtherWindow() != this))
      m_constraints->height.GetOtherWindow()->AddConstraintReference((wxWindow *)this);
    if (m_constraints->centreX.GetOtherWindow() && (m_constraints->centreX.GetOtherWindow() != this))
      m_constraints->centreX.GetOtherWindow()->AddConstraintReference((wxWindow *)this);
    if (m_constraints->centreY.GetOtherWindow() && (m_constraints->centreY.GetOtherWindow() != this))
      m_constraints->centreY.GetOtherWindow()->AddConstraintReference((wxWindow *)this);
  }
;
}
;

void wxWindow::SetAutoLayout( bool autoLayout )
{
  m_autoLayout = autoLayout;
};

bool wxWindow::GetAutoLayout(void) const
{
  return m_autoLayout;
};

wxSizer *wxWindow::GetSizer(void) const
{
  return m_windowSizer;
};

void wxWindow::SetSizerParent( wxWindow *win )
{
  m_sizerParent = win;
};

wxWindow *wxWindow::GetSizerParent(void) const
{
  return m_sizerParent;
};

// This removes any dangling pointers to this window
// in other windows' constraintsInvolvedIn lists.
void wxWindow::UnsetConstraints(wxLayoutConstraints *c)
{
  if (c)
  {
    if (c->left.GetOtherWindow() && (c->top.GetOtherWindow() != this))
      c->left.GetOtherWindow()->RemoveConstraintReference((wxWindow *)this);
    if (c->top.GetOtherWindow() && (c->top.GetOtherWindow() != this))
      c->top.GetOtherWindow()->RemoveConstraintReference((wxWindow *)this);
    if (c->right.GetOtherWindow() && (c->right.GetOtherWindow() != this))
      c->right.GetOtherWindow()->RemoveConstraintReference((wxWindow *)this);
    if (c->bottom.GetOtherWindow() && (c->bottom.GetOtherWindow() != this))
      c->bottom.GetOtherWindow()->RemoveConstraintReference((wxWindow *)this);
    if (c->width.GetOtherWindow() && (c->width.GetOtherWindow() != this))
      c->width.GetOtherWindow()->RemoveConstraintReference((wxWindow *)this);
    if (c->height.GetOtherWindow() && (c->height.GetOtherWindow() != this))
      c->height.GetOtherWindow()->RemoveConstraintReference((wxWindow *)this);
    if (c->centreX.GetOtherWindow() && (c->centreX.GetOtherWindow() != this))
      c->centreX.GetOtherWindow()->RemoveConstraintReference((wxWindow *)this);
    if (c->centreY.GetOtherWindow() && (c->centreY.GetOtherWindow() != this))
      c->centreY.GetOtherWindow()->RemoveConstraintReference((wxWindow *)this);
  }
}

// Back-pointer to other windows we're involved with, so if we delete
// this window, we must delete any constraints we're involved with.
void wxWindow::AddConstraintReference(wxWindow *otherWin)
{
  if (!m_constraintsInvolvedIn)
    m_constraintsInvolvedIn = new wxList;
  if (!m_constraintsInvolvedIn->Member(otherWin))
    m_constraintsInvolvedIn->Append(otherWin);
}

// REMOVE back-pointer to other windows we're involved with.
void wxWindow::RemoveConstraintReference(wxWindow *otherWin)
{
  if (m_constraintsInvolvedIn)
    m_constraintsInvolvedIn->DeleteObject(otherWin);
}

// Reset any constraints that mention this window
void wxWindow::DeleteRelatedConstraints(void)
{
  if (m_constraintsInvolvedIn)
  {
    wxNode *node = m_constraintsInvolvedIn->First();
    while (node)
    {
      wxWindow *win = (wxWindow *)node->Data();
      wxNode *next = node->Next();
      wxLayoutConstraints *constr = win->GetConstraints();

      // Reset any constraints involving this window
      if (constr)
      {
        constr->left.ResetIfWin((wxWindow *)this);
        constr->top.ResetIfWin((wxWindow *)this);
        constr->right.ResetIfWin((wxWindow *)this);
        constr->bottom.ResetIfWin((wxWindow *)this);
        constr->width.ResetIfWin((wxWindow *)this);
        constr->height.ResetIfWin((wxWindow *)this);
        constr->centreX.ResetIfWin((wxWindow *)this);
        constr->centreY.ResetIfWin((wxWindow *)this);
      }
      delete node;
      node = next;
    }
    delete m_constraintsInvolvedIn;
    m_constraintsInvolvedIn = NULL;
  }
}

void wxWindow::SetSizer(wxSizer *sizer)
{
  m_windowSizer = sizer;
  if (sizer)
    sizer->SetSizerParent((wxWindow *)this);
}

/*
 * New version
 */

bool wxWindow::Layout(void)
{
  if (GetConstraints())
  {
    int w, h;
    GetClientSize(&w, &h);
    GetConstraints()->width.SetValue(w);
    GetConstraints()->height.SetValue(h);
  }
  
  // If top level (one sizer), evaluate the sizer's constraints.
  if (GetSizer())
  {
    int noChanges;
    GetSizer()->ResetConstraints();   // Mark all constraints as unevaluated
    GetSizer()->LayoutPhase1(&noChanges);
    GetSizer()->LayoutPhase2(&noChanges);
    GetSizer()->SetConstraintSizes(); // Recursively set the real window sizes
    return TRUE;
  }
  else
  {
    // Otherwise, evaluate child constraints
    ResetConstraints();   // Mark all constraints as unevaluated
    DoPhase(1);           // Just one phase need if no sizers involved
    DoPhase(2);
    SetConstraintSizes(); // Recursively set the real window sizes
  }
  return TRUE;
}


// Do a phase of evaluating constraints:
// the default behaviour. wxSizers may do a similar
// thing, but also impose their own 'constraints'
// and order the evaluation differently.
bool wxWindow::LayoutPhase1(int *noChanges)
{
  wxLayoutConstraints *constr = GetConstraints();
  if (constr)
  {
    return constr->SatisfyConstraints((wxWindow *)this, noChanges);
  }
  else
    return TRUE;
}

bool wxWindow::LayoutPhase2(int *noChanges)
{
  *noChanges = 0;
  
  // Layout children
  DoPhase(1);
  DoPhase(2);
  return TRUE;
}

// Do a phase of evaluating child constraints
bool wxWindow::DoPhase(int phase)
{
  int noIterations = 0;
  int maxIterations = 500;
  int noChanges = 1;
  int noFailures = 0;
  wxList succeeded;
  while ((noChanges > 0) && (noIterations < maxIterations))
  {
    noChanges = 0;
    noFailures = 0;
    wxNode *node = GetChildren()->First();
    while (node)
    {
      wxWindow *child = (wxWindow *)node->Data();
      if (!child->IsKindOf(CLASSINFO(wxFrame)) && !child->IsKindOf(CLASSINFO(wxDialog)))
      {
        wxLayoutConstraints *constr = child->GetConstraints();
        if (constr)
        {
          if (succeeded.Member(child))
          {
          }
          else
          {
            int tempNoChanges = 0;
            bool success = ( (phase == 1) ? child->LayoutPhase1(&tempNoChanges) : child->LayoutPhase2(&tempNoChanges) ) ;
            noChanges += tempNoChanges;
            if (success)
            {
              succeeded.Append(child);
            }
          }
        }
      }
      node = node->Next();
    }
    noIterations ++;
  }
  return TRUE;
}

void wxWindow::ResetConstraints(void)
{
  wxLayoutConstraints *constr = GetConstraints();
  if (constr)
  {
    constr->left.SetDone(FALSE);
    constr->top.SetDone(FALSE);
    constr->right.SetDone(FALSE);
    constr->bottom.SetDone(FALSE);
    constr->width.SetDone(FALSE);
    constr->height.SetDone(FALSE);
    constr->centreX.SetDone(FALSE);
    constr->centreY.SetDone(FALSE);
  }
  wxNode *node = GetChildren()->First();
  while (node)
  {
    wxWindow *win = (wxWindow *)node->Data();
    if (!win->IsKindOf(CLASSINFO(wxFrame)) && !win->IsKindOf(CLASSINFO(wxDialog)))
      win->ResetConstraints();
    node = node->Next();
  }
}

// Need to distinguish between setting the 'fake' size for
// windows and sizers, and setting the real values.
void wxWindow::SetConstraintSizes(bool recurse)
{
  wxLayoutConstraints *constr = GetConstraints();
  if (constr && constr->left.GetDone() && constr->right.GetDone() &&
                constr->width.GetDone() && constr->height.GetDone())
  {
    int x = constr->left.GetValue();
    int y = constr->top.GetValue();
    int w = constr->width.GetValue();
    int h = constr->height.GetValue();

    // If we don't want to resize this window, just move it...
    if ((constr->width.GetRelationship() != wxAsIs) ||
        (constr->height.GetRelationship() != wxAsIs))
    {
      // Calls Layout() recursively. AAAGH. How can we stop that.
      // Simply take Layout() out of non-top level OnSizes.
      SizerSetSize(x, y, w, h);
    }
    else
    {
      SizerMove(x, y);
    }
  }
  else if (constr)
  {
    char *windowClass = this->GetClassInfo()->GetClassName();

    wxString winName;
	if (GetName() == "")
		winName = "unnamed";
	else
		winName = GetName();
    wxDebugMsg("Constraint(s) not satisfied for window of type %s, name %s:\n", (const char *)windowClass, (const char *)winName);
    if (!constr->left.GetDone())
      wxDebugMsg("  unsatisfied 'left' constraint.\n");
    if (!constr->right.GetDone())
      wxDebugMsg("  unsatisfied 'right' constraint.\n");
    if (!constr->width.GetDone())
      wxDebugMsg("  unsatisfied 'width' constraint.\n");
    if (!constr->height.GetDone())
      wxDebugMsg("  unsatisfied 'height' constraint.\n");
    wxDebugMsg("Please check constraints: try adding AsIs() constraints.\n");
  }

  if (recurse)
  {
    wxNode *node = GetChildren()->First();
    while (node)
    {
      wxWindow *win = (wxWindow *)node->Data();
      if (!win->IsKindOf(CLASSINFO(wxFrame)) && !win->IsKindOf(CLASSINFO(wxDialog)))
        win->SetConstraintSizes();
      node = node->Next();
    }
  }
}

// This assumes that all sizers are 'on' the same
// window, i.e. the parent of this window.
void wxWindow::TransformSizerToActual(int *x, int *y) const
{
  if (!m_sizerParent || m_sizerParent->IsKindOf(CLASSINFO(wxDialog)) ||
  			 m_sizerParent->IsKindOf(CLASSINFO(wxFrame)) )
    return;
    
  int xp, yp;
  m_sizerParent->GetPosition(&xp, &yp);
  m_sizerParent->TransformSizerToActual(&xp, &yp);
  *x += xp;
  *y += yp;
}

void wxWindow::SizerSetSize(int x, int y, int w, int h)
{
	int xx = x;
	int yy = y;
  TransformSizerToActual(&xx, &yy);
  SetSize(xx, yy, w, h);
}

void wxWindow::SizerMove(int x, int y)
{
	int xx = x;
	int yy = y;
  TransformSizerToActual(&xx, &yy);
  Move(xx, yy);
}

// Only set the size/position of the constraint (if any)
void wxWindow::SetSizeConstraint(int x, int y, int w, int h)
{
  wxLayoutConstraints *constr = GetConstraints();
  if (constr)
  {
    if (x != -1)
    {
      constr->left.SetValue(x);
      constr->left.SetDone(TRUE);
    }
    if (y != -1)
    {
      constr->top.SetValue(y);
      constr->top.SetDone(TRUE);
    }
    if (w != -1)
    {
      constr->width.SetValue(w);
      constr->width.SetDone(TRUE);
    }
    if (h != -1)
    {
      constr->height.SetValue(h);
      constr->height.SetDone(TRUE);
    }
  }
}

void wxWindow::MoveConstraint(int x, int y)
{
  wxLayoutConstraints *constr = GetConstraints();
  if (constr)
  {
    if (x != -1)
    {
      constr->left.SetValue(x);
      constr->left.SetDone(TRUE);
    }
    if (y != -1)
    {
      constr->top.SetValue(y);
      constr->top.SetDone(TRUE);
    }
  }
}

void wxWindow::GetSizeConstraint(int *w, int *h) const
{
  wxLayoutConstraints *constr = GetConstraints();
  if (constr)
  {
    *w = constr->width.GetValue();
    *h = constr->height.GetValue();
  }
  else
    GetSize(w, h);
}

void wxWindow::GetClientSizeConstraint(int *w, int *h) const
{
  wxLayoutConstraints *constr = GetConstraints();
  if (constr)
  {
    *w = constr->width.GetValue();
    *h = constr->height.GetValue();
  }
  else
    GetClientSize(w, h);
}

void wxWindow::GetPositionConstraint(int *x, int *y) const
{
  wxLayoutConstraints *constr = GetConstraints();
  if (constr)
  {
    *x = constr->left.GetValue();
    *y = constr->top.GetValue();
  }
  else
    GetPosition(x, y);
}

bool wxWindow::AcceptsFocus() const
{
  return IsEnabled() && IsShown();
}
