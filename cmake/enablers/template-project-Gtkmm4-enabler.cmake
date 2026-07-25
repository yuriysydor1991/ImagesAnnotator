cmake_minimum_required(VERSION 3.13)

# ---------------------------------------------------------------------------
# ImagesAnnotator specific: the GTKmm4 (GTK 4) GUI implementation, which is the
# project default.
#
# USE_GTKMM4 is one half of the mutually exclusive GUI toolkit pair that
# cmake/template-project-misc-variables-declare.cmake resolves before any
# enabler runs, so this file only has to guard on its own option and can then
# declare everything the GTKmm4 build needs unconditionally. Every toolkit
# dependent value the rest of the project consumes is produced here and by the
# GTKmm3 counterpart, which is why no other CMakeLists has to branch on
# USE_GTKMM3 / USE_GTKMM4.
# ---------------------------------------------------------------------------
if(NOT USE_GTKMM4)
  return()
endif()

find_package(PkgConfig)

# Probed once, here at project scope, so that the resulting GTKMM_INCLUDE_DIRS /
# GTKMM_LIBRARIES / GTKMM_LIBRARY_DIRS variables are inherited by every target
# under src - the GUI sources, their unit and component tests and the
# AnnotatorController component tests - instead of each of those repeating the
# probe with a hard coded module name. The enablers are included before
# add_subdirectory(src), so the variables are already defined while those
# targets are configured.
pkg_check_modules(GTKMM gtkmm-4.0 REQUIRED)

# The src/gui subdirectory holding this toolkit's implementation.
set(GTKMM_GUI_SUBDIR "gtkmm4")

# Debian runtime package of this toolkit, consumed by the DEB packager enabler.
set(GTKMM_DEB_RUNTIME_DEPENDS "libgtkmm-4.0-0")

# Propagate the active toolkit as a preprocessor define so the toolkit-neutral
# facade src/gui/GuiIniter.h (and the app component that consumes it) select the
# matching GtkmmIniter implementation. add_compile_definitions() runs here at
# project scope, so every target defined afterwards inherits the macro.
add_compile_definitions(USE_GTKMM4)
