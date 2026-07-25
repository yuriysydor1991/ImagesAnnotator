cmake_minimum_required(VERSION 3.13)

# ---------------------------------------------------------------------------
# ImagesAnnotator specific: the wxWidgets GUI implementation, an optional third
# alternative to the GTKmm pair.
#
# USE_WXWIDGETS (default OFF) is resolved by
# cmake/template-project-misc-variables-declare.cmake before any enabler runs, so
# this file only has to guard on its own option and can then declare everything
# the wxWidgets build needs. Like the two GTKmm enablers it publishes the
# toolkit-neutral GTKMM_* variables the rest of the src subtree (and the DEB
# packager) consume, which is why no other CMakeLists has to branch on the toolkit
# beyond the single add_subdirectory() switch in src/gui/CMakeLists.txt.
# ---------------------------------------------------------------------------
if(NOT USE_WXWIDGETS)
  return()
endif()

# wxWidgets ships no pkg-config .pc file; it provides its own CMake find module
# driven by wx-config, so probe with find_package() (the same idiom OpenCV / CURL
# use in this project) rather than pkg_check_modules().
find_package(wxWidgets REQUIRED COMPONENTS core base)

# wxWidgets_LIBRARIES is a list of raw linker flags (e.g. "-L/usr/lib;;-lwx_...");
# drop the empty entries so target_link_libraries() never receives a blank item.
list(FILTER wxWidgets_LIBRARIES EXCLUDE REGEX "^$")

# Republish the probe results under the same neutral names the two GTKmm enablers
# use, so every target under src consumes the active toolkit through one set of
# variables without knowing which toolkit produced them.
set(GTKMM_INCLUDE_DIRS ${wxWidgets_INCLUDE_DIRS})
set(GTKMM_LIBRARIES ${wxWidgets_LIBRARIES})
set(GTKMM_LIBRARY_DIRS ${wxWidgets_LIBRARY_DIRS})

# The src/gui subdirectory holding this toolkit's implementation.
set(GTKMM_GUI_SUBDIR "wxwidgets")

# Debian runtime package of this toolkit, consumed by the DEB packager enabler.
set(GTKMM_DEB_RUNTIME_DEPENDS "libwxgtk3.2-1t64")

# The concrete IImageCropperFacilityProvider the PyTorch Vision export uses. It
# is a GUI-toolkit detail injected into the toolkit-agnostic exporter, so the
# AnnotatorController component tests compile the active toolkit's cropper
# through this neutral variable instead of hard-coding one - the wxImage based
# one here.
set(GUI_IMAGE_CROPPER_SRC
  "${CMAKE_SOURCE_DIR}/src/gui/wxwidgets/main-window/custom-widgets/WxImageCropperProvider.cpp")

# wxWidgets headers only compile with the feature macros find_package reports in
# wxWidgets_DEFINITIONS (WXUSINGDLL, __WXGTK3__, ...). Propagate them together
# with USE_WXWIDGETS - which the toolkit-neutral facade src/gui/GuiIniter.h
# branches on to pick the wxWidgets initer - at project scope, so every target
# defined afterwards inherits them (matches add_compile_definitions(USE_GTKMM<N>)
# in the GTKmm enablers).
add_compile_definitions(USE_WXWIDGETS ${wxWidgets_DEFINITIONS})
