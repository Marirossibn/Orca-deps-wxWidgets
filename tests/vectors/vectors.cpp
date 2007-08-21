///////////////////////////////////////////////////////////////////////////////
// Name:        tests/vectors/vectors.cpp
// Purpose:     wxVector<T> unit test
// Author:      Vaclav Slavik
// Created:     2007-07-07
// RCS-ID:      $Id$
// Copyright:   (c) 2007 Vaclav Slavik
///////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include "testprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif // WX_PRECOMP

#include "wx/vector.h"

// --------------------------------------------------------------------------
// test class
// --------------------------------------------------------------------------

class VectorsTestCase : public CppUnit::TestCase
{
public:
    VectorsTestCase() {}

private:
    CPPUNIT_TEST_SUITE( VectorsTestCase );
        CPPUNIT_TEST( PushPopTest );
        CPPUNIT_TEST( Insert );
        CPPUNIT_TEST( Erase );
        CPPUNIT_TEST( Iterators );
    CPPUNIT_TEST_SUITE_END();

    void PushPopTest();
    void Insert();
    void Erase();
    void Iterators();

    DECLARE_NO_COPY_CLASS(VectorsTestCase)
};

// register in the unnamed registry so that these tests are run by default
CPPUNIT_TEST_SUITE_REGISTRATION( VectorsTestCase );

// also include in it's own registry so that these tests can be run alone
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( VectorsTestCase, "VectorsTestCase" );

void VectorsTestCase::PushPopTest()
{
    wxVector<int> v;

    CPPUNIT_ASSERT( v.size() == 0 );
    v.push_back(1);
    CPPUNIT_ASSERT( v.size() == 1 );
    v.push_back(2);
    CPPUNIT_ASSERT( v.size() == 2 );
    v.push_back(42);
    CPPUNIT_ASSERT( v.size() == 3 );

    CPPUNIT_ASSERT( v[0] == 1 );
    CPPUNIT_ASSERT( v[1] == 2 );
    CPPUNIT_ASSERT( v[2] == 42 );

    v.pop_back();
    CPPUNIT_ASSERT( v.size() == 2 );
    CPPUNIT_ASSERT( v[0] == 1 );
    CPPUNIT_ASSERT( v[1] == 2 );

    v.pop_back();
    CPPUNIT_ASSERT( v.size() == 1 );
    CPPUNIT_ASSERT( v[0] == 1 );

    v.pop_back();
    CPPUNIT_ASSERT( v.size() == 0 );
    CPPUNIT_ASSERT( v.empty() );

    wxVector<char> vEmpty;
}

void VectorsTestCase::Insert()
{
    wxVector<char> v;

    v.insert(v.end(), 'a');
    CPPUNIT_ASSERT( v.size() == 1 );
    CPPUNIT_ASSERT( v[0] == 'a' );

    v.insert(v.end(), 'b');
    CPPUNIT_ASSERT( v.size() == 2 );
    CPPUNIT_ASSERT( v[0] == 'a' );
    CPPUNIT_ASSERT( v[1] == 'b' );

    v.insert(v.begin(), '0');
    CPPUNIT_ASSERT( v.size() == 3 );
    CPPUNIT_ASSERT( v[0] == '0' );
    CPPUNIT_ASSERT( v[1] == 'a' );
    CPPUNIT_ASSERT( v[2] == 'b' );

    v.insert(v.begin() + 2, 'X');
    CPPUNIT_ASSERT( v.size() == 4 );
    CPPUNIT_ASSERT( v[0] == '0' );
    CPPUNIT_ASSERT( v[1] == 'a' );
    CPPUNIT_ASSERT( v[2] == 'X' );
    CPPUNIT_ASSERT( v[3] == 'b' );
}

void VectorsTestCase::Erase()
{
    wxVector<int> v;

    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);
    CPPUNIT_ASSERT( v.size() == 4 );

    v.erase(v.begin(), v.end()-1);
    CPPUNIT_ASSERT( v.size() == 1 );
    CPPUNIT_ASSERT( v[0] == 4 );

    v.clear();
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);
    CPPUNIT_ASSERT( v.size() == 4 );

    v.erase(v.begin());
    CPPUNIT_ASSERT( v.size() == 3 );
    CPPUNIT_ASSERT( v[0] == 2 );
    CPPUNIT_ASSERT( v[1] == 3 );
    CPPUNIT_ASSERT( v[2] == 4 );
}

void VectorsTestCase::Iterators()
{
    wxVector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);

    int value = 1;
    for ( wxVector<int>::iterator i = v.begin(); i != v.end(); ++i, ++value )
    {
        CPPUNIT_ASSERT_EQUAL( value, *i );
    }
}
