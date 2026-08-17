cmake_minimum_required(VERSION 3.13)

# todo: delete Gtkmm assets and replace orig classes with mocks
# The GTKMM_* variables come from the active GUI toolkit enabler
# (cmake/enablers/template-project-Gtkmm{3,4}-enabler.cmake), so the shared
# gtkmm-common cropper source these tests compile is automatically built against
# the same GTKmm the rest of the build uses.
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${GTKMM_INCLUDE_DIRS})
link_libraries(${GTKMM_LIBRARIES})
link_directories(${GTKMM_LIBRARY_DIRS})

set(ANNOTATIONCONTROLLER_CTEST_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(AnnotationsDirDB)
add_subdirectory(ImagesDirLoader)
add_subdirectory(Exporters)
add_subdirectory(Importers)
