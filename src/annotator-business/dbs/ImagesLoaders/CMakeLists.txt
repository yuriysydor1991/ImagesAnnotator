cmake_minimum_required(VERSION 3.13)

add_library(
  ImagesAnnotatorControllerDBsImagesDBObj OBJECT
  ImagesDirLoader.cpp
  ImagesWebPageLoader.cpp
)

target_include_directories(
  ImagesAnnotatorControllerDBsImagesDBObj
  PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
)

annotator_app_targets_link_libraries(ImagesAnnotatorControllerDBsImagesDBObj)
