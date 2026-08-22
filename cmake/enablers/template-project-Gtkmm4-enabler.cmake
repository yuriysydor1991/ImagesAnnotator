cmake_minimum_required(VERSION 3.13)

# ---------------------------------------------------------------------------
# ImagesAnnotator specific: the GTKmm4 (GTK 4) GUI implementation, the
# alternative to the default GTKmm3 one.
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

# The toolkit headers are third party, so the diagnostics raised inside them are
# not actionable from this project. pkg-config hands out its directories as
# plain -I paths, and a -I path is a user directory to the compiler even when it
# sits under /usr/include, so the strict warning set of
# cmake/compile-options/template-project-compile-options-GCC.cmake is applied to
# them in full: the G_DEFINE_AUTOPTR_CLEANUP_FUNC() expansions of
# glib/gmacros.h alone answer -Wold-style-cast several hundred times per
# translation unit that includes gtkmm, and harfbuzz answers -Wuseless-cast.
#
# Repeating the very same directories as SYSTEM ones demotes them to the system
# include chain, which silences everything originating inside them while leaving
# every warning of this project's own sources reported as before. It is done
# here at project scope, before add_subdirectory(src), so it covers the GUI
# sources and every test target that includes the toolkit. The per-target
# target_include_directories() calls under src/gui keep listing
# GTKMM_INCLUDE_DIRS: a directory named by both -I and -isystem is searched at
# its system position, so those stay correct and need no change.
include_directories(SYSTEM ${GTKMM_INCLUDE_DIRS})

# The src/gui subdirectory holding this toolkit's implementation.
set(GTKMM_GUI_SUBDIR "gtkmm4")

# Debian runtime package of this toolkit, consumed by the DEB packager enabler.
set(GTKMM_DEB_RUNTIME_DEPENDS "libgtkmm-4.0-0")

# The concrete IImageCropperFacilityProvider the PyTorch Vision export uses. It
# is a GUI-toolkit detail injected into the toolkit-agnostic exporter, so the
# AnnotatorController component tests compile the active toolkit's cropper
# through this neutral variable instead of hard-coding one.
set(GUI_IMAGE_CROPPER_SRC
  "${CMAKE_SOURCE_DIR}/src/gui/gtkmm-common/GtkmmImageCropperProvider.cpp")

# Propagate the active toolkit as a preprocessor define so the toolkit-neutral
# facade src/gui/GUIIniter.h (and the app component that consumes it) select the
# matching GtkmmIniter implementation. add_compile_definitions() runs here at
# project scope, so every target defined afterwards inherits the macro.
add_compile_definitions(USE_GTKMM4)
