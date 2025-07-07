cmake_minimum_required(VERSION 3.13)

add_library(
  ImagesAnnotatorEventsObj OBJECT
  AnnotationsDirChanged.cpp
  EventsFactory.cpp
  ImagesDirChanged.cpp
  ImageRecord.cpp
  ImagesDirProviderChanged.cpp
  CurrentImageChanged.cpp
  ImageRecordRect.cpp
  StoreRequest.cpp
  ExportPlainTxt2FolderRequest.cpp
)

target_include_directories(
  ImagesAnnotatorEventsObj
  PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
)

target_link_libraries(
  ${PROJECT_BINARY_NAME}
  ImagesAnnotatorEventsObj
)
