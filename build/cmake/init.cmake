#############################################################################
# Name:        build/cmake/init.cmake
# Purpose:     Initialize variables based on user selection and system
#              information before creating build targets
# Author:      Tobias Taschner
# Created:     2016-09-24
# Copyright:   (c) 2016 wxWidgets development team
# Licence:     wxWindows licence
#############################################################################

if(MSVC)
    # Determine MSVC runtime library flag
    set(MSVC_LIB_USE "/MD")
    set(MSVC_LIB_REPLACE "/MT")
    if(wxBUILD_USE_STATIC_RUNTIME)
        set(MSVC_LIB_USE "/MT")
        set(MSVC_LIB_REPLACE "/MD")
    endif()
    # Set MSVC runtime flags for all configurations
    foreach(cfg "" ${CMAKE_CONFIGURATION_TYPES})
        set(c_flag_var CMAKE_C_FLAGS)
        set(cxx_flag_var CMAKE_CXX_FLAGS)
        if(cfg)
            string(TOUPPER ${cfg} cfg_upper)
            wx_string_append(c_flag_var "_${cfg_upper}")
            wx_string_append(cxx_flag_var "_${cfg_upper}")
        endif()
        if(${c_flag_var} MATCHES ${MSVC_LIB_REPLACE})
            string(REPLACE ${MSVC_LIB_REPLACE} ${MSVC_LIB_USE} ${c_flag_var} "${${c_flag_var}}")
            set(${c_flag_var} ${${c_flag_var}} CACHE STRING
              "Flags used by the C compiler during ${cfg_upper} builds." FORCE)
        endif()
        if(${cxx_flag_var} MATCHES ${MSVC_LIB_REPLACE})
            string(REPLACE ${MSVC_LIB_REPLACE} ${MSVC_LIB_USE} ${cxx_flag_var} "${${cxx_flag_var}}")
            set(${cxx_flag_var} ${${cxx_flag_var}} CACHE STRING
              "Flags used by the CXX compiler during ${cfg_upper} builds." FORCE)
        endif()
    endforeach()
    
    if(wxBUILD_MSVC_MULTIPROC)
        wx_string_append(CMAKE_C_FLAGS " /MP")
        wx_string_append(CMAKE_CXX_FLAGS " /MP")
    endif()
elseif(CMAKE_COMPILER_IS_GNUCC)
    if(wxBUILD_USE_STATIC_RUNTIME)
        set(STATIC_LINKER_FLAGS " -static-libgcc -static-libstdc++")
        wx_string_append(CMAKE_EXE_LINKER_FLAGS "${STATIC_LINKER_FLAGS}")
        wx_string_append(CMAKE_SHARED_LINKER_FLAGS "${STATIC_LINKER_FLAGS}")
    endif()
endif()

if(wxBUILD_COMPATIBILITY VERSION_LESS 3.0)
    set(WXWIN_COMPATIBILITY_2_8 ON)
endif()
if(wxBUILD_COMPATIBILITY VERSION_LESS 3.1)
    set(WXWIN_COMPATIBILITY_3_0 ON)
endif()

# Build wxBUILD_FILE_ID used for config and setup path
#TODO: build different id for WIN32
set(wxBUILD_FILE_ID "${wxBUILD_TOOLKIT}${wxBUILD_WIDGETSET}-")
if(wxUSE_UNICODE)
    wx_string_append(wxBUILD_FILE_ID "unicode")
else()
    wx_string_append(wxBUILD_FILE_ID "ansi")
endif()
if(NOT wxBUILD_SHARED)
    wx_string_append(wxBUILD_FILE_ID "-static")
endif()
wx_string_append(wxBUILD_FILE_ID "-${wxMAJOR_VERSION}.${wxMINOR_VERSION}")

set(wxARCH_SUFFIX)

# TODO: include compiler version in wxCOMPILER_PREFIX ?
if(WIN32)
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        set(wxCOMPILER_PREFIX "vc")
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        set(wxCOMPILER_PREFIX "gcc")
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(wxCOMPILER_PREFIX "clang")
    else()
        message(FATAL_ERROR "Unknown WIN32 compiler type")
    endif()

    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(wxARCH_SUFFIX "_x64")
    endif()
