cmake_minimum_required(VERSION 3.13)

add_executable(
  UTEST_AbstractWindowEventHandler
  UTEST_AbstractWindowEventHandler.cpp
)

target_include_directories(
  UTEST_AbstractWindowEventHandler
  PRIVATE ${GTKMM_INCLUDE_DIRS}
  PRIVATE ${CURL_INCLUDE_DIRS}
  PRIVATE ${CMAKE_SOURCE_DIR}
  PRIVATE ${CMAKE_BINARY_DIR}
)

target_link_directories(
  UTEST_AbstractWindowEventHandler
  PRIVATE ${GTKMM_LIBRARY_DIRS}
)

target_link_libraries(
  UTEST_AbstractWindowEventHandler
  ${EVENT_HANDLERS_OBJ_LIBS}
  GTest::gtest_main GTest::gmock ${GTKMM_LIBRARIES} ${CURL_LIBRARIES}
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_AbstractWindowEventHandler
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
