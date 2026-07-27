cmake_minimum_required(VERSION 3.13)

# The GTKMM_* variables come from
# cmake/enablers/template-project-Qt6-enabler.cmake, which probed Qt with
# find_package(Qt6) once at project scope before add_subdirectory(src). They
# carry the Qt include dirs / imported targets here despite the historical name,
# so this file mirrors the gtkmm3 / gtkmm4 / wxwidgets CMakeLists shape.

# template.qrc is listed among the sources so CMAKE_AUTORCC (switched on by the
# Qt6 enabler) compiles it with rcc and links the result in - the Qt analogue of
# the gresourcelib target the gtkmm3 ui/CMakeLists.txt builds. It embeds the
# shared logo and the window style sheet, so the binary needs no installed
# resource file at runtime.
add_library(
  ${GTKMM_OBJECT_NAME} OBJECT
  Qt6Initer.cpp
  MainWindowContext.cpp
  ui/template.qrc
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
  ${GTKMM_LIBRARIES}
)

add_subdirectory(main-window)
add_subdirectory(tests)
