cmake_minimum_required(VERSION 3.13)

add_library(
  ImageAnnotatorGtkmmWindowCustomWidgetsObj OBJECT
  ImagePathLabel.cpp
  CustomWidgetsFactory.cpp
  CentralWorkingCanvas.cpp
  ImageRectsLabel.cpp
  AllAnnotationsLabel.cpp
)

target_include_directories(
  ImageAnnotatorGtkmmWindowCustomWidgetsObj
  PRIVATE 
    ${GTKMM_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
)

target_link_libraries(
  ${PROJECT_BINARY_NAME}
  ImageAnnotatorGtkmmWindowCustomWidgetsObj
)
