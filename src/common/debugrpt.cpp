///////////////////////////////////////////////////////////////////////////////
// Name:        src/common/debugrpt.cpp
// Purpose:     wxDebugReport and related classes implementation
// Author:      Vadim Zeitlin
// Modified by:
// Created:     2005-01-17
// RCS-ID:      $Id$
// Copyright:   (c) 2005 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// License:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/app.h"
    #include "wx/log.h"
    #include "wx/intl.h"
#endif // WX_PRECOMP

#if wxUSE_DEBUGREPORT

#include "wx/debugrpt.h"

#include "wx/filename.h"
#include "wx/dir.h"
#include "wx/dynlib.h"

#include "wx/xml/xml.h"

#if wxUSE_STACKWALKER
    #include "wx/stackwalk.h"
#endif

#if wxUSE_CRASHREPORT
    #include "wx/msw/crashrpt.h"
#endif

#if wxUSE_ZIPSTREAM
    #include "wx/wfstream.h"
    #include "wx/zipstrm.h"
#endif // wxUSE_ZIPSTREAM

#if wxUSE_STACKWALKER

// ----------------------------------------------------------------------------
// XmlStackWalker: stack walker specialization which dumps stack in XML
// ----------------------------------------------------------------------------

class XmlStackWalker : public wxStackWalker
{
public:
    XmlStackWalker(wxXmlNode *nodeStack)
    {
        m_isOk = false;
        m_nodeStack = nodeStack;
    }

    bool IsOk() const { return m_isOk; }

protected:
    virtual void OnStackFrame(const wxStackFrame& frame);

    wxXmlNode *m_nodeStack;
    bool m_isOk;
};

#endif // wxUSE_STACKWALKER

// ----------------------------------------------------------------------------
// local functions
// ----------------------------------------------------------------------------

static inline void
HexProperty(wxXmlNode *node, const wxChar *name, unsigned long value)
{
    node->AddProperty(name, wxString::Format(_T("%08x"), value));
}

static inline void
NumProperty(wxXmlNode *node, const wxChar *name, unsigned long value)
{
    node->AddProperty(name, wxString::Format(_T("%lu"), value));
}

static inline void
TextElement(wxXmlNode *node, const wxChar *name, const wxString& value)
{
    wxXmlNode *nodeChild = new wxXmlNode(wxXML_ELEMENT_NODE, name);
    node->AddChild(nodeChild);
    nodeChild->AddChild(new wxXmlNode(wxXML_TEXT_NODE, _T(""), value));
}

static inline void
HexElement(wxXmlNode *node, const wxChar *name, unsigned long value)
{
    TextElement(node, name, wxString::Format(_T("%08x"), value));
}

#if wxUSE_STACKWALKER

// ============================================================================
// XmlStackWalker implementation
// ============================================================================

void XmlStackWalker::OnStackFrame(const wxStackFrame& frame)
{
    m_isOk = true;

    wxXmlNode *nodeFrame = new wxXmlNode(wxXML_ELEMENT_NODE, _T("frame"));
    m_nodeStack->AddChild(nodeFrame);

    NumProperty(nodeFrame, _T("level"), frame.GetLevel());
    wxString func = frame.GetName();
    if ( !func.empty() )
    {
        nodeFrame->AddProperty(_T("function"), func);
        HexProperty(nodeFrame, _T("offset"), frame.GetOffset());
    }

    if ( frame.HasSourceLocation() )
    {
        nodeFrame->AddProperty(_T("file"), frame.GetFileName());
        NumProperty(nodeFrame, _T("line"), frame.GetLine());
    }

    const size_t nParams = frame.GetParamCount();
    if ( nParams )
    {
        wxXmlNode *nodeParams = new wxXmlNode(wxXML_ELEMENT_NODE, _T("parameters"));
        nodeFrame->AddChild(nodeParams);

        for ( size_t n = 0; n < nParams; n++ )
        {
            wxXmlNode *
                nodeParam = new wxXmlNode(wxXML_ELEMENT_NODE, _T("parameter"));
            nodeParams->AddChild(nodeParam);

            NumProperty(nodeParam, _T("number"), n);

            wxString type, name, value;
            if ( !frame.GetParam(n, &type, &name, &value) )
                continue;

            if ( !type.empty() )
                TextElement(nodeParam, _T("type"), type);

            if ( !name.empty() )
                TextElement(nodeParam, _T("name"), name);

            if ( !value.empty() )
                TextElement(nodeParam, _T("value"), value);
        }
    }
}

#endif // wxUSE_STACKWALKER

// ============================================================================
// wxDebugReport implementation
// ============================================================================

// ----------------------------------------------------------------------------
// initialization and cleanup
// ----------------------------------------------------------------------------

