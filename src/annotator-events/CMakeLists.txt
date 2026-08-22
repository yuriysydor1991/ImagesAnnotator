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

annotator_app_targets_link_libraries(ImagesAnnotatorEventControllerObj)

add_subdirectory(events)
add_subdirectory(tests)
