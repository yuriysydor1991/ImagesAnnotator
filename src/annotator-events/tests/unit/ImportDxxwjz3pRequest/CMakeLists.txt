cmake_minimum_required(VERSION 3.13)

add_executable(
  UTEST_ImportDxxwjz3pRequest
  UTEST_ImportDxxwjz3pRequest.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ImportDxxwjz3pRequest.cpp
)

target_include_directories(
  UTEST_ImportDxxwjz3pRequest
  PRIVATE ${GENERAL_MOCKS_ROOT}/log
  PRIVATE ${CMAKE_SOURCE_DIR}
)

target_link_libraries(
  UTEST_ImportDxxwjz3pRequest
  GTest::gtest_main GTest::gmock
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_ImportDxxwjz3pRequest
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
