cmake_minimum_required(VERSION 3.13)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(
  PROJECT_BINARY_NAME ${CMAKE_PROJECT_NAME}
  CACHE STRING "Project main binary name and target"
)

string(TOLOWER ${PROJECT_BINARY_NAME} PROJECT_BINARY_NAME_lower)

# ---------------------------------------------------------------------------
# ImagesAnnotator specific: the converter only binary.
#
# The project builds two application binaries out of one and the same code. The
# main one carries the graphical interface; the second one, named after it,
# carries the annotations format conversion alone and no GUI toolkit at all -
# it is the very same src/app component compiled with the
# IMAGES_ANNOTATOR_CONVERTER_ONLY definition, which leaves the app::Application
# class (and with it the whole src/gui subtree that class pulls) out. The
# conversion stays available through the command line of the main binary as
# well, so the second one adds a deployment option rather than a feature.
#
#   (nothing given)                             -> both binaries
#   -DENABLE_IMAGES_ANNOTATOR_CONVERTER=OFF     -> the main binary alone
#   -DBUILD_IMAGES_ANNOTATOR_CONVERTER_ONLY=ON  -> the converter alone, and no
#                                                  GUI toolkit is even probed
# ---------------------------------------------------------------------------
option(
  ENABLE_IMAGES_ANNOTATOR_CONVERTER
  "Build the converter only binary beside the main application one"
  ON
)

option(
  BUILD_IMAGES_ANNOTATOR_CONVERTER_ONLY
  "Build the converter only binary alone, with the main application target and its GUI toolkit excluded"
  OFF
)

set(
  PROJECT_CONVERTER_BINARY_NAME "${PROJECT_BINARY_NAME}Converter"
  CACHE STRING "The converter only binary name and target"
)

# Asking for the converter alone is asking for the converter, so the pair can
# never end up contradicting itself.
if(BUILD_IMAGES_ANNOTATOR_CONVERTER_ONLY AND NOT ENABLE_IMAGES_ANNOTATOR_CONVERTER)
  set(ENABLE_IMAGES_ANNOTATOR_CONVERTER ON CACHE BOOL
    "Build the converter only binary beside the main application one" FORCE)
endif()

# The application targets this configure builds, in the order they are declared.
# Every component of the src subtree hands its object library and its sources to
# all of them at once through the annotator_app_targets_* functions, which is
# what keeps the two binaries built out of one and the same code.
set(ANNOTATOR_APP_TARGETS "")

if(NOT BUILD_IMAGES_ANNOTATOR_CONVERTER_ONLY)
  list(APPEND ANNOTATOR_APP_TARGETS ${PROJECT_BINARY_NAME})
endif()

if(ENABLE_IMAGES_ANNOTATOR_CONVERTER)
  list(APPEND ANNOTATOR_APP_TARGETS ${PROJECT_CONVERTER_BINARY_NAME})
endif()

if(NOT ANNOTATOR_APP_TARGETS)
  message(FATAL_ERROR
    "Neither application binary is enabled - ENABLE_IMAGES_ANNOTATOR_CONVERTER "
    "is OFF while BUILD_IMAGES_ANNOTATOR_CONVERTER_ONLY is ON, which asks for "
    "the converter alone and for no converter at the same time.")
endif()

# The binary the profiling and the memory check targets are pointed at: the main
# one whenever it is built, and the converter when it is the only one there.
list(GET ANNOTATOR_APP_TARGETS 0 ANNOTATOR_PRIMARY_APP_TARGET)

message(STATUS "Application targets: ${ANNOTATOR_APP_TARGETS}")

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
option(USE_WXWIDGETS
  "Build the wxWidgets based GUI instead of GTKmm. Overrides USE_GTKMM3 / USE_GTKMM4." OFF)

option(USE_QT6
  "Build the Qt6 based GUI instead of GTKmm. Overrides USE_GTKMM3 / USE_GTKMM4." OFF)

# ImagesAnnotator specific: wxWidgets and Qt6 are the optional non-GTKmm GUI
# toolkits. Each of them, when switched ON, overrides the GTKmm pair: both
# USE_GTKMM3 and USE_GTKMM4 are shadowed OFF (their enablers early-return and the
# mutually exclusive resolver below is skipped) and the desktop application id is
# derived from "wxwidgets" / "qt6" instead of "gtkmm<N>". The GTKmm cache entries
# are left untouched, so dropping the override later restores the previous GTKmm
# choice. Both default to OFF, so the out-of-the-box behaviour is unchanged.
#
# They are alternatives to each other just as the GTKmm pair members are, so
# asking for both at once is an error rather than a silent win for one of them.
if(USE_WXWIDGETS AND USE_QT6)
  message(FATAL_ERROR
    "USE_WXWIDGETS and USE_QT6 are mutually exclusive - exactly one GUI toolkit "
    "is compiled into the binary. Enable just one of them (or neither, to build "
    "the default GTKmm3 GUI).")
