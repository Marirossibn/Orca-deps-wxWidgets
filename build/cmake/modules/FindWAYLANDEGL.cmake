find_package(PkgConfig)
pkg_check_modules(WAYLANDEGL QUIET wayland-egl)

set(VERSION_OK TRUE)
if (WAYLANDEGL_VERSION)
    if (WAYLANDEGL_FIND_VERSION_EXACT)
        if (NOT("${WAYLANDEGL_FIND_VERSION}" VERSION_EQUAL "${WAYLANDEGL_VERSION}"))
            set(VERSION_OK FALSE)
        endif ()
    else ()
        if ("${WAYLANDEGL_VERSION}" VERSION_LESS "${WAYLANDEGL_FIND_VERSION}")
            set(VERSION_OK FALSE)
        endif ()
    endif ()
endif ()

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(WAYLANDEGL DEFAULT_MSG WAYLANDEGL_LIBRARIES VERSION_OK)
