/////////////////////////////////////////////////////////////////////////////
// Name:        filesys.cpp
// Purpose:     wxFileSystem class - interface for opening files
// Author:      Vaclav Slavik
// Copyright:   (c) 1999 Vaclav Slavik
// Licence:     wxWindows Licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation
#endif

#include <wx/wxprec.h>

#ifdef __BORDLANDC__
#pragma hdrstop
#endif

#if wxUSE_FS_INET || wxUSE_FS_ZIP

#ifndef WXPRECOMP
#include <wx/wx.h>
#endif

#include <wx/wfstream.h>
#include <wx/url.h>
#include <wx/module.h>
#include <wx/filesys.h>



//--------------------------------------------------------------------------------
// wxFileSystemHandler
//--------------------------------------------------------------------------------

IMPLEMENT_ABSTRACT_CLASS(wxFileSystemHandler, wxObject)

wxMimeTypesManager wxFileSystemHandler::m_MimeMng;


wxString wxFileSystemHandler::GetMimeTypeFromExt(const wxString& location)
{
    wxString ext = wxEmptyString, mime = wxEmptyString;
    wxString loc = GetRightLocation(location);
    char c;
    int l = loc.Length(), l2;
    wxFileType *ft;

    l2 = l;
    for (int i = l-1; i >= 0; i--) {
        c = loc[i];
        if (c == '#') l2 = i + 1;
        if (c == '.') {ext = loc.Right(l2-i-1); break;}
        if ((c == '/') || (c == '\\') || (c == ':')) {return wxEmptyString;}
    }
    ft = m_MimeMng.GetFileTypeFromExtension(ext);
    if (ft && (ft -> GetMimeType(&mime))) return mime;
    else return wxEmptyString;
}



wxString wxFileSystemHandler::GetProtocol(const wxString& location) const
{
    wxString s = wxEmptyString;
    int i, l = location.Length();
    bool fnd;

    fnd = FALSE;
    for (i = l-1; (i >= 0) && ((location[i] != '#') || (!fnd)); i--) {
        if ((location[i] == ':') && (i != 1 /*win: C:\path*/)) fnd = TRUE;
    }
    if (!fnd) return "file";
    for (++i; (i < l) && (location[i] != ':'); i++) s << location[i];
    return s;
}



wxString wxFileSystemHandler::GetLeftLocation(const wxString& location) const
{
    int i;
    bool fnd;

    fnd = FALSE;
    for (i = location.Length()-1; i >= 0; i--) {
        if ((location[i] == ':') && (i != 1 /*win: C:\path*/)) fnd = TRUE;
        else if (fnd && (location[i] == '#')) return location.Left(i);
    }
    return wxEmptyString;
}



wxString wxFileSystemHandler::GetRightLocation(const wxString& location) const
{
    int i, l = location.Length();
    int l2 = l + 1;
    for (i = l-1; (i >= 0) && ((location[i] != ':') || (i == 1) || (location[i-2] == ':')); i--) {if (location[i] == '#') l2 = i + 1;}
    if (i == 0) return wxEmptyString;
    else return location.Mid(i + 1, l2 - i - 2);
}



wxString wxFileSystemHandler::GetAnchor(const wxString& location) const
{
    char c;
    int l = location.Length();

    for (int i = l-1; i >= 0; i--) {
        c = location[i];
        if (c == '#') return location.Right(l-i-1);
        else if ((c == '.') || (c == '/') || (c == '\\') || (c == ':')) return wxEmptyString;
    }
    return wxEmptyString;
}





//--------------------------------------------------------------------------------
// wxLocalFSHandler
//--------------------------------------------------------------------------------

class wxLocalFSHandler : public wxFileSystemHandler
{
    public:
        virtual bool CanOpen(const wxString& location);
        virtual wxFSFile* OpenFile(wxFileSystem& fs, const wxString& location);
};



