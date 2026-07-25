cmake_minimum_required(VERSION 3.13)

add_library(
  ImageAnnotatorWxWindowObj OBJECT
  WindowLoader.cpp
  WindowDataContext.cpp
  WindowEventsHandler.cpp
)

target_include_directories(
  ImageAnnotatorWxWindowObj
  PRIVATE
    ${GTKMM_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
)

target_link_libraries(
  ${PROJECT_BINARY_NAME}
  ImageAnnotatorWxWindowObj
)

add_subdirectory(custom-widgets)
add_subdirectory(event-handlers)
