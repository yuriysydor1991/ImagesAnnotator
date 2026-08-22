cmake_minimum_required(VERSION 3.13)

add_executable(
  UTEST_ConversionFormats
  UTEST_ConversionFormats.cpp
  ${CMAKE_SOURCE_DIR}/src/app/converter/ConversionFormats.cpp
)

target_include_directories(
  UTEST_ConversionFormats
  PRIVATE ${CMAKE_SOURCE_DIR}
)

target_link_libraries(
  UTEST_ConversionFormats
  GTest::gtest_main GTest::gmock
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_ConversionFormats
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
