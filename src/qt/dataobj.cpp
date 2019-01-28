/////////////////////////////////////////////////////////////////////////////
// Name:        src/qt/dataobj.cpp
// Author:      Peter Most
// Copyright:   (c) Peter Most
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/qt/private/converter.h"
#include "wx/qt/private/utils.h"
#include "wx/dataobj.h"

#include <QtCore/QMimeData>

namespace
{
    wxString DataFormatIdToMimeType(wxDataFormatId formatId)
    {
        switch ( formatId )
        {
            case wxDF_TEXT: return "text/plain";
            case wxDF_BITMAP: return "image/bmp";
            case wxDF_TIFF: return "image/tiff";
            case wxDF_WAVE: return "audio/x-wav";
            case wxDF_UNICODETEXT: return "text/plain";
            case wxDF_HTML: return "text/html";
            case wxDF_METAFILE:
            case wxDF_SYLK:
            case wxDF_DIF:
            case wxDF_OEMTEXT:
            case wxDF_DIB:
            case wxDF_PALETTE:
            case wxDF_PENDATA:
            case wxDF_RIFF:
            case wxDF_ENHMETAFILE:
            case wxDF_FILENAME:
            case wxDF_LOCALE:
            case wxDF_PRIVATE:
            case wxDF_INVALID:
            case wxDF_MAX:
            default:
                return "";
        }
    }
}

wxDataFormat::wxDataFormat(wxDataFormatId formatId)
{
    SetType(formatId);
}

wxDataFormat::wxDataFormat(const wxString &id)
{
    SetId(id);
}

const wxString& wxDataFormat::GetMimeType() const
{
    return m_mimeType;
}

void wxDataFormat::SetMimeType(const wxString& mimeType)
{
    m_mimeType = mimeType;
    m_formatId = wxDF_INVALID;
}

void wxDataFormat::SetId(const wxString& id)
{
    SetMimeType(id);
}

const wxString& wxDataFormat::GetId() const
{
    return m_mimeType;
}

wxDataFormatId wxDataFormat::GetType() const
{
    return m_formatId;
}

void wxDataFormat::SetType(wxDataFormatId formatId)
{
    m_mimeType = DataFormatIdToMimeType(formatId);
    m_formatId = formatId;
}

bool wxDataFormat::operator==(wxDataFormatId format) const
{
    return m_mimeType == DataFormatIdToMimeType(format)
        && m_formatId == format;
}

bool wxDataFormat::operator!=(wxDataFormatId format) const
{
    return !operator==(format);
}

bool wxDataFormat::operator==(const wxDataFormat& format) const
{
    return m_mimeType == format.m_mimeType
        && m_formatId == format.m_formatId;
}

bool wxDataFormat::operator!=(const wxDataFormat& format) const
{
    return !operator==(format);
}

//#############################################################################

wxDataObject::wxDataObject()
{
    m_qtMimeData = new QMimeData;
}

wxDataObject::~wxDataObject()
{
    delete m_qtMimeData;
}

bool wxDataObject::IsSupportedFormat(const wxDataFormat& format, Direction) const
{
    return wxDataFormat(format) != wxDF_INVALID;
}
wxDataFormat wxDataObject::GetPreferredFormat(Direction) const
{
    /* formats are in order of preference */
    if (m_qtMimeData->formats().count())
        return m_qtMimeData->formats().first();

    return wxDataFormat();
}

size_t wxDataObject::GetFormatCount(Direction) const
{
    return m_qtMimeData->formats().count();
}

void wxDataObject::GetAllFormats(wxDataFormat *formats, Direction) const
{
    int i = 0;
    foreach (QString format, m_qtMimeData->formats())
    {
        formats[i] = format;
        i++;
    }
}

size_t wxDataObject::GetDataSize(const wxDataFormat& format) const
{
    return  m_qtMimeData->data( wxQtConvertString(format.m_MimeType) ).count();
}

bool wxDataObject::GetDataHere(const wxDataFormat& format, void *buf) const
{
    if (!m_qtMimeData->hasFormat(wxQtConvertString(format.m_MimeType)))
        return false;

    QByteArray data = m_qtMimeData->data( wxQtConvertString(format.m_MimeType) ).data();
    memcpy(buf, data.constData(), data.size());
    return true;
}

bool wxDataObject::SetData(const wxDataFormat& format, size_t len, const void * buf)
{
    QByteArray bytearray((const char*)buf, len);
    m_qtMimeData->setData(wxQtConvertString(format.m_MimeType), bytearray);

    return true;
}

wxBitmapDataObject::wxBitmapDataObject()
{
}

wxBitmapDataObject::wxBitmapDataObject( const wxBitmap &WXUNUSED(bitmap) )
{
}

wxFileDataObject::wxFileDataObject()
{
}
 
void wxFileDataObject::AddFile( const wxString &WXUNUSED(filename) )
{

}
