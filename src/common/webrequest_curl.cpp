///////////////////////////////////////////////////////////////////////////////
// Name:        src/common/webrequest_curl.h
// Purpose:     wxWebRequest implementation using libcurl
// Author:      Tobias Taschner
// Created:     2018-10-25
// Copyright:   (c) 2018 wxWidgets development team
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#include "wx/webrequest.h"

#if wxUSE_WEBREQUEST_CURL

#include "wx/private/webrequest_curl.h"

#ifndef WX_PRECOMP
    #include "wx/log.h"
    #include "wx/translation.h"
    #include "wx/utils.h"
#endif

// Define symbols that might be missing from older libcurl headers
#ifndef CURL_AT_LEAST_VERSION
#define CURL_VERSION_BITS(x,y,z) ((x)<<16|(y)<<8|z)
#define CURL_AT_LEAST_VERSION(x,y,z) \
  (LIBCURL_VERSION_NUM >= CURL_VERSION_BITS(x, y, z))
#endif

#if CURL_AT_LEAST_VERSION(7, 28, 0)
    #define wxCURL_HAVE_MULTI_WAIT 1
#else
    #define wxCURL_HAVE_MULTI_WAIT 0
#endif

//
// wxWebResponseCURL
//

static size_t wxCURLWriteData(void* buffer, size_t size, size_t nmemb, void* userp)
{
    if ( userp )
        return static_cast<wxWebResponseCURL*>(userp)->WriteData(buffer, size * nmemb);
    else
        return 0;
}

static size_t wxCURLHeader(char *buffer, size_t size, size_t nitems, void *userdata)
{
    if ( userdata )
        return static_cast<wxWebResponseCURL*>(userdata)->AddHeaderData(buffer, size * nitems);
    else
        return 0;
}

wxWebResponseCURL::wxWebResponseCURL(wxWebRequestCURL& request) :
    wxWebResponseImpl(request)
{
    curl_easy_setopt(GetHandle(), CURLOPT_WRITEDATA, static_cast<void*>(this));
    curl_easy_setopt(GetHandle(), CURLOPT_HEADERDATA, static_cast<void*>(this));

    Init();
}

size_t wxWebResponseCURL::WriteData(void* buffer, size_t size)
{
    void* buf = GetDataBuffer(size);
    memcpy(buf, buffer, size);
    ReportDataReceived(size);
    return size;
}

size_t wxWebResponseCURL::AddHeaderData(const char * buffer, size_t size)
{
    wxString hdr(buffer, size);
    hdr.Trim();

    if ( hdr.StartsWith("HTTP/") )
    {
        // First line of the headers contains status text after
        // version and status
        m_statusText = hdr.AfterFirst(' ').AfterFirst(' ');
        m_headers.clear();
    }
    else if ( !hdr.empty() )
    {
        wxString hdrValue;
        wxString hdrName = hdr.BeforeFirst(':', &hdrValue).Strip(wxString::trailing);
        hdrName.MakeUpper();
        m_headers[hdrName] = hdrValue.Strip(wxString::leading);
    }

    return size;
}

wxInt64 wxWebResponseCURL::GetContentLength() const
{
#if CURL_AT_LEAST_VERSION(7, 55, 0)
    curl_off_t len = 0;
    curl_easy_getinfo(GetHandle(), CURLINFO_CONTENT_LENGTH_DOWNLOAD_T, &len);
    return len;
#else
    double len = 0;
    curl_easy_getinfo(GetHandle(), CURLINFO_CONTENT_LENGTH_DOWNLOAD, &len);
    return (wxInt64)len;
#endif
}

wxString wxWebResponseCURL::GetURL() const
{
    char* urlp = NULL;
    curl_easy_getinfo(GetHandle(), CURLINFO_EFFECTIVE_URL, &urlp);
    return wxString(urlp);
}

wxString wxWebResponseCURL::GetHeader(const wxString& name) const
{
    wxWebRequestHeaderMap::const_iterator it = m_headers.find(name.Upper());
    if ( it != m_headers.end() )
        return it->second;
    else
        return wxString();
}

int wxWebResponseCURL::GetStatus() const
{
    long status = 0;
    curl_easy_getinfo(GetHandle(), CURLINFO_RESPONSE_CODE, &status);
    return status;
}

//
// wxWebRequestCURL
//

