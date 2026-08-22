cmake_minimum_required(VERSION 3.13)

option(
  ENABLE_CALLGRIND
  "Set to ON to enable the valgrind (callgrind) application profiler analysis"
  OFF
)

if (NOT ENABLE_CALLGRIND)
  return()
endif()

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
  message(FATAL_ERROR "Profiling is only for the Debug builds")
endif()

find_program(VALGRIND_EXEC valgrind REQUIRED)

message(STATUS "valgrind (callgrind): ${VALGRIND_EXEC}")

add_custom_target(
  callgrind
  COMMAND ${VALGRIND_EXEC} --tool=callgrind $<TARGET_FILE:${ANNOTATOR_PRIMARY_APP_TARGET}>
  COMMENT "Executing valgrind --tool=callgrind command for the project main binary file. Outputs into the ${CMAKE_BINARY_DIR}/callgrind.out.* files"
  DEPENDS ${ANNOTATOR_PRIMARY_APP_TARGET}
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
