/////////////////////////////////////////////////////////////////////////////
// Name:        wx/ptr_scpd.h
// Purpose:     scoped smart pointer class
// Author:      Jesse Lovelace <jllovela@eos.ncsu.edu>
// Modified by:
// Created:     06/01/02
// RCS-ID:      $Id$
// Copyright:   (c) Jesse Lovelace and original Boost authors (see below)
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

//  This class closely follows the implementation of the boost
//  library scoped_ptr and is an adaption for c++ macro's in 
//  the wxWindows project. The original authors of the boost
//  scoped_ptr are given below with their respective copyrights.

//  (C) Copyright Greg Colvin and Beman Dawes 1998, 1999.
//  Copyright (c) 2001, 2002 Peter Dimov
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
//  See http://www.boost.org/libs/smart_ptr/scoped_ptr.htm for documentation.
//

#ifndef __WX_SCOPED_POINTER__
#define __WX_SCOPED_POINTER__

#include "wx/defs.h"

/*
   checked deleters are used to make sure that the type being deleted is really
   a complete type.: otherwise sizeof() would result in a compile-time error

   do { ... } while ( 0 ) construct is used to have an anonymous scope
   (otherwise we could have name clashes between different "complete"s) but
   still force a semicolon after the macro
*/

#define wxCHECKED_DELETE(ptr)                                                 \
    do                                                                        \
    {                                                                         \
        typedef char complete[sizeof(*ptr)];                                  \
        delete ptr;                                                           \
    } while ( 0 )

#define wxCHECKED_DELETE_ARRAY(ptr)                                           \
    do                                                                        \
    {                                                                         \
        typedef char complete[sizeof(*ptr)];                                  \
        delete [] ptr;                                                        \
    } while ( 0 )

/* These scoped pointers are *not* assignable and cannot be used
   within a container.  Look for wxDECLARE_SHARED_PTR for this
   functionality.

   In addition, the type being used *must* be complete at the time
   that wxDEFINE_SCOPED_* is called or a compiler error will result.
   This is because the class checks for the completeness of the type
   being used.
*/


#define wxDECLARE_SCOPED_PTR(T, name) \
class name                          \
{                                   \
private:                            \
    T * m_ptr;                      \
                                    \
    name(name const &);             \
    name & operator=(name const &); \
                                    \
public:                             \
    wxEXPLICIT name(T * ptr = NULL) \
    : m_ptr(ptr) { }                \
                                    \
    ~name();                        \
                                    \
    void reset(T * ptr = NULL)      \
    {                               \
        if (m_ptr != ptr)           \
        {                           \
            delete m_ptr;           \
            m_ptr = ptr;            \
        }                           \
    }                               \
                                    \
    T *release()                    \
    {                               \
        T *ptr = m_ptr;             \
        m_ptr = NULL;               \
        return ptr;                 \
    }                               \
                                    \
    T & operator*() const           \
    {                               \
        wxASSERT(m_ptr != NULL);    \
        return *m_ptr;              \
    }                               \
                                    \
    T * operator->() const          \
    {                               \
        wxASSERT(m_ptr != NULL);    \
        return m_ptr;               \
    }                               \
                                    \
    T * get() const                 \
    {                               \
        return m_ptr;               \
    }                               \
                                    \
    void swap(name & ot)            \
    {                               \
        T * tmp = ot.m_ptr;         \
        ot.m_ptr = m_ptr;           \
        m_ptr = tmp;                \
    }                               \
};

#define wxDEFINE_SCOPED_PTR(T, name)\
name::~name()                       \
{                                   \
    wxCHECKED_DELETE(m_ptr);        \
}

// this macro can be used for the most common case when you want to declare and
// define the scoped pointer at the same time and want to use the standard
// naming convention: auto pointer to Foo is called FooPtr
#define wxDEFINE_SCOPED_PTR_TYPE(T)                                           \
    wxDECLARE_SCOPED_PTR(T, T ## Ptr);                                        \
    wxDEFINE_SCOPED_PTR(T, T ## Ptr)

// the same but for arrays instead of simple pointers
#define wxDECLARE_SCOPED_ARRAY(T, name)\
class name                          \
{                                   \
private:                            \
    T * m_ptr;                      \
    name(name const &);             \
    name & operator=(name const &); \
                                    \
public:                             \
    wxEXPLICIT name(T * p = NULL) : m_ptr(p) \
    {}                              \
                                    \
    ~name();                        \
    void reset(T * p = NULL);       \
                                    \
    T & operator[](long int i) const\
    {                               \
        wxASSERT(m_ptr != NULL);    \
        wxASSERT(i >= 0);           \
        return m_ptr[i];            \
    }                               \
                                    \
    T * get() const                 \
    {                               \
        return m_ptr;               \
    }                               \
                                    \
    void swap(name & ot)            \
    {                               \
        T * tmp = ot.m_ptr;         \
        ot.m_ptr = m_ptr;           \
        m_ptr = tmp;                \
    }                               \
};

#define wxDEFINE_SCOPED_ARRAY(T, name)  \
name::~name()                           \
{                                       \
    wxCHECKED_DELETE_ARRAY(m_ptr);      \
}                                       \
void name::reset(T * p){                \
    if (m_ptr != p)                     \
    {                                   \
       wxCHECKED_DELETE_ARRAY(m_ptr);   \
       m_ptr = p;                       \
    }                                   \
}

#endif // __WX_SCOPED_POINTER__

