cmake_minimum_required(VERSION 3.13)

add_library(
  ImagesAnnotatorGtkmmHelperswObj OBJECT
  GtkmmImageCropperProvider.cpp
)

target_include_directories(
  ImagesAnnotatorGtkmmHelperswObj
  PRIVATE 
    ${GTKMM_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
)

target_link_libraries(
  ${PROJECT_BINARY_NAME}
  ImagesAnnotatorGtkmmHelperswObj
)
