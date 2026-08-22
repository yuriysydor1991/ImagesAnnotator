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

annotator_app_targets_link_libraries(ImagesAnnotatorControllerObj)

# The exporters subdirectory is gone: the plain txt, the YOLO v4 and the
# PyTorch Vision writers now come from the ImagesAnnotator-DataExporters
# library, which the enabler links into every target of the project.
add_subdirectory(dbs)
add_subdirectory(tests)
