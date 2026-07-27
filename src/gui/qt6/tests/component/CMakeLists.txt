cmake_minimum_required(VERSION 3.13)

if(NOT ENABLE_COMPONENT_TESTS)
  return()
endif()

# disabling the asserts for the tests
add_compile_options(-DNDEBUG=1)

# The Qt widget component tests construct real Qt windows and therefore need an
# initialized QApplication. QtInitFixture uses the display when there is one and
# otherwise falls back to Qt's "offscreen" platform plugin, so - unlike the GTKmm
# and wxWidgets component suites, which self-skip without a display - these run
# everywhere, and on a CI agent with xvfb they exercise the real platform plugin.
set(QT_TESTS_COMMON ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(CentralWorkingCanvas)
add_subdirectory(WindowLoader)
