cmake_minimum_required(VERSION 3.13)

add_executable(
  UTEST_ImportUltralyticsDetectRequest
  UTEST_ImportUltralyticsDetectRequest.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ImportUltralyticsDetectRequest.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ImportUltralyticsDetectRequest.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ImportUltralyticsObbRequest.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ImportUltralyticsSegmentRequest.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ImportCocoRequest.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ImportPascalVocRequest.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ImportCreateMLRequest.cpp
)

target_include_directories(
  UTEST_ImportUltralyticsDetectRequest
  PRIVATE ${GENERAL_MOCKS_ROOT}/log
  PRIVATE ${CMAKE_SOURCE_DIR}
)

target_link_libraries(
  UTEST_ImportUltralyticsDetectRequest
  GTest::gtest_main GTest::gmock
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_ImportUltralyticsDetectRequest
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
