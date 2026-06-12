cmake_minimum_required(VERSION 3.13)

add_executable(
  CTEST_ImagePathLabel
  CTEST_ImagePathLabel.cpp
  ${CMAKE_SOURCE_DIR}/src/gtkmm3/main-window/custom-widgets/ImagePathLabel.cpp
  ${CMAKE_SOURCE_DIR}/src/helpers/ImageRecordUrlAndPathHelper.cpp
  ${CMAKE_SOURCE_DIR}/src/CURL/CURLController.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ImageRecord.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ImageRecordRect.cpp
)

target_include_directories(
  CTEST_ImagePathLabel
  PRIVATE ${GTKMM_TESTS_COMMON}
  PRIVATE ${GTKMM_INCLUDE_DIRS}
  PRIVATE ${GENERAL_MOCKS_ROOT}/log
  PRIVATE ${CMAKE_SOURCE_DIR}
  PRIVATE ${CURL_INCLUDE_DIRS}
)

target_link_directories(
  CTEST_ImagePathLabel
  PRIVATE ${GTKMM_LIBRARY_DIRS}
)

target_link_libraries(
  CTEST_ImagePathLabel
  GTest::gtest_main GTest::gmock ${GTKMM_LIBRARIES} ${CURL_LIBRARIES}
)

include(GoogleTest)

gtest_add_tests(
  TARGET CTEST_ImagePathLabel
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
