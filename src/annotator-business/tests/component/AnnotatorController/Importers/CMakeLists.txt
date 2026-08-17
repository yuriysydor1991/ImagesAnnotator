cmake_minimum_required(VERSION 3.13)

set(CCTNAME "CTEST_Importers")

# Drives the AnnotatorController import request handlers, which build their
# importers through the ImagesAnnotator-DataImporters library facade.

add_executable(
  ${CCTNAME}
  CTEST_Importers.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-business/AnnotatorController.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-business/dbs/ImagesLoaders/ImagesDirLoader.cpp
  ${CMAKE_SOURCE_DIR}/src/log/default-logger/DefaultLogger.cpp
  ${CMAKE_SOURCE_DIR}/src/log/default-logger/real-default-logger/RealDefaultLogger.cpp
  ${CMAKE_SOURCE_DIR}/src/app/ApplicationContext.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ImagesDirChanged.cpp
  ${ANNOTATIONCONTROLLER_CTEST_ROOT}/CTEST_AnnotatorController.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/EventsFactory.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ImagesDirProviderChanged.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/CurrentImageChanged.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/AnnotationsDirChanged.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/StoreRequest.cpp
  ${CMAKE_SOURCE_DIR}/src/helpers/SortHelper.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ExportPlainTxt2FolderRequest.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ExportYolo4FolderRequest.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ExportUltralyticsDetectRequest.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ExportUltralyticsObbRequest.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ExportUltralyticsSegmentRequest.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ExportCocoRequest.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ExportPascalVocRequest.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ExportCreateMLRequest.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ImportPlainTxt2FolderRequest.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ImportYolo4FolderRequest.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ImportPyTorchVisionRequest.cpp
  ${GUI_IMAGE_CROPPER_SRC}
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/Export2PyTorchVisionRequest.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/DeleteCurrentImageRequest.cpp
  ${CMAKE_SOURCE_DIR}/src/helpers/ImageLoader.cpp
  ${CMAKE_SOURCE_DIR}/src/helpers/ImageRecordUrlAndPathHelper.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/DisplayErrorEvent.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-business/dbs/ImagesLoaders/ImagesWebPageLoader.cpp
  ${CMAKE_SOURCE_DIR}/src/CURL/CURLController.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/LoadImagesFromWebPage.cpp
)

target_include_directories(
  ${CCTNAME}
  PRIVATE 
    ${ANNOTATOR_BUSINESS_MOCKS_ROOT}/ImagesAnnotatorEventController
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${ANNOTATOR_BUSINESS_CTESTS_INCLUDE_DIR}
)

target_link_libraries(
  ${CCTNAME}
  GTest::gtest_main GTest::gmock ${CURL_LIBRARIES} ${GTKMM_LIBRARIES}
)

include(GoogleTest)

gtest_add_tests(
  TARGET ${CCTNAME}
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