else()
    set(wxCOMPILER_PREFIX)
endif()

if(MSVC OR MINGW)
    if(wxBUILD_SHARED)
        set(lib_suffix "dll")
    else()
        set(lib_suffix "lib")
    endif()

    if(MSVC)
        # Include generator expression to suppress default Debug/Release pair
        set(wxPLATFORM_LIB_DIR "$<1:/>${wxCOMPILER_PREFIX}${wxARCH_SUFFIX}_${lib_suffix}")
    else()
        set(wxPLATFORM_LIB_DIR "/${wxCOMPILER_PREFIX}${wxARCH_SUFFIX}_${lib_suffix}")
    endif()
else()
    set(wxPLATFORM_LIB_DIR)
endif()

if(wxBUILD_CUSTOM_SETUP_HEADER_PATH)
    if(NOT EXISTS "${wxBUILD_CUSTOM_SETUP_HEADER_PATH}/wx/setup.h")
        message(FATAL_ERROR "wxBUILD_CUSTOM_SETUP_HEADER_PATH needs to contain a wx/setup.h file")
    endif()
    set(wxSETUP_HEADER_PATH ${wxBUILD_CUSTOM_SETUP_HEADER_PATH})
else()
    # Set path where setup.h will be created
    if(MSVC OR MINGW)
        if(wxUSE_UNICODE)
            set(lib_unicode u)
        else()
            set(lib_unicode)
        endif()
        set(wxSETUP_HEADER_PATH
            ${wxOUTPUT_DIR}/${wxCOMPILER_PREFIX}${wxARCH_SUFFIX}_${lib_suffix}/${wxBUILD_TOOLKIT}${lib_unicode})
        file(MAKE_DIRECTORY ${wxSETUP_HEADER_PATH}/wx)
        file(MAKE_DIRECTORY ${wxSETUP_HEADER_PATH}d/wx)
        set(wxSETUP_HEADER_FILE_DEBUG ${wxSETUP_HEADER_PATH}d/wx/setup.h)
    else()
        set(wxSETUP_HEADER_PATH
            ${wxOUTPUT_DIR}/wx/include/${wxBUILD_FILE_ID})
        file(MAKE_DIRECTORY ${wxSETUP_HEADER_PATH}/wx)
    endif()
endif()
set(wxSETUP_HEADER_FILE ${wxSETUP_HEADER_PATH}/wx/setup.h)

if(DEFINED wxSETUP_HEADER_FILE_DEBUG)
    # Append configuration specific suffix to setup header path
    wx_string_append(wxSETUP_HEADER_PATH "$<$<CONFIG:Debug>:d>")
endif()

if(wxUSE_ON_FATAL_EXCEPTION AND MSVC AND (MSVC_VERSION GREATER 1800) )
    # see include/wx/msw/seh.h for more details
    add_compile_options("/EHa")
endif()

if(NOT wxBUILD_DEBUG_LEVEL STREQUAL "Default")
    add_compile_options("-DwxDEBUG_LEVEL=${wxBUILD_DEBUG_LEVEL}")
endif()

# Constants for setup.h creation
set(wxUSE_STD_DEFAULT ON)
if(wxUSE_UNICODE)
    set(wxUSE_WCHAR_T ON)
endif()
if(NOT wxUSE_EXPAT)
    set(wxUSE_XRC OFF)
endif()
set(wxUSE_XML ${wxUSE_XRC})

if(DEFINED wxUSE_OLE AND wxUSE_OLE)
    set(wxUSE_OLE_AUTOMATION ON)
endif()

if(wxUSE_OPENGL)
    set(wxUSE_GLCANVAS ON)
endif()

if(wxUSE_ARCHIVE_STREAMS AND NOT wxUSE_STREAMS)
    message(WARNING "wxArchive requires wxStreams... disabled")
    wx_option_force_value(wxUSE_ARCHIVE_STREAMS OFF)
endif()

if(wxUSE_ZIPSTREAM AND (NOT wxUSE_ARCHIVE_STREAMS OR NOT wxUSE_ZLIB))
    message(WARNING "wxZip requires wxArchive or wxZlib... disabled")
    wx_option_force_value(wxUSE_ZIPSTREAM OFF)
