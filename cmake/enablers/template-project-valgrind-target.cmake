cmake_minimum_required(VERSION 3.13)

option(
  ENABLE_VALGRIND
  "Set to ON if to enable the valgrind memory check execution target."
  OFF
)

if(NOT ENABLE_VALGRIND)
  return()
endif()

find_program(VALGRIND_EXECUTABLE NAMES valgrind)

if (NOT VALGRIND_EXECUTABLE)
    message(
      WARNING
      "No valgrind executable found. "
      "No memory check target will be declared. "
      "Install valgrind by using command (GNU/Linux based):\n"
      "sudo apt install -y valgrind\n"
    )

    return()
endif()

message(STATUS "valgrind: ${VALGRIND_EXECUTABLE}")

add_custom_target(
  valgrind
  COMMAND ${VALGRIND_EXECUTABLE} --leak-check=full
    --error-exitcode=1 
    --show-leak-kinds=all 
    $<TARGET_FILE:${PROJECT_BINARY_NAME}>
  COMMENT "Executing valgrind command for the project main binary file. Output all findings into the stdout."
  DEPENDS ${PROJECT_BINARY_NAME}
)
