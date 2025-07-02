cmake_minimum_required(VERSION 3.13)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(ANNOTATIONCONTROLLER_CTEST_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(AnnotationsDirDB)
add_subdirectory(ImagesDirLoader)
