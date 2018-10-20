///////////////////////////////////////////////////////////////////////////////
// Name:        wx/msw/webrequest_winhttp.h
// Purpose:     wxWebRequest WinHTTP implementation
// Author:      Tobias Taschner
// Created:     2018-10-17
// Copyright:   (c) 2018 wxWidgets development team
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_MSW_WEBREQUEST_WINHTTP_H
#define _WX_MSW_WEBREQUEST_WINHTTP_H

#include "wx/msw/wrapwin.h"
#include <winhttp.h>
#include "wx/buffer.h"

class wxWebSessionWinHTTP;
class wxWebRequestWinHTTP;

class WXDLLIMPEXP_NET wxWebResponseWinHTTP : public wxWebResponse
{
public:
    wxWebResponseWinHTTP(wxWebRequestWinHTTP& request);

    wxInt64 GetContentLength() const wxOVERRIDE { return m_contentLength; }

    wxString GetURL() const wxOVERRIDE;

    wxString GetHeader(const wxString& name) const wxOVERRIDE;

    int GetStatus() const wxOVERRIDE;

    wxString GetStatusText() const wxOVERRIDE;

    wxInputStream* GetStream() const wxOVERRIDE;

    wxString AsString(wxMBConv* conv = NULL) const wxOVERRIDE;

    bool ReadData();

    bool ReportAvailableData(DWORD dataLen);

    void ReportDataComplete();

private:
    wxWebRequestWinHTTP& m_request;
    wxInt64 m_contentLength;
    long m_readSize;
    wxMemoryBuffer m_readBuffer;
    wxScopedPtr<wxInputStream> m_stream;

    wxDECLARE_NO_COPY_CLASS(wxWebResponseWinHTTP);
};

class WXDLLIMPEXP_NET wxWebRequestWinHTTP : public wxWebRequest
{
public:
    wxWebRequestWinHTTP(int id, wxWebSessionWinHTTP& session, const wxString& url);

    ~wxWebRequestWinHTTP();

    void SetMethod(const wxString& method) wxOVERRIDE;

    void SetData(const wxString& text, const wxString& contentType) wxOVERRIDE;

    void SetData(const wxInputStream& dataStream, const wxString& contentType) wxOVERRIDE;

    void Start() wxOVERRIDE;

    void Cancel() wxOVERRIDE;

    wxWebResponse* GetResponse() wxOVERRIDE;

    void HandleCallback(DWORD dwInternetStatus, LPVOID lpvStatusInformation,
        DWORD dwStatusInformationLength);

    HINTERNET GetHandle() const { return m_request; }

private:
    wxWebSessionWinHTTP& m_session;
    wxString m_url;
    HINTERNET m_connect;
    HINTERNET m_request;
    wxScopedPtr<wxWebResponseWinHTTP> m_response;

    void SetFailedWithLastError();

    wxDECLARE_NO_COPY_CLASS(wxWebRequestWinHTTP);
};

class WXDLLIMPEXP_NET wxWebSessionWinHTTP: public wxWebSession
{
public:
    wxWebSessionWinHTTP();

    ~wxWebSessionWinHTTP();

    wxWebRequest* CreateRequest(const wxString& url, int id = wxID_ANY) wxOVERRIDE;

    HINTERNET GetHandle() const { return m_handle; }

private:
    bool m_initialized;
    HINTERNET m_handle;

    void Init();

    wxDECLARE_NO_COPY_CLASS(wxWebSessionWinHTTP);
};

class WXDLLIMPEXP_NET wxWebSessionFactoryWinHTTP: public wxWebSessionFactory
{
public:
    wxWebSession* Create() wxOVERRIDE
        { return new wxWebSessionWinHTTP(); }
};

#endif // _WX_MSW_WEBREQUEST_WINHTTP_H
