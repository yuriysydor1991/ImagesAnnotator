cmake_minimum_required(VERSION 3.13)

option(
  ENABLE_CPPCHECK
  "Set to ON if to enable the cppcheck execution target."
  OFF
)

if(NOT ENABLE_CPPCHECK)
  return()
endif()

find_program(CPPCHECK_EXEC cppcheck cppcheck.exe)

if (NOT CPPCHECK_EXEC)
    message(
      WARNING
      "No cppcheck executable found. "
      "No code check target will be declared. "
      "Install cppcheck by using command (GNU/Linux based):\n"
      "sudo apt install -y cppcheck\n"
    )

    return()
endif()

message(STATUS "cppcheck: ${CPPCHECK_EXEC}")

add_custom_target (
  cppcheck
  COMMAND ${CPPCHECK_EXEC} --language=c++ --std=c++${CMAKE_CXX_STANDARD} 
    --error-exitcode=1 --inconclusive --enable=all -v 
    --suppressions-list=${CMAKE_SOURCE_DIR}/misc/.cppcheck-suppress
    -I${CMAKE_SOURCE_DIR} -I${CMAKE_BINARY_DIR} ${ALLSOURCES}
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  COMMENT "Executes cppcheck command for all project sources. Just outputs all findings into stdout."
)
