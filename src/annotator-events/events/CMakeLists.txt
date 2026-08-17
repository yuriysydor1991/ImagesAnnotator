cmake_minimum_required(VERSION 3.13)

# ImageRecord.cpp and ImageRecordRect.cpp are gone: both record value objects
# now come from the ImagesAnnotator-DataDrivers library and the headers left
# behind here are the aliases that keep the events::events names. The library
# itself is linked into every target of the project by its enabler
# (cmake/enablers/template-project-ImagesAnnotator-DataDrivers-enabler.cmake).
add_library(
  ImagesAnnotatorEventsObj OBJECT
  AnnotationsDirChanged.cpp
  EventsFactory.cpp
  ImagesDirChanged.cpp
  ImagesDirProviderChanged.cpp
  CurrentImageChanged.cpp
  StoreRequest.cpp
  ExportPlainTxt2FolderRequest.cpp
  ExportYolo4FolderRequest.cpp
  Export2PyTorchVisionRequest.cpp
  ExportUltralyticsDetectRequest.cpp
  ExportUltralyticsObbRequest.cpp
  ExportUltralyticsSegmentRequest.cpp
  ExportCocoRequest.cpp
  ExportPascalVocRequest.cpp
  ExportCreateMLRequest.cpp
  ImportPlainTxt2FolderRequest.cpp
  ImportYolo4FolderRequest.cpp
  ImportPyTorchVisionRequest.cpp
  DeleteCurrentImageRequest.cpp
  LoadImagesFromWebPage.cpp
  DisplayErrorEvent.cpp
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
