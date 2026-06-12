cmake_minimum_required(VERSION 3.13)

add_executable(
  UTEST_ImagesWebPageLoader
  UTEST_ImagesWebPageLoader.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-business/dbs/ImagesLoaders/ImagesWebPageLoader.cpp
  ${CMAKE_SOURCE_DIR}/src/CURL/CURLController.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ImageRecord.cpp
  ${CMAKE_SOURCE_DIR}/src/annotator-events/events/ImageRecordRect.cpp
)

target_include_directories(
  UTEST_ImagesWebPageLoader
  PRIVATE ${GENERAL_MOCKS_ROOT}/log
  PRIVATE ${GENERAL_MOCKS_ROOT}/project-global-decls
  PRIVATE ${CMAKE_SOURCE_DIR}
  PRIVATE ${CURL_INCLUDE_DIRS}
)

target_link_libraries(
  UTEST_ImagesWebPageLoader
  GTest::gtest_main GTest::gmock ${CURL_LIBRARIES}
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_ImagesWebPageLoader
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
