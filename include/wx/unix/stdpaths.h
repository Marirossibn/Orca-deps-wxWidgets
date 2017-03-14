///////////////////////////////////////////////////////////////////////////////
// Name:        wx/unix/stdpaths.h
// Purpose:     wxStandardPaths for Unix systems
// Author:      Vadim Zeitlin
// Modified by:
// Created:     2004-10-19
// Copyright:   (c) 2004 Vadim Zeitlin <vadim@wxwindows.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_UNIX_STDPATHS_H_
#define _WX_UNIX_STDPATHS_H_

// ----------------------------------------------------------------------------
// wxStandardPaths
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_BASE wxStandardPaths : public wxStandardPathsBase
{
public:
    // tries to determine the installation prefix automatically (Linux only right
    // now) and returns /usr/local if it failed
    void DetectPrefix();

    // set the program installation directory which is /usr/local by default
    //
    // under some systems (currently only Linux) the program directory can be
    // determined automatically but for portable programs you should always set
    // it explicitly
    void SetInstallPrefix(const wxString& prefix);

    // get the program installation prefix
    //
    // if the prefix had been previously by SetInstallPrefix, returns that
    // value, otherwise calls DetectPrefix()
    wxString GetInstallPrefix() const;


    // implement base class pure virtuals
    virtual wxString GetExecutablePath() const wxOVERRIDE;
    virtual wxString GetConfigDir() const wxOVERRIDE;
    virtual wxString GetUserConfigDir() const wxOVERRIDE;
    virtual wxString GetDataDir() const wxOVERRIDE;
    virtual wxString GetLocalDataDir() const wxOVERRIDE;
    virtual wxString GetUserDataDir() const wxOVERRIDE;
    virtual wxString GetPluginsDir() const wxOVERRIDE;
    virtual wxString GetLocalizedResourcesDir(const wxString& lang,
                                              ResourceCat category) const wxOVERRIDE;
#ifndef __VMS
    virtual wxString GetUserDir(Dir userDir) const wxOVERRIDE;
#endif
    virtual wxString MakeConfigFileName(const wxString& basename,
                                        ConfigFileConv conv) const wxOVERRIDE;

protected:
    // Ctor is protected, use wxStandardPaths::Get() instead of instantiating
    // objects of this class directly.
    wxStandardPaths() { }

private:
    wxString m_prefix;
};

#endif // _WX_UNIX_STDPATHS_H_

