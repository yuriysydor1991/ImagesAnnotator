cmake_minimum_required(VERSION 3.13)

if(NOT ENABLE_UNIT_TESTS)
  return()
endif()

# disabling the asserts for the tests
add_compile_options(-DNDEBUG=1)

# The event value objects + factory that the business logic builds upon. The
# two image record value objects are no longer listed: they come from the
# ImagesAnnotator-DataDrivers library, which the enabler links into every
# target of the project.
set(ANNOTATOR_EVENTS_SRC
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/EventsFactory.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/AnnotationsDirChanged.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ImagesDirChanged.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ImagesDirProviderChanged.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/CurrentImageChanged.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/StoreRequest.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ExportPlainTxt2FolderRequest.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ExportYolo4FolderRequest.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/Export2PyTorchVisionRequest.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/DeleteCurrentImageRequest.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/LoadImagesFromWebPage.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/DisplayErrorEvent.cpp
)

# The converters, the mergers and the AnnotationsJSONSerializator test
# directories are gone along with the classes they covered: those now live in
# the ImagesAnnotator-DataDrivers library and are tested there. The exporters
# test directory went the same way into the ImagesAnnotator-DataExporters
# library.
add_subdirectory(ImagesWebPageLoader)