bool wxLocalFSHandler::CanOpen(const wxString& location)
{
    return GetProtocol(location) == "file";
}



wxFSFile* wxLocalFSHandler::OpenFile(wxFileSystem& WXUNUSED(fs), const wxString& location)
{
    wxString right = GetRightLocation(location);
    if (wxFileExists(right))
        return new wxFSFile(new wxFileInputStream(right),
                            right,
                            GetMimeTypeFromExt(location),
                            GetAnchor(location));
    else return NULL;
}






//-----------------------------------------------------------------------------
// wxFileSystem
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxFileSystem, wxObject)


wxList wxFileSystem::m_Handlers;



void wxFileSystem::ChangePathTo(const wxString& location, bool is_dir)
{
    int i, pathpos = -1;
    m_Path = location;

    for (i = m_Path.Length()-1; i >= 0; i--)
        if (m_Path[i] == '\\') m_Path.GetWritableChar(i) = '/';         // wanna be windows-safe

    if (is_dir == FALSE) {
        for (i = m_Path.Length()-1; i >= 0; i--) {
            if (m_Path[i] == '/') {
                if ((i > 1) && (m_Path[i-1] == '/') && (m_Path[i-2] == ':')) {
                    i -= 2;
                    continue;
                }
                else {
                    pathpos = i; 
                    break;
                }
            }
        else if (m_Path[i] == ':') {
            pathpos = i;
        break;
        }
    }
        if (pathpos == -1) {
            for (i = 0; i < (int) m_Path.Length(); i++) {
                if (m_Path[i] == ':') {
                    //m_Path << '/';
                    m_Path.Remove(i+1);
                    break;
                }
            }
            if (i == (int) m_Path.Length()) m_Path = wxEmptyString;
        }
        else {
            if (m_Path[m_Path.Length()-1] != '/') m_Path << '/';
            m_Path.Remove(pathpos+1);
        }
    }
}



wxFSFile* wxFileSystem::OpenFile(const wxString& location)
{
    wxString loc = location;
    int i, ln;
    char meta;
    wxFSFile *s = NULL;
    wxNode *node;

    ln = loc.Length();
    meta = 0;
    for (i = 0; i < ln; i++) {
        if (loc[i] == '\\') loc.GetWritableChar(i) = '/';         // wanna be windows-safe
        if (!meta) switch (loc[i]) {
            case '/' : case ':' : case '#' : meta = loc[i];
        }
    }
    m_LastName = wxEmptyString;

    // try relative paths first :
    if (meta != ':') {
        node = m_Handlers.GetFirst();
        while (node){
            wxFileSystemHandler *h = (wxFileSystemHandler*) node -> GetData();
            if (h -> CanOpen(m_Path + location)) {
                s = h -> OpenFile(*this, m_Path + location);
                if (s) {m_LastName = m_Path + location; break;}
            }
            node = node -> GetNext();
        }
    }

    // if failed, try absolute paths :
    if (s == NULL) {
        node = m_Handlers.GetFirst();
        while (node){
            wxFileSystemHandler *h = (wxFileSystemHandler*) node -> GetData();
            if (h -> CanOpen(location)) {
                s = h -> OpenFile(*this, location);
                if (s) {m_LastName = location; break; }
            }
            node = node -> GetNext();
        }
    }
    return (s);
}



void wxFileSystem::AddHandler(wxFileSystemHandler *handler)
{
    m_Handlers.Append(handler);
}











///// Module:

class wxFileSystemModule : public wxModule
{
    DECLARE_DYNAMIC_CLASS(wxFileSystemModule)

    public:
        virtual bool OnInit()
        {
            wxFileSystem::AddHandler(new wxLocalFSHandler);
            return TRUE;
        }
        virtual void OnExit() {}
};

IMPLEMENT_DYNAMIC_CLASS(wxFileSystemModule, wxModule)



#endif // wxUSE_FS_INET || wxUSE_FS_ZIP

