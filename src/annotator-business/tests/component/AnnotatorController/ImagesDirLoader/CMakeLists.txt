cmake_minimum_required(VERSION 3.13)

set(CCTNAME "CTEST_ImagesDirLoader")

add_executable(
  ${CCTNAME}
  CTEST_ImagesDirLoader.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-business/AnnotatorController.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-business/dbs/AnnotationsDBs/AnnotationsDirDB.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-business/dbs/ImagesLoaders/ImagesDirLoader.cpp
  ${CMAKE_SOURCE_DIR}/src/log/simple-logger/SimpleLogger.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ImageRecord.cpp
  ${CMAKE_SOURCE_DIR}/src/app/ApplicationContext.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ImagesDirChanged.cpp
  ${ANNOTATIONCONTROLLER_CTEST_ROOT}/CTEST_AnnotatorController.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-business/dbs/AnnotationsDBs/converters/JSON2ImageRecordsConverter.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-business/dbs/AnnotationsDBs/AnnotationsJSONSerializator.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/EventsFactory.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ImageRecordRect.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/AnnotationsDirChanged.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ImagesDirProviderChanged.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/CurrentImageChanged.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-business/dbs/AnnotationsDBs/converters/ImageRecords2JSONConverter.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/StoreRequest.cpp
  ${CMAKE_SOURCE_DIR}/src/helpers/SortHelper.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-business/dbs/AnnotationsDBs/converters/ImageRecordRectJSON2RecordConverter.cpp
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
  GTest::gtest_main GTest::gmock
)

include(GoogleTest)

gtest_add_tests(
  TARGET ${CCTNAME}
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
