/////////////////////////////////////////////////////////////////////////////
// Name:        wx/xtiprop.h
// Purpose:     XTI properties
// Author:      Stefan Csomor
// Modified by: Francesco Montorsi
// Created:     27/07/03
// RCS-ID:      $Id: xti.h 47299 2007-07-10 15:58:27Z FM $
// Copyright:   (c) 1997 Julian Smart
//              (c) 2003 Stefan Csomor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _XTIPROP_H_
#define _XTIPROP_H_

#include "wx/defs.h"

#if wxUSE_EXTENDED_RTTI

#include "wx/string.h"
#include "wx/variant.h"
#include "wx/intl.h"
#include "wx/log.h"
#include "wx/xtitypes.h"

class WXDLLIMPEXP_BASE wxObject;
class WXDLLIMPEXP_BASE wxClassInfo;
class WXDLLIMPEXP_BASE wxDynamicClassInfo;
class WXDLLIMPEXP_BASE wxHashTable;
class WXDLLIMPEXP_BASE wxHashTable_Node;
class WXDLLIMPEXP_BASE wxObjectRefData;
class WXDLLIMPEXP_BASE wxEvent;
class WXDLLIMPEXP_BASE wxEvtHandler;

// ----------------------------------------------------------------------------
// Property Accessors
//
// wxPropertySetter/Getter/CollectionGetter/CollectionAdder are all property
// accessors which are managed by wxPropertyAccessor class which in turn is
// handled by wxPropertyInfo.
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_BASE wxPropertySetter
{
public:
    wxPropertySetter( const wxString name ) { m_name = name; }
    virtual ~wxPropertySetter() {}

    virtual void Set( wxObject *object, const wxVariantBase &variantValue ) const = 0;
    const wxString& GetName() const { return m_name; }

private:
    wxString m_name;
};

class WXDLLIMPEXP_BASE wxPropertyGetter
{
public:
    wxPropertyGetter( const wxString name ) { m_name = name; }
    virtual ~wxPropertyGetter() {}

    virtual void Get( const wxObject *object, wxVariantBase& result) const = 0;
    const wxString& GetName() const { return m_name; }

private:
    wxString m_name;
};

class WXDLLIMPEXP_BASE wxPropertyCollectionGetter
{
public:
    wxPropertyCollectionGetter( const wxString name ) { m_name = name; }
    virtual ~wxPropertyCollectionGetter() {}

    virtual void Get( const wxObject *object, wxVariantBaseArray& result) const = 0;
    const wxString& GetName() const { return m_name; }

private:
    wxString m_name;
};

template<typename coll_t> void WXDLLIMPEXP_BASE \
    wxCollectionToVariantArray( const coll_t& coll, wxVariantBaseArray& result );

class WXDLLIMPEXP_BASE wxPropertyCollectionAdder
{
public:
    wxPropertyCollectionAdder( const wxString name ) { m_name = name; }
    virtual ~wxPropertyCollectionAdder() {}

    virtual void Add( wxObject *object, const wxVariantBase &variantValue ) const= 0;
    const wxString& GetName() const { return m_name; }

private:
    wxString m_name;
};

#define wxPROPERTY_SETTER( property, Klass, valueType, setterMethod )            \
class wxPropertySetter##property : public wxPropertySetter                              \
{                                                                       \
public:                                                                 \
    wxINFUNC_CLASS_TYPE_FIX(Klass)                                      \
    wxPropertySetter##property() : wxPropertySetter( wxT(#setterMethod) ) {}            \
    virtual ~wxPropertySetter##property() {}                                    \
                                                                        \
    void Set( wxObject *object, const wxVariantBase &variantValue ) const  \
    {                                                                   \
        Klass *obj = dynamic_cast<Klass*>(object);                      \
        if ( variantValue.wxTEMPLATED_MEMBER_CALL(HasData, valueType) ) \
            obj->setterMethod(variantValue.wxTEMPLATED_MEMBER_CALL(Get, valueType));   \
        else                                                                            \
            obj->setterMethod(*variantValue.wxTEMPLATED_MEMBER_CALL(Get, valueType*)); \
    }                                                                                   \
};

