cmake_minimum_required(VERSION 3.13)

# todo: delete Gtkmm assets and replace orig classes with mocks
find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0 REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${GTKMM_INCLUDE_DIRS})
link_libraries(${GTKMM_LIBRARIES})
link_directories(${GTKMM_LIBRARY_DIRS})

set(ANNOTATIONCONTROLLER_CTEST_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(AnnotationsDirDB)
add_subdirectory(ImagesDirLoader)
