cmake_minimum_required(VERSION 3.13)

add_executable(
  UTEST_ImportPlainTxt2FolderRequest
  UTEST_ImportPlainTxt2FolderRequest.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ImportPlainTxt2FolderRequest.cpp
)

target_include_directories(
  UTEST_ImportPlainTxt2FolderRequest
  PRIVATE ${GENERAL_MOCKS_ROOT}/log
  PRIVATE ${CMAKE_SOURCE_DIR}
)

target_link_libraries(
  UTEST_ImportPlainTxt2FolderRequest
  GTest::gtest_main GTest::gmock
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_ImportPlainTxt2FolderRequest
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