endif()

if(USE_WXWIDGETS)
  set(USE_GTKMM3 OFF)
  set(USE_GTKMM4 OFF)
  set(GUI_TOOLKIT_NAME "wxwidgets")
  message(STATUS "GUI toolkit: wxWidgets (USE_WXWIDGETS=ON)")
elseif(USE_QT6)
  set(USE_GTKMM3 OFF)
  set(USE_GTKMM4 OFF)
  set(GUI_TOOLKIT_NAME "qt6")
  message(STATUS "GUI toolkit: Qt6 (USE_QT6=ON)")
else()

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

set(GUI_TOOLKIT_NAME "gtkmm${GTKMM_SELECTED}")

endif()

# The block above is resolved even for a converter only build, since the
# application id it derives is written into the produced binaries whether or not
# a graphical interface is compiled in. Nothing of the toolkit it names is
# probed, built or linked in that case, so the resolved name is said out loud
# not to read as a promise of a GUI.
if(BUILD_IMAGES_ANNOTATOR_CONVERTER_ONLY)
  message(STATUS
    "GUI toolkit: none is probed, built or linked "
    "(BUILD_IMAGES_ANNOTATOR_CONVERTER_ONLY=ON)")
endif()

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
# GResource prefix (/ua/org/kytok/template/gtkmm<N>/...) and with the
# application id of the flatpak carrying that toolkit.
#
# IMPORTANT: defining it here - unconditionally, before add_subdirectory(src)
# configures project-global-decls.h.in - is also what finally gives the C++
# PROJECT_FLATPAK_URL a non-empty value in ordinary (non-flatpak) builds. It
# used to be set only inside the flatpak enabler (guarded by ENABLE_FLATPAK and
# included after src), so the application ran with an empty application id.
set(APPLICATION_ID "ua.org.kytok.template.${GUI_TOOLKIT_NAME}.${PROJECT_BINARY_NAME}")

# The flatpak application id of the graphical binary is the same reverse-DNS
# name; keep them in sync from this single source instead of re-hardcoding it in
# the flatpak enabler.
set(FLATPAK_PROJECT_URL "${APPLICATION_ID}")

message(STATUS "Application id: ${APPLICATION_ID}")

# ---------------------------------------------------------------------------
# ImagesAnnotator specific: the application id of every binary.
#
# The packagers address each binary they carry by an id of its own. The
# graphical binary keeps the APPLICATION_ID above. The converter only binary
# gets that id with a .Converter component appended, which is the shape flatpak
# exports the files of an application under: it exports the <app id>.desktop
# and the <app id>.<anything>.desktop launchers, but no <app id><anything> one.
# So a flatpak carrying both binaries registers the converter under an id of
# its own - the way the org.libreoffice.LibreOffice flatpak registers its
# org.libreoffice.LibreOffice.writer - and a flatpak carrying the converter
# alone is that very id.
#
# ANNOTATOR_PRIMARY_APP_ID is the id of the ANNOTATOR_PRIMARY_APP_TARGET binary,
# the one a package starts unless it is asked for another one.
# ---------------------------------------------------------------------------
set(CONVERTER_APPLICATION_ID "${APPLICATION_ID}.Converter")

if(BUILD_IMAGES_ANNOTATOR_CONVERTER_ONLY)
  set(ANNOTATOR_PRIMARY_APP_ID "${CONVERTER_APPLICATION_ID}")
else()
  set(ANNOTATOR_PRIMARY_APP_ID "${APPLICATION_ID}")
endif()

if(ENABLE_IMAGES_ANNOTATOR_CONVERTER)
  message(STATUS "Converter application id: ${CONVERTER_APPLICATION_ID}")
endif()

# The configuration files of the flatpak and the snap packagers differ per GUI
# toolkit, each toolkit bringing an SDK and packages of its own along, so they
# are picked by this name: the GUI_TOOLKIT_NAME of a graphical build, or
# "converter" for a converter only build, which needs no toolkit at all.
if(BUILD_IMAGES_ANNOTATOR_CONVERTER_ONLY)
  set(ANNOTATOR_PACKAGE_FLAVOR "converter")
else()
  set(ANNOTATOR_PACKAGE_FLAVOR "${GUI_TOOLKIT_NAME}")
endif()

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
