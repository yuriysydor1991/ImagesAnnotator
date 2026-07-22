cmake_minimum_required(VERSION 3.13)

# todo: delete Gtkmm assets and replace orig classes with mocks
find_package(PkgConfig)
# Follow the active GUI toolkit selected by USE_GTKMM3 / USE_GTKMM4 so the
# shared gtkmm-common cropper source these tests compile is built against the
# same GTKmm the rest of the build uses.
pkg_check_modules(GTKMM ${GTKMM_PC} REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${GTKMM_INCLUDE_DIRS})
link_libraries(${GTKMM_LIBRARIES})
link_directories(${GTKMM_LIBRARY_DIRS})

set(ANNOTATIONCONTROLLER_CTEST_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(AnnotationsDirDB)
add_subdirectory(ImagesDirLoader)
