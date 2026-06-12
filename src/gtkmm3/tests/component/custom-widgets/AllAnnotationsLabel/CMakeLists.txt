cmake_minimum_required(VERSION 3.13)

add_executable(
  CTEST_AllAnnotationsLabel
  CTEST_AllAnnotationsLabel.cpp
  ${CMAKE_SOURCE_DIR}/src/gtkmm3/main-window/custom-widgets/AllAnnotationsLabel.cpp
)

target_include_directories(
  CTEST_AllAnnotationsLabel
  PRIVATE ${GTKMM_TESTS_COMMON}
  PRIVATE ${GTKMM_INCLUDE_DIRS}
  PRIVATE ${GENERAL_MOCKS_ROOT}/log
  PRIVATE ${CMAKE_SOURCE_DIR}
)

target_link_directories(
  CTEST_AllAnnotationsLabel
  PRIVATE ${GTKMM_LIBRARY_DIRS}
)

target_link_libraries(
  CTEST_AllAnnotationsLabel
  GTest::gtest_main GTest::gmock ${GTKMM_LIBRARIES}
)

include(GoogleTest)

gtest_add_tests(
  TARGET CTEST_AllAnnotationsLabel
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
