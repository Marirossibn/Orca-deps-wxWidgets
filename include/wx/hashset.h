/////////////////////////////////////////////////////////////////////////////
// Name:        wx/hashset.h
// Purpose:     wxHashSet class
// Author:      Mattia Barbon
// Modified by:
// Created:     11/08/2003
// RCS-ID:      $Id$
// Copyright:   (c) Mattia Barbon
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_HASHSET_H_
#define _WX_HASHSET_H_

#include "wx/hashmap.h"

#if wxUSE_STL && defined(HAVE_STL_HASH_MAP)

#if defined(HAVE_EXT_HASH_MAP)
    #include <ext/hash_set>
#elif defined(HAVE_HASH_MAP)
    #include <hash_set>
#endif

#define _WX_DECLARE_HASH_SET( KEY_T, HASH_T, KEY_EQ_T, CLASSNAME, CLASSEXP )\
    typedef WX_HASH_MAP_NAMESPACE::hash_set< KEY_T, HASH_T, KEY_EQ_T > CLASSNAME;

#else // !wxUSE_STL || !defined(HAVE_STL_HASH_MAP)

// this is a complex way of defining an easily inlineable identity function...
#define _WX_DECLARE_HASH_SET_KEY_EX( KEY_T, CLASSNAME, CLASSEXP )            \
CLASSEXP CLASSNAME                                                           \
{                                                                            \
    typedef KEY_T key_type;                                                  \
    typedef const key_type const_key_type;                                   \
    typedef const_key_type& const_key_reference;                             \
public:                                                                      \
    CLASSNAME() { }                                                          \
    const_key_reference operator()( const_key_reference key ) const          \
        { return key; }                                                      \
                                                                             \
    /* the dummy assignment operator is needed to suppress compiler */       \
    /* warnings from hash table class' operator=(): gcc complains about */   \
    /* "statement with no effect" without it */                              \
    CLASSNAME& operator=(const CLASSNAME&) { return *this; }                 \
};

#define _WX_DECLARE_HASH_SET( KEY_T, HASH_T, KEY_EQ_T, CLASSNAME, CLASSEXP )\
_WX_DECLARE_HASH_SET_KEY_EX( KEY_T, CLASSNAME##_wxImplementation_KeyEx, CLASSEXP ) \
_WX_DECLARE_HASHTABLE( KEY_T, KEY_T, HASH_T, CLASSNAME##_wxImplementation_KeyEx, KEY_EQ_T, CLASSNAME##_wxImplementation_HashTable, CLASSEXP, grow_lf70, never_shrink ) \
CLASSEXP CLASSNAME:public CLASSNAME##_wxImplementation_HashTable             \
{                                                                            \
public:                                                                      \
    wxEXPLICIT CLASSNAME( size_type hint = 100, hasher hf = hasher(),        \
                          key_equal eq = key_equal() )                       \
        : CLASSNAME##_wxImplementation_HashTable( hint, hf, eq,              \
                      CLASSNAME##_wxImplementation_KeyEx() ) {}              \
                                                                             \
    void insert( const key_type& key )                                       \
    {                                                                        \
        GetOrCreateNode( key );                                              \
    }                                                                        \
                                                                             \
    const_iterator find( const const_key_type& key ) const                   \
    {                                                                        \
        return const_iterator( GetNode( key ), this );                       \
    }                                                                        \
                                                                             \
    iterator find( const const_key_type& key )                               \
    {                                                                        \
        return iterator( GetNode( key ), this );                             \
    }                                                                        \
                                                                             \
    size_type erase( const key_type& k )                                     \
        { return CLASSNAME##_wxImplementation_HashTable::erase( k ); }       \
    void erase( const iterator& it ) { erase( *it ); }                       \
    void erase( const const_iterator& it ) { erase( *it ); }                 \
                                                                             \
    /* count() == 0 | 1 */                                                   \
    size_type count( const const_key_type& key )                             \
        { return GetNode( key ) ? 1 : 0; }                                   \
}

#endif // !wxUSE_STL || !defined(HAVE_STL_HASH_MAP)

// these macros are to be used in the user code
#define WX_DECLARE_HASH_SET( KEY_T, HASH_T, KEY_EQ_T, CLASSNAME) \
    _WX_DECLARE_HASH_SET( KEY_T, HASH_T, KEY_EQ_T, CLASSNAME, class )

// and these do exactly the same thing but should be used inside the
// library
#define WX_DECLARE_HASH_SET_WITH_DECL( KEY_T, HASH_T, KEY_EQ_T, CLASSNAME, DECL) \
    _WX_DECLARE_HASH_SET( KEY_T, HASH_T, KEY_EQ_T, CLASSNAME, DECL )

#define WX_DECLARE_EXPORTED_HASH_SET( KEY_T, HASH_T, KEY_EQ_T, CLASSNAME) \
    WX_DECLARE_HASH_SET_WITH_DECL( KEY_T, HASH_T, KEY_EQ_T, \
                                   CLASSNAME, class WXDLLEXPORT )

// delete all hash elements
//
// NB: the class declaration of the hash elements must be visible from the
//     place where you use this macro, otherwise the proper destructor may not
//     be called (a decent compiler should give a warning about it, but don't
//     count on it)!
#define WX_CLEAR_HASH_SET(type, hashset)                                     \
    WX_CLEAR_HASH_MAP(type, hashset)

#endif // _WX_HASHSET_H_
