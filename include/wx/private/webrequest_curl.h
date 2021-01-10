///////////////////////////////////////////////////////////////////////////////
// Name:        wx/webrequest_curl.h
// Purpose:     wxWebRequest implementation using libcurl
// Author:      Tobias Taschner
// Created:     2018-10-25
// Copyright:   (c) 2018 wxWidgets development team
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_WEBREQUEST_CURL_H
#define _WX_WEBREQUEST_CURL_H

#if wxUSE_WEBREQUEST_CURL

#include "wx/private/webrequest.h"

#include "wx/thread.h"
#include "wx/vector.h"

#include "curl/curl.h"

class wxWebRequestCURL;
class wxWebResponseCURL;
class wxWebSessionCURL;

class wxWebAuthChallengeCURL : public wxWebAuthChallengeImpl
{
public:
    wxWebAuthChallengeCURL(wxWebAuthChallenge::Source source,
                           wxWebRequestCURL& request);

    bool Init();

    void SetCredentials(const wxWebCredentials& cred) wxOVERRIDE;

private:
    wxWebRequestCURL& m_request;

    wxDECLARE_NO_COPY_CLASS(wxWebAuthChallengeCURL);
};

class wxWebRequestCURL : public wxWebRequestImpl
{
public:
    wxWebRequestCURL(wxWebSession& session,
                     wxWebSessionCURL& sessionImpl,
                     wxEvtHandler* handler,
                     const wxString& url,
                     int id);

    ~wxWebRequestCURL();

    void Start() wxOVERRIDE;

    void Cancel() wxOVERRIDE;

    wxWebResponseImplPtr GetResponse() const wxOVERRIDE
        { return m_response; }

    wxWebAuthChallengeImplPtr GetAuthChallenge() const wxOVERRIDE
        { return m_authChallenge; }

    wxFileOffset GetBytesSent() const wxOVERRIDE;

    wxFileOffset GetBytesExpectedToSend() const wxOVERRIDE;

    CURL* GetHandle() const { return m_handle; }

    bool StartRequest();

    void HandleCompletion();

    wxString GetError() const;

    size_t ReadData(char* buffer, size_t size);

private:
    wxWebSessionCURL& m_sessionImpl;

    CURL* m_handle;
    char m_errorBuffer[CURL_ERROR_SIZE];
    struct curl_slist *m_headerList;
    wxObjectDataPtr<wxWebResponseCURL> m_response;
    wxObjectDataPtr<wxWebAuthChallengeCURL> m_authChallenge;
    wxFileOffset m_bytesSent;

    void DestroyHeaderList();

    wxDECLARE_NO_COPY_CLASS(wxWebRequestCURL);
};

class wxWebResponseCURL : public wxWebResponseImpl
{
public:
    explicit wxWebResponseCURL(wxWebRequestCURL& request);

    wxInt64 GetContentLength() const wxOVERRIDE;

    wxString GetURL() const wxOVERRIDE;

    wxString GetHeader(const wxString& name) const wxOVERRIDE;

    int GetStatus() const wxOVERRIDE;

    wxString GetStatusText() const wxOVERRIDE { return m_statusText; }

    size_t WriteData(void *buffer, size_t size);

    size_t AddHeaderData(const char* buffer, size_t size);

private:
    wxWebRequestHeaderMap m_headers;
    wxString m_statusText;

    CURL* GetHandle() const
    { return static_cast<wxWebRequestCURL&>(m_request).GetHandle(); }

    wxDECLARE_NO_COPY_CLASS(wxWebResponseCURL);
};

class wxWebSessionCURL : public wxWebSessionImpl, private wxThreadHelper
{
public:
    wxWebSessionCURL();

    ~wxWebSessionCURL();

    wxWebRequestImplPtr
    CreateRequest(wxWebSession& session,
                  wxEvtHandler* handler,
                  const wxString& url,
                  int id = wxID_ANY) wxOVERRIDE;

    wxVersionInfo GetLibraryVersionInfo() wxOVERRIDE;

    bool StartRequest(wxWebRequestCURL& request);

    void CancelRequest(wxWebRequestCURL* request);

protected:
    wxThread::ExitCode Entry() wxOVERRIDE;

private:
    CURLM* m_handle;
    wxMutex m_mutex;
    wxCondition m_condition;
    bool m_shuttingDown;

    // MT-safe vector of requests for which Cancel() was called.
    struct CancelledData
    {
        wxCriticalSection cs;
        wxVector< wxObjectDataPtr<wxWebRequestCURL> > requests;
    } m_cancelled;

    static int ms_activeSessions;

    wxDECLARE_NO_COPY_CLASS(wxWebSessionCURL);
};

class wxWebSessionFactoryCURL : public wxWebSessionFactory
{
public:
    wxWebSessionImpl* Create() wxOVERRIDE
    { return new wxWebSessionCURL(); }
};

#endif // wxUSE_WEBREQUEST_CURL

#endif
