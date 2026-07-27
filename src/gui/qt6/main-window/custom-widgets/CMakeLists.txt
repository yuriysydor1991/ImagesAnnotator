cmake_minimum_required(VERSION 3.13)

add_library(
  ImageAnnotatorQt6WindowCustomWidgetsObj OBJECT
  ImagePathLabel.cpp
  ImageRectsLabel.cpp
  AllAnnotationsLabel.cpp
  CentralWorkingCanvas.cpp
  CustomWidgetsFactory.cpp
  QtImageCropperProvider.cpp
)

target_include_directories(
  ImageAnnotatorQt6WindowCustomWidgetsObj
  PRIVATE
    ${GTKMM_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
)

target_link_libraries(
  ${PROJECT_BINARY_NAME}
  ImageAnnotatorQt6WindowCustomWidgetsObj
)
