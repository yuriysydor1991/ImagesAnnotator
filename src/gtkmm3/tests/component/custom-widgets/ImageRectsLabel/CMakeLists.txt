cmake_minimum_required(VERSION 3.13)

add_executable(
  CTEST_ImageRectsLabel
  CTEST_ImageRectsLabel.cpp
  ${CMAKE_SOURCE_DIR}/src/gtkmm3/main-window/custom-widgets/ImageRectsLabel.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ImageRecordRect.cpp
)

target_include_directories(
  CTEST_ImageRectsLabel
  PRIVATE ${GTKMM_TESTS_COMMON}
  PRIVATE ${GTKMM_INCLUDE_DIRS}
  PRIVATE ${GENERAL_MOCKS_ROOT}/log
  PRIVATE ${CMAKE_SOURCE_DIR}
)

target_link_directories(
  CTEST_ImageRectsLabel
  PRIVATE ${GTKMM_LIBRARY_DIRS}
)

target_link_libraries(
  CTEST_ImageRectsLabel
  GTest::gtest_main GTest::gmock ${GTKMM_LIBRARIES}
)

include(GoogleTest)

gtest_add_tests(
  TARGET CTEST_ImageRectsLabel
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
