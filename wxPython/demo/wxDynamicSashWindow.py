
from wxPython.wx import *
from wxPython.gizmos import *
from wxPython.stc import *

#----------------------------------------------------------------------
# This is an example of the complex view that manages its own scrollbars
# as described in the overview below.

class TestView(wxStyledTextCtrl):
    def __init__(self, parent, ID, log):
        wxStyledTextCtrl.__init__(self, parent, ID, style=wxNO_BORDER)
        self.dyn_sash = parent
        self.log = log
        self.SetMarginWidth(1,0)
        self.StyleSetFont(wxSTC_STYLE_DEFAULT,
                          wxFont(10, wxMODERN, wxNORMAL, wxNORMAL))
        self.SetupScrollBars()
        EVT_DYNAMIC_SASH_SPLIT(self, -1, self.OnSplit)
        EVT_DYNAMIC_SASH_UNIFY(self, -1, self.OnUnify)


    def SetupScrollBars(self):
        # hook the scrollbars provided by the wxDynamicSashWindow
        # to this view
        v_bar = self.dyn_sash.GetVScrollBar(self)
        h_bar = self.dyn_sash.GetHScrollBar(self)
        EVT_SCROLL(v_bar, self.OnSBScroll)
        EVT_SCROLL(h_bar, self.OnSBScroll)
        EVT_SET_FOCUS(v_bar, self.OnSBFocus)
        EVT_SET_FOCUS(h_bar, self.OnSBFocus)

        # And set the wxStyledText to use these scrollbars instead
        # of its built-in ones.
        self.SetVScrollBar(v_bar)
        self.SetHScrollBar(h_bar)


    def __del__(self):
        self.log.write("TestView.__del__\n")

    def OnSplit(self, evt):
        self.log.write("TestView.OnSplit\n");
        newview = TestView(self.dyn_sash, -1, self.log)
        newview.SetDocPointer(self.GetDocPointer())     # use the same document
        self.SetupScrollBars()


    def OnUnify(self, evt):
        self.log.write("TestView.OnUnify\n");
        self.SetupScrollBars()


    def OnSBScroll(self, evt):
        # redirect the scroll events from the dyn_sash's scrollbars to the STC
        self.GetEventHandler().ProcessEvent(evt)

    def OnSBFocus(self, evt):
        # when the scrollbar gets the focus move it back to the STC
        self.SetFocus()


sampleText="""\
You can drag the little tabs above the vertical scrollbar, or to the
left of the horizontal scrollbar to split this view, and you can
continue splitting the new views as much as you like.  Try it and see.

In this case the views also share the same document so changes in one
are instantly seen in the others.  This is a feature of the
wxStyledTextCtrl that is used for the view class in this sample.
"""

#----------------------------------------------------------------------
# This one is simpler, but doesn't do anything with the scrollbars
# except the default wxDynamicSashWindow behaviour

class SimpleView(wxPanel):
    def __init__(self, parent, ID, log):
        wxPanel.__init__(self, parent, ID)
        self.dyn_sash = parent
        self.log = log
        self.SetBackgroundColour("LIGHT BLUE")
        EVT_DYNAMIC_SASH_SPLIT(self, -1, self.OnSplit)

    def OnSplit(self, evt):
        v = SimpleView(self.dyn_sash, -1, self.log)


#----------------------------------------------------------------------

def runTest(frame, nb, log):
    if 1:
        win = wxDynamicSashWindow(nb, -1, style = 0
                                  | wxCLIP_CHILDREN
                                  #| wxDS_MANAGE_SCROLLBARS
                                  #| wxDS_DRAG_CORNER
                                  )
        win.SetFont(wxFont(10, wxMODERN, wxNORMAL, wxNORMAL))
        view = TestView(win, -1, log)
        view.SetText(sampleText)
    else:
        win = wxDynamicSashWindow(nb, -1)
        view = SimpleView(win, -1, log)
    return win

#----------------------------------------------------------------------

overview = """\
<html><body>
<h2>wxDynamicSashWindow</h2>
<p>
wxDynamicSashWindow widgets manages the way other widgets are viewed.
When a wxDynamicSashWindow is first shown, it will contain one child
view, a viewport for that child, and a pair of scrollbars to allow the
user to navigate the child view area.  Next to each scrollbar is a small
tab.  By clicking on either tab and dragging to the appropriate spot, a
user can split the view area into two smaller views separated by a
draggable sash.  Later, when the user wishes to reunify the two subviews,
the user simply drags the sash to the side of the window.
wxDynamicSashWindow will automatically reparent the appropriate child
view back up the window hierarchy, and the wxDynamicSashWindow will have
only one child view once again.
<p>
As an application developer, you will simply create a wxDynamicSashWindow
using either the Create() function or the more complex constructor
provided below, and then create a view window whose parent is the
wxDynamicSashWindow.  The child should respond to
wxDynamicSashSplitEvents -- perhaps with an OnSplit() event handler -- by
constructing a new view window whose parent is also the
wxDynamicSashWindow.  That's it!  Now your users can dynamically split
and reunify the view you provided.
<p>
If you wish to handle the scrollbar events for your view, rather than
allowing wxDynamicSashWindow to do it for you, things are a bit more
complex.  (You might want to handle scrollbar events yourself, if,
for instance, you wish to scroll a subwindow of the view you add to
your wxDynamicSashWindow object, rather than scrolling the whole view.)
In this case, you will need to construct your wxDynamicSashWindow without
the wxDS_MANAGE_SCROLLBARS style and  you will need to use the
GetHScrollBar() and GetVScrollBar() methods to retrieve the scrollbar
controls and call SetEventHanler() on them to redirect the scrolling
events whenever your window is reparented by wxDyanmicSashWindow.
You will need to set the scrollbars' event handler at three times:
<p>
<ul>
<li>  When your view is created
<li>  When your view receives a wxDynamicSashSplitEvent
<li>  When your view receives a wxDynamicSashUnifyEvent
</ul>
</body></html>
"""
