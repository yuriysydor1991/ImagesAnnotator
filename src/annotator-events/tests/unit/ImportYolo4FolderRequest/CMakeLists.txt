cmake_minimum_required(VERSION 3.13)

add_executable(
  UTEST_ImportYolo4FolderRequest
  UTEST_ImportYolo4FolderRequest.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ImportYolo4FolderRequest.cpp
)

target_include_directories(
  UTEST_ImportYolo4FolderRequest
  PRIVATE ${GENERAL_MOCKS_ROOT}/log
  PRIVATE ${CMAKE_SOURCE_DIR}
)

target_link_libraries(
  UTEST_ImportYolo4FolderRequest
  GTest::gtest_main GTest::gmock
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_ImportYolo4FolderRequest
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
