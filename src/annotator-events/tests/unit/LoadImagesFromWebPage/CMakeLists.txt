cmake_minimum_required(VERSION 3.13)

add_executable(
  UTEST_LoadImagesFromWebPage
  UTEST_LoadImagesFromWebPage.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/LoadImagesFromWebPage.cpp
)

target_include_directories(
  UTEST_LoadImagesFromWebPage
  PRIVATE ${GENERAL_MOCKS_ROOT}/log
  PRIVATE ${CMAKE_SOURCE_DIR}
)

target_link_libraries(
  UTEST_LoadImagesFromWebPage
  GTest::gtest_main GTest::gmock
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_LoadImagesFromWebPage
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
