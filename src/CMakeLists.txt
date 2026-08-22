cmake_minimum_required(VERSION 3.13)

include(template-project-compile-options)

# Applied here at the directory scope so that the object library of every
# component below, every test target and both application targets are compiled
# with one and the same warning set and one and the same -DMAX_LOG_LEVEL.
add_compile_options(${EXTRA_COMPILE_OPTIONS})
add_link_options(${EXTRA_LINK_OPTIONS})

# The root of the project sources, used by the two target files below and by
# the annotator_configure_app_target() function they call: an include() does
# not change CMAKE_CURRENT_SOURCE_DIR, but a function called from one runs in
# whatever directory the call is made from.
set(ANNOTATOR_SRC_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")

configure_file(
  project-global-decls.h.in
  ${CMAKE_BINARY_DIR}/project-global-decls.h
)

# Root of the project wide shared test mocks. Every test (unit or component)
# reuses the mock classes living under this directory instead of carrying its
# own private copy, to avoid scattered identical mocks across the test tree.
set(GENERAL_MOCKS_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/tests/mocks")

# The application targets, one file each. They are declared before the
# components below, since every component hands its sources and its object
# library to all of them through the annotator_app_targets_* functions.
include(${CMAKE_CURRENT_SOURCE_DIR}/ImagesAnnotator-target.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/ImagesAnnotatorConverter-target.cmake)

add_subdirectory(app)
add_subdirectory(log)

# The graphical interface belongs to the main application target alone, so a
# converter only build does not add it at all.
if(NOT BUILD_IMAGES_ANNOTATOR_CONVERTER_ONLY)
  add_subdirectory(gui)
endif()

add_subdirectory(annotator-business)
add_subdirectory(annotator-events)
add_subdirectory(helpers)
add_subdirectory(CURL)
add_subdirectory(tests)
