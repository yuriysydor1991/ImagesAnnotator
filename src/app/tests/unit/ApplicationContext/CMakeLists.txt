cmake_minimum_required(VERSION 3.13)

add_executable(
  UTEST_ApplicationContext  
  UTEST_ApplicationContext.cpp
  ${CMAKE_SOURCE_DIR}/src/app/ApplicationContext.cpp
)

target_include_directories(
  UTEST_ApplicationContext
  PRIVATE ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR}
)

target_link_libraries(
  UTEST_ApplicationContext
  GTest::gtest_main GTest::gmock
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_ApplicationContext
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 60)
