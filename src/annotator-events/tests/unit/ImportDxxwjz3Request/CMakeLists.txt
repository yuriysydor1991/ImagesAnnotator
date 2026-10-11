cmake_minimum_required(VERSION 3.13)

add_executable(
  UTEST_ImportDxxwjz3Request
  UTEST_ImportDxxwjz3Request.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ImportDxxwjz3Request.cpp
)

target_include_directories(
  UTEST_ImportDxxwjz3Request
  PRIVATE ${GENERAL_MOCKS_ROOT}/log
  PRIVATE ${CMAKE_SOURCE_DIR}
)

target_link_libraries(
  UTEST_ImportDxxwjz3Request
  GTest::gtest_main GTest::gmock
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_ImportDxxwjz3Request
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
