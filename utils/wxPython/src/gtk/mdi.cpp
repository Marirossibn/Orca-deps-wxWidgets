/*
 * FILE : gtk/mdi.cpp
 * 
 * This file was automatically generated by :
 * Simplified Wrapper and Interface Generator (SWIG)
 * Version 1.1 (Patch 5)
 * 
 * Portions Copyright (c) 1995-1998
 * The University of Utah and The Regents of the University of California.
 * Permission is granted to distribute this file in any manner provided
 * this notice remains intact.
 * 
 * Do not make changes to this file--changes will be lost!
 *
 */


#define SWIGCODE
/* Implementation : PYTHON */

#define SWIGPYTHON
#include <string.h>
#include <stdlib.h>
/* Definitions for Windows/Unix exporting */
#if defined(__WIN32__)
#   if defined(_MSC_VER)
#	define SWIGEXPORT(a,b) __declspec(dllexport) a b
#   else
#	if defined(__BORLANDC__)
#	    define SWIGEXPORT(a,b) a _export b
#	else
#	    define SWIGEXPORT(a,b) a b
#	endif
#   endif
#else
#   define SWIGEXPORT(a,b) a b
#endif

#ifdef __cplusplus
extern "C" {
#endif
#include "Python.h"
extern void SWIG_MakePtr(char *, void *, char *);
extern void SWIG_RegisterMapping(char *, char *, void *(*)(void *));
extern char *SWIG_GetPtr(char *, void **, char *);
extern void SWIG_addvarlink(PyObject *, char *, PyObject *(*)(void), int (*)(PyObject *));
extern PyObject *SWIG_newvarlink(void);
#ifdef __cplusplus
}
#endif

#define SWIG_init    initmdic

#define SWIG_name    "mdic"

#include "helpers.h"

static PyObject* l_output_helper(PyObject* target, PyObject* o) {
    PyObject*   o2;
    PyObject*   o3;
    if (!target) {                   
        target = o;
    } else if (target == Py_None) {  
        Py_DECREF(Py_None);
        target = o;
    } else {                         
        if (!PyList_Check(target)) {
            o2 = target;
            target = PyList_New(0);
            PyList_Append(target, o2);
	    Py_XDECREF(o2);
        }
        PyList_Append(target,o);
	Py_XDECREF(o);
    }
    return target;
}

static PyObject* t_output_helper(PyObject* target, PyObject* o) {
    PyObject*   o2;
    PyObject*   o3;

    if (!target) {                   
        target = o;
    } else if (target == Py_None) {  
        Py_DECREF(Py_None);
        target = o;
    } else {                         
        if (!PyTuple_Check(target)) {
            o2 = target;
            target = PyTuple_New(1);
            PyTuple_SetItem(target, 0, o2);
        }
        o3 = PyTuple_New(1);            
        PyTuple_SetItem(o3, 0, o);      

        o2 = target;
        target = PySequence_Concat(o2, o3); 
        Py_DECREF(o2);                      
        Py_DECREF(o3);
    }
    return target;
}


extern byte* byte_LIST_helper(PyObject* source);
extern int* int_LIST_helper(PyObject* source);
extern long* long_LIST_helper(PyObject* source);
extern char** string_LIST_helper(PyObject* source);
extern wxPoint* wxPoint_LIST_helper(PyObject* source);
extern wxBitmap** wxBitmap_LIST_helper(PyObject* source);
extern wxString* wxString_LIST_helper(PyObject* source);
extern wxAcceleratorEntry* wxAcceleratorEntry_LIST_helper(PyObject* source);


static char* wxStringErrorMsg = "string type is required for parameter";
static void *SwigwxMDIParentFrameTowxFrame(void *ptr) {
    wxMDIParentFrame *src;
    wxFrame *dest;
    src = (wxMDIParentFrame *) ptr;
    dest = (wxFrame *) src;
    return (void *) dest;
}

static void *SwigwxMDIParentFrameTowxWindow(void *ptr) {
    wxMDIParentFrame *src;
    wxWindow *dest;
    src = (wxMDIParentFrame *) ptr;
    dest = (wxWindow *) src;
    return (void *) dest;
}

static void *SwigwxMDIParentFrameTowxEvtHandler(void *ptr) {
    wxMDIParentFrame *src;
    wxEvtHandler *dest;
    src = (wxMDIParentFrame *) ptr;
    dest = (wxEvtHandler *) src;
    return (void *) dest;
}

#define new_wxMDIParentFrame(_swigarg0,_swigarg1,_swigarg2,_swigarg3,_swigarg4,_swigarg5,_swigarg6) (new wxMDIParentFrame(_swigarg0,_swigarg1,_swigarg2,_swigarg3,_swigarg4,_swigarg5,_swigarg6))
static PyObject *_wrap_new_wxMDIParentFrame(PyObject *self, PyObject *args) {
    PyObject * _resultobj;
    wxMDIParentFrame * _result;
    wxWindow * _arg0;
    wxWindowID  _arg1;
    wxString * _arg2;
    wxPoint * _arg3 = &wxPyDefaultPosition;
    wxSize * _arg4 = &wxPyDefaultSize;
    long  _arg5 = (wxDEFAULT_FRAME_STYLE)|(wxVSCROLL)|(wxHSCROLL);
    char * _arg6 = "frame";
    char * _argc0 = 0;
    PyObject * _obj2 = 0;
    char * _argc3 = 0;
    char * _argc4 = 0;
    char _ptemp[128];

    self = self;
    if(!PyArg_ParseTuple(args,"siO|ssls:new_wxMDIParentFrame",&_argc0,&_arg1,&_obj2,&_argc3,&_argc4,&_arg5,&_arg6)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_wxWindow_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of new_wxMDIParentFrame. Expected _wxWindow_p.");
        return NULL;
        }
    }
{
    if (!PyString_Check(_obj2)) {
        PyErr_SetString(PyExc_TypeError, wxStringErrorMsg);
        return NULL;
    }
    _arg2 = new wxString(PyString_AsString(_obj2));
}
    if (_argc3) {
        if (SWIG_GetPtr(_argc3,(void **) &_arg3,"_wxPoint_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 4 of new_wxMDIParentFrame. Expected _wxPoint_p.");
        return NULL;
        }
    }
    if (_argc4) {
        if (SWIG_GetPtr(_argc4,(void **) &_arg4,"_wxSize_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 5 of new_wxMDIParentFrame. Expected _wxSize_p.");
        return NULL;
        }
    }
    _result = (wxMDIParentFrame *)new_wxMDIParentFrame(_arg0,_arg1,*_arg2,*_arg3,*_arg4,_arg5,_arg6);
    SWIG_MakePtr(_ptemp, (char *) _result,"_wxMDIParentFrame_p");
    _resultobj = Py_BuildValue("s",_ptemp);
{
    if (_obj2)
        delete _arg2;
}
    return _resultobj;
}

