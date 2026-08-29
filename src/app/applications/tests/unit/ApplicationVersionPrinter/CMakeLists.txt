cmake_minimum_required(VERSION 3.13)

add_executable(
  UTEST_ApplicationVersionPrinter 
  UTEST_ApplicationVersionPrinter.cpp
  ${CMAKE_SOURCE_DIR}/src/app/applications/ApplicationVersionPrinter.cpp
  ${APP_MOCKS_ROOT}/ApplicationContext/src/app/ApplicationContext.cpp
)

target_include_directories(
  UTEST_ApplicationVersionPrinter
  PRIVATE ${APP_MOCKS_ROOT}/ApplicationContext
  PRIVATE ${APP_MOCKS_ROOT}/project-global-decls
  PRIVATE ${APP_MOCKS_ROOT}/log
  PRIVATE ${CMAKE_SOURCE_DIR}
)

target_link_libraries(
  UTEST_ApplicationVersionPrinter
  GTest::gtest_main GTest::gmock
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_ApplicationVersionPrinter
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