wxDebugReport::wxDebugReport()
{
    // get a temporary directory name
    wxString appname(wxTheApp ? wxTheApp->GetAppName() : _T("wx"));

    // we can't use CreateTempFileName() because it creates a file, not a
    // directory, so do our best to create a unique name ourselves
    //
    // of course, this doesn't protect us against malicious users...
    wxFileName fn;
    fn.AssignTempFileName(appname);
    m_dir.Printf(_T("%s%c%s_dbgrpt-%lu-%s"),
                 fn.GetPath(), wxFILE_SEP_PATH, appname.c_str(),
                 wxGetProcessId(),
                 wxDateTime::Now().Format(_T("%Y%m%dT%H%M%S")).c_str());

    // as we are going to save the process state there use restrictive
    // permissions
    if ( !wxMkdir(m_dir, 0700) )
    {
        wxLogSysError(_("Failed to create directory \"%s\""), m_dir.c_str());
        wxLogError(_("Debug report couldn't be created."));

        Reset();
    }
}

wxDebugReport::~wxDebugReport()
{
    if ( !m_dir.empty() )
    {
        // remove all files in this directory
        wxDir dir(m_dir);
        wxString file;
        for ( bool cont = dir.GetFirst(&file); cont; cont = dir.GetNext(&file) )
        {
            if ( wxRemove(wxFileName(m_dir, file).GetFullPath()) != 0 )
            {
                wxLogSysError(_("Failed to remove debug report file \"%s\""),
                              file.c_str());
                m_dir.clear();
                break;
            }
        }
    }

    if ( !m_dir.empty() )
    {
        if ( wxRmDir(m_dir) != 0 )
        {
            wxLogSysError(_("Failed to clean up debug report directory \"%s\""),
                          m_dir.c_str());
        }
    }
}

// ----------------------------------------------------------------------------
// various helpers
// ----------------------------------------------------------------------------

wxString wxDebugReport::GetReportName() const
{
    return wxString(wxTheApp ? wxTheApp->GetAppName() : _T("wx"));
}

void wxDebugReport::AddFile(const wxString& name, const wxString& description)
{
    m_files.Add(name);
    m_descriptions.Add(description);
}

void wxDebugReport::RemoveFile(const wxString& name)
{
    const int n = m_files.Index(name);
    wxCHECK_RET( n != wxNOT_FOUND, _T("No such file in wxDebugReport") );

    m_files.RemoveAt(n);
    m_descriptions.RemoveAt(n);

    wxRemove(wxFileName(GetDirectory(), name).GetFullPath());
}

bool wxDebugReport::GetFile(size_t n, wxString *name, wxString *desc) const
{
    if ( n >= m_files.GetCount() )
        return false;

    if ( name )
        *name = m_files[n];
    if ( desc )
        *desc = m_descriptions[n];

    return true;
}

void wxDebugReport::AddAll(Context context)
{
#if wxUSE_STACKWALKER
    AddContext(context);
#endif // wxUSE_STACKWALKER

#if wxUSE_CRASHREPORT
    AddDump(context);
#endif // wxUSE_CRASHREPORT
}

// ----------------------------------------------------------------------------
// adding basic text information about current context
// ----------------------------------------------------------------------------

#if wxUSE_STACKWALKER

bool wxDebugReport::DoAddSystemInfo(wxXmlNode *nodeSystemInfo)
{
    nodeSystemInfo->AddProperty(_T("description"), wxGetOsDescription());

    return true;
}

bool wxDebugReport::DoAddLoadedModules(wxXmlNode *nodeModules)
{
    wxDynamicLibraryDetailsArray modules(wxDynamicLibrary::ListLoaded());
    const size_t count = modules.GetCount();
    if ( !count )
        return false;

    for ( size_t n = 0; n < count; n++ )
    {
        const wxDynamicLibraryDetails& info = modules[n];

        wxXmlNode *nodeModule = new wxXmlNode(wxXML_ELEMENT_NODE, _T("module"));
        nodeModules->AddChild(nodeModule);

        wxString path = info.GetPath();
        if ( path.empty() )
            path = info.GetName();
        if ( !path.empty() )
            nodeModule->AddProperty(_T("path"), path);

        void *addr;
        size_t len;
        if ( info.GetAddress(&addr, &len) )
        {
            HexProperty(nodeModule, _T("address"), (unsigned long)addr);
            HexProperty(nodeModule, _T("size"), len);
        }

        wxString ver = info.GetVersion();
        if ( !ver.empty() )
        {
            nodeModule->AddProperty(_T("version"), ver);
        }
    }

    return true;
}

