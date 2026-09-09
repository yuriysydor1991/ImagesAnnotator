cmake_minimum_required(VERSION 3.13)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/tools")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/enablers")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/compile-options")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/compile-options/sanitizers")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/enablers/profilers")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/enablers/dockerers")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/enablers/packagers")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/enablers/compression")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/enablers/images")

include(template-project-enabler-function)
include(template-project-git-enabler)

# ImagesAnnotator specific: the GUI toolkit enablers. Exactly one of them gets
# past its own guard - the mutually exclusive USE_GTKMM3 / USE_GTKMM4 pair is
# resolved by template-project-misc-variables-declare.cmake, which bootstrap
# includes before this file. They run here, ahead of add_subdirectory(src) and
# ahead of the packagers below, because they publish the GTKMM_* probe results
# consumed all over the src subtree, GTKMM_GUI_SUBDIR and the
# GTKMM_DEB_RUNTIME_DEPENDS the DEB packager needs.
#
# A converter only build (BUILD_IMAGES_ANNOTATOR_CONVERTER_ONLY) has no
# graphical interface target at all, so none of the four toolkits is probed
# here and none has to be installed for such a build to configure. The desktop
# launcher below belongs to the graphical binary as well and is skipped with
# them.
if(NOT BUILD_IMAGES_ANNOTATOR_CONVERTER_ONLY)

include(template-project-Gtkmm3-enabler)
include(template-project-Gtkmm4-enabler)

# ImagesAnnotator specific: the optional non-GTKmm GUI toolkits. Each is guarded
# on its own option (USE_WXWIDGETS / USE_QT6, both default OFF and resolved by
# misc-variables-declare above); when one is ON it publishes the same neutral
# GTKMM_* probe results / GTKMM_GUI_SUBDIR / GTKMM_DEB_RUNTIME_DEPENDS the two
# GTKmm enablers do, so nothing downstream has to branch on which toolkit is
# active. The Qt6 enabler additionally turns on AUTOMOC / AUTORCC here, which
# only takes effect for the targets defined after this point.
include(template-project-wxWidgets-enabler)
include(template-project-Qt6-enabler)

# ImagesAnnotator specific: install the .desktop launcher and themed icon so the
# app gets a distinctive taskbar / GNOME Shell dash icon (the only route that
# works on Wayland). Runs here, before add_subdirectory(src), because it only
# registers configure-time install() rules and depends on APPLICATION_ID, which
# template-project-misc-variables-declare.cmake has already resolved.
include(template-project-desktop-entry-enabler)

endif()

# ImagesAnnotator specific: these libraries are made available before the src
# subdirectory is added because the project sources consume them while their
# targets are configured. nlohmann_json is propagated through link_libraries()
# to every object library, and libcurl exposes the CURL_* variables used by the
# src/CURL module and the tests. The upstream template enables them in the
# post step (where they only link against the final executable target).
include(template-project-nlohmann-json-enabler)
include(template-project-libcurl-enabler)

# ImagesAnnotator specific: the annotations project files (their parsing, their
# serialization, the image record value objects and the duplicate aware merging)
# no longer live in this repository - they were extracted into the standalone
# ImagesAnnotator-DataDrivers library, which this enabler finds in the system.
# It runs here, next to the two libraries above and ahead of
# add_subdirectory(src), because the extracted types are compiled into
# practically every object library of the src subtree, so its usage requirements
# have to be propagated before any of them is defined.
include(template-project-ImagesAnnotator-DataDrivers-enabler)

# ImagesAnnotator specific: the dataset exporters (the plain txt, the YOLO v4
# and the PyTorch Vision writers) were extracted the very same way, into the
# standalone ImagesAnnotator-DataExporters library. It comes after the data
# drivers enabler because the exporters library is built against them and its
# installed package configuration resolves them as its own dependency.
include(template-project-ImagesAnnotator-DataExporters-enabler)

# ImagesAnnotator specific: the dataset importers - the mirror image of the
# exporters above, reading a training dataset back into the annotations - live
# in the standalone ImagesAnnotator-DataImporters library. It comes last of the
# three because it is built against the data drivers and, like the exporters,
# resolves them as its own dependency.
include(template-project-ImagesAnnotator-DataImporters-enabler)

include(template-project-GTest-enabler)
include(template-project-clang-format-target)
include(template-project-valgrind-target)

# packagers
include(template-project-deb-enabler)
include(template-project-flatpak-target)
include(template-project-snap-enabler)
include(template-project-freebsd-pkg-enabler)
include(template-project-wix-enabler)
include(template-project-rpm-enabler)
include(template-project-appimage-general-enabler)

# compile options
include(template-project-sanitizers)

# analyzers
include(template-project-cppcheck-target)
include(template-project-clang-tidy-target)

# profilers
include(template-project-profiler-gprof)
include(template-project-profiler-valgrind-callgrind)
