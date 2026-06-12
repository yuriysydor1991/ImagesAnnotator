cmake_minimum_required(VERSION 3.13)

add_executable(
  UTEST_DeleteCurrentImageRequest
  UTEST_DeleteCurrentImageRequest.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/DeleteCurrentImageRequest.cpp
)

target_include_directories(
  UTEST_DeleteCurrentImageRequest
  PRIVATE ${GENERAL_MOCKS_ROOT}/log
  PRIVATE ${CMAKE_SOURCE_DIR}
)

target_link_libraries(
  UTEST_DeleteCurrentImageRequest
  GTest::gtest_main GTest::gmock
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_DeleteCurrentImageRequest
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
