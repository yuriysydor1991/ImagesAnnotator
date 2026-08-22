cmake_minimum_required(VERSION 3.13)

set(CCTNAME "CTEST_FormatsConverter")

set(EVENTS_SRC ${CMAKE_SOURCE_DIR}/src/annotator-events/events)

# Nothing of the conversion pipeline is mocked: the real event controller
# carries the request events to the real annotator business controller, which
# reaches the two dataset libraries through them.

add_executable(
  ${CCTNAME}
  CTEST_FormatsConverter.cpp
  ${CMAKE_SOURCE_DIR}/src/app/ApplicationContext.cpp
  ${CMAKE_SOURCE_DIR}/src/app/converter/ApplicationConverterComponent.cpp
  ${CMAKE_SOURCE_DIR}/src/app/converter/ApplicationFormatsConverter.cpp
  ${CMAKE_SOURCE_DIR}/src/app/converter/ConversionFormats.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-business/AnnotatorController.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-business/dbs/ImagesLoaders/ImagesDirLoader.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-business/dbs/ImagesLoaders/ImagesWebPageLoader.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/ImagesAnnotatorEventController.cpp
  ${CMAKE_SOURCE_DIR}/src/CURL/CURLController.cpp
  ${CMAKE_SOURCE_DIR}/src/helpers/SortHelper.cpp
  ${CMAKE_SOURCE_DIR}/src/helpers/ImageRecordUrlAndPathHelper.cpp
  ${CMAKE_SOURCE_DIR}/src/log/default-logger/DefaultLogger.cpp
  ${CMAKE_SOURCE_DIR}/src/log/default-logger/real-default-logger/RealDefaultLogger.cpp
  ${EVENTS_SRC}/EventsFactory.cpp
  ${EVENTS_SRC}/AnnotationsDirChanged.cpp
  ${EVENTS_SRC}/ImagesDirChanged.cpp
  ${EVENTS_SRC}/ImagesDirProviderChanged.cpp
  ${EVENTS_SRC}/CurrentImageChanged.cpp
  ${EVENTS_SRC}/StoreRequest.cpp
  ${EVENTS_SRC}/ExportPlainTxt2FolderRequest.cpp
  ${EVENTS_SRC}/ExportYolo4FolderRequest.cpp
  ${EVENTS_SRC}/Export2PyTorchVisionRequest.cpp
  ${EVENTS_SRC}/ExportUltralyticsDetectRequest.cpp
  ${EVENTS_SRC}/ExportUltralyticsObbRequest.cpp
  ${EVENTS_SRC}/ExportUltralyticsSegmentRequest.cpp
  ${EVENTS_SRC}/ExportCocoRequest.cpp
  ${EVENTS_SRC}/ExportPascalVocRequest.cpp
  ${EVENTS_SRC}/ExportCreateMLRequest.cpp
  ${EVENTS_SRC}/ImportPlainTxt2FolderRequest.cpp
  ${EVENTS_SRC}/ImportYolo4FolderRequest.cpp
  ${EVENTS_SRC}/ImportUltralyticsDetectRequest.cpp
  ${EVENTS_SRC}/ImportUltralyticsObbRequest.cpp
  ${EVENTS_SRC}/ImportUltralyticsSegmentRequest.cpp
  ${EVENTS_SRC}/ImportCocoRequest.cpp
  ${EVENTS_SRC}/ImportPascalVocRequest.cpp
  ${EVENTS_SRC}/ImportCreateMLRequest.cpp
  ${EVENTS_SRC}/ImportPyTorchVisionRequest.cpp
  ${EVENTS_SRC}/DeleteCurrentImageRequest.cpp
  ${EVENTS_SRC}/LoadImagesFromWebPage.cpp
  ${EVENTS_SRC}/DisplayErrorEvent.cpp
)

target_include_directories(
  ${CCTNAME}
  PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${APP_CTESTS_INCLUDE_DIR}
)

target_link_libraries(
  ${CCTNAME}
  GTest::gtest_main GTest::gmock ${CURL_LIBRARIES}
)

include(GoogleTest)

gtest_add_tests(
  TARGET ${CCTNAME}
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
