cmake_minimum_required(VERSION 3.13)

add_executable(
  UTEST_PlainTxt2FolderExporter
  UTEST_PlainTxt2FolderExporter.cpp
  ${EXPORTERS_SRC}/PlainTxt2FolderExporter.cpp
  ${HELPERS_SRC}/ImageLoader.cpp
  ${HELPERS_SRC}/ImageRecordUrlAndPathHelper.cpp
  ${CMAKE_SOURCE_DIR}/src/CURL/CURLController.cpp
  ${EVENTS_SRC}/ImageRecord.cpp
  ${EVENTS_SRC}/ImageRecordRect.cpp
)

target_include_directories(
  UTEST_PlainTxt2FolderExporter
  PRIVATE ${GENERAL_MOCKS_ROOT}/log
  PRIVATE ${GENERAL_MOCKS_ROOT}/project-global-decls
  PRIVATE ${CMAKE_SOURCE_DIR}
  PRIVATE ${CURL_INCLUDE_DIRS}
)

target_link_libraries(
  UTEST_PlainTxt2FolderExporter
  GTest::gtest_main GTest::gmock ${CURL_LIBRARIES}
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_PlainTxt2FolderExporter
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