endif()

if(wxUSE_TARSTREAM AND NOT wxUSE_ARCHIVE_STREAMS)
    message(WARNING "wxTar requires wxArchive... disabled")
    wx_option_force_value(wxUSE_TARSTREAM OFF)
endif()

if(wxUSE_FILESYSTEM AND (NOT wxUSE_STREAMS OR (NOT wxUSE_FILE AND NOT wxUSE_FFILE)))
    message(WARNING "wxFileSystem requires wxStreams and wxFile or wxFFile... disabled")
    wx_option_force_value(wxUSE_FILESYSTEM OFF)
endif()

if(wxUSE_FS_ARCHIVE AND (NOT wxUSE_FILESYSTEM OR NOT wxUSE_ARCHIVE_STREAMS))
    message(WARNING "wxArchiveFSHandler requires wxArchive and wxFileSystem... disabled")
    wx_option_force_value(wxUSE_FS_ARCHIVE OFF)
endif()

if(wxUSE_FS_ARCHIVE AND (NOT wxUSE_FILESYSTEM OR NOT wxUSE_ARCHIVE_STREAMS))
    message(WARNING "wxArchiveFSHandler requires wxArchive and wxFileSystem... disabled")
    wx_option_force_value(wxUSE_FS_ARCHIVE OFF)
endif()

if(wxUSE_FS_ZIP AND NOT wxUSE_FS_ARCHIVE)
    message(WARNING "wxZipFSHandler requires wxArchiveFSHandler... disabled")
    wx_option_force_value(wxUSE_FS_ZIP OFF)
endif()

if(wxUSE_TEXTFILE AND (NOT wxUSE_FILE OR NOT wxUSE_TEXTBUFFER))
    message(WARNING "wxTextFile requires wxFile and wxTextBuffer... disabled")
    wx_option_force_value(wxUSE_TEXTFILE OFF)
endif()

if(wxUSE_CONFIG)
    if(NOT wxUSE_TEXTFILE)
        message(WARNING "wxConfig requires wxTextFile... disabled")
        wx_option_force_value(wxUSE_CONFIG OFF)
    else()
        set(wxUSE_CONFIG_NATIVE ON)
    endif()
endif()

if(wxUSE_INTL AND NOT wxUSE_FILE)
    message(WARNING "I18n code requires wxFile... disabled")
    wx_option_force_value(wxUSE_INTL OFF)
endif()

if(wxUSE_THREADS)
    find_package(Threads REQUIRED)
endif()

if(wxUSE_LIBLZMA)
    find_package(LibLZMA)
    if(NOT LIBLZMA_FOUND)
        message(WARNING "libLZMA not found, LZMA compression won't be available")
        wx_option_force_value(wxUSE_LIBLZMA OFF)
    endif()
endif()

if(UNIX)
    if(wxUSE_SECRETSTORE AND NOT APPLE)
        # The required APIs are always available under MSW and OS X but we must
        # have GNOME libsecret under Unix to be able to compile this class.
        find_package(Libsecret)
        if(NOT LIBSECRET_FOUND)
            message(WARNING "libsecret not found, wxSecretStore won't be available")
            wx_option_force_value(wxUSE_SECRETSTORE OFF)
        endif()
    endif()

    if(wxUSE_LIBICONV)
        find_package(Iconv)
        if(NOT ICONV_FOUND)
            message(WARNING "iconv not found")
            wx_option_force_value(wxUSE_LIBICONV OFF)
        endif()
    endif()
endif(UNIX)