static size_t wxCURLRead(char *buffer, size_t size, size_t nitems, void *userdata)
{
    if ( userdata )
        return static_cast<wxWebRequestCURL*>(userdata)->ReadData(buffer, size * nitems);
    else
        return 0;
}

wxWebRequestCURL::wxWebRequestCURL(wxWebSession & session,
                                   wxWebSessionCURL& sessionImpl,
                                   wxEvtHandler* handler,
                                   const wxString & url,
                                   int id):
    wxWebRequestImpl(session, handler, id),
    m_sessionImpl(sessionImpl)
{
    m_headerList = NULL;

    m_handle = curl_easy_init();
    // Set error buffer to get more detailed CURL status
    m_errorBuffer[0] = '\0';
    curl_easy_setopt(m_handle, CURLOPT_ERRORBUFFER, m_errorBuffer);
    // Set this request in the private pointer
    curl_easy_setopt(m_handle, CURLOPT_PRIVATE, static_cast<void*>(this));
    // Set URL to handle
    curl_easy_setopt(m_handle, CURLOPT_URL, static_cast<const char*>(url.mb_str()));
    // Set callback functions
    curl_easy_setopt(m_handle, CURLOPT_WRITEFUNCTION, wxCURLWriteData);
    curl_easy_setopt(m_handle, CURLOPT_HEADERFUNCTION, wxCURLHeader);
    curl_easy_setopt(m_handle, CURLOPT_READFUNCTION, wxCURLRead);
    curl_easy_setopt(m_handle, CURLOPT_READDATA, static_cast<void*>(this));
    // Enable gzip, etc decompression
    curl_easy_setopt(m_handle, CURLOPT_ACCEPT_ENCODING, "");
    // Enable redirection handling
    curl_easy_setopt(m_handle, CURLOPT_FOLLOWLOCATION, 1L);
    // Limit redirect to HTTP
    curl_easy_setopt(m_handle, CURLOPT_REDIR_PROTOCOLS,
        CURLPROTO_HTTP | CURLPROTO_HTTPS);
    // Enable all supported authentication methods
    curl_easy_setopt(m_handle, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
    curl_easy_setopt(m_handle, CURLOPT_PROXYAUTH, CURLAUTH_ANY);
}

wxWebRequestCURL::~wxWebRequestCURL()
{
    DestroyHeaderList();

    curl_easy_cleanup(m_handle);
}

void wxWebRequestCURL::Start()
{
    if ( GetState() != wxWebRequest::State_Idle )
        return;

    m_response.reset(new wxWebResponseCURL(*this));

    if ( m_dataSize )
    {
        if ( m_method.empty() || m_method.IsSameAs("POST", false) )
        {
            curl_easy_setopt(m_handle, CURLOPT_POSTFIELDSIZE_LARGE,
                static_cast<curl_off_t>(m_dataSize));
            curl_easy_setopt(m_handle, CURLOPT_POST, 1L);
        }
        else if ( m_method.IsSameAs("PUT", false) )
        {
            curl_easy_setopt(m_handle, CURLOPT_UPLOAD, 1L);
            curl_easy_setopt(m_handle, CURLOPT_INFILESIZE_LARGE,
                static_cast<curl_off_t>(m_dataSize));
        }
    }

    if ( m_method.IsSameAs("HEAD", false) )
    {
        curl_easy_setopt(m_handle, CURLOPT_NOBODY, 1L);
    }
    else if ( !m_method.empty() )
    {
        curl_easy_setopt(m_handle, CURLOPT_CUSTOMREQUEST,
            static_cast<const char*>(m_method.mb_str()));
    }

    for ( wxWebRequestHeaderMap::const_iterator it = m_headers.begin();
        it != m_headers.end(); ++it )
    {
        wxString hdrStr = wxString::Format("%s: %s", it->first, it->second);
        m_headerList = curl_slist_append(m_headerList, hdrStr.mb_str());
    }
    curl_easy_setopt(m_handle, CURLOPT_HTTPHEADER, m_headerList);

    StartRequest();
}

bool wxWebRequestCURL::StartRequest()
{
    m_bytesSent = 0;

    if ( m_sessionImpl.StartRequest(*this) )
    {
        SetState(wxWebRequest::State_Active);
        return true;
    }
    else
    {
        SetState(wxWebRequest::State_Failed);
        return false;
    }
}

void wxWebRequestCURL::Cancel()
{
    m_sessionImpl.CancelRequest(this);
}

void wxWebRequestCURL::HandleCompletion()
{
    int status = m_response ? m_response->GetStatus() : 0;

    if ( status == 0)
        SetState(wxWebRequest::State_Failed, GetError());
    else if ( status == 401 || status == 407 )
    {
        m_authChallenge.reset(new wxWebAuthChallengeCURL(
            (status == 407) ? wxWebAuthChallenge::Source_Proxy : wxWebAuthChallenge::Source_Server, *this));
        if ( m_authChallenge->Init() )
            SetState(wxWebRequest::State_Unauthorized, m_response->GetStatusText());
        else
            SetState(wxWebRequest::State_Failed);
    }
    else if ( CheckServerStatus() )
        SetState(wxWebRequest::State_Completed);
}

wxString wxWebRequestCURL::GetError() const
{
    return wxString(m_errorBuffer);
}

size_t wxWebRequestCURL::ReadData(char* buffer, size_t size)
{
    if ( m_dataStream )
    {
        m_dataStream->Read((void*)buffer, size);
        size_t readSize = m_dataStream->LastRead();
        m_bytesSent += readSize;
        return readSize;
    }
    else
        return 0;
}

void wxWebRequestCURL::DestroyHeaderList()
{
    if ( m_headerList )
    {
        curl_slist_free_all(m_headerList);
        m_headerList = NULL;
    }
}

wxFileOffset wxWebRequestCURL::GetBytesSent() const
{
    return m_bytesSent;
}

wxFileOffset wxWebRequestCURL::GetBytesExpectedToSend() const
{
    return m_dataSize;
}

//
// wxWebAuthChallengeCURL
//

wxWebAuthChallengeCURL::wxWebAuthChallengeCURL(wxWebAuthChallenge::Source source,
                                               wxWebRequestCURL& request) :
    wxWebAuthChallengeImpl(source),
    m_request(request)
{
}

bool wxWebAuthChallengeCURL::Init()
{
    return true;
}

void wxWebAuthChallengeCURL::SetCredentials(const wxString& user, const wxString& password)
{
    wxString authStr = wxString::Format("%s:%s", user, password);
    curl_easy_setopt(m_request.GetHandle(),
        (GetSource() == wxWebAuthChallenge::Source_Proxy) ? CURLOPT_PROXYUSERPWD : CURLOPT_USERPWD,
        static_cast<const char*>(authStr.mb_str()));
    m_request.StartRequest();
}

//
// wxWebSessionCURL
//

int wxWebSessionCURL::ms_activeSessions = 0;

wxWebSessionCURL::wxWebSessionCURL() :
    m_handle(NULL),
    m_condition(m_mutex),
    m_shuttingDown(false)
{
    // Initialize CURL globally if no sessions are active
    if ( ms_activeSessions == 0 )
    {
        if ( curl_global_init(CURL_GLOBAL_ALL) )
            wxLogError(_("libcurl could not be initialized"));
    }

    ms_activeSessions++;
}

wxWebSessionCURL::~wxWebSessionCURL()
{
    {
        // Notify the work thread
        m_shuttingDown = true;
        wxMutexLocker lock(m_mutex);
        m_condition.Broadcast();
    }

    // Wait for work thread to finish
    if ( GetThread() && GetThread()->IsRunning() )
        GetThread()->Wait(wxTHREAD_WAIT_BLOCK);

    if ( m_handle )
        curl_multi_cleanup(m_handle);

    // Global CURL cleanup if this is the last session
    --ms_activeSessions;
    if ( ms_activeSessions == 0 )
        curl_global_cleanup();
}

wxWebRequestImplPtr
wxWebSessionCURL::CreateRequest(wxWebSession& session,
                                wxEvtHandler* handler,
                                const wxString& url,
                                int id)
{
    // Allocate our handle on demand.
    if ( !m_handle )
    {
        m_handle = curl_multi_init();
        if ( !m_handle )
        {
            wxLogDebug("curl_multi_init() failed");
            return wxWebRequestImplPtr();
        }
    }

    return wxWebRequestImplPtr(new wxWebRequestCURL(session, *this, handler, url, id));
}

static CURLMcode wx_curl_multi_wait(CURLM *multi_handle, int timeout_ms,
    int *ret)
{
    // since libcurl 7.28.0 the curl_multi_wait method is more convient than
    // calling multiple curl_multi_... methods.
    // When support for older libcurl versions is dropped this wrapper can be
    // removed.
#if wxCURL_HAVE_MULTI_WAIT
    return curl_multi_wait(multi_handle, NULL, 0, timeout_ms, ret);
#else
    wxASSERT(ret != NULL);

    fd_set fdread;
    fd_set fdwrite;
    fd_set fdexcep;
    timeval timeout;

    long curl_timeo;

    curl_multi_timeout(multi_handle, &curl_timeo);
    if ( curl_timeo < 0 )
        curl_timeo = timeout_ms;

    timeout.tv_sec = curl_timeo / 1000;
    timeout.tv_usec = (curl_timeo % 1000) * 1000;

    FD_ZERO(&fdread);
    FD_ZERO(&fdwrite);
    FD_ZERO(&fdexcep);

    curl_multi_fdset(multi_handle, &fdread, &fdwrite, &fdexcep, ret);
    if ( *ret == -1 )
        return CURLM_OK;
    else if ( select(*ret + 1, &fdread, &fdwrite, &fdexcep, &timeout) == -1 )
        return CURLM_BAD_SOCKET;
    else
        return CURLM_OK;
#endif
}

wxThread::ExitCode wxWebSessionCURL::Entry()
{
    m_mutex.Lock();

    int activeRequests = -1;
    int repeats = 0;

    while ( activeRequests )
    {
        // Handle cancelled requests
        {
            wxMutexLocker lock(m_cancelledMutex);
            while ( !m_cancelledRequests.empty() )
            {
                wxObjectDataPtr<wxWebRequestCURL> request(m_cancelledRequests.back());
                m_cancelledRequests.pop_back();
                curl_multi_remove_handle(m_handle, request->GetHandle());
                request->SetState(wxWebRequest::State_Cancelled);
            }
        }

        // Instruct CURL to work on requests
        curl_multi_perform(m_handle, &activeRequests);

        // Process CURL message queue
        int msgQueueCount;
        while ( CURLMsg* msg = curl_multi_info_read(m_handle, &msgQueueCount) )
        {
            if ( msg->msg == CURLMSG_DONE )
            {
                wxWebRequestCURL* request;
                curl_easy_getinfo(msg->easy_handle, CURLINFO_PRIVATE, &request);
                curl_multi_remove_handle(m_handle, msg->easy_handle);
                request->HandleCompletion();
            }
        }

        if ( activeRequests )
        {
            // Wait for CURL work to finish
            int numfds;
            wx_curl_multi_wait(m_handle, 500, &numfds);

            if ( !numfds )
            {
                repeats++; // count number of repeated zero numfds
                if ( repeats > 1 )
                    wxMilliSleep(100);
            }
            else
                repeats = 0;
        }
        else
        {
            // Wait for new requests or shutdown of the session
            m_condition.Wait();
            if ( !m_shuttingDown )
                activeRequests = -1;
        }
    }

    return (wxThread::ExitCode)0;
}

bool wxWebSessionCURL::StartRequest(wxWebRequestCURL & request)
{
    // Add request easy handle to multi handle
    curl_multi_add_handle(m_handle, request.GetHandle());

    // Create and start session thread if not yet running
    if ( !GetThread() )
    {
        if ( CreateThread() )
            return false;

        if ( GetThread()->Run() != wxTHREAD_NO_ERROR )
            return false;
    }

    // Signal the worker thread to resume work
    wxMutexLocker lock(m_mutex);
    m_condition.Broadcast();

    return true;
}

void wxWebSessionCURL::CancelRequest(wxWebRequestCURL* request)
{
    // Add the request to a list of threads that will be removed from the curl
    // multi handle in the worker thread
    wxMutexLocker lock(m_cancelledMutex);
    request->IncRef();
    m_cancelledRequests.push_back(wxObjectDataPtr<wxWebRequestCURL>(request));
}

wxVersionInfo  wxWebSessionCURL::GetLibraryVersionInfo()
{
    const curl_version_info_data* vi = curl_version_info(CURLVERSION_NOW);
    wxString desc = wxString::Format("libcurl/%s", vi->version);
    if (vi->ssl_version[0])
        desc += " " + wxString(vi->ssl_version);
    return wxVersionInfo("libcurl",
        vi->version_num >> 16 & 0xff,
        vi->version_num >> 8 & 0xff,
        vi->version_num & 0xff,
        desc);
}

#endif // wxUSE_WEBREQUEST_CURL
