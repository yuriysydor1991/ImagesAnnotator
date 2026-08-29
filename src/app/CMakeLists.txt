cmake_minimum_required(VERSION 3.13)

# The two mock roots of the component, declared here rather than in the tests
# directory of each half, so that a suite of either one reaches both: the
# stand-ins for the IApplication descendants live beside those classes in the
# applications subdirectory, while the stand-ins for everything else - the
# context, the command line parser and the classes of the other components -
# live under this directory.
set(APP_MOCKS_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/tests/mocks")
set(APPLICATIONS_MOCKS_ROOT
    "${CMAKE_CURRENT_SOURCE_DIR}/applications/tests/mocks")

# What the component carries beside the applications themselves: the context
# they are handed, the command line parser which fills it and the factory which
# creates them. The IApplication descendants are handed to the application
# targets by src/app/applications/CMakeLists.txt instead.
annotator_app_targets_sources(
  ${CMAKE_CURRENT_SOURCE_DIR}/ApplicationContext.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ApplicationFactory.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/CommandLineParser.cpp
)

add_subdirectory(applications)
add_subdirectory(tests)
