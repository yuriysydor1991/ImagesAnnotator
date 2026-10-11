cmake_minimum_required(VERSION 3.13)

set(EVENTS_SRC ${CMAKE_SOURCE_DIR}/src/annotator-events/events)

add_executable(
  UTEST_ImagesAnnotatorEventController
  UTEST_ImagesAnnotatorEventController.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/ImagesAnnotatorEventController.cpp
  ${EVENTS_SRC}/EventsFactory.cpp
  ${EVENTS_SRC}/AnnotationsDirChanged.cpp
  ${EVENTS_SRC}/ImagesDirChanged.cpp
  ${EVENTS_SRC}/ImagesDirProviderChanged.cpp
  ${EVENTS_SRC}/CurrentImageChanged.cpp
  ${EVENTS_SRC}/StoreRequest.cpp
  ${EVENTS_SRC}/ExportPlainTxt2FolderRequest.cpp
  ${EVENTS_SRC}/ExportYolo4FolderRequest.cpp
  ${EVENTS_SRC}/ExportUltralyticsDetectRequest.cpp
  ${EVENTS_SRC}/ExportUltralyticsObbRequest.cpp
  ${EVENTS_SRC}/ExportUltralyticsSegmentRequest.cpp
  ${EVENTS_SRC}/ExportCocoRequest.cpp
  ${EVENTS_SRC}/ExportPascalVocRequest.cpp
  ${EVENTS_SRC}/ExportCreateMLRequest.cpp
  ${EVENTS_SRC}/ExportDxxwjz2Request.cpp
  ${EVENTS_SRC}/ExportDxxwjz2pRequest.cpp
  ${EVENTS_SRC}/ExportDxxwjz3Request.cpp
  ${EVENTS_SRC}/ExportDxxwjz3pRequest.cpp
  ${EVENTS_SRC}/ImportPlainTxt2FolderRequest.cpp
  ${EVENTS_SRC}/ImportYolo4FolderRequest.cpp
  ${EVENTS_SRC}/ImportUltralyticsDetectRequest.cpp
  ${EVENTS_SRC}/ImportUltralyticsObbRequest.cpp
  ${EVENTS_SRC}/ImportUltralyticsSegmentRequest.cpp
  ${EVENTS_SRC}/ImportCocoRequest.cpp
  ${EVENTS_SRC}/ImportPascalVocRequest.cpp
  ${EVENTS_SRC}/ImportCreateMLRequest.cpp
  ${EVENTS_SRC}/ImportPyTorchVisionRequest.cpp
  ${EVENTS_SRC}/Export2PyTorchVisionRequest.cpp
  ${EVENTS_SRC}/DeleteCurrentImageRequest.cpp
  ${EVENTS_SRC}/LoadImagesFromWebPage.cpp
  ${EVENTS_SRC}/DisplayErrorEvent.cpp
)

target_include_directories(
  UTEST_ImagesAnnotatorEventController
  PRIVATE ${GENERAL_MOCKS_ROOT}/log
  PRIVATE ${GENERAL_MOCKS_ROOT}/project-global-decls
  PRIVATE ${CMAKE_SOURCE_DIR}
)

target_link_libraries(
  UTEST_ImagesAnnotatorEventController
  GTest::gtest_main GTest::gmock
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_ImagesAnnotatorEventController
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
