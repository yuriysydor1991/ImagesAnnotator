cmake_minimum_required(VERSION 3.13)

add_library(
  ImagesAnnotatorHelpersObj OBJECT
  TypeHelper.cpp
  SortHelper.cpp
  ImageRecordUrlAndPathHelper.cpp
  ImageLoader.cpp
)

target_include_directories(
  ImagesAnnotatorHelpersObj
  PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
)

target_link_libraries(
  ${PROJECT_BINARY_NAME}
  ImagesAnnotatorHelpersObj
)
