cmake_minimum_required(VERSION 3.13)

add_executable(
  UTEST_AnnotationsDirChanged
  UTEST_AnnotationsDirChanged.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/AnnotationsDirChanged.cpp
)

target_include_directories(
  UTEST_AnnotationsDirChanged
  PRIVATE ${GENERAL_MOCKS_ROOT}/log
  PRIVATE ${CMAKE_SOURCE_DIR}
)

target_link_libraries(
  UTEST_AnnotationsDirChanged
  GTest::gtest_main GTest::gmock
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_AnnotationsDirChanged
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
