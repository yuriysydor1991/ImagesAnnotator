cmake_minimum_required(VERSION 3.13)

add_executable(
  UTEST_ImageRecordUrlAndPathHelper
  UTEST_ImageRecordUrlAndPathHelper.cpp
  ${CMAKE_SOURCE_DIR}/src/helpers/ImageRecordUrlAndPathHelper.cpp
  ${CMAKE_SOURCE_DIR}/src/CURL/CURLController.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ImageRecord.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ImageRecordRect.cpp
)

target_include_directories(
  UTEST_ImageRecordUrlAndPathHelper
  PRIVATE ${GENERAL_MOCKS_ROOT}/log
  PRIVATE ${CMAKE_SOURCE_DIR}
  PRIVATE ${CURL_INCLUDE_DIRS}
)

target_link_libraries(
  UTEST_ImageRecordUrlAndPathHelper
  GTest::gtest_main GTest::gmock ${CURL_LIBRARIES}
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_ImageRecordUrlAndPathHelper
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
