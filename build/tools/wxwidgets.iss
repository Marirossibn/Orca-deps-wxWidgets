; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define INNODIR GetENV("INNO")

#if GetEnv("WXW_VER") == "SVN"
    #define INFOFILE  GetENV("INNO") + "\BuildSVN.txt"
    #define WX_VERSION "SVN"
#else
    #define INFOFILE GetENV("INNO") + "\docs\msw\install.txt"
    #define WX_VERSION GetENV("WXW_VER")
#endif

#define SETUPFILENAME  "wxMSW-" + GetENV("WXW_VER") + "-Setup"



[Setup]
AppName=wxWidgets
AppVerName=wxWidgets {#WX_VERSION}
AppPublisher=wxWidgets
AppPublisherURL=http://www.wxwidgets.org
AppSupportURL=http://www.wxwidgets.org
AppUpdatesURL=http://www.wxwidgets.org
DefaultDirName={sd}\wxWidgets-{#WX_VERSION}
DefaultGroupName=wxWidgets {#WX_VERSION}
UsePreviousAppDir=no
DisableProgramGroupPage=yes
LicenseFile={#INNODIR}\docs\licence.txt
InfoBeforeFile={#INNODIR}\docs\readme.txt
InfoAfterFile={#INFOFILE}
OutputDir=c:\daily
OutputBaseFilename={#SETUPFILENAME}
SetupIconFile={#INNODIR}\art\wxwin.ico
Compression=lzma
SolidCompression=yes

[Files]
; source files
Source: "{#INNODIR}\*"; DestDir: "{app}"; Excludes: ".cvsignore,cocoa,dfb,mac,gtk,gtk1,mgl,motif,msdos,os2,palmos,wxWindows.xcod*,x11,debian,distrib,docs,misc,tests,wxPython,*.pch,*.pch++,*.pchmm"; Flags: ignoreversion recursesubdirs createallsubdirs
; docs and helpfile
Source: "{#INNODIR}\docs\htmlhelp\*"; DestDir: "{app}\docs\htmlhelp\";  Flags: ignoreversion recursesubdirs createallsubdirs
Source: "{#INNODIR}\docs\licence.txt"; DestDir: "{app}\docs\";  Flags: ignoreversion recursesubdirs createallsubdirs
Source: "{#INNODIR}\docs\readme.txt"; DestDir: "{app}\docs\";  Flags: ignoreversion recursesubdirs createallsubdirs
Source: "{#INNODIR}\docs\changes.txt"; DestDir: "{app}\docs\";  Flags: ignoreversion recursesubdirs createallsubdirs
Source: "{#INNODIR}\docs\msw\install.txt"; DestDir: "{app}\docs\msw";  Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[INI]
Filename: "{app}\wx.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://www.wxwidgets.org"

[Icons]
Name: "{group}\{cm:ProgramOnTheWeb,wxWidgets {#WX_VERSION}}"; Filename: "{app}\wx.url"
Name: {group}\wxWidgets Manual; Filename: {app}\docs\htmlhelp\wx.chm; WorkingDir: {app}; IconIndex: 0; Flags: useapppaths
Name: {group}\Changes; Filename: {app}\docs\changes.txt; WorkingDir: {app}; IconIndex: 0; Flags: useapppaths
Name: {group}\Readme; Filename: {app}\docs\readme.txt; WorkingDir: {app}; IconIndex: 0; Flags: useapppaths
Name: {group}\Compiling wxWidgets; Filename: {app}\docs\msw\install.txt; WorkingDir: {app}; IconIndex: 0; Flags: useapppaths
Name: "{group}\Uninstall wxWidgets {#WX_VERSION}"; Filename: "{uninstallexe}"


[UninstallDelete]
Type: files; Name: "{app}\wx.url"

