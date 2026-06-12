cmake_minimum_required(VERSION 3.13)

add_executable(
  UTEST_CURLController
  UTEST_CURLController.cpp
  ${CMAKE_SOURCE_DIR}/src/CURL/CURLController.cpp
)

target_include_directories(
  UTEST_CURLController
  PRIVATE ${GENERAL_MOCKS_ROOT}/log
  PRIVATE ${CMAKE_SOURCE_DIR}
  PRIVATE ${CURL_INCLUDE_DIRS}
)

target_link_libraries(
  UTEST_CURLController
  GTest::gtest_main GTest::gmock ${CURL_LIBRARIES}
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_CURLController
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
