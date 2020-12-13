///////////////////////////////////////////////////////////////////////////////
// Name:        wx/webrequest.h
// Purpose:     wxWebRequest base classes
// Author:      Tobias Taschner
// Created:     2018-10-17
// Copyright:   (c) 2018 wxWidgets development team
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_WEBREQUEST_H
#define _WX_WEBREQUEST_H

#include "wx/defs.h"

#if wxUSE_WEBREQUEST

#include "wx/event.h"
#include "wx/ffile.h"
#include "wx/hashmap.h"
#include "wx/object.h"
#include "wx/scopedptr.h"
#include "wx/sharedptr.h"
#include "wx/stream.h"
#include "wx/vector.h"
#include "wx/versioninfo.h"

class wxWebResponse;
class wxWebSession;
class wxWebAuthChallenge;

WX_DECLARE_STRING_HASH_MAP(wxString, wxWebRequestHeaderMap);

class WXDLLIMPEXP_NET wxWebRequest : public wxEvtHandler, public wxRefCounter
{
public:
    enum State
    {
        State_Idle,
        State_Unauthorized,
        State_Active,
        State_Completed,
        State_Failed,
        State_Cancelled
    };

    enum Storage
    {
        Storage_Memory,
        Storage_File,
        Storage_None
    };

    virtual ~wxWebRequest() { }

    virtual void SetHeader(const wxString& name, const wxString& value)
    { m_headers[name] = value; }

    virtual void SetMethod(const wxString& method) { m_method = method; }

    void SetData(const wxString& text, const wxString& contentType, const wxMBConv& conv = wxConvUTF8);

    bool SetData(const wxSharedPtr<wxInputStream>& dataStream, const wxString& contentType, wxFileOffset dataSize = wxInvalidOffset);

    void SetIgnoreServerErrorStatus(bool ignore) { m_ignoreServerErrorStatus = ignore; }

    virtual void SetStorage(Storage storage) { m_storage = storage; }

    Storage GetStorage() const { return m_storage; }

    virtual void Start() = 0;

    virtual void Cancel() = 0;

    virtual wxWebResponse* GetResponse() const = 0;

    virtual wxWebAuthChallenge* GetAuthChallenge() const = 0;

    int GetId() const { return m_id; }

    wxWebSession& GetSession() const { return m_session; }

    State GetState() const { return m_state; }

    virtual wxFileOffset GetBytesSent() const = 0;

    virtual wxFileOffset GetBytesExpectedToSend() const = 0;

    virtual wxFileOffset GetBytesReceived() const;

    virtual wxFileOffset GetBytesExpectedToReceive() const;

    void SetState(State state, const wxString& failMsg = "");

    void ReportDataReceived(size_t sizeReceived);

protected:
    wxString m_method;
    Storage m_storage;
    wxWebRequestHeaderMap m_headers;
    wxFileOffset m_dataSize;
    wxSharedPtr<wxInputStream> m_dataStream;

    wxWebRequest(wxWebSession& session, int id);

    bool CheckServerStatus();

    static bool IsActiveState(State state);

private:
    wxWebSession& m_session;
    int m_id;
    State m_state;
    bool m_ignoreServerErrorStatus;
    wxFileOffset m_bytesReceived;
    wxCharBuffer m_dataText;

    void ProcessStateEvent(State state, const wxString& failMsg);

    wxDECLARE_NO_COPY_CLASS(wxWebRequest);
};

class WXDLLIMPEXP_NET wxWebResponse
{
public:
    virtual ~wxWebResponse();

    virtual wxInt64 GetContentLength() const = 0;

    virtual wxString GetURL() const = 0;

    virtual wxString GetHeader(const wxString& name) const = 0;

    virtual wxString GetMimeType() const;

    virtual int GetStatus() const = 0;

    virtual wxString GetStatusText() const = 0;

    virtual wxInputStream* GetStream() const;

    virtual wxString GetSuggestedFileName() const;

    wxString AsString() const;

    virtual wxString GetFileName() const;

protected:
    wxWebRequest& m_request;
    size_t m_readSize;

    wxWebResponse(wxWebRequest& request);

    // Called from derived class ctor to finish initialization which can't be
    // performed in ctor itself as it needs to use pure virtual method.
    void Init();

    void* GetDataBuffer(size_t sizeNeeded);

    void ReportDataReceived(size_t sizeReceived);

private:
    // Called by wxWebRequest only.
    void Finalize();

