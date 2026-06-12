cmake_minimum_required(VERSION 3.13)

add_executable(
  UTEST_Export2PyTorchVisionRequest
  UTEST_Export2PyTorchVisionRequest.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/Export2PyTorchVisionRequest.cpp
)

target_include_directories(
  UTEST_Export2PyTorchVisionRequest
  PRIVATE ${GENERAL_MOCKS_ROOT}/log
  PRIVATE ${CMAKE_SOURCE_DIR}
)

target_link_libraries(
  UTEST_Export2PyTorchVisionRequest
  GTest::gtest_main GTest::gmock
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_Export2PyTorchVisionRequest
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
