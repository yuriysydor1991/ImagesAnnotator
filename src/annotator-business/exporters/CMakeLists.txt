cmake_minimum_required(VERSION 3.13)

add_library(
  ImagesAnnotatorControllerExportersObj OBJECT
  PlainTxt2FolderExporter.cpp
  Yolo42FolderExporter.cpp
  PyTorchVisionFolderExporter.cpp
)

target_include_directories(
  ImagesAnnotatorControllerExportersObj
  PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
)

target_link_libraries(
  ${PROJECT_BINARY_NAME}
  ImagesAnnotatorControllerExportersObj
)
