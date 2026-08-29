cmake_minimum_required(VERSION 3.13)

# The very same ApplicationFactory.cpp the UTEST_ApplicationFactory suite drives,
# compiled the way the converter only binary compiles it. No mock of the
# app::Application class is on the include path on purpose: in this build that
# class does not exist at all.
add_executable(
  UTEST_ConverterOnlyApplicationFactory
  UTEST_ConverterOnlyApplicationFactory.cpp
  ${CMAKE_SOURCE_DIR}/src/app/ApplicationFactory.cpp
  ${APP_MOCKS_ROOT}/ApplicationContext/src/app/ApplicationContext.cpp
)

target_compile_definitions(
  UTEST_ConverterOnlyApplicationFactory
  PRIVATE IMAGES_ANNOTATOR_CONVERTER_ONLY
)

target_include_directories(
  UTEST_ConverterOnlyApplicationFactory
  PRIVATE ${APP_MOCKS_ROOT}/ApplicationContext
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
  UTEST_ConverterOnlyApplicationFactory
  GTest::gtest_main GTest::gmock
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_ConverterOnlyApplicationFactory
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