    wxMemoryBuffer m_readBuffer;
    mutable wxFFile m_file;
    mutable wxScopedPtr<wxInputStream> m_stream;

    friend class wxWebRequest;

    wxDECLARE_NO_COPY_CLASS(wxWebResponse);
};

class WXDLLIMPEXP_NET wxWebAuthChallenge
{
public:
    enum Source
    {
        Source_Server,
        Source_Proxy
    };

    virtual ~wxWebAuthChallenge() { }

    Source GetSource() const { return m_source; }

    virtual void SetCredentials(const wxString& user, const wxString& password) = 0;

protected:
    wxWebAuthChallenge(Source source): m_source(source) { }

private:
    Source m_source;

    wxDECLARE_NO_COPY_CLASS(wxWebAuthChallenge);
};

class WXDLLIMPEXP_NET wxWebSessionFactory
{
public:
    virtual wxWebSession* Create() = 0;

    virtual ~wxWebSessionFactory() { }
};

extern WXDLLIMPEXP_DATA_NET(const char) wxWebSessionBackendDefault[];
extern WXDLLIMPEXP_DATA_NET(const char) wxWebSessionBackendWinHTTP[];
extern WXDLLIMPEXP_DATA_NET(const char) wxWebSessionBackendURLSession[];
extern WXDLLIMPEXP_DATA_NET(const char) wxWebSessionBackendCURL[];

class WXDLLIMPEXP_NET wxWebSession
{
public:
    virtual ~wxWebSession() { }

    virtual wxWebRequest* CreateRequest(const wxString& url, int id = wxID_ANY) = 0;

    virtual wxVersionInfo GetLibraryVersionInfo() = 0;

    virtual void SetHeader(const wxString& name, const wxString& value)
    { m_headers[name] = value; }

    void SetTempDir(const wxString& dir) { m_tempDir = dir; }

    wxString GetTempDir() const;

    static wxWebSession& GetDefault();

    static wxWebSession* New(const wxString& backend = wxWebSessionBackendDefault);

    static void RegisterFactory(const wxString& backend,
                                const wxSharedPtr<wxWebSessionFactory>& factory);

    static bool IsBackendAvailable(const wxString& backend);

protected:
    wxWebSession();

    const wxWebRequestHeaderMap& GetHeaders() const { return m_headers; }

private:
    // Make it a friend to allow accessing our m_headers.
    friend class wxWebRequest;

    wxWebRequestHeaderMap m_headers;
    wxString m_tempDir;

    static void InitFactoryMap();
};

class WXDLLIMPEXP_NET wxWebRequestEvent : public wxEvent
{
public:
    wxWebRequestEvent(wxEventType type = wxEVT_NULL,
                      int id = wxID_ANY,
                      wxWebRequest::State state = wxWebRequest::State_Idle,
                      wxWebResponse* response = NULL,
                      const wxString& errorDesc = wxString())
        : wxEvent(id, type),
        m_state(state), m_response(response), m_data(NULL), m_dataSize(0),
        m_errorDescription(errorDesc)
    { }

    wxWebRequest::State GetState() const { return m_state; }

    wxWebResponse* GetResponse() const { return m_response; }

    const wxString& GetErrorDescription() const { return m_errorDescription; }

    const wxString& GetResponseFileName() const { return m_responseFileName; }

    void SetResponseFileName(const wxString& filename) { m_responseFileName = filename; }

    const void* GetDataBuffer() const { return m_data; }

    size_t GetDataSize() const { return m_dataSize; }

    void SetDataBuffer(const void* buffer, size_t size)
    { m_data = buffer; m_dataSize = size; }

    wxEvent* Clone() const wxOVERRIDE { return new wxWebRequestEvent(*this); }

private:
    wxWebRequest::State m_state;
    wxWebResponse* const m_response; // non-owning, may be NULL
    wxString m_responseFileName;
    const void* m_data;
    size_t m_dataSize;
    wxString m_errorDescription;
};

wxDECLARE_EXPORTED_EVENT(WXDLLIMPEXP_NET, wxEVT_WEBREQUEST_STATE, wxWebRequestEvent);
wxDECLARE_EXPORTED_EVENT(WXDLLIMPEXP_NET, wxEVT_WEBREQUEST_DATA, wxWebRequestEvent);

#endif // wxUSE_WEBREQUEST

#endif // _WX_WEBREQUEST_H
