cmake_minimum_required(VERSION 3.13)

add_executable(
  UTEST_WxWindowEventHandlersFactory
  UTEST_WindowEventHandlersFactory.cpp
)

target_include_directories(
  UTEST_WxWindowEventHandlersFactory
  PRIVATE ${GTKMM_INCLUDE_DIRS}
  PRIVATE ${CURL_INCLUDE_DIRS}
  PRIVATE ${CMAKE_SOURCE_DIR}
  PRIVATE ${CMAKE_BINARY_DIR}
)

target_link_directories(
  UTEST_WxWindowEventHandlersFactory
  PRIVATE ${GTKMM_LIBRARY_DIRS}
)

target_link_libraries(
  UTEST_WxWindowEventHandlersFactory
  ${EVENT_HANDLERS_OBJ_LIBS}
  GTest::gtest_main GTest::gmock ${GTKMM_LIBRARIES} ${CURL_LIBRARIES}
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_WxWindowEventHandlersFactory
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