if(wxUSE_GUI)
    if(WXMSW AND wxUSE_METAFILE)
        # this one should probably be made separately configurable
        set(wxUSE_ENH_METAFILE ON)
    endif()

    # Direct2D check
    if(WIN32 AND wxUSE_GRAPHICS_DIRECT2D)
        check_include_file(d2d1.h HAVE_D2D1_H)
        if (NOT HAVE_D2D1_H)
            wx_option_force_value(wxUSE_GRAPHICS_DIRECT2D OFF)
        endif()
    endif()
     if(MSVC) # match setup.h
        if(MSVC_VERSION LESS 1600)
            wx_option_force_value(wxUSE_GRAPHICS_DIRECT2D OFF)
        else()
            wx_option_force_value(wxUSE_GRAPHICS_DIRECT2D ${wxUSE_GRAPHICS_CONTEXT})
        endif()
     endif()

    # WXQT checks
    if(WXQT)
        wx_option_force_value(wxUSE_WEBVIEW OFF)
        wx_option_force_value(wxUSE_METAFILE OFF)
        if(WIN32)
            wx_option_force_value(wxUSE_ACCESSIBILITY OFF)
            wx_option_force_value(wxUSE_OWNER_DRAWN OFF)
        endif()
    endif()

    # WXGTK checks, match include/wx/gtk/chkconf.h
    if(WXGTK)
        wx_option_force_value(wxUSE_METAFILE OFF)

        if(WIN32)
            wx_option_force_value(wxUSE_CAIRO ON)
            wx_option_force_value(wxUSE_ACCESSIBILITY OFF)
            wx_option_force_value(wxUSE_OWNER_DRAWN OFF)
        endif()

        if(NOT UNIX)
            wx_option_force_value(wxUSE_WEBVIEW OFF)
            wx_option_force_value(wxUSE_MEDIACTRL OFF)
            wx_option_force_value(wxUSE_UIACTIONSIMULATOR OFF)
            wx_option_force_value(wxUSE_OPENGL OFF)
            set(wxUSE_GLCANVAS OFF)
        endif()
    endif()

    # extra dependencies
    if(wxUSE_OPENGL)
        find_package(OpenGL)
        if(NOT OPENGL_FOUND)
            message(WARNING "opengl not found, wxGLCanvas won't be available")
            wx_option_force_value(wxUSE_OPENGL OFF)
        endif()
    endif()

    if(wxUSE_WEBVIEW)
        if(WXGTK)
            if(wxUSE_WEBVIEW_WEBKIT)
                find_package(LibSoup)
                if(WXGTK2)
                    find_package(Webkit 1.0)
                elseif(WXGTK3)
                    find_package(Webkit2)
                    if(NOT WEBKIT2_FOUND)
                        find_package(Webkit 3.0)
                    endif()
                endif()
            endif()
            set(wxUSE_WEBVIEW_WEBKIT OFF)
            set(wxUSE_WEBVIEW_WEBKIT2 OFF)
            if(WEBKIT_FOUND AND LIBSOUP_FOUND)
                set(wxUSE_WEBVIEW_WEBKIT ON)
            elseif(WEBKIT2_FOUND AND LIBSOUP_FOUND)
                set(wxUSE_WEBVIEW_WEBKIT2 ON)
            else()
                message(WARNING "webkit not found or enabled, wxWebview won't be available")
                wx_option_force_value(wxUSE_WEBVIEW OFF)
            endif()
        elseif(WXMSW)
            if(NOT wxUSE_WEBVIEW_IE)
                message(WARNING "WebviewIE not found or enabled, wxWebview won't be available")
                wx_option_force_value(wxUSE_WEBVIEW OFF)
            endif()
        elseif(APPLE)
            if(NOT wxUSE_WEBVIEW_WEBKIT)
                message(WARNING "webkit not found or enabled, wxWebview won't be available")
                wx_option_force_value(wxUSE_WEBVIEW OFF)
            endif()
        endif()
    endif()

    if(wxUSE_PRIVATE_FONTS AND WXGTK)
        find_package(Fontconfig)
        if(NOT FONTCONFIG_FOUND)
            message(WARNING "Fontconfig not found, Private fonts won't be available")
            wx_option_force_value(wxUSE_PRIVATE_FONTS OFF)
        endif()
    endif()

    if(wxUSE_MEDIACTRL AND UNIX AND NOT APPLE AND NOT WIN32)
        find_package(GStreamer 1.0 COMPONENTS video)
        if(NOT GSTREAMER_FOUND)
            find_package(GStreamer 0.10 COMPONENTS interfaces)
        endif()

        set(wxUSE_GSTREAMER ${GSTREAMER_FOUND})
        set(wxUSE_GSTREAMER_PLAYER OFF)
        if(GSTREAMER_PLAYER_INCLUDE_DIRS)
            set(wxUSE_GSTREAMER_PLAYER ON)
        endif()

        if(NOT GSTREAMER_FOUND)
            message(WARNING "GStreamer not found, wxMediaCtrl won't be available")
            wx_option_force_value(wxUSE_MEDIACTRL OFF)
        endif()
    else()
        set(wxUSE_GSTREAMER OFF)
        set(wxUSE_GSTREAMER_PLAYER OFF)
    endif()

    if(wxUSE_SOUND AND wxUSE_LIBSDL AND UNIX AND NOT APPLE)
        find_package(SDL2)
        if(NOT SDL2_FOUND)
            find_package(SDL)
        endif()
        if(NOT SDL2_FOUND AND NOT SDL_FOUND)
            message(WARNING "SDL not found, SDL Audio back-end won't be available")
            wx_option_force_value(wxUSE_LIBSDL OFF)
        endif()
    else()
        set(wxUSE_LIBSDL OFF)
    endif()

    if(wxUSE_NOTIFICATION_MESSAGE AND UNIX AND WXGTK2 AND wxUSE_LIBNOTIFY)
        find_package(LibNotify)
        if(NOT LIBNOTIFY_FOUND)
            message(WARNING "Libnotify not found, it won't be used for notifications")
            wx_option_force_value(wxUSE_LIBNOTIFY OFF)
        elseif((LIBNOTIFY_VERSION GREATER 0.7) OR (LIBNOTIFY_VERSION EQUAL 0.7))
            set(wxUSE_LIBNOTIFY_0_7 ON)
        endif()
    else()
        set(wxUSE_LIBNOTIFY OFF)
    endif()

    if(wxUSE_UIACTIONSIMULATOR AND UNIX AND WXGTK)
        if(wxUSE_XTEST)
            find_package(XTest)
            if(XTEST_FOUND)
                list(APPEND wxTOOLKIT_INCLUDE_DIRS ${XTEST_INCLUDE_DIRS})
                list(APPEND wxTOOLKIT_LIBRARIES ${XTEST_LIBRARIES})
            else()
                if(WXGTK3)
                    # This class can't work without XTest with GTK+ 3
                    # which uses XInput2 and so ignores XSendEvent().
                    message(STATUS "XTest not found, wxUIActionSimulator won't be available")
                    wx_option_force_value(wxUSE_UIACTIONSIMULATOR OFF)
                endif()
                # The other ports can use XSendEvent(), so don't warn
                wx_option_force_value(wxUSE_XTEST OFF)
            endif()
        else(WXGTK3)
            # As per above, wxUIActionSimulator can't be used in this case,
            # but there is no need to warn, presumably the user knows what
            # he's doing if wxUSE_XTEST was explicitly disabled.
            wx_option_force_value(wxUSE_UIACTIONSIMULATOR OFF)
        endif()
    endif()

    if(wxUSE_HTML AND UNIX AND wxUSE_LIBMSPACK)
        find_package(MSPACK)
        if(NOT MSPACK_FOUND)
            message(STATUS "libmspack not found")
            wx_option_force_value(wxUSE_LIBMSPACK OFF)
        endif()
    else()
        set(wxUSE_LIBMSPACK OFF)
    endif()

    if(WXGTK2 AND wxUSE_MIMETYPE AND wxUSE_LIBGNOMEVFS)
        find_package(GnomeVFS2)
        if(GNOMEVFS2_FOUND)
            list(APPEND wxTOOLKIT_INCLUDE_DIRS ${GNOMEVFS2_INCLUDE_DIRS})
            list(APPEND wxTOOLKIT_LIBRARIES ${GNOMEVFS2_LIBRARIES})
        else()
            message(STATUS "libgnomevfs not found, library won't be used to associate MIME type")
            wx_option_force_value(wxUSE_LIBGNOMEVFS OFF)
        endif()
    else()
        set(wxUSE_LIBGNOMEVFS OFF)
    endif()
endif()
