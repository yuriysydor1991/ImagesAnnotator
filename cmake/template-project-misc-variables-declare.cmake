cmake_minimum_required(VERSION 3.13)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(
  PROJECT_BINARY_NAME ${CMAKE_PROJECT_NAME}
  CACHE STRING "Project main binary name and target"
)

string(TOLOWER ${PROJECT_BINARY_NAME} PROJECT_BINARY_NAME_lower)

# ImagesAnnotator specific: the GTKmm3 based object library target name shared
# between the gtkmm3 sources and their component/unit tests.
set(
  GTKMM_OBJECT_NAME "${PROJECT_NAME}GTKmmObj"
  CACHE STRING "Project GTKmm based object library name and target"
)

# ---------------------------------------------------------------------------
# ImagesAnnotator specific: GUI toolkit selection.
#
# The project ships two interchangeable GTKmm GUI implementations under src/gui
# (gtkmm3 and gtkmm4) and compiles exactly one of them into the single
# application binary. This block is the only place in the whole project that
# branches on the choice: everything that follows from it (the pkg-config probe,
# the src/gui subdirectory, the USE_GTKMM<N> compile definition and the DEB
# runtime dependency) is declared unconditionally by the matching enabler,
# cmake/enablers/template-project-Gtkmm3-enabler.cmake or
# cmake/enablers/template-project-Gtkmm4-enabler.cmake.
#
# Whichever of the two options the user leaves unspecified is derived from the
# one that was given, so the pair can never end up inconsistent by accident:
#
#   (nothing given)   ->  USE_GTKMM3=ON,  USE_GTKMM4=OFF   (the default)
#   -DUSE_GTKMM3=ON   ->  USE_GTKMM4=OFF  (derived)
#   -DUSE_GTKMM3=OFF  ->  USE_GTKMM4=ON   (derived)
#   -DUSE_GTKMM4=ON   ->  USE_GTKMM3=OFF  (derived)
#   -DUSE_GTKMM4=OFF  ->  USE_GTKMM3=ON   (derived)
#   both given        ->  honoured when exactly one is ON, fatal error otherwise
#
# GTKMM_SELECTED_VERSION records the resolved choice in the cache, which is what
# makes re-configuring an existing build tree behave: a bare "cmake ." keeps the
# toolkit the tree was configured with, while passing a single option again (for
# example switching a GTKmm4 tree with -DUSE_GTKMM3=ON) is understood as a
# deliberate change of that one option rather than as "both are now enabled".
# ---------------------------------------------------------------------------
set(GTKMM_DEFAULT_VERSION 3)

# ON/OFF that the previous configure run resolved for each option, empty on a
# fresh build tree.
set(GTKMM_PREVIOUS_3 "")
set(GTKMM_PREVIOUS_4 "")
if(GTKMM_SELECTED_VERSION)
  set(GTKMM_PREVIOUS_3 OFF)
  set(GTKMM_PREVIOUS_4 OFF)
  set(GTKMM_PREVIOUS_${GTKMM_SELECTED_VERSION} ON)
endif()

# An option counts as requested when it is present in the cache with a value
# that differs from the one the previous run resolved for it - that is, when the
# user actually asked for something new on this command line. On a fresh tree
# there is nothing to compare against, so any given value counts.
foreach(GTKMM_OPTION_VERSION 3 4)
  set(GTKMM_REQUESTED_${GTKMM_OPTION_VERSION} "")

  if(DEFINED USE_GTKMM${GTKMM_OPTION_VERSION})
    # Normalise whatever the user wrote (ON / TRUE / 1 / YES / ...) to a strict
    # ON or OFF so the comparison below is reliable.
    set(GTKMM_GIVEN_VALUE OFF)
    if(USE_GTKMM${GTKMM_OPTION_VERSION})
      set(GTKMM_GIVEN_VALUE ON)
    endif()

    if(NOT "${GTKMM_GIVEN_VALUE}" STREQUAL "${GTKMM_PREVIOUS_${GTKMM_OPTION_VERSION}}")
      set(GTKMM_REQUESTED_${GTKMM_OPTION_VERSION} ${GTKMM_GIVEN_VALUE})
    endif()
  endif()
endforeach()

if(GTKMM_REQUESTED_3 AND GTKMM_REQUESTED_4)
  message(FATAL_ERROR
    "USE_GTKMM3 and USE_GTKMM4 are mutually exclusive - enable exactly one "
    "(USE_GTKMM3 is the default). Enabling one alone is enough, the other is "
    "switched off automatically: re-run cmake with just -DUSE_GTKMM4=ON.")
endif()

if("${GTKMM_REQUESTED_3}" STREQUAL "OFF" AND "${GTKMM_REQUESTED_4}" STREQUAL "OFF")
  message(FATAL_ERROR
    "Neither USE_GTKMM3 nor USE_GTKMM4 is enabled - enable exactly one GUI "
    "toolkit (USE_GTKMM3 is the default).")
