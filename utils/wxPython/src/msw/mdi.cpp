/*
 * FILE : msw/mdi.cpp
 * 
 * This file was automatically generated by :
 * Simplified Wrapper and Interface Generator (SWIG)
 * Version 1.1 (Build 810)
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
#	define SWIGEXPORT(a) __declspec(dllexport) a
#   else
#	if defined(__BORLANDC__)
#	    define SWIGEXPORT(a) a _export 
#	else
#	    define SWIGEXPORT(a) a 
#	endif
#   endif
#else
#   define SWIGEXPORT(a) a 
#endif

#ifdef __cplusplus
extern "C" {
#endif
#include "Python.h"
extern void SWIG_MakePtr(char *, void *, char *);
extern void SWIG_RegisterMapping(char *, char *, void *(*)(void *));
extern char *SWIG_GetPtr(char *, void **, char *);
extern char *SWIG_GetPtrObj(PyObject *, void **, char *);
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


HELPEREXPORT byte* byte_LIST_helper(PyObject* source);
HELPEREXPORT int* int_LIST_helper(PyObject* source);
HELPEREXPORT long* long_LIST_helper(PyObject* source);
HELPEREXPORT char** string_LIST_helper(PyObject* source);
HELPEREXPORT wxPoint* wxPoint_LIST_helper(PyObject* source);
HELPEREXPORT wxBitmap** wxBitmap_LIST_helper(PyObject* source);
HELPEREXPORT wxString* wxString_LIST_helper(PyObject* source);
HELPEREXPORT wxAcceleratorEntry* wxAcceleratorEntry_LIST_helper(PyObject* source);


static char* wxStringErrorMsg = "string type is required for parameter";
#ifdef __cplusplus
extern "C" {
#endif
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
static PyObject *_wrap_new_wxMDIParentFrame(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxMDIParentFrame * _result;
    wxWindow * _arg0;
    wxWindowID  _arg1;
    wxString * _arg2;
    wxPoint * _arg3 = (wxPoint *) &wxPyDefaultPosition;
    wxSize * _arg4 = (wxSize *) &wxPyDefaultSize;
    long  _arg5 = (long ) wxDEFAULT_FRAME_STYLE|wxVSCROLL|wxHSCROLL;
    char * _arg6 = (char *) "frame";
    PyObject * _argo0 = 0;
    PyObject * _obj2 = 0;
    PyObject * _argo3 = 0;
    PyObject * _argo4 = 0;
    char *_kwnames[] = { "parent","id","title","pos","size","style","name", NULL };
    char _ptemp[128];

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"OiO|OOls:new_wxMDIParentFrame",_kwnames,&_argo0,&_arg1,&_obj2,&_argo3,&_argo4,&_arg5,&_arg6)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxWindow_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of new_wxMDIParentFrame. Expected _wxWindow_p.");
        return NULL;
        }
    }
{
    if (!PyString_Check(_obj2)) {
        PyErr_SetString(PyExc_TypeError, wxStringErrorMsg);
        return NULL;
    }
    _arg2 = new wxString(PyString_AsString(_obj2), PyString_Size(_obj2));
}
    if (_argo3) {
        if (_argo3 == Py_None) { _arg3 = NULL; }
        else if (SWIG_GetPtrObj(_argo3,(void **) &_arg3,"_wxPoint_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 4 of new_wxMDIParentFrame. Expected _wxPoint_p.");
        return NULL;
        }
    }
    if (_argo4) {
        if (_argo4 == Py_None) { _arg4 = NULL; }
        else if (SWIG_GetPtrObj(_argo4,(void **) &_arg4,"_wxSize_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 5 of new_wxMDIParentFrame. Expected _wxSize_p.");
        return NULL;
        }
    }
{
    wxPy_BEGIN_ALLOW_THREADS;
        _result = (wxMDIParentFrame *)new_wxMDIParentFrame(_arg0,_arg1,*_arg2,*_arg3,*_arg4,_arg5,_arg6);

    wxPy_END_ALLOW_THREADS;
}    if (_result) {
        SWIG_MakePtr(_ptemp, (char *) _result,"_wxMDIParentFrame_p");
        _resultobj = Py_BuildValue("s",_ptemp);
    } else {
        Py_INCREF(Py_None);
        _resultobj = Py_None;
    }
{
    if (_obj2)
        delete _arg2;
}
    return _resultobj;
}

#define wxMDIParentFrame_ActivateNext(_swigobj)  (_swigobj->ActivateNext())
static PyObject *_wrap_wxMDIParentFrame_ActivateNext(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxMDIParentFrame * _arg0;
    PyObject * _argo0 = 0;
    char *_kwnames[] = { "self", NULL };

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"O:wxMDIParentFrame_ActivateNext",_kwnames,&_argo0)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxMDIParentFrame_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxMDIParentFrame_ActivateNext. Expected _wxMDIParentFrame_p.");
        return NULL;
        }
    }
{
    wxPy_BEGIN_ALLOW_THREADS;
        wxMDIParentFrame_ActivateNext(_arg0);

    wxPy_END_ALLOW_THREADS;
}    Py_INCREF(Py_None);
    _resultobj = Py_None;
    return _resultobj;
}

#define wxMDIParentFrame_ActivatePrevious(_swigobj)  (_swigobj->ActivatePrevious())
static PyObject *_wrap_wxMDIParentFrame_ActivatePrevious(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxMDIParentFrame * _arg0;
    PyObject * _argo0 = 0;
    char *_kwnames[] = { "self", NULL };

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"O:wxMDIParentFrame_ActivatePrevious",_kwnames,&_argo0)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxMDIParentFrame_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxMDIParentFrame_ActivatePrevious. Expected _wxMDIParentFrame_p.");
        return NULL;
        }
    }
{
    wxPy_BEGIN_ALLOW_THREADS;
        wxMDIParentFrame_ActivatePrevious(_arg0);

    wxPy_END_ALLOW_THREADS;
}    Py_INCREF(Py_None);
    _resultobj = Py_None;
    return _resultobj;
}

#define wxMDIParentFrame_ArrangeIcons(_swigobj)  (_swigobj->ArrangeIcons())
static PyObject *_wrap_wxMDIParentFrame_ArrangeIcons(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxMDIParentFrame * _arg0;
    PyObject * _argo0 = 0;
    char *_kwnames[] = { "self", NULL };

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"O:wxMDIParentFrame_ArrangeIcons",_kwnames,&_argo0)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxMDIParentFrame_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxMDIParentFrame_ArrangeIcons. Expected _wxMDIParentFrame_p.");
        return NULL;
        }
    }
{
    wxPy_BEGIN_ALLOW_THREADS;
        wxMDIParentFrame_ArrangeIcons(_arg0);

    wxPy_END_ALLOW_THREADS;
}    Py_INCREF(Py_None);
    _resultobj = Py_None;
    return _resultobj;
}

#define wxMDIParentFrame_Cascade(_swigobj)  (_swigobj->Cascade())
static PyObject *_wrap_wxMDIParentFrame_Cascade(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxMDIParentFrame * _arg0;
    PyObject * _argo0 = 0;
    char *_kwnames[] = { "self", NULL };

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"O:wxMDIParentFrame_Cascade",_kwnames,&_argo0)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxMDIParentFrame_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxMDIParentFrame_Cascade. Expected _wxMDIParentFrame_p.");
        return NULL;
        }
    }
{
    wxPy_BEGIN_ALLOW_THREADS;
        wxMDIParentFrame_Cascade(_arg0);

    wxPy_END_ALLOW_THREADS;
}    Py_INCREF(Py_None);
    _resultobj = Py_None;
    return _resultobj;
}

#define wxMDIParentFrame_GetActiveChild(_swigobj)  (_swigobj->GetActiveChild())
static PyObject *_wrap_wxMDIParentFrame_GetActiveChild(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxMDIChildFrame * _result;
    wxMDIParentFrame * _arg0;
    PyObject * _argo0 = 0;
    char *_kwnames[] = { "self", NULL };
    char _ptemp[128];

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"O:wxMDIParentFrame_GetActiveChild",_kwnames,&_argo0)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxMDIParentFrame_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxMDIParentFrame_GetActiveChild. Expected _wxMDIParentFrame_p.");
        return NULL;
        }
    }
{
    wxPy_BEGIN_ALLOW_THREADS;
        _result = (wxMDIChildFrame *)wxMDIParentFrame_GetActiveChild(_arg0);

    wxPy_END_ALLOW_THREADS;
}    if (_result) {
        SWIG_MakePtr(_ptemp, (char *) _result,"_wxMDIChildFrame_p");
        _resultobj = Py_BuildValue("s",_ptemp);
    } else {
        Py_INCREF(Py_None);
        _resultobj = Py_None;
    }
    return _resultobj;
}

#define wxMDIParentFrame_GetClientWindow(_swigobj)  (_swigobj->GetClientWindow())
static PyObject *_wrap_wxMDIParentFrame_GetClientWindow(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxMDIClientWindow * _result;
    wxMDIParentFrame * _arg0;
    PyObject * _argo0 = 0;
    char *_kwnames[] = { "self", NULL };
    char _ptemp[128];

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"O:wxMDIParentFrame_GetClientWindow",_kwnames,&_argo0)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxMDIParentFrame_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxMDIParentFrame_GetClientWindow. Expected _wxMDIParentFrame_p.");
        return NULL;
        }
    }
{
    wxPy_BEGIN_ALLOW_THREADS;
        _result = (wxMDIClientWindow *)wxMDIParentFrame_GetClientWindow(_arg0);

    wxPy_END_ALLOW_THREADS;
}    if (_result) {
        SWIG_MakePtr(_ptemp, (char *) _result,"_wxMDIClientWindow_p");
        _resultobj = Py_BuildValue("s",_ptemp);
    } else {
        Py_INCREF(Py_None);
        _resultobj = Py_None;
    }
    return _resultobj;
}

#define wxMDIParentFrame_GetToolBar(_swigobj)  (_swigobj->GetToolBar())
static PyObject *_wrap_wxMDIParentFrame_GetToolBar(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxWindow * _result;
    wxMDIParentFrame * _arg0;
    PyObject * _argo0 = 0;
    char *_kwnames[] = { "self", NULL };
    char _ptemp[128];

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"O:wxMDIParentFrame_GetToolBar",_kwnames,&_argo0)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxMDIParentFrame_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxMDIParentFrame_GetToolBar. Expected _wxMDIParentFrame_p.");
        return NULL;
        }
    }
{
    wxPy_BEGIN_ALLOW_THREADS;
        _result = (wxWindow *)wxMDIParentFrame_GetToolBar(_arg0);

    wxPy_END_ALLOW_THREADS;
}    if (_result) {
        SWIG_MakePtr(_ptemp, (char *) _result,"_wxWindow_p");
        _resultobj = Py_BuildValue("s",_ptemp);
    } else {
        Py_INCREF(Py_None);
        _resultobj = Py_None;
    }
    return _resultobj;
}

#define wxMDIParentFrame_SetToolBar(_swigobj,_swigarg0)  (_swigobj->SetToolBar(_swigarg0))
static PyObject *_wrap_wxMDIParentFrame_SetToolBar(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxMDIParentFrame * _arg0;
    wxToolBar * _arg1;
    PyObject * _argo0 = 0;
    PyObject * _argo1 = 0;
    char *_kwnames[] = { "self","toolbar", NULL };

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"OO:wxMDIParentFrame_SetToolBar",_kwnames,&_argo0,&_argo1)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxMDIParentFrame_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxMDIParentFrame_SetToolBar. Expected _wxMDIParentFrame_p.");
        return NULL;
        }
    }
    if (_argo1) {
        if (_argo1 == Py_None) { _arg1 = NULL; }
        else if (SWIG_GetPtrObj(_argo1,(void **) &_arg1,"_wxToolBar_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 2 of wxMDIParentFrame_SetToolBar. Expected _wxToolBar_p.");
        return NULL;
        }
    }
{
    wxPy_BEGIN_ALLOW_THREADS;
        wxMDIParentFrame_SetToolBar(_arg0,_arg1);

    wxPy_END_ALLOW_THREADS;
}    Py_INCREF(Py_None);
    _resultobj = Py_None;
    return _resultobj;
}

#define wxMDIParentFrame_Tile(_swigobj)  (_swigobj->Tile())
static PyObject *_wrap_wxMDIParentFrame_Tile(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxMDIParentFrame * _arg0;
    PyObject * _argo0 = 0;
    char *_kwnames[] = { "self", NULL };

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"O:wxMDIParentFrame_Tile",_kwnames,&_argo0)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxMDIParentFrame_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxMDIParentFrame_Tile. Expected _wxMDIParentFrame_p.");
        return NULL;
        }
    }
{
    wxPy_BEGIN_ALLOW_THREADS;
        wxMDIParentFrame_Tile(_arg0);

    wxPy_END_ALLOW_THREADS;
}    Py_INCREF(Py_None);
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
static PyObject *_wrap_new_wxMDIChildFrame(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxMDIChildFrame * _result;
    wxMDIParentFrame * _arg0;
    wxWindowID  _arg1;
    wxString * _arg2;
    wxPoint * _arg3 = (wxPoint *) &wxPyDefaultPosition;
    wxSize * _arg4 = (wxSize *) &wxPyDefaultSize;
    long  _arg5 = (long ) wxDEFAULT_FRAME_STYLE;
    char * _arg6 = (char *) "frame";
    PyObject * _argo0 = 0;
    PyObject * _obj2 = 0;
    PyObject * _argo3 = 0;
    PyObject * _argo4 = 0;
    char *_kwnames[] = { "parent","id","title","pos","size","style","name", NULL };
    char _ptemp[128];

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"OiO|OOls:new_wxMDIChildFrame",_kwnames,&_argo0,&_arg1,&_obj2,&_argo3,&_argo4,&_arg5,&_arg6)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxMDIParentFrame_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of new_wxMDIChildFrame. Expected _wxMDIParentFrame_p.");
        return NULL;
        }
    }
{
    if (!PyString_Check(_obj2)) {
        PyErr_SetString(PyExc_TypeError, wxStringErrorMsg);
        return NULL;
    }
    _arg2 = new wxString(PyString_AsString(_obj2), PyString_Size(_obj2));
}
    if (_argo3) {
        if (_argo3 == Py_None) { _arg3 = NULL; }
        else if (SWIG_GetPtrObj(_argo3,(void **) &_arg3,"_wxPoint_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 4 of new_wxMDIChildFrame. Expected _wxPoint_p.");
        return NULL;
        }
    }
    if (_argo4) {
        if (_argo4 == Py_None) { _arg4 = NULL; }
        else if (SWIG_GetPtrObj(_argo4,(void **) &_arg4,"_wxSize_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 5 of new_wxMDIChildFrame. Expected _wxSize_p.");
        return NULL;
        }
    }
{
    wxPy_BEGIN_ALLOW_THREADS;
        _result = (wxMDIChildFrame *)new_wxMDIChildFrame(_arg0,_arg1,*_arg2,*_arg3,*_arg4,_arg5,_arg6);

    wxPy_END_ALLOW_THREADS;
}    if (_result) {
        SWIG_MakePtr(_ptemp, (char *) _result,"_wxMDIChildFrame_p");
        _resultobj = Py_BuildValue("s",_ptemp);
    } else {
        Py_INCREF(Py_None);
        _resultobj = Py_None;
    }
{
    if (_obj2)
        delete _arg2;
}
    return _resultobj;
}

#define wxMDIChildFrame_Activate(_swigobj)  (_swigobj->Activate())
static PyObject *_wrap_wxMDIChildFrame_Activate(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxMDIChildFrame * _arg0;
    PyObject * _argo0 = 0;
    char *_kwnames[] = { "self", NULL };

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"O:wxMDIChildFrame_Activate",_kwnames,&_argo0)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxMDIChildFrame_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxMDIChildFrame_Activate. Expected _wxMDIChildFrame_p.");
        return NULL;
        }
    }
{
    wxPy_BEGIN_ALLOW_THREADS;
        wxMDIChildFrame_Activate(_arg0);

    wxPy_END_ALLOW_THREADS;
}    Py_INCREF(Py_None);
    _resultobj = Py_None;
    return _resultobj;
}

#define wxMDIChildFrame_Maximize(_swigobj,_swigarg0)  (_swigobj->Maximize(_swigarg0))
static PyObject *_wrap_wxMDIChildFrame_Maximize(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxMDIChildFrame * _arg0;
    bool  _arg1;
    PyObject * _argo0 = 0;
    int tempbool1;
    char *_kwnames[] = { "self","maximize", NULL };

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"Oi:wxMDIChildFrame_Maximize",_kwnames,&_argo0,&tempbool1)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxMDIChildFrame_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxMDIChildFrame_Maximize. Expected _wxMDIChildFrame_p.");
        return NULL;
        }
    }
    _arg1 = (bool ) tempbool1;
{
    wxPy_BEGIN_ALLOW_THREADS;
        wxMDIChildFrame_Maximize(_arg0,_arg1);

    wxPy_END_ALLOW_THREADS;
}    Py_INCREF(Py_None);
    _resultobj = Py_None;
    return _resultobj;
}

#define wxMDIChildFrame_Restore(_swigobj)  (_swigobj->Restore())
static PyObject *_wrap_wxMDIChildFrame_Restore(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxMDIChildFrame * _arg0;
    PyObject * _argo0 = 0;
    char *_kwnames[] = { "self", NULL };

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"O:wxMDIChildFrame_Restore",_kwnames,&_argo0)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxMDIChildFrame_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of wxMDIChildFrame_Restore. Expected _wxMDIChildFrame_p.");
        return NULL;
        }
    }
{
    wxPy_BEGIN_ALLOW_THREADS;
        wxMDIChildFrame_Restore(_arg0);

    wxPy_END_ALLOW_THREADS;
}    Py_INCREF(Py_None);
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
static PyObject *_wrap_new_wxMDIClientWindow(PyObject *self, PyObject *args, PyObject *kwargs) {
    PyObject * _resultobj;
    wxMDIClientWindow * _result;
    wxMDIParentFrame * _arg0;
    long  _arg1 = (long ) 0;
    PyObject * _argo0 = 0;
    char *_kwnames[] = { "parent","style", NULL };
    char _ptemp[128];

    self = self;
    if(!PyArg_ParseTupleAndKeywords(args,kwargs,"O|l:new_wxMDIClientWindow",_kwnames,&_argo0,&_arg1)) 
        return NULL;
    if (_argo0) {
        if (_argo0 == Py_None) { _arg0 = NULL; }
        else if (SWIG_GetPtrObj(_argo0,(void **) &_arg0,"_wxMDIParentFrame_p")) {
            PyErr_SetString(PyExc_TypeError,"Type error in argument 1 of new_wxMDIClientWindow. Expected _wxMDIParentFrame_p.");
        return NULL;
        }
    }
{
    wxPy_BEGIN_ALLOW_THREADS;
        _result = (wxMDIClientWindow *)new_wxMDIClientWindow(_arg0,_arg1);

    wxPy_END_ALLOW_THREADS;
}    if (_result) {
        SWIG_MakePtr(_ptemp, (char *) _result,"_wxMDIClientWindow_p");
        _resultobj = Py_BuildValue("s",_ptemp);
    } else {
        Py_INCREF(Py_None);
        _resultobj = Py_None;
    }
    return _resultobj;
}

static PyMethodDef mdicMethods[] = {
	 { "new_wxMDIClientWindow", (PyCFunction) _wrap_new_wxMDIClientWindow, METH_VARARGS | METH_KEYWORDS },
	 { "wxMDIChildFrame_Restore", (PyCFunction) _wrap_wxMDIChildFrame_Restore, METH_VARARGS | METH_KEYWORDS },
	 { "wxMDIChildFrame_Maximize", (PyCFunction) _wrap_wxMDIChildFrame_Maximize, METH_VARARGS | METH_KEYWORDS },
	 { "wxMDIChildFrame_Activate", (PyCFunction) _wrap_wxMDIChildFrame_Activate, METH_VARARGS | METH_KEYWORDS },
	 { "new_wxMDIChildFrame", (PyCFunction) _wrap_new_wxMDIChildFrame, METH_VARARGS | METH_KEYWORDS },
	 { "wxMDIParentFrame_Tile", (PyCFunction) _wrap_wxMDIParentFrame_Tile, METH_VARARGS | METH_KEYWORDS },
	 { "wxMDIParentFrame_SetToolBar", (PyCFunction) _wrap_wxMDIParentFrame_SetToolBar, METH_VARARGS | METH_KEYWORDS },
	 { "wxMDIParentFrame_GetToolBar", (PyCFunction) _wrap_wxMDIParentFrame_GetToolBar, METH_VARARGS | METH_KEYWORDS },
	 { "wxMDIParentFrame_GetClientWindow", (PyCFunction) _wrap_wxMDIParentFrame_GetClientWindow, METH_VARARGS | METH_KEYWORDS },
	 { "wxMDIParentFrame_GetActiveChild", (PyCFunction) _wrap_wxMDIParentFrame_GetActiveChild, METH_VARARGS | METH_KEYWORDS },
	 { "wxMDIParentFrame_Cascade", (PyCFunction) _wrap_wxMDIParentFrame_Cascade, METH_VARARGS | METH_KEYWORDS },
	 { "wxMDIParentFrame_ArrangeIcons", (PyCFunction) _wrap_wxMDIParentFrame_ArrangeIcons, METH_VARARGS | METH_KEYWORDS },
	 { "wxMDIParentFrame_ActivatePrevious", (PyCFunction) _wrap_wxMDIParentFrame_ActivatePrevious, METH_VARARGS | METH_KEYWORDS },
	 { "wxMDIParentFrame_ActivateNext", (PyCFunction) _wrap_wxMDIParentFrame_ActivateNext, METH_VARARGS | METH_KEYWORDS },
	 { "new_wxMDIParentFrame", (PyCFunction) _wrap_new_wxMDIParentFrame, METH_VARARGS | METH_KEYWORDS },
	 { NULL, NULL }
};
#ifdef __cplusplus
}
#endif
/*
 * This table is used by the pointer type-checker
 */
