cmake_minimum_required(VERSION 3.13)

# The window event handlers sit on top of the whole window subsystem (the
# loader, the main window context, the annotator controller, exporters, the
# CURL controller, ...). Rather than re-listing that closure of source files,
# the component test links against the project object libraries directly.
add_executable(
  CTEST_WindowEventHandlers
  CTEST_WindowEventHandlers.cpp
)

target_include_directories(
  CTEST_WindowEventHandlers
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}     # FakeWindowLoader.h
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..  # GtkInitFixture.h
  PRIVATE ${GTKMM_INCLUDE_DIRS}
  PRIVATE ${CURL_INCLUDE_DIRS}
  PRIVATE ${CMAKE_SOURCE_DIR}
  PRIVATE ${CMAKE_BINARY_DIR}
)

target_link_directories(
  CTEST_WindowEventHandlers
  PRIVATE ${GTKMM_LIBRARY_DIRS}
)

target_link_libraries(
  CTEST_WindowEventHandlers
  ${EVENT_HANDLERS_OBJ_LIBS}
  GTest::gtest_main GTest::gmock ${GTKMM_LIBRARIES} ${CURL_LIBRARIES}
)

include(GoogleTest)

gtest_add_tests(
  TARGET CTEST_WindowEventHandlers
  TEST_SUFFIX .noArgs
  TEST_LIST noArgsTests
)

set_tests_properties(${noArgsTests} PROPERTIES TIMEOUT 600)
