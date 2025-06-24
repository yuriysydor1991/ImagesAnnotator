cmake_minimum_required(VERSION 3.13)

add_library(
  ImagesAnnotatorEventControllerObj OBJECT
  ImagesAnnotatorEventController.cpp
)

target_include_directories(
  ImagesAnnotatorEventControllerObj
  PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
)

target_link_libraries(
  ${PROJECT_BINARY_NAME}
  ImagesAnnotatorEventControllerObj
)

add_subdirectory(events)
