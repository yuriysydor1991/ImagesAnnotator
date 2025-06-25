cmake_minimum_required(VERSION 3.13)

add_executable(
  CTEST_AnnotatorController  
  CTEST_AnnotatorController.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-business/AnnotatorController.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-business/dbs/AnnotationsDirDB.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-business/dbs/ImagesDirDB.cpp
  ${CMAKE_SOURCE_DIR}/src/log/simple-logger/SimpleLogger.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ImageRecord.cpp
  ${CMAKE_SOURCE_DIR}/src/app/ApplicationContext.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ImagesDirChanged.cpp
)

target_include_directories(
  CTEST_AnnotatorController
  PRIVATE 
    ${ANNOTATOR_BUSINESS_MOCKS_ROOT}/ImagesAnnotatorEventController
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${ANNOTATOR_BUSINESS_CTESTS_INCLUDE_DIR}
)

target_link_libraries(
  CTEST_AnnotatorController
  GTest::gtest_main GTest::gmock
)

include(GoogleTest)

gtest_add_tests(
  TARGET CTEST_AnnotatorController
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