#define wxPROPERTY_GETTER( property, Klass, valueType, gettermethod )           \
class wxPropertyGetter##property : public wxPropertyGetter                              \
{                                                                       \
public:                                                                 \
    wxINFUNC_CLASS_TYPE_FIX(Klass)                                      \
    wxPropertyGetter##property() : wxPropertyGetter( wxT(#gettermethod) ) {}            \
    virtual ~wxPropertyGetter##property() {}                                    \
                                                                        \
    void Get( const wxObject *object, wxVariantBase &result) const        \
    {                                                                   \
        const Klass *obj = dynamic_cast<const Klass*>(object);          \
        result = wxVariantBase( obj->gettermethod() );                     \
    }                                                                   \
};

#define wxPROPERTY_COLLECTION_ADDER( property, Klass, valueType, addermethod )             \
class wxPropertyCollectionAdder##property : public wxPropertyCollectionAdder                                \
{                                                                       \
public:                                                                 \
    wxINFUNC_CLASS_TYPE_FIX(Klass)                                      \
    wxPropertyCollectionAdder##property() : wxPropertyCollectionAdder( wxT(#addermethod) ) {}               \
    virtual ~wxPropertyCollectionAdder##property() {}                                     \
                                                                        \
    void Add( wxObject *object, const wxVariantBase &variantValue ) const  \
    {                                                                   \
        Klass *obj = dynamic_cast<Klass*>(object);                      \
        if ( variantValue.wxTEMPLATED_MEMBER_CALL(HasData, valueType) ) \
            obj->addermethod(variantValue.wxTEMPLATED_MEMBER_CALL(Get, valueType));    \
        else                                                                            \
            obj->addermethod(*variantValue.wxTEMPLATED_MEMBER_CALL(Get, valueType*));  \
    }                                                                                   \
};

#define wxPROPERTY_COLLECTION_GETTER( property, Klass, valueType, gettermethod )    \
class wxPropertyCollectionGetter##property : public wxPropertyCollectionGetter              \
{                                                                           \
public:                                                                     \
    wxINFUNC_CLASS_TYPE_FIX(Klass)                                          \
    wxPropertyCollectionGetter##property() : wxPropertyCollectionGetter( wxT(#gettermethod) ) {} \
    virtual ~wxPropertyCollectionGetter##property() {}                              \
                                                                            \
    void Get( const wxObject *object, wxVariantBaseArray &result) const       \
    {                                                                       \
        const Klass *obj = dynamic_cast<const Klass*>(object);              \
        wxCollectionToVariantArray( obj->gettermethod(), result );         \
    }                                                                       \
};

class WXDLLIMPEXP_BASE wxPropertyAccessor
{
public:
    wxPropertyAccessor( wxPropertySetter *setter, wxPropertyGetter *getter, 
                        wxPropertyCollectionAdder *adder, wxPropertyCollectionGetter *collectionGetter )
    { m_setter = setter; m_getter = getter; m_adder = adder; 
      m_collectionGetter = collectionGetter; }

    virtual ~wxPropertyAccessor() {}

    // Setting a simple property (non-collection)
    virtual void SetProperty(wxObject *object, const wxVariantBase &value) const
    { 
        if ( m_setter ) 
            m_setter->Set( object, value ); 
        else 
            wxLogError( _("SetProperty called w/o valid setter") ); 
    }

    // Getting a simple property (non-collection)
    virtual void GetProperty(const wxObject *object, wxVariantBase &result) const
    { 
        if ( m_getter ) 
            m_getter->Get( object, result ); 
        else 
            wxLogError( _("GetProperty called w/o valid getter") ); 
    }

    // Adding an element to a collection property
    virtual void AddToPropertyCollection(wxObject *object, const wxVariantBase &value) const
    { 
        if ( m_adder ) 
            m_adder->Add( object, value ); 
        else 
            wxLogError( _("AddToPropertyCollection called w/o valid adder") ); 
    }

    // Getting a collection property
    virtual void GetPropertyCollection( const wxObject *obj, wxVariantBaseArray &result) const
    { 
        if ( m_collectionGetter ) 
            m_collectionGetter->Get( obj, result); 
        else 
            wxLogError( _("GetPropertyCollection called w/o valid collection getter") ); 
    }

