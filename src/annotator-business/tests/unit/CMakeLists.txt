cmake_minimum_required(VERSION 3.13)

if(NOT ENABLE_UNIT_TESTS)
  return()
endif()

# disabling the asserts for the tests
add_compile_options(-DNDEBUG=1)

# The event value objects + factory that the business logic builds upon. Shared
# by the tests that exercise the JSON <-> record converters.
set(ANNOTATOR_EVENTS_SRC
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/EventsFactory.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/AnnotationsDirChanged.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ImagesDirChanged.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ImagesDirProviderChanged.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/CurrentImageChanged.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ImageRecord.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ImageRecordRect.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/StoreRequest.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ExportPlainTxt2FolderRequest.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ExportYolo4FolderRequest.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/Export2PyTorchVisionRequest.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/DeleteCurrentImageRequest.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/LoadImagesFromWebPage.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/DisplayErrorEvent.cpp
)

add_subdirectory(converters)
add_subdirectory(mergers)
add_subdirectory(AnnotationsJSONSerializator)
add_subdirectory(exporters)
