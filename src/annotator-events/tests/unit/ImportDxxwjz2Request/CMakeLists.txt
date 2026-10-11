cmake_minimum_required(VERSION 3.13)

add_executable(
  UTEST_ImportDxxwjz2Request
  UTEST_ImportDxxwjz2Request.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ImportDxxwjz2Request.cpp
)

target_include_directories(
  UTEST_ImportDxxwjz2Request
  PRIVATE ${GENERAL_MOCKS_ROOT}/log
  PRIVATE ${CMAKE_SOURCE_DIR}
)

target_link_libraries(
  UTEST_ImportDxxwjz2Request
  GTest::gtest_main GTest::gmock
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_ImportDxxwjz2Request
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
