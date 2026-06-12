cmake_minimum_required(VERSION 3.13)

if(NOT ENABLE_COMPONENT_TESTS)
  return()
endif()

# disabling the asserts for the tests
add_compile_options(-DNDEBUG=1)

# The custom widget component tests construct real GTK widgets and therefore
# need an initialized GTK display. They self-skip (GTEST_SKIP) when GTK cannot
# initialize, so they stay green on headless agents without a display.
add_subdirectory(custom-widgets)
