cmake_minimum_required(VERSION 3.13)

add_executable(
  UTEST_ImagesDirProviderChanged
  UTEST_ImagesDirProviderChanged.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ImagesDirProviderChanged.cpp
)

target_include_directories(
  UTEST_ImagesDirProviderChanged
  PRIVATE ${GENERAL_MOCKS_ROOT}/log
  PRIVATE ${CMAKE_SOURCE_DIR}
)

target_link_libraries(
  UTEST_ImagesDirProviderChanged
  GTest::gtest_main GTest::gmock
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_ImagesDirProviderChanged
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
