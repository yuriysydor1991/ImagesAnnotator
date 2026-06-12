cmake_minimum_required(VERSION 3.13)

add_executable(
  UTEST_SimpleLogger
  UTEST_SimpleLogger.cpp
  ${CMAKE_SOURCE_DIR}/src/log/simple-logger/SimpleLogger.cpp
)

target_include_directories(
  UTEST_SimpleLogger
  PRIVATE ${GENERAL_MOCKS_ROOT}/project-global-decls
  PRIVATE ${CMAKE_SOURCE_DIR}
)

target_link_libraries(
  UTEST_SimpleLogger
  GTest::gtest_main GTest::gmock
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_SimpleLogger
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
