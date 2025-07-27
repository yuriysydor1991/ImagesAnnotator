cmake_minimum_required(VERSION 3.13)

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
  message(FATAL_ERROR "Profiling is only for the Debug builds")
endif()

find_program(GPROF_EXEC gprof REQUIRED)

set(EXTRA_COMPILE_OPTIONS
  ${EXTRA_COMPILE_OPTIONS}
  -pg -fprofile-generate
)

set(EXTRA_LINK_OPTIONS
  ${EXTRA_LINK_OPTIONS}
  -pg -fprofile-generate
)

set(
  GPROF_ANALYZE_TXT_DST
  ${CMAKE_BINARY_DIR}/gprof-analyze.txt
  CACHE STRING "The gprof analyze result txt file"
)

message(STATUS "gprof: ${GPROF_EXEC}")
message(STATUS "gprof analyze dst: ${GPROF_ANALYZE_TXT_DST}")

add_custom_target(
  gprof-analyze
  COMMAND $<TARGET_FILE:${PROJECT_BINARY_NAME}> 
    && ${GPROF_EXEC} -I${CMAKE_SOURCE_DIR} $<TARGET_FILE:${PROJECT_BINARY_NAME}> 
       ${CMAKE_BINARY_DIR}/gmon.out > ${GPROF_ANALYZE_TXT_DST}
  COMMENT "Executing gprof command for the project main binary file. Outputs into the ${GPROF_ANALYZE_TXT_DST}."
  DEPENDS ${PROJECT_BINARY_NAME}
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)