cmake_minimum_required(VERSION 3.13)

add_library(
  ImageAnnotatorWxWindowCustomWidgetsObj OBJECT
  ImagePathLabel.cpp
  ImageRectsLabel.cpp
  AllAnnotationsLabel.cpp
  CentralWorkingCanvas.cpp
  CustomWidgetsFactory.cpp
  WxImageCropperProvider.cpp
)

target_include_directories(
  ImageAnnotatorWxWindowCustomWidgetsObj
  PRIVATE
    ${GTKMM_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
)

target_link_libraries(
  ${PROJECT_BINARY_NAME}
  ImageAnnotatorWxWindowCustomWidgetsObj
)
