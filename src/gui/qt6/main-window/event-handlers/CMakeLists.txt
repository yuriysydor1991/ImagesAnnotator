cmake_minimum_required(VERSION 3.13)

add_library(
  ImageAnnotatorQt6WindowEventHandlersObj OBJECT
  WindowOps.cpp
  AbstractWindowEventHandler.cpp
  WindowEventHandlersFactory.cpp
  WindowSystemEventsHandler.cpp
  MouseDrawEventsHandler.cpp
  KeyboardEventsHandler.cpp
  ZoomEventsHandler.cpp
  ImageNavigationEventsHandler.cpp
  ImageListEventsHandler.cpp
  AnnotationsEventsHandler.cpp
  ProjectFileEventsHandler.cpp
  ExportEventsHandler.cpp
  ImportEventsHandler.cpp
  WebPageEventsHandler.cpp
  AboutEventsHandler.cpp
)

target_include_directories(
  ImageAnnotatorQt6WindowEventHandlersObj
  PRIVATE
    ${GTKMM_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
)

target_link_libraries(
  ${PROJECT_BINARY_NAME}
  ImageAnnotatorQt6WindowEventHandlersObj
)
