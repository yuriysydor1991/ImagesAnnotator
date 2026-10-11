cmake_minimum_required(VERSION 3.13)

add_executable(
  UTEST_ImportDxxwjz2pRequest
  UTEST_ImportDxxwjz2pRequest.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ImportDxxwjz2pRequest.cpp
)

target_include_directories(
  UTEST_ImportDxxwjz2pRequest
  PRIVATE ${GENERAL_MOCKS_ROOT}/log
  PRIVATE ${CMAKE_SOURCE_DIR}
)

target_link_libraries(
  UTEST_ImportDxxwjz2pRequest
  GTest::gtest_main GTest::gmock
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_ImportDxxwjz2pRequest
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