#define wxMDIParentFrame_ActivateNext(_swigobj)  (_swigobj->ActivateNext())
static PyObject *_wrap_wxMDIParentFrame_ActivateNext(PyObject *self, PyObject *args) {
    PyObject * _resultobj;
    wxMDIParentFrame * _arg0;
    char * _argc0 = 0;

    self = self;
    if(!PyArg_ParseTuple(args,"s:wxMDIParentFrame_ActivateNext",&_argc0)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_wxMDIParentFrame_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxMDIParentFrame_ActivateNext. Expected _wxMDIParentFrame_p.");
        return NULL;
        }
    }
    wxMDIParentFrame_ActivateNext(_arg0);
    Py_INCREF(Py_None);
    _resultobj = Py_None;
    return _resultobj;
}

#define wxMDIParentFrame_ActivatePrevious(_swigobj)  (_swigobj->ActivatePrevious())
static PyObject *_wrap_wxMDIParentFrame_ActivatePrevious(PyObject *self, PyObject *args) {
    PyObject * _resultobj;
    wxMDIParentFrame * _arg0;
    char * _argc0 = 0;

    self = self;
    if(!PyArg_ParseTuple(args,"s:wxMDIParentFrame_ActivatePrevious",&_argc0)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_wxMDIParentFrame_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxMDIParentFrame_ActivatePrevious. Expected _wxMDIParentFrame_p.");
        return NULL;
        }
    }
    wxMDIParentFrame_ActivatePrevious(_arg0);
    Py_INCREF(Py_None);
    _resultobj = Py_None;
    return _resultobj;
}

#define wxMDIParentFrame_ArrangeIcons(_swigobj)  (_swigobj->ArrangeIcons())
static PyObject *_wrap_wxMDIParentFrame_ArrangeIcons(PyObject *self, PyObject *args) {
    PyObject * _resultobj;
    wxMDIParentFrame * _arg0;
    char * _argc0 = 0;

    self = self;
    if(!PyArg_ParseTuple(args,"s:wxMDIParentFrame_ArrangeIcons",&_argc0)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_wxMDIParentFrame_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxMDIParentFrame_ArrangeIcons. Expected _wxMDIParentFrame_p.");
        return NULL;
        }
    }
    wxMDIParentFrame_ArrangeIcons(_arg0);
    Py_INCREF(Py_None);
    _resultobj = Py_None;
    return _resultobj;
}

#define wxMDIParentFrame_Cascade(_swigobj)  (_swigobj->Cascade())
static PyObject *_wrap_wxMDIParentFrame_Cascade(PyObject *self, PyObject *args) {
    PyObject * _resultobj;
    wxMDIParentFrame * _arg0;
    char * _argc0 = 0;

    self = self;
    if(!PyArg_ParseTuple(args,"s:wxMDIParentFrame_Cascade",&_argc0)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_wxMDIParentFrame_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxMDIParentFrame_Cascade. Expected _wxMDIParentFrame_p.");
        return NULL;
        }
    }
    wxMDIParentFrame_Cascade(_arg0);
    Py_INCREF(Py_None);
    _resultobj = Py_None;
    return _resultobj;
}

#define wxMDIParentFrame_GetActiveChild(_swigobj)  (_swigobj->GetActiveChild())
static PyObject *_wrap_wxMDIParentFrame_GetActiveChild(PyObject *self, PyObject *args) {
    PyObject * _resultobj;
    wxMDIChildFrame * _result;
    wxMDIParentFrame * _arg0;
    char * _argc0 = 0;
    char _ptemp[128];

    self = self;
    if(!PyArg_ParseTuple(args,"s:wxMDIParentFrame_GetActiveChild",&_argc0)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_wxMDIParentFrame_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxMDIParentFrame_GetActiveChild. Expected _wxMDIParentFrame_p.");
        return NULL;
        }
    }
    _result = (wxMDIChildFrame *)wxMDIParentFrame_GetActiveChild(_arg0);
    SWIG_MakePtr(_ptemp, (char *) _result,"_wxMDIChildFrame_p");
    _resultobj = Py_BuildValue("s",_ptemp);
    return _resultobj;
}

#define wxMDIParentFrame_GetClientWindow(_swigobj)  (_swigobj->GetClientWindow())
static PyObject *_wrap_wxMDIParentFrame_GetClientWindow(PyObject *self, PyObject *args) {
    PyObject * _resultobj;
    wxMDIClientWindow * _result;
    wxMDIParentFrame * _arg0;
    char * _argc0 = 0;
    char _ptemp[128];

    self = self;
    if(!PyArg_ParseTuple(args,"s:wxMDIParentFrame_GetClientWindow",&_argc0)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_wxMDIParentFrame_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxMDIParentFrame_GetClientWindow. Expected _wxMDIParentFrame_p.");
        return NULL;
        }
    }
    _result = (wxMDIClientWindow *)wxMDIParentFrame_GetClientWindow(_arg0);
    SWIG_MakePtr(_ptemp, (char *) _result,"_wxMDIClientWindow_p");
    _resultobj = Py_BuildValue("s",_ptemp);
    return _resultobj;
}

