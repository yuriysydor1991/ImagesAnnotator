cmake_minimum_required(VERSION 3.13)

add_library(
  ImagesAnnotatorControllerObj OBJECT
  AnnotatorController.cpp
)

target_include_directories(
  ImagesAnnotatorControllerObj
  PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
)

target_link_libraries(
  ${PROJECT_BINARY_NAME}
  ImagesAnnotatorControllerObj
)

add_subdirectory(dbs)
add_subdirectory(tests)
