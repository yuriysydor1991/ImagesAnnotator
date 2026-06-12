cmake_minimum_required(VERSION 3.13)

# The nlohmann_json library is made available (system wide or through the
# Internet via FetchContent) and propagated to the project targets by the
# template-project-nlohmann-json-enabler module.

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
)

add_subdirectory(converters)
add_subdirectory(mergers)
