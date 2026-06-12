cmake_minimum_required(VERSION 3.13)

add_executable(
  UTEST_CurrentImageChanged
  UTEST_CurrentImageChanged.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/CurrentImageChanged.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ImageRecord.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ImageRecordRect.cpp
)

target_include_directories(
  UTEST_CurrentImageChanged
  PRIVATE ${GENERAL_MOCKS_ROOT}/log
  PRIVATE ${CMAKE_SOURCE_DIR}
)

target_link_libraries(
  UTEST_CurrentImageChanged
  GTest::gtest_main GTest::gmock
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_CurrentImageChanged
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