bool wxDebugReport::DoAddExceptionInfo(wxXmlNode *nodeContext)
{
#if wxUSE_CRASHREPORT
    wxCrashContext c;
    if ( !c.code )
        return false;

    wxXmlNode *nodeExc = new wxXmlNode(wxXML_ELEMENT_NODE, _T("exception"));
    nodeContext->AddChild(nodeExc);

    HexProperty(nodeExc, _T("code"), c.code);
    nodeExc->AddProperty(_T("name"), c.GetExceptionString());
    HexProperty(nodeExc, _T("address"), (unsigned long)c.addr);

#ifdef __INTEL__
    wxXmlNode *nodeRegs = new wxXmlNode(wxXML_ELEMENT_NODE, _T("registers"));
    nodeContext->AddChild(nodeRegs);
    HexElement(nodeRegs, _T("eax"), c.regs.eax);
    HexElement(nodeRegs, _T("ebx"), c.regs.ebx);
    HexElement(nodeRegs, _T("ecx"), c.regs.edx);
    HexElement(nodeRegs, _T("edx"), c.regs.edx);
    HexElement(nodeRegs, _T("esi"), c.regs.esi);
    HexElement(nodeRegs, _T("edi"), c.regs.edi);

    HexElement(nodeRegs, _T("ebp"), c.regs.ebp);
    HexElement(nodeRegs, _T("esp"), c.regs.esp);
    HexElement(nodeRegs, _T("eip"), c.regs.eip);

    HexElement(nodeRegs, _T("cs"), c.regs.cs);
    HexElement(nodeRegs, _T("ds"), c.regs.ds);
    HexElement(nodeRegs, _T("es"), c.regs.es);
    HexElement(nodeRegs, _T("fs"), c.regs.fs);
    HexElement(nodeRegs, _T("gs"), c.regs.gs);
    HexElement(nodeRegs, _T("ss"), c.regs.ss);

    HexElement(nodeRegs, _T("flags"), c.regs.flags);
#endif // __INTEL__

    return true;
#else // !wxUSE_CRASHREPORT
    wxUnusedVar(nodeContext);

    return false;
#endif // wxUSE_CRASHREPORT/!wxUSE_CRASHREPORT
}

bool wxDebugReport::AddContext(wxDebugReport::Context ctx)
{
    wxCHECK_MSG( IsOk(), false, _T("use IsOk() first") );

    // create XML dump of current context
    wxXmlDocument xmldoc;
    wxXmlNode *nodeRoot = new wxXmlNode(wxXML_ELEMENT_NODE, _T("report"));
    xmldoc.SetRoot(nodeRoot);
    nodeRoot->AddProperty(_T("version"), _T("1.0"));
    nodeRoot->AddProperty(_T("kind"), ctx == Context_Curent ? _T("user")
                                                            : _T("exception"));

    // add system information
    wxXmlNode *nodeSystemInfo = new wxXmlNode(wxXML_ELEMENT_NODE, _T("system"));
    if ( DoAddSystemInfo(nodeSystemInfo) )
        nodeRoot->AddChild(nodeSystemInfo);
    else
        delete nodeSystemInfo;

    // add information about the loaded modules
    wxXmlNode *nodeModules = new wxXmlNode(wxXML_ELEMENT_NODE, _T("modules"));
    if ( DoAddLoadedModules(nodeModules) )
        nodeRoot->AddChild(nodeModules);
    else
        delete nodeModules;

    // add CPU context information: this only makes sense for exceptions as our
    // current context is not very interesting otherwise
    if ( ctx == Context_Exception )
    {
        wxXmlNode *nodeContext = new wxXmlNode(wxXML_ELEMENT_NODE, _T("context"));
        if ( DoAddExceptionInfo(nodeContext) )
            nodeRoot->AddChild(nodeContext);
        else
            delete nodeContext;
    }

    // add stack traceback
#if wxUSE_STACKWALKER
    wxXmlNode *nodeStack = new wxXmlNode(wxXML_ELEMENT_NODE, _T("stack"));
    XmlStackWalker sw(nodeStack);
    if ( ctx == Context_Exception )
    {
        sw.WalkFromException();
    }
    else // Context_Curent
    {
        sw.Walk();
    }

    if ( sw.IsOk() )
        nodeRoot->AddChild(nodeStack);
    else
        delete nodeStack;
#endif // wxUSE_STACKWALKER

    // finally let the user add any extra information he needs
    DoAddCustomContext(nodeRoot);


    // save the entire context dump in a file
    wxFileName fn(m_dir, GetReportName(), _T("xml"));

    if ( !xmldoc.Save(fn.GetFullPath()) )
        return false;

    AddFile(fn.GetFullName(), _("process context description"));

    return true;
}

#endif // wxUSE_STACKWALKER

// ----------------------------------------------------------------------------
// adding core dump
// ----------------------------------------------------------------------------

