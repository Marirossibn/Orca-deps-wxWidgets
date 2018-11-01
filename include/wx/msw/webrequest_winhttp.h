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

    bool ReadData();

    bool ReportAvailableData(DWORD dataLen);

private:
    HINTERNET m_requestHandle;
    wxInt64 m_contentLength;

    wxDECLARE_NO_COPY_CLASS(wxWebResponseWinHTTP);
};

class WXDLLIMPEXP_NET wxWebAuthChallengeWinHTTP : public wxWebAuthChallenge
{
public:
    explicit wxWebAuthChallengeWinHTTP(Source source, wxWebRequestWinHTTP& request);

    bool Init();

    void SetCredentials(const wxString& user, const wxString& password) wxOVERRIDE;

private:
    wxWebRequestWinHTTP& m_request;
    DWORD m_target;
    DWORD m_selectedScheme;

    wxDECLARE_NO_COPY_CLASS(wxWebAuthChallengeWinHTTP);
};

class WXDLLIMPEXP_NET wxWebRequestWinHTTP : public wxWebRequest
{
public:
    wxWebRequestWinHTTP(int id, wxWebSessionWinHTTP& session, const wxString& url);

    ~wxWebRequestWinHTTP();

    void Start() wxOVERRIDE;

    void Cancel() wxOVERRIDE;

    wxWebResponse* GetResponse() const wxOVERRIDE;

    wxWebAuthChallenge* GetAuthChallenge() const wxOVERRIDE { return m_authChallenge.get(); }

    wxFileOffset GetBytesSent() const wxOVERRIDE { return m_dataWritten; }

    wxFileOffset GetBytesExpectedToSend() const wxOVERRIDE { return m_dataSize; }

    wxFileOffset GetBytesReceived() const wxOVERRIDE { return m_bytesReceived; }

    wxFileOffset GetBytesExpectedToReceive() const wxOVERRIDE { return m_bytesExpectedToReceive; }

    void HandleCallback(DWORD dwInternetStatus, LPVOID lpvStatusInformation,
        DWORD dwStatusInformationLength);

    HINTERNET GetHandle() const { return m_request; }

private:
    wxString m_url;
    HINTERNET m_connect;
    HINTERNET m_request;
    wxScopedPtr<wxWebResponseWinHTTP> m_response;
    wxScopedPtr<wxWebAuthChallengeWinHTTP> m_authChallenge;
    wxMemoryBuffer m_dataWriteBuffer;
    wxFileOffset m_dataWritten;
    wxFileOffset m_bytesExpectedToReceive;
    wxFileOffset m_bytesReceived;

    void SendRequest();

    void WriteData();

    void CreateResponse();

    void SetFailedWithLastError();

    friend class wxWebAuthChallengeWinHTTP;
    friend class wxWebResponseWinHTTP;

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
