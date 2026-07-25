cmake_minimum_required(VERSION 3.13)

# The GTKMM_* variables come from cmake/enablers/template-project-Gtkmm3-enabler.cmake,
# which probed pkg-config once at project scope before add_subdirectory(src).

add_library(
  ${GTKMM_OBJECT_NAME} OBJECT
  GtkmmIniter.cpp
  MainWindowContext.cpp
)

target_include_directories(
  ${GTKMM_OBJECT_NAME}
  PRIVATE 
    ${GTKMM_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
)

target_link_directories(
  ${GTKMM_OBJECT_NAME}
  PRIVATE ${GTKMM_LIBRARY_DIRS}
)

target_link_libraries(
  ${GTKMM_OBJECT_NAME}
  PRIVATE ${GTKMM_LIBRARIES}
)

target_include_directories(
  ${PROJECT_BINARY_NAME}
  PRIVATE ${GTKMM_INCLUDE_DIRS}
)

target_link_libraries(
  ${PROJECT_BINARY_NAME}
  ${GTKMM_OBJECT_NAME}
)

add_subdirectory(ui)
add_subdirectory(main-window)
add_subdirectory(tests)