#if wxUSE_CRASHREPORT

bool wxDebugReport::AddDump(Context ctx)
{
    wxCHECK_MSG( IsOk(), false, _T("use IsOk() first") );

    wxFileName fn(m_dir, GetReportName(), _T("dmp"));
    wxCrashReport::SetFileName(fn.GetFullPath());

    if ( !(ctx == Context_Exception ? wxCrashReport::Generate()
                                    : wxCrashReport::GenerateNow()) )
            return false;

    AddFile(fn.GetFullName(), _("dump of the process state (binary)"));

    return true;
}

#endif // wxUSE_CRASHREPORT

// ----------------------------------------------------------------------------
// report processing
// ----------------------------------------------------------------------------

bool wxDebugReport::Process()
{
    if ( !GetFilesCount() )
    {
        wxLogError(_("Debug report generation has failed."));

        return false;
    }

    if ( !DoProcess() )
    {
        wxLogError(_("Processing debug report has failed, leaving the files in \"%s\" directory."),
                   GetDirectory().c_str());

        Reset();

        return false;
    }

    return true;
}

bool wxDebugReport::DoProcess()
{
    wxString msg = _("*** A debug report has been generated\n");
    msg += wxString::Format(_("*** It can be found in \"%s\"\n"),
                            GetDirectory().c_str());
    msg += _("*** And includes the following files:\n");

    wxString name, desc;
    const size_t count = GetFilesCount();
    for ( size_t n = 0; n < count; n++ )
    {
        GetFile(n, &name, &desc);
        msg += wxString::Format(_("\t%s: %s\n"), name.c_str(), desc.c_str());
    }

    msg += _("\nPlease send this report to the program maintainer, thank you!\n");

    wxLogMessage(_T("%s"), msg.c_str());

    // we have to do this or the report would be deleted, and we don't even
    // have any way to ask the user if he wants to keep it from here
    Reset();

    return true;
}

// ============================================================================
// wxDebugReport-derived classes
// ============================================================================

#if wxUSE_ZIPSTREAM

// ----------------------------------------------------------------------------
// wxDebugReportCompress
// ----------------------------------------------------------------------------

bool wxDebugReportCompress::DoProcess()
{
    const size_t count = GetFilesCount();
    if ( !count )
        return false;

    // create the streams
    wxFileName fn(GetDirectory(), GetReportName(), _T("zip"));
    wxFFileOutputStream os(fn.GetFullPath(), _T("wb"));
    wxZipOutputStream zos(os, 9);

    // add all files to the ZIP one
    wxString name, desc;
    for ( size_t n = 0; n < count; n++ )
    {
        GetFile(n, &name, &desc);

        wxZipEntry *ze = new wxZipEntry(name);
        ze->SetComment(desc);

        if ( !zos.PutNextEntry(ze) )
            return false;

        wxFFileInputStream is(wxFileName(fn.GetPath(), name).GetFullPath());
        if ( !is.IsOk() || !zos.Write(is).IsOk() )
            return false;
    }

    if ( !zos.Close() )
        return false;

    m_zipfile = fn.GetFullPath();

    return true;
}

// ----------------------------------------------------------------------------
// wxDebugReportUpload
// ----------------------------------------------------------------------------

wxDebugReportUpload::wxDebugReportUpload(const wxString& url,
                                         const wxString& input,
                                         const wxString& action,
                                         const wxString& curl)
                   : m_uploadURL(url),
                     m_inputField(input),
                     m_curlCmd(curl)
{
    if ( m_uploadURL.Last() != _T('/') )
        m_uploadURL += _T('/');
    m_uploadURL += action;
}

bool wxDebugReportUpload::DoProcess()
{
    if ( !wxDebugReportCompress::DoProcess() )
        return false;


    wxArrayString output, errors;
    int rc = wxExecute(wxString::Format
                       (
                            _T("%s -F %s=@%s %s"),
                            m_curlCmd.c_str(),
                            m_inputField.c_str(),
                            GetCompressedFileName().c_str(),
                            m_uploadURL.c_str()
                       ),
                       output,
                       errors);
    if ( rc == -1 )
    {
        wxLogError(_("Failed to execute curl, please install it in PATH."));
    }
    else if ( rc != 0 )
    {
        const size_t count = errors.GetCount();
        if ( count )
        {
            for ( size_t n = 0; n < count; n++ )
            {
                wxLogWarning(_T("%s"), errors[n].c_str());
            }
        }

        wxLogError(_("Failed to upload the debug report (error code %d)."), rc);
    }
    else // rc == 0
    {
        if ( OnServerReply(output) )
            return true;
    }

    return false;
}

#endif // wxUSE_ZIPSTREAM

#endif // wxUSE_DEBUGREPORT

