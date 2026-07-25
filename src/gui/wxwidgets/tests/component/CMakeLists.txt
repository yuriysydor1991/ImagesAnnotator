cmake_minimum_required(VERSION 3.13)

if(NOT ENABLE_COMPONENT_TESTS)
  return()
endif()

# disabling the asserts for the tests
add_compile_options(-DNDEBUG=1)

# The wx widget component tests construct real wx windows and therefore need an
# initialized wxApp + display. They self-skip (GTEST_SKIP via WxInitFixture) when
# wx cannot initialize, so they stay green on headless agents without a display.
set(WX_TESTS_COMMON ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(CentralWorkingCanvas)