    virtual bool HasSetter() const { return m_setter != NULL; }
    virtual bool HasCollectionGetter() const { return m_collectionGetter != NULL; }
    virtual bool HasGetter() const { return m_getter != NULL; }
    virtual bool HasAdder() const { return m_adder != NULL; }

    virtual const wxString& GetCollectionGetterName() const
        { return m_collectionGetter->GetName(); }
    virtual const wxString&  GetGetterName() const
        { return m_getter->GetName(); }
    virtual const wxString& GetSetterName() const
        { return m_setter->GetName(); }
    virtual const wxString& GetAdderName() const
        { return m_adder->GetName(); }

protected:
    wxPropertySetter *m_setter;
    wxPropertyCollectionAdder *m_adder;
    wxPropertyGetter *m_getter;
    wxPropertyCollectionGetter* m_collectionGetter;
};

class WXDLLIMPEXP_BASE wxGenericPropertyAccessor : public wxPropertyAccessor
{
public:
    wxGenericPropertyAccessor( const wxString &propName );
    virtual ~wxGenericPropertyAccessor();

    void RenameProperty( const wxString& WXUNUSED_UNLESS_DEBUG(oldName),
        const wxString& newName )
    {
        wxASSERT( oldName == m_propertyName ); m_propertyName = newName;
    }

    virtual bool HasSetter() const { return true; }
    virtual bool HasGetter() const { return true; }
    virtual bool HasAdder() const { return false; }
    virtual bool HasCollectionGetter() const { return false; }

    virtual const wxString&  GetGetterName() const
        { return m_getterName; }
    virtual const wxString& GetSetterName() const
        { return m_setterName; }

    virtual void SetProperty(wxObject *object, const wxVariantBase &value) const;
    virtual void GetProperty(const wxObject *object, wxVariantBase &value) const;

    // Adding an element to a collection property
    virtual void AddToPropertyCollection(wxObject *WXUNUSED(object), 
                                         const wxVariantBase &WXUNUSED(value)) const
    { 
        wxLogError( _("AddToPropertyCollection called on a generic accessor") ); 
    }

    // Getting a collection property
    virtual void GetPropertyCollection( const wxObject *WXUNUSED(obj), 
                                        wxVariantBaseArray &WXUNUSED(result)) const
    { 
        wxLogError ( _("GetPropertyCollection called on a generic accessor") ); 
    }

private:
    struct wxGenericPropertyAccessorInternal;
    wxGenericPropertyAccessorInternal* m_data;
    wxString m_propertyName;
    wxString m_setterName;
    wxString m_getterName;
};

typedef long wxPropertyInfoFlags;
enum 
{
    // will be removed in future releases
    wxPROP_DEPRECATED       = 0x00000001,

    // object graph property, will be streamed with priority (after constructor properties)
    wxPROP_OBJECT_GRAPH     = 0x00000002,

    // this will only be streamed out and in as enum/set, the internal representation 
    // is still a long
    wxPROP_ENUM_STORE_LONG  = 0x00000004,

    // don't stream out this property, needed eg to avoid streaming out children 
    // that are always created by their parents
    wxPROP_DONT_STREAM      = 0x00000008
};


