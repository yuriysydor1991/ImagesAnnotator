cmake_minimum_required(VERSION 3.13)

set(EVENTS_SRC ${CMAKE_SOURCE_DIR}/src/annotator-events/events)

add_executable(
  UTEST_EventsFactory
  UTEST_EventsFactory.cpp
  ${EVENTS_SRC}/EventsFactory.cpp
  ${EVENTS_SRC}/AnnotationsDirChanged.cpp
  ${EVENTS_SRC}/ImagesDirChanged.cpp
  ${EVENTS_SRC}/ImagesDirProviderChanged.cpp
  ${EVENTS_SRC}/CurrentImageChanged.cpp
  ${EVENTS_SRC}/ImageRecord.cpp
  ${EVENTS_SRC}/ImageRecordRect.cpp
  ${EVENTS_SRC}/StoreRequest.cpp
  ${EVENTS_SRC}/ExportPlainTxt2FolderRequest.cpp
  ${EVENTS_SRC}/ExportYolo4FolderRequest.cpp
  ${EVENTS_SRC}/Export2PyTorchVisionRequest.cpp
  ${EVENTS_SRC}/DeleteCurrentImageRequest.cpp
  ${EVENTS_SRC}/LoadImagesFromWebPage.cpp
  ${EVENTS_SRC}/DisplayErrorEvent.cpp
)

target_include_directories(
  UTEST_EventsFactory
  PRIVATE ${GENERAL_MOCKS_ROOT}/log
  PRIVATE ${CMAKE_SOURCE_DIR}
)

target_link_libraries(
  UTEST_EventsFactory
  GTest::gtest_main GTest::gmock
)

include(GoogleTest)

gtest_add_tests(
  TARGET UTEST_EventsFactory
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
