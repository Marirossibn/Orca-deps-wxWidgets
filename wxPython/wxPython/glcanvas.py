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

import wx.glcanvas

sys.modules['wxPython.wx'] = _wx
del sys, _wx


# Now assign all the reverse-renamed names:
wxGLContext = wx.glcanvas.GLContext
WX_GL_RGBA = wx.glcanvas.WX_GL_RGBA
WX_GL_BUFFER_SIZE = wx.glcanvas.WX_GL_BUFFER_SIZE
WX_GL_LEVEL = wx.glcanvas.WX_GL_LEVEL
WX_GL_DOUBLEBUFFER = wx.glcanvas.WX_GL_DOUBLEBUFFER
WX_GL_STEREO = wx.glcanvas.WX_GL_STEREO
WX_GL_AUX_BUFFERS = wx.glcanvas.WX_GL_AUX_BUFFERS
WX_GL_MIN_RED = wx.glcanvas.WX_GL_MIN_RED
WX_GL_MIN_GREEN = wx.glcanvas.WX_GL_MIN_GREEN
WX_GL_MIN_BLUE = wx.glcanvas.WX_GL_MIN_BLUE
WX_GL_MIN_ALPHA = wx.glcanvas.WX_GL_MIN_ALPHA
WX_GL_DEPTH_SIZE = wx.glcanvas.WX_GL_DEPTH_SIZE
WX_GL_STENCIL_SIZE = wx.glcanvas.WX_GL_STENCIL_SIZE
WX_GL_MIN_ACCUM_RED = wx.glcanvas.WX_GL_MIN_ACCUM_RED
WX_GL_MIN_ACCUM_GREEN = wx.glcanvas.WX_GL_MIN_ACCUM_GREEN
WX_GL_MIN_ACCUM_BLUE = wx.glcanvas.WX_GL_MIN_ACCUM_BLUE
WX_GL_MIN_ACCUM_ALPHA = wx.glcanvas.WX_GL_MIN_ACCUM_ALPHA
wxGLCanvasWithContext = wx.glcanvas.GLCanvasWithContext
wxGLCanvas = wx.glcanvas.GLCanvas