// ----------------------------------------------------------------------------
// Property Support
//
// wxPropertyInfo is used to inquire of the property by name.  It doesn't
// provide access to the property, only information about it.  If you
// want access, look at wxPropertyAccessor.
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_BASE wxPropertyInfo
{
    friend class WXDLLIMPEXP_BASE wxDynamicClassInfo;

public:
    wxPropertyInfo(wxPropertyInfo* &iter,
                   wxClassInfo* itsClass,
                   const wxString& name,
                   const wxString& typeName,
                   wxPropertyAccessor *accessor,
                   wxVariantBase dv,
                   wxPropertyInfoFlags flags = 0,
                   const wxString& helpString = wxEmptyString,
                   const wxString& groupString = wxEmptyString) :
                   m_itsClass(itsClass),
           m_name(name),
           m_typeInfo(NULL),
           m_typeName(typeName),
           m_collectionElementTypeInfo(NULL),
           m_accessor(accessor),
           m_defaultValue(dv),
           m_flags(flags),
           m_helpString(helpString),
           m_groupString(groupString)
       {
           Insert(iter);
       }

#if wxUSE_UNICODE
    wxPropertyInfo(wxPropertyInfo* &iter,
                   wxClassInfo* itsClass,
                   const wxString& name,
                   const char* typeName,
                   wxPropertyAccessor *accessor,
                   wxVariantBase dv,
                   wxPropertyInfoFlags flags = 0,
                   const wxString& helpString = wxEmptyString,
                   const wxString& groupString = wxEmptyString) :
                   m_itsClass(itsClass),
           m_name(name),
           m_typeInfo(NULL),
           m_typeName(wxString::FromAscii(typeName)),
           m_collectionElementTypeInfo(NULL),
           m_accessor(accessor),
           m_defaultValue(dv),
           m_flags(flags),
           m_helpString(helpString),
           m_groupString(groupString)
       {
           Insert(iter);
       }
#endif
    wxPropertyInfo(wxPropertyInfo* &iter,
                   wxClassInfo* itsClass,
                   const wxString& name,
                   wxEventSourceTypeInfo* type,
                   wxPropertyAccessor *accessor,
                   wxVariantBase dv,
                   wxPropertyInfoFlags flags = 0,
                   const wxString& helpString = wxEmptyString,
                   const wxString& groupString = wxEmptyString) :
           m_itsClass(itsClass),
           m_name(name),
           m_typeInfo(type),
           m_collectionElementTypeInfo(NULL),
           m_accessor(accessor),
           m_defaultValue(dv),
           m_flags(flags),
           m_helpString(helpString),
           m_groupString(groupString)
       {
           Insert(iter);
       }

    wxPropertyInfo(wxPropertyInfo* &iter,
                    wxClassInfo* itsClass, const wxString& name,
                    const wxString& collectionTypeName,
                    const wxString& elementTypeName,
                    wxPropertyAccessor *accessor,
                    wxPropertyInfoFlags flags = 0,
                    const wxString& helpString = wxEmptyString,
                    const wxString& groupString = wxEmptyString) :
        m_itsClass(itsClass),
        m_name(name),
        m_typeInfo(NULL),
        m_typeName(collectionTypeName),
        m_collectionElementTypeInfo(NULL),
        m_collectionElementTypeName(elementTypeName),
        m_accessor(accessor),
        m_flags(flags),
        m_helpString(helpString),
        m_groupString(groupString)
    {
        Insert(iter);
    }

#if wxUSE_UNICODE
    wxPropertyInfo(wxPropertyInfo* &iter,
            wxClassInfo* itsClass, const wxString& name,
            const char* collectionTypeName,
            const char* elementTypeName,
            wxPropertyAccessor *accessor,
            wxPropertyInfoFlags flags = 0,
            const wxString& helpString = wxEmptyString,
            const wxString& groupString = wxEmptyString) :
        m_itsClass(itsClass),
        m_name(name),
        m_typeInfo(NULL),
        m_typeName(wxString::FromAscii(collectionTypeName)),
        m_collectionElementTypeInfo(NULL),
        m_collectionElementTypeName(wxString::FromAscii(elementTypeName)),
        m_accessor(accessor),
        m_flags(flags),
        m_helpString(helpString),
        m_groupString(groupString)
    {
        Insert(iter);
    }
#endif
    ~wxPropertyInfo()
        { Remove(); }

    // return the class this property is declared in
    const wxClassInfo*  GetDeclaringClass() const { return m_itsClass; }

    // return the name of this property
    const wxString&     GetName() const { return m_name; }

    // returns the flags of this property
    wxPropertyInfoFlags GetFlags() const { return m_flags; }

    // returns the short help string of this property
    const wxString&     GetHelpString() const { return m_helpString; }

    // returns the group string of this property
    const wxString&     GetGroupString() const { return m_groupString; }

    // return the element type info of this property (for collections, otherwise NULL)
    const wxTypeInfo *  GetCollectionElementTypeInfo() const
    {
        if ( m_collectionElementTypeInfo == NULL )
            m_collectionElementTypeInfo = wxTypeInfo::FindType(m_collectionElementTypeName);
        return m_collectionElementTypeInfo;
    }

    // return the type info of this property
    const wxTypeInfo *  GetTypeInfo() const
    {
        if ( m_typeInfo == NULL )
            m_typeInfo = wxTypeInfo::FindType(m_typeName);
        return m_typeInfo;
    }

    // return the accessor for this property
    wxPropertyAccessor* GetAccessor() const { return m_accessor; }

    // returns NULL if this is the last property of this class
    wxPropertyInfo*     GetNext() const { return m_next; }

    // returns the default value of this property, its kind may be wxT_VOID if it is not valid
    wxVariantBase          GetDefaultValue() const { return m_defaultValue; }

private:

    // inserts this property at the end of the linked chain which begins
    // with "iter" property.
    void Insert(wxPropertyInfo* &iter);

    // removes this property from the linked chain of the m_itsClass properties.
    void Remove();

    wxClassInfo*        m_itsClass;
    wxString            m_name;
    mutable wxTypeInfo* m_typeInfo;
    wxString            m_typeName;
    mutable wxTypeInfo* m_collectionElementTypeInfo;
    wxString            m_collectionElementTypeName;
    wxPropertyAccessor* m_accessor;
    wxVariantBase          m_defaultValue;
    wxPropertyInfoFlags m_flags;
    wxString            m_helpString;
    wxString            m_groupString;
    wxPropertyInfo*     m_next;

    // FIXME: what's this comment about??
    // string representation of the default value
    // to be assigned by the designer to the property
    // when the component is dropped on the container.
};

