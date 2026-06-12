cmake_minimum_required(VERSION 3.13)

add_executable(
  UTEST_DisplayErrorEvent
  UTEST_DisplayErrorEvent.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/DisplayErrorEvent.cpp
)

target_include_directories(
  UTEST_DisplayErrorEvent
  PRIVATE ${GENERAL_MOCKS_ROOT}/log
  PRIVATE ${CMAKE_SOURCE_DIR}
)

target_link_libraries(
  UTEST_DisplayErrorEvent
  GTest::gtest_main GTest::gmock
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_DisplayErrorEvent
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
