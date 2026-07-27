cmake_minimum_required(VERSION 3.13)

add_executable(
  CTEST_Qt6CentralWorkingCanvas
  CTEST_CentralWorkingCanvas.cpp
)

target_include_directories(
  CTEST_Qt6CentralWorkingCanvas
  PRIVATE
  ${QT_TESTS_COMMON}
  PRIVATE ${GTKMM_INCLUDE_DIRS}
  PRIVATE ${CURL_INCLUDE_DIRS}
  PRIVATE ${CMAKE_SOURCE_DIR}
  PRIVATE ${CMAKE_BINARY_DIR}
)

target_link_directories(
  CTEST_Qt6CentralWorkingCanvas
  PRIVATE ${GTKMM_LIBRARY_DIRS}
)

target_link_libraries(
  CTEST_Qt6CentralWorkingCanvas
  ${EVENT_HANDLERS_OBJ_LIBS}
  GTest::gtest_main GTest::gmock ${GTKMM_LIBRARIES} ${CURL_LIBRARIES}
)

include(GoogleTest)

gtest_add_tests(
  TARGET CTEST_Qt6CentralWorkingCanvas
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
