cmake_minimum_required(VERSION 3.13)

add_executable(
  UTEST_WindowDataContext
  UTEST_WindowDataContext.cpp
  ${CMAKE_SOURCE_DIR}/src/gtkmm3/main-window/WindowDataContext.cpp
)

target_include_directories(
  UTEST_WindowDataContext
  PRIVATE ${GTKMM_INCLUDE_DIRS}
  PRIVATE ${GENERAL_MOCKS_ROOT}/log
  PRIVATE ${CMAKE_SOURCE_DIR}
)

target_link_directories(
  UTEST_WindowDataContext
  PRIVATE ${GTKMM_LIBRARY_DIRS}
)

target_link_libraries(
  UTEST_WindowDataContext
  GTest::gtest_main GTest::gmock ${GTKMM_LIBRARIES}
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_WindowDataContext
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