endif()

if(GTKMM_REQUESTED_3)
  set(GTKMM_SELECTED 3)
elseif(GTKMM_REQUESTED_4)
  set(GTKMM_SELECTED 4)
elseif("${GTKMM_REQUESTED_3}" STREQUAL "OFF")
  set(GTKMM_SELECTED 4)
elseif("${GTKMM_REQUESTED_4}" STREQUAL "OFF")
  set(GTKMM_SELECTED 3)
elseif(GTKMM_SELECTED_VERSION)
  set(GTKMM_SELECTED ${GTKMM_SELECTED_VERSION})
else()
  set(GTKMM_SELECTED ${GTKMM_DEFAULT_VERSION})
endif()

# Publish the resolution. Both options are always written back, so the cache (and
# ccmake / cmake-gui) always shows a consistent pair no matter which single
# option the user passed, and the choice is remembered for the next run.
set(GTKMM_RESOLVED_3 OFF)
set(GTKMM_RESOLVED_4 OFF)
set(GTKMM_RESOLVED_${GTKMM_SELECTED} ON)

set(USE_GTKMM3 ${GTKMM_RESOLVED_3} CACHE BOOL
  "Build the GTKmm3 (GTK 3) based GUI. Enabling it switches USE_GTKMM4 off." FORCE)
set(USE_GTKMM4 ${GTKMM_RESOLVED_4} CACHE BOOL
  "Build the GTKmm4 (GTK 4) based GUI. Enabling it switches USE_GTKMM3 off." FORCE)

set(GTKMM_SELECTED_VERSION ${GTKMM_SELECTED} CACHE INTERNAL
  "GTKmm major version resolved from the USE_GTKMM3 / USE_GTKMM4 pair")

message(STATUS "GUI toolkit: GTKmm${GTKMM_SELECTED} "
  "(USE_GTKMM3=${USE_GTKMM3}, USE_GTKMM4=${USE_GTKMM4})")

# ---------------------------------------------------------------------------
# ImagesAnnotator specific: the desktop application id (reverse-DNS).
#
# This single, always-defined name is what ties the whole desktop integration
# together: it is the GApplication id handed to Gtk::Application::create(), the
# Wayland app_id (pinned at runtime via Glib::set_prgname()), the basename of
# the installed <id>.desktop launcher and the name of the installed themed icon.
# On Wayland (and the GNOME Shell dash) the panel icon is resolved by matching
# the running window's app_id to that .desktop file - the icon embedded in the
# binary is ignored - so all four must agree on this exact string.
#
# It is derived from the resolved toolkit so it lines up with the per-toolkit
# GResource prefix (/ua/org/kytok/template/gtkmm<N>/...) and, for the GTKmm3
# flatpak, with the flatpak application id.
#
# IMPORTANT: defining it here - unconditionally, before add_subdirectory(src)
# configures project-global-decls.h.in - is also what finally gives the C++
# PROJECT_FLATPAK_URL a non-empty value in ordinary (non-flatpak) builds. It
# used to be set only inside the flatpak enabler (guarded by ENABLE_FLATPAK and
# included after src), so the application ran with an empty application id.
set(APPLICATION_ID "ua.org.kytok.template.gtkmm${GTKMM_SELECTED}.${PROJECT_BINARY_NAME}")

# The flatpak application id is the same reverse-DNS name; keep them in sync from
# this single source instead of re-hardcoding it in the flatpak enabler.
set(FLATPAK_PROJECT_URL "${APPLICATION_ID}")

message(STATUS "Application id: ${APPLICATION_ID}")

set(
  PROJECT_MAINTAINER "Yuriy Sydor"
  CACHE STRING "Project maintainer name (used for the DEB package)"
)

set(
  PROJECT_MAINTAINER_EMAIL "yuriysydor1991@gmail.com"
  CACHE STRING "Project maintainer email (used for the DEB package)"
)

set(
  MAX_LOG_LEVEL "3"
  CACHE STRING 
  "Sets the maximum severity of the compiled log messages. Error=0. Trace=5. By default Info=3"
)

set(
  PROJECT_BINARY_INSTALLATION_DIR "bin"
  CACHE STRING 
  "Sets the project main binary installation directory"
)

set(
  DEFAULT_LOG_FILE_PATH ""
  CACHE STRING 
  "Sets the default log file path"
)

option(
  ENABLE_LOGS_MICROSECONDS_TIME
  "Set to OFF to disable the microseconds fields included into the log messages"
  ON
)

string(TIMESTAMP PROJECT_CONFIGURE_DATE "%Y-%m-%d %H:%M:%S")

message(STATUS "CMAKE_PROJECT_NAME: ${CMAKE_PROJECT_NAME}")
message(STATUS "PROJECT_BINARY_NAME: ${PROJECT_BINARY_NAME}")
message(STATUS "PROJECT_CONFIGURE_DATE: ${PROJECT_CONFIGURE_DATE}")
