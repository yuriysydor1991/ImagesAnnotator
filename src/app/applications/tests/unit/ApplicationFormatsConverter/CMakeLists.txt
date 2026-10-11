cmake_minimum_required(VERSION 3.13)

set(EVENTS_SRC ${CMAKE_SOURCE_DIR}/src/annotator-events/events)

# The real events factory and the real request events are compiled in: the
# format dispatch of the converter is checked by the very type of the event it
# submits, so a stand-in for them would check nothing.

add_executable(
  UTEST_ApplicationFormatsConverter
  UTEST_ApplicationFormatsConverter.cpp
  ${CMAKE_SOURCE_DIR}/src/app/applications/converter/ApplicationFormatsConverter.cpp
  ${CMAKE_SOURCE_DIR}/src/app/applications/converter/ConversionFormats.cpp
  ${APP_MOCKS_ROOT}/ApplicationContext/src/app/ApplicationContext.cpp
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
  ${EVENTS_SRC}/ImportDxxwjz3pRequest.cpp
  ${EVENTS_SRC}/ImportDxxwjz2pRequest.cpp
  ${EVENTS_SRC}/ImportDxxwjz3Request.cpp
  ${EVENTS_SRC}/ImportDxxwjz2Request.cpp
  ${EVENTS_SRC}/Export2PyTorchVisionRequest.cpp
  ${EVENTS_SRC}/DeleteCurrentImageRequest.cpp
  ${EVENTS_SRC}/LoadImagesFromWebPage.cpp
  ${EVENTS_SRC}/DisplayErrorEvent.cpp
)

target_include_directories(
  UTEST_ApplicationFormatsConverter
  PRIVATE ${APP_MOCKS_ROOT}/ApplicationContext
  PRIVATE ${APP_MOCKS_ROOT}/ImagesAnnotatorEventController
  PRIVATE ${APP_MOCKS_ROOT}/project-global-decls
  PRIVATE ${APP_MOCKS_ROOT}/log
  PRIVATE ${CMAKE_SOURCE_DIR}
)

target_link_libraries(
  UTEST_ApplicationFormatsConverter
  GTest::gtest_main GTest::gmock
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_ApplicationFormatsConverter
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