static struct { char *n1; char *n2; void *(*pcnv)(void *); } _swig_mapping[] = {
    { "_wxAcceleratorTable","_class_wxAcceleratorTable",0},
    { "_wxEvent","_class_wxEvent",0},
    { "_class_wxActivateEvent","_wxActivateEvent",0},
    { "_signed_long","_long",0},
    { "_wxMenuEvent","_class_wxMenuEvent",0},
    { "_wxPrintQuality","_int",0},
    { "_wxPrintQuality","_signed_int",0},
    { "_wxPrintQuality","_unsigned_int",0},
    { "_wxPrintQuality","_wxWindowID",0},
    { "_wxPrintQuality","_uint",0},
    { "_wxPrintQuality","_EBool",0},
    { "_wxPrintQuality","_size_t",0},
    { "_class_wxRegionIterator","_wxRegionIterator",0},
    { "_class_wxMenuBar","_wxMenuBar",0},
    { "_class_wxEvtHandler","_class_wxMDIClientWindow",SwigwxMDIClientWindowTowxEvtHandler},
    { "_class_wxEvtHandler","_wxMDIClientWindow",SwigwxMDIClientWindowTowxEvtHandler},
    { "_class_wxEvtHandler","_class_wxMDIChildFrame",SwigwxMDIChildFrameTowxEvtHandler},
    { "_class_wxEvtHandler","_wxMDIChildFrame",SwigwxMDIChildFrameTowxEvtHandler},
    { "_class_wxEvtHandler","_class_wxMDIParentFrame",SwigwxMDIParentFrameTowxEvtHandler},
    { "_class_wxEvtHandler","_wxMDIParentFrame",SwigwxMDIParentFrameTowxEvtHandler},
    { "_class_wxEvtHandler","_wxEvtHandler",0},
    { "_wxPaintEvent","_class_wxPaintEvent",0},
    { "_wxIndividualLayoutConstraint","_class_wxIndividualLayoutConstraint",0},
    { "_wxCursor","_class_wxCursor",0},
    { "_wxNotifyEvent","_class_wxNotifyEvent",0},
    { "_wxMask","_class_wxMask",0},
    { "_wxPen","_class_wxPen",0},
    { "_wxUpdateUIEvent","_class_wxUpdateUIEvent",0},
    { "_byte","_unsigned_char",0},
    { "_wxStaticBox","_class_wxStaticBox",0},
    { "_wxChoice","_class_wxChoice",0},
    { "_wxSlider","_class_wxSlider",0},
    { "_long","_wxDash",0},
    { "_long","_unsigned_long",0},
    { "_long","_signed_long",0},
    { "_wxImageList","_class_wxImageList",0},
    { "_wxDropFilesEvent","_class_wxDropFilesEvent",0},
    { "_wxBitmapButton","_class_wxBitmapButton",0},
    { "_class_wxAcceleratorTable","_wxAcceleratorTable",0},
    { "_class_wxGauge","_wxGauge",0},
    { "_wxDC","_class_wxDC",0},
    { "_wxSpinEvent","_class_wxSpinEvent",0},
    { "_size_t","_wxPrintQuality",0},
    { "_size_t","_unsigned_int",0},
    { "_size_t","_int",0},
    { "_size_t","_wxWindowID",0},
    { "_size_t","_uint",0},
    { "_class_wxRealPoint","_wxRealPoint",0},
    { "_wxPrinterDC","_class_wxPrinterDC",0},
    { "_class_wxMenuItem","_wxMenuItem",0},
    { "_class_wxPaintEvent","_wxPaintEvent",0},
    { "_wxSysColourChangedEvent","_class_wxSysColourChangedEvent",0},
    { "_class_wxStatusBar","_wxStatusBar",0},
    { "_wxPanel","_class_wxPanel",0},
    { "_wxInitDialogEvent","_class_wxInitDialogEvent",0},
    { "_wxCheckBox","_class_wxCheckBox",0},
    { "_wxPyEvent","_class_wxPyEvent",0},
    { "_wxTextCtrl","_class_wxTextCtrl",0},
    { "_class_wxMask","_wxMask",0},
    { "_class_wxKeyEvent","_wxKeyEvent",0},
    { "_wxColour","_class_wxColour",0},
    { "_class_wxDialog","_wxDialog",0},
    { "_wxIdleEvent","_class_wxIdleEvent",0},
    { "_class_wxUpdateUIEvent","_wxUpdateUIEvent",0},
    { "_wxToolBar","_class_wxToolBar",0},
    { "_wxStaticLine","_class_wxStaticLine",0},
    { "_wxBrush","_class_wxBrush",0},
    { "_wxMiniFrame","_class_wxMiniFrame",0},
    { "_wxShowEvent","_class_wxShowEvent",0},
    { "_uint","_wxPrintQuality",0},
    { "_uint","_size_t",0},
    { "_uint","_unsigned_int",0},
    { "_uint","_int",0},
    { "_uint","_wxWindowID",0},
    { "_class_wxEvent","_wxEvent",0},
    { "_wxCheckListBox","_class_wxCheckListBox",0},
    { "_wxRect","_class_wxRect",0},
    { "_wxCommandEvent","_class_wxCommandEvent",0},
    { "_wxSizeEvent","_class_wxSizeEvent",0},
    { "_wxPoint","_class_wxPoint",0},
    { "_class_wxButton","_wxButton",0},
    { "_wxRadioBox","_class_wxRadioBox",0},
    { "_wxBitmap","_class_wxBitmap",0},
    { "_wxPyTimer","_class_wxPyTimer",0},
    { "_wxWindowDC","_class_wxWindowDC",0},
    { "_wxScrollBar","_class_wxScrollBar",0},
    { "_wxSpinButton","_class_wxSpinButton",0},
    { "_wxToolBarTool","_class_wxToolBarTool",0},
    { "_class_wxIndividualLayoutConstraint","_wxIndividualLayoutConstraint",0},
    { "_class_wxNotifyEvent","_wxNotifyEvent",0},
    { "_class_wxPyEvent","_wxPyEvent",0},
    { "_class_wxIconizeEvent","_wxIconizeEvent",0},
    { "_class_wxStaticBitmap","_wxStaticBitmap",0},
    { "_wxMDIChildFrame","_class_wxMDIChildFrame",0},
    { "_class_wxToolBar","_wxToolBar",0},
    { "_class_wxStaticLine","_wxStaticLine",0},
    { "_wxScrollEvent","_class_wxScrollEvent",0},
    { "_EBool","_wxPrintQuality",0},
    { "_EBool","_signed_int",0},
    { "_EBool","_int",0},
    { "_EBool","_wxWindowID",0},
    { "_class_wxRegion","_wxRegion",0},
    { "_class_wxDropFilesEvent","_wxDropFilesEvent",0},
    { "_wxStaticText","_class_wxStaticText",0},
    { "_wxFont","_class_wxFont",0},
    { "_wxCloseEvent","_class_wxCloseEvent",0},
    { "_unsigned_long","_wxDash",0},
    { "_unsigned_long","_long",0},
    { "_class_wxRect","_wxRect",0},
    { "_class_wxDC","_wxDC",0},
    { "_wxScrollWinEvent","_class_wxScrollWinEvent",0},
    { "_wxMDIParentFrame","_class_wxMDIParentFrame",0},
    { "_class_wxPyTimer","_wxPyTimer",0},
    { "_wxFocusEvent","_class_wxFocusEvent",0},
    { "_wxMaximizeEvent","_class_wxMaximizeEvent",0},
    { "_class_wxSpinButton","_wxSpinButton",0},
    { "_wxAcceleratorEntry","_class_wxAcceleratorEntry",0},
    { "_class_wxPanel","_wxPanel",0},
    { "_class_wxCheckBox","_wxCheckBox",0},
    { "_wxComboBox","_class_wxComboBox",0},
    { "_wxRadioButton","_class_wxRadioButton",0},
    { "_signed_int","_wxPrintQuality",0},
    { "_signed_int","_EBool",0},
    { "_signed_int","_wxWindowID",0},
    { "_signed_int","_int",0},
    { "_class_wxTextCtrl","_wxTextCtrl",0},
    { "_wxLayoutConstraints","_class_wxLayoutConstraints",0},
    { "_wxMetaFileDC","_class_wxMetaFileDC",0},
    { "_wxMenu","_class_wxMenu",0},
    { "_class_wxMoveEvent","_wxMoveEvent",0},
    { "_wxListBox","_class_wxListBox",0},
    { "_wxScreenDC","_class_wxScreenDC",0},
    { "_class_wxMDIChildFrame","_wxMDIChildFrame",0},
    { "_WXTYPE","_short",0},
    { "_WXTYPE","_signed_short",0},
    { "_WXTYPE","_unsigned_short",0},
    { "_class_wxMDIClientWindow","_wxMDIClientWindow",0},
    { "_class_wxBrush","_wxBrush",0},
    { "_unsigned_short","_WXTYPE",0},
    { "_unsigned_short","_short",0},
    { "_class_wxWindow","_class_wxMDIClientWindow",SwigwxMDIClientWindowTowxWindow},
    { "_class_wxWindow","_wxMDIClientWindow",SwigwxMDIClientWindowTowxWindow},
    { "_class_wxWindow","_class_wxMDIChildFrame",SwigwxMDIChildFrameTowxWindow},
    { "_class_wxWindow","_wxMDIChildFrame",SwigwxMDIChildFrameTowxWindow},
    { "_class_wxWindow","_class_wxMDIParentFrame",SwigwxMDIParentFrameTowxWindow},
    { "_class_wxWindow","_wxMDIParentFrame",SwigwxMDIParentFrameTowxWindow},
    { "_class_wxWindow","_wxWindow",0},
    { "_class_wxStaticText","_wxStaticText",0},
    { "_class_wxFont","_wxFont",0},
    { "_class_wxCloseEvent","_wxCloseEvent",0},
    { "_class_wxMenuEvent","_wxMenuEvent",0},
    { "_wxClientDC","_class_wxClientDC",0},
    { "_wxMouseEvent","_class_wxMouseEvent",0},
    { "_class_wxPoint","_wxPoint",0},
    { "_wxRealPoint","_class_wxRealPoint",0},
    { "_class_wxRadioBox","_wxRadioBox",0},
    { "_signed_short","_WXTYPE",0},
    { "_signed_short","_short",0},
    { "_wxMemoryDC","_class_wxMemoryDC",0},
    { "_wxPaintDC","_class_wxPaintDC",0},
    { "_class_wxWindowDC","_wxWindowDC",0},
    { "_class_wxFocusEvent","_wxFocusEvent",0},
    { "_class_wxMaximizeEvent","_wxMaximizeEvent",0},
    { "_wxStatusBar","_class_wxStatusBar",0},
    { "_class_wxToolBarTool","_wxToolBarTool",0},
    { "_class_wxAcceleratorEntry","_wxAcceleratorEntry",0},
    { "_class_wxCursor","_wxCursor",0},
    { "_wxScrolledWindow","_class_wxScrolledWindow",0},
    { "_unsigned_char","_byte",0},
    { "_class_wxMetaFileDC","_wxMetaFileDC",0},
    { "_class_wxMenu","_wxMenu",0},
    { "_wxControl","_class_wxControl",0},
    { "_class_wxListBox","_wxListBox",0},
    { "_unsigned_int","_wxPrintQuality",0},
    { "_unsigned_int","_size_t",0},
    { "_unsigned_int","_uint",0},
    { "_unsigned_int","_wxWindowID",0},
    { "_unsigned_int","_int",0},
    { "_wxIcon","_class_wxIcon",0},
    { "_wxDialog","_class_wxDialog",0},
    { "_class_wxPen","_wxPen",0},
    { "_short","_WXTYPE",0},
    { "_short","_unsigned_short",0},
    { "_short","_signed_short",0},
    { "_class_wxStaticBox","_wxStaticBox",0},
    { "_class_wxScrollEvent","_wxScrollEvent",0},
    { "_wxJoystickEvent","_class_wxJoystickEvent",0},
    { "_class_wxChoice","_wxChoice",0},
    { "_class_wxSlider","_wxSlider",0},
    { "_class_wxImageList","_wxImageList",0},
    { "_class_wxBitmapButton","_wxBitmapButton",0},
    { "_wxFrame","_class_wxMDIChildFrame",SwigwxMDIChildFrameTowxFrame},
    { "_wxFrame","_wxMDIChildFrame",SwigwxMDIChildFrameTowxFrame},
    { "_wxFrame","_class_wxMDIParentFrame",SwigwxMDIParentFrameTowxFrame},
    { "_wxFrame","_wxMDIParentFrame",SwigwxMDIParentFrameTowxFrame},
    { "_wxFrame","_class_wxFrame",0},
    { "_wxWindowID","_wxPrintQuality",0},
    { "_wxWindowID","_size_t",0},
    { "_wxWindowID","_EBool",0},
    { "_wxWindowID","_uint",0},
    { "_wxWindowID","_int",0},
    { "_wxWindowID","_signed_int",0},
    { "_wxWindowID","_unsigned_int",0},
    { "_class_wxScrollWinEvent","_wxScrollWinEvent",0},
    { "_int","_wxPrintQuality",0},
    { "_int","_size_t",0},
    { "_int","_EBool",0},
    { "_int","_uint",0},
    { "_int","_wxWindowID",0},
    { "_int","_unsigned_int",0},
    { "_int","_signed_int",0},
    { "_class_wxMouseEvent","_wxMouseEvent",0},
    { "_class_wxSpinEvent","_wxSpinEvent",0},
    { "_wxButton","_class_wxButton",0},
    { "_wxSize","_class_wxSize",0},
    { "_wxRegionIterator","_class_wxRegionIterator",0},
    { "_class_wxPrinterDC","_wxPrinterDC",0},
    { "_class_wxMDIParentFrame","_wxMDIParentFrame",0},
    { "_class_wxPaintDC","_wxPaintDC",0},
    { "_class_wxSysColourChangedEvent","_wxSysColourChangedEvent",0},
    { "_class_wxInitDialogEvent","_wxInitDialogEvent",0},
    { "_class_wxComboBox","_wxComboBox",0},
    { "_class_wxRadioButton","_wxRadioButton",0},
    { "_class_wxLayoutConstraints","_wxLayoutConstraints",0},
    { "_wxIconizeEvent","_class_wxIconizeEvent",0},
    { "_class_wxControl","_wxControl",0},
    { "_wxStaticBitmap","_class_wxStaticBitmap",0},
    { "_class_wxIcon","_wxIcon",0},
    { "_class_wxColour","_wxColour",0},
    { "_class_wxScreenDC","_wxScreenDC",0},
    { "_wxPalette","_class_wxPalette",0},
    { "_class_wxIdleEvent","_wxIdleEvent",0},
    { "_wxEraseEvent","_class_wxEraseEvent",0},
    { "_class_wxJoystickEvent","_wxJoystickEvent",0},
    { "_class_wxMiniFrame","_wxMiniFrame",0},
    { "_wxRegion","_class_wxRegion",0},
    { "_class_wxShowEvent","_wxShowEvent",0},
    { "_wxActivateEvent","_class_wxActivateEvent",0},
    { "_wxGauge","_class_wxGauge",0},
    { "_class_wxCheckListBox","_wxCheckListBox",0},
    { "_class_wxCommandEvent","_wxCommandEvent",0},
    { "_class_wxClientDC","_wxClientDC",0},
    { "_class_wxSizeEvent","_wxSizeEvent",0},
    { "_class_wxSize","_wxSize",0},
    { "_class_wxBitmap","_wxBitmap",0},
    { "_class_wxMemoryDC","_wxMemoryDC",0},
    { "_wxMenuBar","_class_wxMenuBar",0},
    { "_wxEvtHandler","_class_wxMDIClientWindow",SwigwxMDIClientWindowTowxEvtHandler},
    { "_wxEvtHandler","_wxMDIClientWindow",SwigwxMDIClientWindowTowxEvtHandler},
    { "_wxEvtHandler","_class_wxMDIChildFrame",SwigwxMDIChildFrameTowxEvtHandler},
    { "_wxEvtHandler","_wxMDIChildFrame",SwigwxMDIChildFrameTowxEvtHandler},
    { "_wxEvtHandler","_class_wxMDIParentFrame",SwigwxMDIParentFrameTowxEvtHandler},
    { "_wxEvtHandler","_wxMDIParentFrame",SwigwxMDIParentFrameTowxEvtHandler},
    { "_wxEvtHandler","_class_wxEvtHandler",0},
    { "_wxMenuItem","_class_wxMenuItem",0},
    { "_class_wxScrollBar","_wxScrollBar",0},
    { "_wxDash","_unsigned_long",0},
    { "_wxDash","_long",0},
    { "_class_wxScrolledWindow","_wxScrolledWindow",0},
    { "_wxKeyEvent","_class_wxKeyEvent",0},
    { "_wxMoveEvent","_class_wxMoveEvent",0},
    { "_class_wxPalette","_wxPalette",0},
    { "_class_wxEraseEvent","_wxEraseEvent",0},
    { "_wxMDIClientWindow","_class_wxMDIClientWindow",0},
    { "_wxWindow","_class_wxMDIClientWindow",SwigwxMDIClientWindowTowxWindow},
    { "_wxWindow","_wxMDIClientWindow",SwigwxMDIClientWindowTowxWindow},
    { "_wxWindow","_class_wxMDIChildFrame",SwigwxMDIChildFrameTowxWindow},
    { "_wxWindow","_wxMDIChildFrame",SwigwxMDIChildFrameTowxWindow},
    { "_wxWindow","_class_wxMDIParentFrame",SwigwxMDIParentFrameTowxWindow},
    { "_wxWindow","_wxMDIParentFrame",SwigwxMDIParentFrameTowxWindow},
    { "_wxWindow","_class_wxWindow",0},
    { "_class_wxFrame","_class_wxMDIChildFrame",SwigwxMDIChildFrameTowxFrame},
    { "_class_wxFrame","_wxMDIChildFrame",SwigwxMDIChildFrameTowxFrame},
    { "_class_wxFrame","_class_wxMDIParentFrame",SwigwxMDIParentFrameTowxFrame},
    { "_class_wxFrame","_wxMDIParentFrame",SwigwxMDIParentFrameTowxFrame},
    { "_class_wxFrame","_wxFrame",0},
{0,0,0}};

static PyObject *SWIG_globals;
#ifdef __cplusplus
extern "C" 
#endif
SWIGEXPORT(void) initmdic() {
	 PyObject *m, *d;
	 SWIG_globals = SWIG_newvarlink();
	 m = Py_InitModule("mdic", mdicMethods);
	 d = PyModule_GetDict(m);
{
   int i;
   for (i = 0; _swig_mapping[i].n1; i++)
        SWIG_RegisterMapping(_swig_mapping[i].n1,_swig_mapping[i].n2,_swig_mapping[i].pcnv);
}
}
