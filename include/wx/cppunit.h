/////////////////////////////////////////////////////////////////////////////
// Name:        wx/cppunit.h
// Purpose:     wrapper header for CppUnit headers
// Author:      Vadim Zeitlin
// Created:     15.02.04
// RCS-ID:      $Id$
// Copyright:   (c) 2004 Vadim Zeitlin
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_CPPUNIT_H_
#define _WX_CPPUNIT_H_

///////////////////////////////////////////////////////////////////////////////
// using CPPUNIT_TEST() macro results in this warning, disable it as there is
// no other way to get rid of it and it's not very useful anyhow
#ifdef __VISUALC__
    // typedef-name 'foo' used as synonym for class-name 'bar'
    #pragma warning(disable:4097)

    // unreachable code: we don't care about warnings in CppUnit headers
    #pragma warning(disable:4702)

    // 'id': identifier was truncated to 'num' characters in the debug info
    #pragma warning(disable:4786)
#endif // __VISUALC__

#ifdef __BORLANDC__
    #pragma warn -8022
#endif


///////////////////////////////////////////////////////////////////////////////
// Include all needed cppunit headers.
//

#include "wx/beforestd.h"
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include "wx/afterstd.h"


///////////////////////////////////////////////////////////////////////////////
// Set of helpful test macros.
//

// Base macro for wrapping CPPUNIT_TEST macros and so making them conditional
// tests, meaning that the test only get registered and thus run when a given
// runtime condition is true. 
// In case the condition is evaluated as false a skip message is logged
// (the message will only be shown in verbose mode).
#define WXTEST_ANY_WITH_CONDITION(suiteName, Condition, testMethod, anyTest) \
    if (Condition) \
        { anyTest; } \
    else \
        wxLogInfo(wxString::Format(_T("skipping: %s.%s\n  reason: %s equals false\n"), \
                                    wxString(suiteName, wxConvUTF8).c_str(), \
                                    wxString(#testMethod, wxConvUTF8).c_str(), \
                                    wxString(#Condition, wxConvUTF8).c_str()))

// Conditional CPPUNIT_TEST macro.
#define WXTEST_WITH_CONDITION(suiteName, Condition, testMethod) \
    WXTEST_ANY_WITH_CONDITION(suiteName, Condition, testMethod, CPPUNIT_TEST(testMethod))
// Conditional CPPUNIT_TEST_FAIL macro.
#define WXTEST_FAIL_WITH_CONDITION(suiteName, Condition, testMethod) \
    WXTEST_ANY_WITH_CONDITION(suiteName, Condition, testMethod, CPPUNIT_TEST_FAIL(testMethod))


///////////////////////////////////////////////////////////////////////////////
// Some more compiler warning tweaking and auto linking.
//

#ifdef __BORLANDC__
    #pragma warn .8022
#endif

#ifdef _MSC_VER
  #pragma warning(default:4702)
#endif // _MSC_VER

// for VC++ automatically link in cppunit library
#ifdef __VISUALC__
  #ifdef NDEBUG
    #pragma comment(lib, "cppunit.lib")
  #else // Debug
    #pragma comment(lib, "cppunitd.lib")
  #endif // Release/Debug
#endif

#endif // _WX_CPPUNIT_H_

