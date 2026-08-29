cmake_minimum_required(VERSION 3.13)

add_executable(
  UTEST_ApplicationFactory  
  UTEST_ApplicationFactory.cpp
  ${CMAKE_SOURCE_DIR}/src/app/ApplicationFactory.cpp
  ${APP_MOCKS_ROOT}/ApplicationContext/src/app/ApplicationContext.cpp
)

target_include_directories(
  UTEST_ApplicationFactory
  PRIVATE ${APP_MOCKS_ROOT}/ApplicationContext
  PRIVATE ${APPLICATIONS_MOCKS_ROOT}/Application
  PRIVATE ${APPLICATIONS_MOCKS_ROOT}/ApplicationHelpPrinter
  PRIVATE ${APPLICATIONS_MOCKS_ROOT}/ApplicationVersionPrinter
  PRIVATE ${APP_MOCKS_ROOT}/CommandLineParser
  PRIVATE ${APPLICATIONS_MOCKS_ROOT}/ApplicationConverterComponent
  PRIVATE ${APPLICATIONS_MOCKS_ROOT}/ApplicationFormatsConverter
  PRIVATE ${APP_MOCKS_ROOT}/project-global-decls
  PRIVATE ${APP_MOCKS_ROOT}/log
  PRIVATE ${CMAKE_SOURCE_DIR}
)

target_link_libraries(
  UTEST_ApplicationFactory
  GTest::gtest_main GTest::gmock
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_ApplicationFactory
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
