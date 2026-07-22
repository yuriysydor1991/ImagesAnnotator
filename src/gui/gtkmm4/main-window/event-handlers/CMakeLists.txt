cmake_minimum_required(VERSION 3.13)

add_library(
  ImageAnnotatorGtkmmWindowEventHandlersObj OBJECT
  WindowOps.cpp
  AbstractWindowEventHandler.cpp
  MouseDrawEventsHandler.cpp
  KeyboardEventsHandler.cpp
  ZoomEventsHandler.cpp
  ImageNavigationEventsHandler.cpp
  ImageListEventsHandler.cpp
  AnnotationsEventsHandler.cpp
  ProjectFileEventsHandler.cpp
  ExportEventsHandler.cpp
  WebPageEventsHandler.cpp
  AboutEventsHandler.cpp
  WindowSystemEventsHandler.cpp
  WindowEventHandlersFactory.cpp
)

target_include_directories(
  ImageAnnotatorGtkmmWindowEventHandlersObj
  PRIVATE
    ${GTKMM_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
)

target_link_libraries(
  ${PROJECT_BINARY_NAME}
  ImageAnnotatorGtkmmWindowEventHandlersObj
)
