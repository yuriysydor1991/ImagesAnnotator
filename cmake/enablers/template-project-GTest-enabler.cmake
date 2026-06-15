cmake_minimum_required(VERSION 3.13)

option(
  ENABLE_UNIT_TESTS 
  "Set to ON value if unit tests build and run should be available"
  OFF
)

option(
  ENABLE_COMPONENT_TESTS 
  "Set to ON value if the component tests build and run should be available"
  OFF
)

option(
  GTEST_TRY_SYSTEM_PROBE 
  "Set to ON value if current project CMake files should probe the system GTest"
  ON
)

if(NOT (ENABLE_UNIT_TESTS OR ENABLE_COMPONENT_TESTS))
  return()
endif()

enable_testing()

set(TEMPLATE_APP_GTEST_GIT "https://github.com/google/googletest.git" CACHE STRING "The GoogleTest git source repository")
set(TEMPLATE_APP_GTEST_GIT_TAG "v1.16.0" CACHE STRING "The GoogleTest project git repository tag of interest")

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  # For Windows: Prevent overriding the parent project's compiler/linker settings
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

if (GTEST_TRY_SYSTEM_PROBE)
  set(DISABLE_SYSTEM_PROBE "")
else()
  set(DISABLE_SYSTEM_PROBE "DISABLE_SYSTEM_PROBE")
endif()

template_project_default_3rdparty_enabler(
    NAME GTest 
    GIT_REPOSITORY ${TEMPLATE_APP_GTEST_GIT}
    GIT_TAG ${TEMPLATE_APP_GTEST_GIT_TAG}
    "${DISABLE_SYSTEM_PROBE}"
)