#define wxMDIParentFrame_GetToolBar(_swigobj)  (_swigobj->GetToolBar())
static PyObject *_wrap_wxMDIParentFrame_GetToolBar(PyObject *self, PyObject *args) {
    PyObject * _resultobj;
    wxWindow * _result;
    wxMDIParentFrame * _arg0;
    char * _argc0 = 0;
    char _ptemp[128];

    self = self;
    if(!PyArg_ParseTuple(args,"s:wxMDIParentFrame_GetToolBar",&_argc0)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_wxMDIParentFrame_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxMDIParentFrame_GetToolBar. Expected _wxMDIParentFrame_p.");
        return NULL;
        }
    }
    _result = (wxWindow *)wxMDIParentFrame_GetToolBar(_arg0);
    SWIG_MakePtr(_ptemp, (char *) _result,"_wxWindow_p");
    _resultobj = Py_BuildValue("s",_ptemp);
    return _resultobj;
}

#define wxMDIParentFrame_Tile(_swigobj)  (_swigobj->Tile())
static PyObject *_wrap_wxMDIParentFrame_Tile(PyObject *self, PyObject *args) {
    PyObject * _resultobj;
    wxMDIParentFrame * _arg0;
    char * _argc0 = 0;

    self = self;
    if(!PyArg_ParseTuple(args,"s:wxMDIParentFrame_Tile",&_argc0)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_wxMDIParentFrame_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxMDIParentFrame_Tile. Expected _wxMDIParentFrame_p.");
        return NULL;
        }
    }
    wxMDIParentFrame_Tile(_arg0);
    Py_INCREF(Py_None);
    _resultobj = Py_None;
    return _resultobj;
}

static void *SwigwxMDIChildFrameTowxFrame(void *ptr) {
    wxMDIChildFrame *src;
    wxFrame *dest;
    src = (wxMDIChildFrame *) ptr;
    dest = (wxFrame *) src;
    return (void *) dest;
}

static void *SwigwxMDIChildFrameTowxWindow(void *ptr) {
    wxMDIChildFrame *src;
    wxWindow *dest;
    src = (wxMDIChildFrame *) ptr;
    dest = (wxWindow *) src;
    return (void *) dest;
}

static void *SwigwxMDIChildFrameTowxEvtHandler(void *ptr) {
    wxMDIChildFrame *src;
    wxEvtHandler *dest;
    src = (wxMDIChildFrame *) ptr;
    dest = (wxEvtHandler *) src;
    return (void *) dest;
}

#define new_wxMDIChildFrame(_swigarg0,_swigarg1,_swigarg2,_swigarg3,_swigarg4,_swigarg5,_swigarg6) (new wxMDIChildFrame(_swigarg0,_swigarg1,_swigarg2,_swigarg3,_swigarg4,_swigarg5,_swigarg6))
static PyObject *_wrap_new_wxMDIChildFrame(PyObject *self, PyObject *args) {
    PyObject * _resultobj;
    wxMDIChildFrame * _result;
    wxMDIParentFrame * _arg0;
    wxWindowID  _arg1;
    wxString * _arg2;
    wxPoint * _arg3 = &wxPyDefaultPosition;
    wxSize * _arg4 = &wxPyDefaultSize;
    long  _arg5 = (wxDEFAULT_FRAME_STYLE);
    char * _arg6 = "frame";
    char * _argc0 = 0;
    PyObject * _obj2 = 0;
    char * _argc3 = 0;
    char * _argc4 = 0;
    char _ptemp[128];

    self = self;
    if(!PyArg_ParseTuple(args,"siO|ssls:new_wxMDIChildFrame",&_argc0,&_arg1,&_obj2,&_argc3,&_argc4,&_arg5,&_arg6)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_wxMDIParentFrame_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of new_wxMDIChildFrame. Expected _wxMDIParentFrame_p.");
        return NULL;
        }
    }
{
    if (!PyString_Check(_obj2)) {
        PyErr_SetString(PyExc_TypeError, wxStringErrorMsg);
        return NULL;
    }
    _arg2 = new wxString(PyString_AsString(_obj2));
}
    if (_argc3) {
        if (SWIG_GetPtr(_argc3,(void **) &_arg3,"_wxPoint_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 4 of new_wxMDIChildFrame. Expected _wxPoint_p.");
        return NULL;
        }
    }
    if (_argc4) {
        if (SWIG_GetPtr(_argc4,(void **) &_arg4,"_wxSize_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 5 of new_wxMDIChildFrame. Expected _wxSize_p.");
        return NULL;
        }
    }
    _result = (wxMDIChildFrame *)new_wxMDIChildFrame(_arg0,_arg1,*_arg2,*_arg3,*_arg4,_arg5,_arg6);
    SWIG_MakePtr(_ptemp, (char *) _result,"_wxMDIChildFrame_p");
    _resultobj = Py_BuildValue("s",_ptemp);
{
    if (_obj2)
        delete _arg2;
}
    return _resultobj;
}

#define wxMDIChildFrame_Activate(_swigobj)  (_swigobj->Activate())
static PyObject *_wrap_wxMDIChildFrame_Activate(PyObject *self, PyObject *args) {
    PyObject * _resultobj;
    wxMDIChildFrame * _arg0;
    char * _argc0 = 0;

    self = self;
    if(!PyArg_ParseTuple(args,"s:wxMDIChildFrame_Activate",&_argc0)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_wxMDIChildFrame_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxMDIChildFrame_Activate. Expected _wxMDIChildFrame_p.");
        return NULL;
        }
    }
    wxMDIChildFrame_Activate(_arg0);
    Py_INCREF(Py_None);
    _resultobj = Py_None;
    return _resultobj;
}

