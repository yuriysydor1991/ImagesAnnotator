cmake_minimum_required(VERSION 3.13)

# Toolkit-agnostic GUI helpers shared by every GTKmm implementation (gtkmm3 and
# gtkmm4). These sources use only cross-version APIs (Gdk::Pixbuf, Cairo,
# std::filesystem) and are compiled once against whichever GTKmm toolkit the
# mutually exclusive USE_GTKMM3 / USE_GTKMM4 switch selected. The GTKMM_*
# variables are provided by the parent src/gui/CMakeLists.txt.

add_library(
  ImagesAnnotatorGtkmmCommonObj OBJECT
  GtkmmImageCropperProvider.cpp
)

target_include_directories(
  ImagesAnnotatorGtkmmCommonObj
  PRIVATE
    ${GTKMM_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
)

target_link_libraries(
  ${PROJECT_BINARY_NAME}
  ImagesAnnotatorGtkmmCommonObj
)