WX_DECLARE_STRING_HASH_MAP_WITH_DECL( wxPropertyInfo*, wxPropertyInfoMap, 
                                      class WXDLLIMPEXP_BASE );

#define wxBEGIN_PROPERTIES_TABLE(theClass)                      \
    wxPropertyInfo *theClass::GetPropertiesStatic()             \
    {                                                           \
        typedef theClass class_t;                               \
        static wxPropertyInfo* first = NULL;

#define wxEND_PROPERTIES_TABLE() \
    return first; }

#define wxHIDE_PROPERTY( pname )                                                      \
    static wxPropertyInfo _propertyInfo##pname( first, class_t::GetClassInfoStatic(), \
            wxT(#pname), typeid(void).name(), NULL, wxVariantBase(), wxPROP_DONT_STREAM, \
            wxEmptyString, wxEmptyString );

#define wxPROPERTY( pname, type, setter, getter, defaultValue, flags, help, group)    \
    wxPROPERTY_SETTER( pname, class_t, type, setter )                                 \
    static wxPropertySetter##pname _setter##pname;                                    \
    wxPROPERTY_GETTER( pname, class_t, type, getter )                                 \
    static wxPropertyGetter##pname _getter##pname;                                    \
    static wxPropertyAccessor _accessor##pname( &_setter##pname,                      \
                                                &_getter##pname, NULL, NULL );        \
    static wxPropertyInfo _propertyInfo##pname( first, class_t::GetClassInfoStatic(), \
            wxT(#pname), typeid(type).name(), &_accessor##pname,                      \
            wxVariantBase(defaultValue), flags, group, help );

#define wxPROPERTY_FLAGS( pname, flags, type, setter, getter,defaultValue,            \
                          pflags, help, group)                                        \
    wxPROPERTY_SETTER( pname, class_t, type, setter )                                 \
    static wxPropertySetter##pname _setter##pname;                                    \
    wxPROPERTY_GETTER( pname, class_t, type, getter )                                 \
    static wxPropertyGetter##pname _getter##pname;                                    \
    static wxPropertyAccessor _accessor##pname( &_setter##pname,                      \
                                                &_getter##pname, NULL, NULL );        \
    static wxPropertyInfo _propertyInfo##pname( first, class_t::GetClassInfoStatic(), \
            wxT(#pname), typeid(flags).name(), &_accessor##pname,                     \
            wxVariantBase(defaultValue), wxPROP_ENUM_STORE_LONG | pflags, help, group );