#define wxMDIChildFrame_Maximize(_swigobj)  (_swigobj->Maximize())
static PyObject *_wrap_wxMDIChildFrame_Maximize(PyObject *self, PyObject *args) {
    PyObject * _resultobj;
    wxMDIChildFrame * _arg0;
    char * _argc0 = 0;

    self = self;
    if(!PyArg_ParseTuple(args,"s:wxMDIChildFrame_Maximize",&_argc0)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_wxMDIChildFrame_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxMDIChildFrame_Maximize. Expected _wxMDIChildFrame_p.");
        return NULL;
        }
    }
    wxMDIChildFrame_Maximize(_arg0);
    Py_INCREF(Py_None);
    _resultobj = Py_None;
    return _resultobj;
}

#define wxMDIChildFrame_Restore(_swigobj)  (_swigobj->Restore())
static PyObject *_wrap_wxMDIChildFrame_Restore(PyObject *self, PyObject *args) {
    PyObject * _resultobj;
    wxMDIChildFrame * _arg0;
    char * _argc0 = 0;

    self = self;
    if(!PyArg_ParseTuple(args,"s:wxMDIChildFrame_Restore",&_argc0)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_wxMDIChildFrame_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxMDIChildFrame_Restore. Expected _wxMDIChildFrame_p.");
        return NULL;
        }
    }
    wxMDIChildFrame_Restore(_arg0);
    Py_INCREF(Py_None);
    _resultobj = Py_None;
    return _resultobj;
}

static void *SwigwxMDIClientWindowTowxWindow(void *ptr) {
    wxMDIClientWindow *src;
    wxWindow *dest;
    src = (wxMDIClientWindow *) ptr;
    dest = (wxWindow *) src;
    return (void *) dest;
}

static void *SwigwxMDIClientWindowTowxEvtHandler(void *ptr) {
    wxMDIClientWindow *src;
    wxEvtHandler *dest;
    src = (wxMDIClientWindow *) ptr;
    dest = (wxEvtHandler *) src;
    return (void *) dest;
}

#define new_wxMDIClientWindow(_swigarg0,_swigarg1) (new wxMDIClientWindow(_swigarg0,_swigarg1))
static PyObject *_wrap_new_wxMDIClientWindow(PyObject *self, PyObject *args) {
    PyObject * _resultobj;
    wxMDIClientWindow * _result;
    wxMDIParentFrame * _arg0;
    long  _arg1 = 0;
    char * _argc0 = 0;
    char _ptemp[128];

    self = self;
    if(!PyArg_ParseTuple(args,"s|l:new_wxMDIClientWindow",&_argc0,&_arg1)) 
        return NULL;
    if (_argc0) {
        if (SWIG_GetPtr(_argc0,(void **) &_arg0,"_wxMDIParentFrame_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of new_wxMDIClientWindow. Expected _wxMDIParentFrame_p.");
        return NULL;
        }
    }
    _result = (wxMDIClientWindow *)new_wxMDIClientWindow(_arg0,_arg1);
    SWIG_MakePtr(_ptemp, (char *) _result,"_wxMDIClientWindow_p");
    _resultobj = Py_BuildValue("s",_ptemp);
    return _resultobj;
}

