/////////////////////////////////////////////////////////////////////////////
// Name:        src/common/sckstrm.cpp
// Purpose:     wxSocket*Stream
// Author:      Guilhem Lavaux
// Modified by:
// Created:     17/07/97
// RCS-ID:      $Id$
// Copyright:   (c)
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_SOCKETS && wxUSE_STREAMS

#ifndef WX_PRECOMP
#endif

#include "wx/stream.h"
#include "wx/socket.h"
#include "wx/sckstrm.h"

// ---------------------------------------------------------------------------
// wxSocketOutputStream
// ---------------------------------------------------------------------------

wxSocketOutputStream::wxSocketOutputStream(wxSocketBase& s)
  : m_o_socket(&s)
{
}

wxSocketOutputStream::~wxSocketOutputStream()
{
}

size_t wxSocketOutputStream::OnSysWrite(const void *buffer, size_t size)
{
    size_t ret = m_o_socket->Write((const char *)buffer, size).LastCount();

    m_lasterror = m_o_socket->Error() ? wxSTREAM_WRITE_ERROR : wxSTREAM_NO_ERROR;

    return ret;
}

// ---------------------------------------------------------------------------
// wxSocketInputStream
// ---------------------------------------------------------------------------

wxSocketInputStream::wxSocketInputStream(wxSocketBase& s)
 : m_i_socket(&s)
{
}

wxSocketInputStream::~wxSocketInputStream()
{
}

size_t wxSocketInputStream::OnSysRead(void *buffer, size_t size)
{
    size_t ret = m_i_socket->Read((char *)buffer, size).LastCount();

    m_lasterror = m_i_socket->Error() ? wxSTREAM_READ_ERROR : wxSTREAM_NO_ERROR;

    return ret;
}

// ---------------------------------------------------------------------------
// wxSocketStream
// ---------------------------------------------------------------------------

wxSocketStream::wxSocketStream(wxSocketBase& s)
  : wxSocketInputStream(s), wxSocketOutputStream(s)
{
}

wxSocketStream::~wxSocketStream()
{
}

#endif
  // wxUSE_STREAMS && wxUSE_SOCKETS
