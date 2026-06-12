cmake_minimum_required(VERSION 3.13)

add_executable(
  UTEST_TypeHelper
  UTEST_TypeHelper.cpp
)

target_include_directories(
  UTEST_TypeHelper
  PRIVATE ${GENERAL_MOCKS_ROOT}/log
  PRIVATE ${CMAKE_SOURCE_DIR}
)

target_link_libraries(
  UTEST_TypeHelper
  GTest::gtest_main GTest::gmock
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_TypeHelper
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