static PyMethodDef mdicMethods[] = {
	 { "new_wxMDIClientWindow", _wrap_new_wxMDIClientWindow, 1 },
	 { "wxMDIChildFrame_Restore", _wrap_wxMDIChildFrame_Restore, 1 },
	 { "wxMDIChildFrame_Maximize", _wrap_wxMDIChildFrame_Maximize, 1 },
	 { "wxMDIChildFrame_Activate", _wrap_wxMDIChildFrame_Activate, 1 },
	 { "new_wxMDIChildFrame", _wrap_new_wxMDIChildFrame, 1 },
	 { "wxMDIParentFrame_Tile", _wrap_wxMDIParentFrame_Tile, 1 },
	 { "wxMDIParentFrame_GetToolBar", _wrap_wxMDIParentFrame_GetToolBar, 1 },
	 { "wxMDIParentFrame_GetClientWindow", _wrap_wxMDIParentFrame_GetClientWindow, 1 },
	 { "wxMDIParentFrame_GetActiveChild", _wrap_wxMDIParentFrame_GetActiveChild, 1 },
	 { "wxMDIParentFrame_Cascade", _wrap_wxMDIParentFrame_Cascade, 1 },
	 { "wxMDIParentFrame_ArrangeIcons", _wrap_wxMDIParentFrame_ArrangeIcons, 1 },
	 { "wxMDIParentFrame_ActivatePrevious", _wrap_wxMDIParentFrame_ActivatePrevious, 1 },
	 { "wxMDIParentFrame_ActivateNext", _wrap_wxMDIParentFrame_ActivateNext, 1 },
	 { "new_wxMDIParentFrame", _wrap_new_wxMDIParentFrame, 1 },
	 { NULL, NULL }
};
static PyObject *SWIG_globals;
#ifdef __cplusplus
extern "C" 
#endif
SWIGEXPORT(void,initmdic)() {
	 PyObject *m, *d;
	 SWIG_globals = SWIG_newvarlink();
	 m = Py_InitModule("mdic", mdicMethods);
	 d = PyModule_GetDict(m);
/*
 * These are the pointer type-equivalency mappings. 
 * (Used by the SWIG pointer type-checker).
 */
	 SWIG_RegisterMapping("_wxAcceleratorTable","_class_wxAcceleratorTable",0);
	 SWIG_RegisterMapping("_wxEvent","_class_wxEvent",0);
	 SWIG_RegisterMapping("_class_wxActivateEvent","_wxActivateEvent",0);
	 SWIG_RegisterMapping("_signed_long","_long",0);
	 SWIG_RegisterMapping("_wxMenuEvent","_class_wxMenuEvent",0);
	 SWIG_RegisterMapping("_class_wxRegionIterator","_wxRegionIterator",0);
	 SWIG_RegisterMapping("_class_wxMenuBar","_wxMenuBar",0);
	 SWIG_RegisterMapping("_class_wxEvtHandler","_class_wxMDIClientWindow",SwigwxMDIClientWindowTowxEvtHandler);
	 SWIG_RegisterMapping("_class_wxEvtHandler","_wxMDIClientWindow",SwigwxMDIClientWindowTowxEvtHandler);
	 SWIG_RegisterMapping("_class_wxEvtHandler","_class_wxMDIChildFrame",SwigwxMDIChildFrameTowxEvtHandler);
	 SWIG_RegisterMapping("_class_wxEvtHandler","_wxMDIChildFrame",SwigwxMDIChildFrameTowxEvtHandler);
	 SWIG_RegisterMapping("_class_wxEvtHandler","_class_wxMDIParentFrame",SwigwxMDIParentFrameTowxEvtHandler);
	 SWIG_RegisterMapping("_class_wxEvtHandler","_wxMDIParentFrame",SwigwxMDIParentFrameTowxEvtHandler);
	 SWIG_RegisterMapping("_class_wxEvtHandler","_wxEvtHandler",0);
	 SWIG_RegisterMapping("_wxPaintEvent","_class_wxPaintEvent",0);
	 SWIG_RegisterMapping("_wxIndividualLayoutConstraint","_class_wxIndividualLayoutConstraint",0);
	 SWIG_RegisterMapping("_wxCursor","_class_wxCursor",0);
	 SWIG_RegisterMapping("_wxToolTip","_class_wxToolTip",0);
	 SWIG_RegisterMapping("_wxMask","_class_wxMask",0);
	 SWIG_RegisterMapping("_wxPyMenu","_class_wxPyMenu",0);
	 SWIG_RegisterMapping("_wxPen","_class_wxPen",0);
	 SWIG_RegisterMapping("_wxUpdateUIEvent","_class_wxUpdateUIEvent",0);
	 SWIG_RegisterMapping("_byte","_unsigned_char",0);
	 SWIG_RegisterMapping("_wxStaticBox","_class_wxStaticBox",0);
	 SWIG_RegisterMapping("_wxChoice","_class_wxChoice",0);
	 SWIG_RegisterMapping("_wxSlider","_class_wxSlider",0);
	 SWIG_RegisterMapping("_long","_wxDash",0);
	 SWIG_RegisterMapping("_long","_unsigned_long",0);
	 SWIG_RegisterMapping("_long","_signed_long",0);
	 SWIG_RegisterMapping("_wxImageList","_class_wxImageList",0);
	 SWIG_RegisterMapping("_wxDropFilesEvent","_class_wxDropFilesEvent",0);
	 SWIG_RegisterMapping("_wxBitmapButton","_class_wxBitmapButton",0);
	 SWIG_RegisterMapping("_class_wxAcceleratorTable","_wxAcceleratorTable",0);
	 SWIG_RegisterMapping("_class_wxGauge","_wxGauge",0);
	 SWIG_RegisterMapping("_wxDC","_class_wxDC",0);
	 SWIG_RegisterMapping("_wxSpinEvent","_class_wxSpinEvent",0);
	 SWIG_RegisterMapping("_class_wxRealPoint","_wxRealPoint",0);
	 SWIG_RegisterMapping("_class_wxMenuItem","_wxMenuItem",0);
	 SWIG_RegisterMapping("_class_wxPaintEvent","_wxPaintEvent",0);
	 SWIG_RegisterMapping("_wxSysColourChangedEvent","_class_wxSysColourChangedEvent",0);
	 SWIG_RegisterMapping("_class_wxStatusBar","_wxStatusBar",0);
	 SWIG_RegisterMapping("_class_wxPostScriptDC","_wxPostScriptDC",0);
	 SWIG_RegisterMapping("_wxPanel","_class_wxPanel",0);
	 SWIG_RegisterMapping("_wxInitDialogEvent","_class_wxInitDialogEvent",0);
	 SWIG_RegisterMapping("_wxCheckBox","_class_wxCheckBox",0);
	 SWIG_RegisterMapping("_wxTextCtrl","_class_wxTextCtrl",0);
	 SWIG_RegisterMapping("_class_wxToolTip","_wxToolTip",0);
	 SWIG_RegisterMapping("_class_wxMask","_wxMask",0);
	 SWIG_RegisterMapping("_class_wxKeyEvent","_wxKeyEvent",0);
	 SWIG_RegisterMapping("_wxColour","_class_wxColour",0);
	 SWIG_RegisterMapping("_class_wxDialog","_wxDialog",0);
	 SWIG_RegisterMapping("_wxIdleEvent","_class_wxIdleEvent",0);
	 SWIG_RegisterMapping("_class_wxUpdateUIEvent","_wxUpdateUIEvent",0);
	 SWIG_RegisterMapping("_wxToolBar","_class_wxToolBar",0);
	 SWIG_RegisterMapping("_wxBrush","_class_wxBrush",0);
	 SWIG_RegisterMapping("_wxMiniFrame","_class_wxMiniFrame",0);
	 SWIG_RegisterMapping("_wxShowEvent","_class_wxShowEvent",0);
	 SWIG_RegisterMapping("_uint","_unsigned_int",0);
	 SWIG_RegisterMapping("_uint","_int",0);
	 SWIG_RegisterMapping("_uint","_wxWindowID",0);
	 SWIG_RegisterMapping("_class_wxEvent","_wxEvent",0);
	 SWIG_RegisterMapping("_wxCheckListBox","_class_wxCheckListBox",0);
	 SWIG_RegisterMapping("_wxRect","_class_wxRect",0);
	 SWIG_RegisterMapping("_wxCommandEvent","_class_wxCommandEvent",0);
	 SWIG_RegisterMapping("_wxSizeEvent","_class_wxSizeEvent",0);
	 SWIG_RegisterMapping("_wxPoint","_class_wxPoint",0);
	 SWIG_RegisterMapping("_class_wxButton","_wxButton",0);
	 SWIG_RegisterMapping("_wxRadioBox","_class_wxRadioBox",0);
	 SWIG_RegisterMapping("_wxBitmap","_class_wxBitmap",0);
	 SWIG_RegisterMapping("_wxPyTimer","_class_wxPyTimer",0);
	 SWIG_RegisterMapping("_wxWindowDC","_class_wxWindowDC",0);
	 SWIG_RegisterMapping("_wxScrollBar","_class_wxScrollBar",0);
	 SWIG_RegisterMapping("_wxSpinButton","_class_wxSpinButton",0);
	 SWIG_RegisterMapping("_wxToolBarTool","_class_wxToolBarTool",0);
	 SWIG_RegisterMapping("_class_wxIndividualLayoutConstraint","_wxIndividualLayoutConstraint",0);
	 SWIG_RegisterMapping("_class_wxIconizeEvent","_wxIconizeEvent",0);
	 SWIG_RegisterMapping("_class_wxStaticBitmap","_wxStaticBitmap",0);
	 SWIG_RegisterMapping("_wxMDIChildFrame","_class_wxMDIChildFrame",0);
	 SWIG_RegisterMapping("_class_wxToolBar","_wxToolBar",0);
	 SWIG_RegisterMapping("_wxScrollEvent","_class_wxScrollEvent",0);
	 SWIG_RegisterMapping("_EBool","_signed_int",0);
	 SWIG_RegisterMapping("_EBool","_int",0);
	 SWIG_RegisterMapping("_EBool","_wxWindowID",0);
	 SWIG_RegisterMapping("_class_wxRegion","_wxRegion",0);
	 SWIG_RegisterMapping("_class_wxDropFilesEvent","_wxDropFilesEvent",0);
	 SWIG_RegisterMapping("_wxStaticText","_class_wxStaticText",0);
	 SWIG_RegisterMapping("_wxFont","_class_wxFont",0);
	 SWIG_RegisterMapping("_wxCloseEvent","_class_wxCloseEvent",0);
	 SWIG_RegisterMapping("_unsigned_long","_wxDash",0);
	 SWIG_RegisterMapping("_unsigned_long","_long",0);
	 SWIG_RegisterMapping("_class_wxRect","_wxRect",0);
	 SWIG_RegisterMapping("_class_wxDC","_wxDC",0);
	 SWIG_RegisterMapping("_wxMDIParentFrame","_class_wxMDIParentFrame",0);
	 SWIG_RegisterMapping("_class_wxPyTimer","_wxPyTimer",0);
	 SWIG_RegisterMapping("_wxFocusEvent","_class_wxFocusEvent",0);
	 SWIG_RegisterMapping("_wxMaximizeEvent","_class_wxMaximizeEvent",0);
	 SWIG_RegisterMapping("_class_wxSpinButton","_wxSpinButton",0);
	 SWIG_RegisterMapping("_wxAcceleratorEntry","_class_wxAcceleratorEntry",0);
	 SWIG_RegisterMapping("_class_wxPanel","_wxPanel",0);
	 SWIG_RegisterMapping("_class_wxCheckBox","_wxCheckBox",0);
	 SWIG_RegisterMapping("_wxComboBox","_class_wxComboBox",0);
	 SWIG_RegisterMapping("_wxRadioButton","_class_wxRadioButton",0);
	 SWIG_RegisterMapping("_signed_int","_EBool",0);
	 SWIG_RegisterMapping("_signed_int","_wxWindowID",0);
	 SWIG_RegisterMapping("_signed_int","_int",0);
	 SWIG_RegisterMapping("_class_wxTextCtrl","_wxTextCtrl",0);
	 SWIG_RegisterMapping("_wxLayoutConstraints","_class_wxLayoutConstraints",0);
	 SWIG_RegisterMapping("_wxMenu","_class_wxMenu",0);
	 SWIG_RegisterMapping("_class_wxMoveEvent","_wxMoveEvent",0);
	 SWIG_RegisterMapping("_wxListBox","_class_wxListBox",0);
	 SWIG_RegisterMapping("_wxScreenDC","_class_wxScreenDC",0);
	 SWIG_RegisterMapping("_class_wxMDIChildFrame","_wxMDIChildFrame",0);
	 SWIG_RegisterMapping("_WXTYPE","_short",0);
	 SWIG_RegisterMapping("_WXTYPE","_signed_short",0);
	 SWIG_RegisterMapping("_WXTYPE","_unsigned_short",0);
	 SWIG_RegisterMapping("_class_wxMDIClientWindow","_wxMDIClientWindow",0);
	 SWIG_RegisterMapping("_class_wxBrush","_wxBrush",0);
	 SWIG_RegisterMapping("_unsigned_short","_WXTYPE",0);
	 SWIG_RegisterMapping("_unsigned_short","_short",0);
	 SWIG_RegisterMapping("_class_wxWindow","_class_wxMDIClientWindow",SwigwxMDIClientWindowTowxWindow);
	 SWIG_RegisterMapping("_class_wxWindow","_wxMDIClientWindow",SwigwxMDIClientWindowTowxWindow);
	 SWIG_RegisterMapping("_class_wxWindow","_class_wxMDIChildFrame",SwigwxMDIChildFrameTowxWindow);
	 SWIG_RegisterMapping("_class_wxWindow","_wxMDIChildFrame",SwigwxMDIChildFrameTowxWindow);
	 SWIG_RegisterMapping("_class_wxWindow","_class_wxMDIParentFrame",SwigwxMDIParentFrameTowxWindow);
	 SWIG_RegisterMapping("_class_wxWindow","_wxMDIParentFrame",SwigwxMDIParentFrameTowxWindow);
	 SWIG_RegisterMapping("_class_wxWindow","_wxWindow",0);
	 SWIG_RegisterMapping("_class_wxStaticText","_wxStaticText",0);
	 SWIG_RegisterMapping("_class_wxFont","_wxFont",0);
	 SWIG_RegisterMapping("_class_wxCloseEvent","_wxCloseEvent",0);
	 SWIG_RegisterMapping("_class_wxMenuEvent","_wxMenuEvent",0);
	 SWIG_RegisterMapping("_wxClientDC","_class_wxClientDC",0);
	 SWIG_RegisterMapping("_wxMouseEvent","_class_wxMouseEvent",0);
	 SWIG_RegisterMapping("_class_wxPoint","_wxPoint",0);
	 SWIG_RegisterMapping("_wxRealPoint","_class_wxRealPoint",0);
	 SWIG_RegisterMapping("_class_wxRadioBox","_wxRadioBox",0);
	 SWIG_RegisterMapping("_signed_short","_WXTYPE",0);
	 SWIG_RegisterMapping("_signed_short","_short",0);
	 SWIG_RegisterMapping("_wxMemoryDC","_class_wxMemoryDC",0);
	 SWIG_RegisterMapping("_wxPaintDC","_class_wxPaintDC",0);
	 SWIG_RegisterMapping("_class_wxWindowDC","_wxWindowDC",0);
	 SWIG_RegisterMapping("_class_wxFocusEvent","_wxFocusEvent",0);
	 SWIG_RegisterMapping("_class_wxMaximizeEvent","_wxMaximizeEvent",0);
	 SWIG_RegisterMapping("_wxStatusBar","_class_wxStatusBar",0);
	 SWIG_RegisterMapping("_class_wxToolBarTool","_wxToolBarTool",0);
	 SWIG_RegisterMapping("_class_wxAcceleratorEntry","_wxAcceleratorEntry",0);
	 SWIG_RegisterMapping("_class_wxCursor","_wxCursor",0);
	 SWIG_RegisterMapping("_wxPostScriptDC","_class_wxPostScriptDC",0);
	 SWIG_RegisterMapping("_wxScrolledWindow","_class_wxScrolledWindow",0);
	 SWIG_RegisterMapping("_unsigned_char","_byte",0);
	 SWIG_RegisterMapping("_class_wxMenu","_wxMenu",0);
	 SWIG_RegisterMapping("_wxControl","_class_wxControl",0);
	 SWIG_RegisterMapping("_class_wxListBox","_wxListBox",0);
	 SWIG_RegisterMapping("_unsigned_int","_uint",0);
	 SWIG_RegisterMapping("_unsigned_int","_wxWindowID",0);
	 SWIG_RegisterMapping("_unsigned_int","_int",0);
	 SWIG_RegisterMapping("_wxIcon","_class_wxIcon",0);
	 SWIG_RegisterMapping("_wxDialog","_class_wxDialog",0);
	 SWIG_RegisterMapping("_class_wxPyMenu","_wxPyMenu",0);
	 SWIG_RegisterMapping("_class_wxPen","_wxPen",0);
	 SWIG_RegisterMapping("_short","_WXTYPE",0);
	 SWIG_RegisterMapping("_short","_unsigned_short",0);
	 SWIG_RegisterMapping("_short","_signed_short",0);
	 SWIG_RegisterMapping("_class_wxStaticBox","_wxStaticBox",0);
	 SWIG_RegisterMapping("_class_wxScrollEvent","_wxScrollEvent",0);
	 SWIG_RegisterMapping("_wxJoystickEvent","_class_wxJoystickEvent",0);
	 SWIG_RegisterMapping("_class_wxChoice","_wxChoice",0);
	 SWIG_RegisterMapping("_class_wxSlider","_wxSlider",0);
	 SWIG_RegisterMapping("_class_wxImageList","_wxImageList",0);
	 SWIG_RegisterMapping("_class_wxBitmapButton","_wxBitmapButton",0);
	 SWIG_RegisterMapping("_wxFrame","_class_wxMDIChildFrame",SwigwxMDIChildFrameTowxFrame);
	 SWIG_RegisterMapping("_wxFrame","_wxMDIChildFrame",SwigwxMDIChildFrameTowxFrame);
	 SWIG_RegisterMapping("_wxFrame","_class_wxMDIParentFrame",SwigwxMDIParentFrameTowxFrame);
	 SWIG_RegisterMapping("_wxFrame","_wxMDIParentFrame",SwigwxMDIParentFrameTowxFrame);
	 SWIG_RegisterMapping("_wxFrame","_class_wxFrame",0);
	 SWIG_RegisterMapping("_wxWindowID","_EBool",0);
	 SWIG_RegisterMapping("_wxWindowID","_uint",0);
	 SWIG_RegisterMapping("_wxWindowID","_int",0);
	 SWIG_RegisterMapping("_wxWindowID","_signed_int",0);
	 SWIG_RegisterMapping("_wxWindowID","_unsigned_int",0);
	 SWIG_RegisterMapping("_int","_EBool",0);
	 SWIG_RegisterMapping("_int","_uint",0);
	 SWIG_RegisterMapping("_int","_wxWindowID",0);
	 SWIG_RegisterMapping("_int","_unsigned_int",0);
	 SWIG_RegisterMapping("_int","_signed_int",0);
	 SWIG_RegisterMapping("_class_wxMouseEvent","_wxMouseEvent",0);
	 SWIG_RegisterMapping("_class_wxSpinEvent","_wxSpinEvent",0);
	 SWIG_RegisterMapping("_wxButton","_class_wxButton",0);
	 SWIG_RegisterMapping("_wxSize","_class_wxSize",0);
	 SWIG_RegisterMapping("_wxRegionIterator","_class_wxRegionIterator",0);
	 SWIG_RegisterMapping("_class_wxMDIParentFrame","_wxMDIParentFrame",0);
	 SWIG_RegisterMapping("_class_wxPaintDC","_wxPaintDC",0);
	 SWIG_RegisterMapping("_class_wxSysColourChangedEvent","_wxSysColourChangedEvent",0);
	 SWIG_RegisterMapping("_class_wxInitDialogEvent","_wxInitDialogEvent",0);
	 SWIG_RegisterMapping("_class_wxComboBox","_wxComboBox",0);
	 SWIG_RegisterMapping("_class_wxRadioButton","_wxRadioButton",0);
	 SWIG_RegisterMapping("_class_wxLayoutConstraints","_wxLayoutConstraints",0);
	 SWIG_RegisterMapping("_wxIconizeEvent","_class_wxIconizeEvent",0);
	 SWIG_RegisterMapping("_class_wxControl","_wxControl",0);
	 SWIG_RegisterMapping("_wxStaticBitmap","_class_wxStaticBitmap",0);
	 SWIG_RegisterMapping("_class_wxIcon","_wxIcon",0);
	 SWIG_RegisterMapping("_class_wxColour","_wxColour",0);
	 SWIG_RegisterMapping("_class_wxScreenDC","_wxScreenDC",0);
	 SWIG_RegisterMapping("_wxPalette","_class_wxPalette",0);
	 SWIG_RegisterMapping("_class_wxIdleEvent","_wxIdleEvent",0);
	 SWIG_RegisterMapping("_wxEraseEvent","_class_wxEraseEvent",0);
	 SWIG_RegisterMapping("_class_wxJoystickEvent","_wxJoystickEvent",0);
	 SWIG_RegisterMapping("_class_wxMiniFrame","_wxMiniFrame",0);
	 SWIG_RegisterMapping("_wxRegion","_class_wxRegion",0);
	 SWIG_RegisterMapping("_class_wxShowEvent","_wxShowEvent",0);
	 SWIG_RegisterMapping("_wxActivateEvent","_class_wxActivateEvent",0);
	 SWIG_RegisterMapping("_wxGauge","_class_wxGauge",0);
	 SWIG_RegisterMapping("_class_wxCheckListBox","_wxCheckListBox",0);
	 SWIG_RegisterMapping("_class_wxCommandEvent","_wxCommandEvent",0);
	 SWIG_RegisterMapping("_class_wxClientDC","_wxClientDC",0);
	 SWIG_RegisterMapping("_class_wxSizeEvent","_wxSizeEvent",0);
	 SWIG_RegisterMapping("_class_wxSize","_wxSize",0);
	 SWIG_RegisterMapping("_class_wxBitmap","_wxBitmap",0);
	 SWIG_RegisterMapping("_class_wxMemoryDC","_wxMemoryDC",0);
	 SWIG_RegisterMapping("_wxMenuBar","_class_wxMenuBar",0);
	 SWIG_RegisterMapping("_wxEvtHandler","_class_wxMDIClientWindow",SwigwxMDIClientWindowTowxEvtHandler);
	 SWIG_RegisterMapping("_wxEvtHandler","_wxMDIClientWindow",SwigwxMDIClientWindowTowxEvtHandler);
	 SWIG_RegisterMapping("_wxEvtHandler","_class_wxMDIChildFrame",SwigwxMDIChildFrameTowxEvtHandler);
	 SWIG_RegisterMapping("_wxEvtHandler","_wxMDIChildFrame",SwigwxMDIChildFrameTowxEvtHandler);
	 SWIG_RegisterMapping("_wxEvtHandler","_class_wxMDIParentFrame",SwigwxMDIParentFrameTowxEvtHandler);
	 SWIG_RegisterMapping("_wxEvtHandler","_wxMDIParentFrame",SwigwxMDIParentFrameTowxEvtHandler);
	 SWIG_RegisterMapping("_wxEvtHandler","_class_wxEvtHandler",0);
	 SWIG_RegisterMapping("_wxMenuItem","_class_wxMenuItem",0);
	 SWIG_RegisterMapping("_class_wxScrollBar","_wxScrollBar",0);
	 SWIG_RegisterMapping("_wxDash","_unsigned_long",0);
	 SWIG_RegisterMapping("_wxDash","_long",0);
	 SWIG_RegisterMapping("_class_wxScrolledWindow","_wxScrolledWindow",0);
	 SWIG_RegisterMapping("_wxKeyEvent","_class_wxKeyEvent",0);
	 SWIG_RegisterMapping("_wxMoveEvent","_class_wxMoveEvent",0);
	 SWIG_RegisterMapping("_class_wxPalette","_wxPalette",0);
	 SWIG_RegisterMapping("_class_wxEraseEvent","_wxEraseEvent",0);
	 SWIG_RegisterMapping("_wxMDIClientWindow","_class_wxMDIClientWindow",0);
	 SWIG_RegisterMapping("_wxWindow","_class_wxMDIClientWindow",SwigwxMDIClientWindowTowxWindow);
	 SWIG_RegisterMapping("_wxWindow","_wxMDIClientWindow",SwigwxMDIClientWindowTowxWindow);
	 SWIG_RegisterMapping("_wxWindow","_class_wxMDIChildFrame",SwigwxMDIChildFrameTowxWindow);
	 SWIG_RegisterMapping("_wxWindow","_wxMDIChildFrame",SwigwxMDIChildFrameTowxWindow);
	 SWIG_RegisterMapping("_wxWindow","_class_wxMDIParentFrame",SwigwxMDIParentFrameTowxWindow);
	 SWIG_RegisterMapping("_wxWindow","_wxMDIParentFrame",SwigwxMDIParentFrameTowxWindow);
	 SWIG_RegisterMapping("_wxWindow","_class_wxWindow",0);
	 SWIG_RegisterMapping("_class_wxFrame","_class_wxMDIChildFrame",SwigwxMDIChildFrameTowxFrame);
	 SWIG_RegisterMapping("_class_wxFrame","_wxMDIChildFrame",SwigwxMDIChildFrameTowxFrame);
	 SWIG_RegisterMapping("_class_wxFrame","_class_wxMDIParentFrame",SwigwxMDIParentFrameTowxFrame);
	 SWIG_RegisterMapping("_class_wxFrame","_wxMDIParentFrame",SwigwxMDIParentFrameTowxFrame);
	 SWIG_RegisterMapping("_class_wxFrame","_wxFrame",0);
}
