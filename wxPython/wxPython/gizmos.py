## This file reverse renames symbols in the wx package to give
## them their wx prefix again, for backwards compatibility.
##
## Generated by ./distrib/build_renamers.py

# This silly stuff here is so the wxPython.wx module doesn't conflict
# with the wx package.  We need to import modules from the wx package
# here, then we'll put the wxPython.wx entry back in sys.modules.
import sys
_wx = None
if sys.modules.has_key('wxPython.wx'):
    _wx = sys.modules['wxPython.wx']
    del sys.modules['wxPython.wx']

import wx.gizmos

sys.modules['wxPython.wx'] = _wx
del sys, _wx


# Now assign all the reverse-renamed names:
wxDynamicSashNameStr = wx.gizmos.DynamicSashNameStr
wxEditableListBoxNameStr = wx.gizmos.EditableListBoxNameStr
wxTreeListCtrlNameStr = wx.gizmos.TreeListCtrlNameStr
wxEVT_DYNAMIC_SASH_SPLIT = wx.gizmos.wxEVT_DYNAMIC_SASH_SPLIT
wxEVT_DYNAMIC_SASH_UNIFY = wx.gizmos.wxEVT_DYNAMIC_SASH_UNIFY
wxDS_MANAGE_SCROLLBARS = wx.gizmos.DS_MANAGE_SCROLLBARS
wxDS_DRAG_CORNER = wx.gizmos.DS_DRAG_CORNER
wxDynamicSashSplitEvent = wx.gizmos.DynamicSashSplitEvent
wxDynamicSashSplitEventPtr = wx.gizmos.DynamicSashSplitEventPtr
wxDynamicSashUnifyEvent = wx.gizmos.DynamicSashUnifyEvent
wxDynamicSashUnifyEventPtr = wx.gizmos.DynamicSashUnifyEventPtr
wxDynamicSashWindow = wx.gizmos.DynamicSashWindow
wxDynamicSashWindowPtr = wx.gizmos.DynamicSashWindowPtr
wxPreDynamicSashWindow = wx.gizmos.PreDynamicSashWindow
wxEL_ALLOW_NEW = wx.gizmos.EL_ALLOW_NEW
wxEL_ALLOW_EDIT = wx.gizmos.EL_ALLOW_EDIT
wxEL_ALLOW_DELETE = wx.gizmos.EL_ALLOW_DELETE
wxEditableListBox = wx.gizmos.EditableListBox
wxEditableListBoxPtr = wx.gizmos.EditableListBoxPtr
wxRemotelyScrolledTreeCtrl = wx.gizmos.RemotelyScrolledTreeCtrl
wxRemotelyScrolledTreeCtrlPtr = wx.gizmos.RemotelyScrolledTreeCtrlPtr
wxTreeCompanionWindow = wx.gizmos.TreeCompanionWindow
wxTreeCompanionWindowPtr = wx.gizmos.TreeCompanionWindowPtr
wxThinSplitterWindow = wx.gizmos.ThinSplitterWindow
wxThinSplitterWindowPtr = wx.gizmos.ThinSplitterWindowPtr
wxSplitterScrolledWindow = wx.gizmos.SplitterScrolledWindow
wxSplitterScrolledWindowPtr = wx.gizmos.SplitterScrolledWindowPtr
wxLED_ALIGN_LEFT = wx.gizmos.LED_ALIGN_LEFT
wxLED_ALIGN_RIGHT = wx.gizmos.LED_ALIGN_RIGHT
wxLED_ALIGN_CENTER = wx.gizmos.LED_ALIGN_CENTER
wxLED_ALIGN_MASK = wx.gizmos.LED_ALIGN_MASK
wxLED_DRAW_FADED = wx.gizmos.LED_DRAW_FADED
wxLEDNumberCtrl = wx.gizmos.LEDNumberCtrl
wxLEDNumberCtrlPtr = wx.gizmos.LEDNumberCtrlPtr
wxPreLEDNumberCtrl = wx.gizmos.PreLEDNumberCtrl
wxTL_ALIGN_LEFT = wx.gizmos.TL_ALIGN_LEFT
wxTL_ALIGN_RIGHT = wx.gizmos.TL_ALIGN_RIGHT
wxTL_ALIGN_CENTER = wx.gizmos.TL_ALIGN_CENTER
wxTREE_HITTEST_ONITEMCOLUMN = wx.gizmos.TREE_HITTEST_ONITEMCOLUMN
wxTL_SEARCH_VISIBLE = wx.gizmos.TL_SEARCH_VISIBLE
wxTL_SEARCH_LEVEL = wx.gizmos.TL_SEARCH_LEVEL
wxTL_SEARCH_FULL = wx.gizmos.TL_SEARCH_FULL
wxTL_SEARCH_PARTIAL = wx.gizmos.TL_SEARCH_PARTIAL
wxTL_SEARCH_NOCASE = wx.gizmos.TL_SEARCH_NOCASE
wxTreeListColumnInfo = wx.gizmos.TreeListColumnInfo
wxTreeListColumnInfoPtr = wx.gizmos.TreeListColumnInfoPtr
wxTreeListCtrl = wx.gizmos.TreeListCtrl
wxTreeListCtrlPtr = wx.gizmos.TreeListCtrlPtr
wxPreTreeListCtrl = wx.gizmos.PreTreeListCtrl


d = globals()
for k, v in wx.gizmos.__dict__.iteritems():
    if k.startswith('EVT'):
        d[k] = v
del d, k, v



