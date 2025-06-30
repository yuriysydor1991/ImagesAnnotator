cmake_minimum_required(VERSION 3.13)

add_library(
  ImagesAnnotatorControllerDBsAnnotationsConvertersObj OBJECT
  JSON2ImageRecordsConverter.cpp
)

target_include_directories(
  ImagesAnnotatorControllerDBsAnnotationsConvertersObj
  PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
)

target_link_libraries(
  ${PROJECT_BINARY_NAME}
  ImagesAnnotatorControllerDBsAnnotationsConvertersObj
)