#define wxREADONLY_PROPERTY( pname, type, getter,defaultValue, flags, help, group)    \
    wxPROPERTY_GETTER( pname, class_t, type, getter )                                 \
    static wxPropertyGetter##pname _getter##pname;                                    \
    static wxPropertyAccessor _accessor##pname( NULL, &_getter##pname, NULL, NULL );  \
    static wxPropertyInfo _propertyInfo##pname( first, class_t::GetClassInfoStatic(), \
            wxT(#pname), typeid(type).name(),&_accessor##pname,                       \
            wxVariantBase(defaultValue), flags, help, group );

#define wxREADONLY_PROPERTY_FLAGS( pname, flags, type, getter,defaultValue,           \
                                   pflags, help, group)                               \
    wxPROPERTY_GETTER( pname, class_t, type, getter )                                 \
    static wxPropertyGetter##pname _getter##pname;                                    \
    static wxPropertyAccessor _accessor##pname( NULL, &_getter##pname, NULL, NULL );  \
    static wxPropertyInfo _propertyInfo##pname( first, class_t::GetClassInfoStatic(), \
            wxT(#pname), typeid(flags).name(),&_accessor##pname,                      \
            wxVariantBase(defaultValue), wxPROP_ENUM_STORE_LONG | pflags, help, group );

#define wxPROPERTY_COLLECTION( pname, colltype, addelemtype, adder, getter,           \
                               flags, help, group )                                   \
    wxPROPERTY_COLLECTION_ADDER( pname, class_t, addelemtype, adder )                 \
    static wxPropertyCollectionAdder##pname _adder##pname;                            \
    wxPROPERTY_COLLECTION_GETTER( pname, class_t, colltype, getter )                  \
    static wxPropertyCollectionGetter##pname _collectionGetter##pname;                \
    static wxPropertyAccessor _accessor##pname( NULL, NULL,&_adder##pname,            \
                                                &_collectionGetter##pname );          \
    static wxPropertyInfo _propertyInfo##pname( first, class_t::GetClassInfoStatic(), \
            wxT(#pname), typeid(colltype).name(),typeid(addelemtype).name(),          \
            &_accessor##pname, flags, help, group );

#define wxREADONLY_PROPERTY_COLLECTION( pname, colltype, addelemtype, getter,         \
                                        flags, help, group)                           \
    wxPROPERTY_COLLECTION_GETTER( pname, class_t, colltype, getter )                  \
    static wxPropertyCollectionGetter##pname _collectionGetter##pname;                \
    static wxPropertyAccessor _accessor##pname( NULL, NULL, NULL,                     \
                                                &_collectionGetter##pname );          \
    static wxPropertyInfo _propertyInfo##pname( first,class_t::GetClassInfoStatic(),  \
        wxT(#pname), typeid(colltype).name(),typeid(addelemtype).name(),              \
        &_accessor##pname, flags, help, group  );

#define wxEVENT_PROPERTY( name, eventType, eventClass )                               \
    static wxEventSourceTypeInfo _typeInfo##name( eventType, CLASSINFO( eventClass ) );  \
    static wxPropertyInfo _propertyInfo##name( first,class_t::GetClassInfoStatic(),   \
        wxT(#name), &_typeInfo##name, NULL, wxVariantBase() );

#define wxEVENT_RANGE_PROPERTY( name, eventType, lastEventType, eventClass )          \
    static wxEventSourceTypeInfo _typeInfo##name( eventType, lastEventType,              \
                                               CLASSINFO( eventClass ) );             \
    static wxPropertyInfo _propertyInfo##name( first, class_t::GetClassInfoStatic(),  \
        wxT(#name), &_typeInfo##name, NULL, wxVariantBase() );

// ----------------------------------------------------------------------------
// Implementation Helper for Simple Properties
// ----------------------------------------------------------------------------

#define wxIMPLEMENT_PROPERTY(name, type)                \
private:                                                \
    type m_##name;                                      \
public:                                                 \
  void  Set##name( type const & p) { m_##name = p; }    \
  type const & Get##name() const  { return m_##name; }

#endif      // wxUSE_EXTENDED_RTTI
#endif      // _XTIPROP_H_
