cmake_minimum_required(VERSION 3.13)

add_executable(
  UTEST_ImportCreateMLRequest
  UTEST_ImportCreateMLRequest.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ImportCreateMLRequest.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ImportUltralyticsDetectRequest.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ImportUltralyticsObbRequest.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ImportUltralyticsSegmentRequest.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ImportCocoRequest.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ImportPascalVocRequest.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ImportCreateMLRequest.cpp
)

target_include_directories(
  UTEST_ImportCreateMLRequest
  PRIVATE ${GENERAL_MOCKS_ROOT}/log
  PRIVATE ${CMAKE_SOURCE_DIR}
)

target_link_libraries(
  UTEST_ImportCreateMLRequest
  GTest::gtest_main GTest::gmock
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_ImportCreateMLRequest
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
