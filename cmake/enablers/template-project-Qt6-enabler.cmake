cmake_minimum_required(VERSION 3.13)

# ---------------------------------------------------------------------------
# ImagesAnnotator specific: the Qt6 (Qt Widgets) GUI implementation, an optional
# fourth alternative to the GTKmm pair and to wxWidgets.
#
# USE_QT6 (default OFF) is resolved by
# cmake/template-project-misc-variables-declare.cmake before any enabler runs, so
# this file only has to guard on its own option and can then declare everything
# the Qt6 build needs. Like the GTKmm and wxWidgets enablers it publishes the
# toolkit-neutral GTKMM_* variables the rest of the src subtree (and the DEB
# packager) consume, which is why no other CMakeLists has to branch on the
# toolkit beyond the single add_subdirectory() switch in src/gui/CMakeLists.txt.
# ---------------------------------------------------------------------------
if(NOT USE_QT6)
  return()
endif()

# Qt's meta-object compiler and resource compiler have to run before the C++
# compiler sees the sources. Enabling them here, at project scope and ahead of
# add_subdirectory(src), makes every target defined afterwards inherit them:
#   - AUTOMOC generates the meta-object code for the Q_OBJECT classes (the
#     working canvas, which declares the mouse/wheel signals the event handlers
#     connect to);
#   - AUTORCC compiles src/gui/qt6/ui/template.qrc - the Qt analogue of the
#     GResource the GTKmm build embeds - into the binary, so the logo and the
#     style sheet need no installed files at runtime.
# AUTOUIC is deliberately left off: the Qt window is built in code rather than
# from a Qt Designer form, and the only *.ui files in this repository are the
# GTK builder XML of the gtkmm3 / gtkmm4 components, which uic must never touch.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Qt6 is built with reduce_relocations, so anything compiled against its headers
# has to be position independent. The object libraries under src consume the Qt
# include directories directly (see GTKMM_INCLUDE_DIRS below) rather than linking
# the imported targets, so the property those targets would have propagated is
# requested explicitly here.
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Qt ships no pkg-config .pc files; it provides its own CMake package config, so
# probe with find_package() (the same idiom the wxWidgets / OpenCV / CURL
# enablers use) rather than pkg_check_modules().
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

# Republish the probe results under the same neutral names the GTKmm and
# wxWidgets enablers use, so every target under src consumes the active toolkit
# through one set of variables without knowing which toolkit produced them.
#
# GTKMM_LIBRARIES holds the imported targets: target_link_libraries() accepts
# them from a variable and they carry Qt's own compile definitions and link
# flags. GTKMM_INCLUDE_DIRS is filled from those same targets because the object
# libraries under src only ever add the include directories (they are OBJECT
# libraries, the real linking happens in the consuming executable/test target),
# so the include paths have to be available as plain directories too.
set(GTKMM_LIBRARIES Qt6::Widgets Qt6::Gui Qt6::Core)

set(GTKMM_INCLUDE_DIRS "")
foreach(QT6_COMPONENT IN ITEMS Core Gui Widgets)
  get_target_property(QT6_COMPONENT_INCLUDES
    Qt6::${QT6_COMPONENT} INTERFACE_INCLUDE_DIRECTORIES)

  if(QT6_COMPONENT_INCLUDES)
    list(APPEND GTKMM_INCLUDE_DIRS ${QT6_COMPONENT_INCLUDES})
  endif()
endforeach()
list(REMOVE_DUPLICATES GTKMM_INCLUDE_DIRS)

# The imported targets carry their own link directories, so there is nothing to
# publish here; the variable is still defined so the target_link_directories()
# calls under src stay valid with an empty list.
set(GTKMM_LIBRARY_DIRS "")

# The src/gui subdirectory holding this toolkit's implementation.
set(GTKMM_GUI_SUBDIR "qt6")

# Debian runtime package of this toolkit, consumed by the DEB packager enabler.
set(GTKMM_DEB_RUNTIME_DEPENDS "libqt6widgets6")

# The concrete IImageCropperFacilityProvider the PyTorch Vision export uses. It
# is a GUI-toolkit detail injected into the toolkit-agnostic exporter, so the
# AnnotatorController component tests compile the active toolkit's cropper
# through this neutral variable instead of hard-coding one - the QImage based
# one here.
set(GUI_IMAGE_CROPPER_SRC
  "${CMAKE_SOURCE_DIR}/src/gui/qt6/main-window/custom-widgets/QtImageCropperProvider.cpp")

# Propagate the active toolkit as a preprocessor define so the toolkit-neutral
# facade src/gui/GUIIniter.h (and the app component that consumes it) select the
# matching initer implementation. add_compile_definitions() runs here at project
# scope, so every target defined afterwards inherits the macro.
add_compile_definitions(USE_QT6)

message(STATUS "Qt6 version: ${Qt6_VERSION}")
