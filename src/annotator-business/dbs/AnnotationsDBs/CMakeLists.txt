cmake_minimum_required(VERSION 3.13)

find_package(nlohmann_json REQUIRED)

add_library(
  ImagesAnnotatorControllerDBsObj OBJECT
  AnnotationsDirDB.cpp
  AnnotationsJSONSerializator.cpp
)

target_include_directories(
  ImagesAnnotatorControllerDBsObj
  PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
)

target_link_libraries(
  ${PROJECT_BINARY_NAME}
  ImagesAnnotatorControllerDBsObj
  nlohmann_json::nlohmann_json
)

add_subdirectory(converters)
