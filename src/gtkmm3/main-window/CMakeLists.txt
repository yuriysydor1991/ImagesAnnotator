cmake_minimum_required(VERSION 3.13)

add_library(
  ImageAnnotatorGtkmmWindowObj OBJECT
  WindowLoader.cpp
  WindowDataContext.cpp
  WindowEventsHandler.cpp
)

target_include_directories(
  ImageAnnotatorGtkmmWindowObj
  PRIVATE 
    ${GTKMM_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
)

target_link_libraries(
  ${PROJECT_BINARY_NAME}
  ImageAnnotatorGtkmmWindowObj
)

add_subdirectory(custom-widgets)
