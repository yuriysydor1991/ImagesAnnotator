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
include(template-project-Gtkmm3-enabler)
include(template-project-Gtkmm4-enabler)

# ImagesAnnotator specific: the optional wxWidgets GUI toolkit. Guarded on
# USE_WXWIDGETS (default OFF, resolved by misc-variables-declare above); when ON
# it publishes the same neutral GTKMM_* probe results / GTKMM_GUI_SUBDIR /
# GTKMM_DEB_RUNTIME_DEPENDS the two GTKmm enablers do, so nothing downstream has
# to branch on which toolkit is active.
include(template-project-wxWidgets-enabler)

# ImagesAnnotator specific: install the .desktop launcher and themed icon so the
# app gets a distinctive taskbar / GNOME Shell dash icon (the only route that
# works on Wayland). Runs here, before add_subdirectory(src), because it only
# registers configure-time install() rules and depends on APPLICATION_ID, which
# template-project-misc-variables-declare.cmake has already resolved.
include(template-project-desktop-entry-enabler)

# ImagesAnnotator specific: these libraries are made available before the src
# subdirectory is added because the project sources consume them while their
# targets are configured. nlohmann_json is propagated through link_libraries()
# to every object library, and libcurl exposes the CURL_* variables used by the
# src/CURL module and the tests. The upstream template enables them in the
# post step (where they only link against the final executable target).
include(template-project-nlohmann-json-enabler)
include(template-project-libcurl-enabler)

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

# compile options
include(template-project-sanitizers)

# analyzers
include(template-project-cppcheck-target)
include(template-project-clang-tidy-target)

# profilers
include(template-project-profiler-gprof)
include(template-project-profiler-valgrind-callgrind)
