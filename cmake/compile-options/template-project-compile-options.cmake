cmake_minimum_required(VERSION 3.13)

option(
  COMPILE_WARNINGS_AS_ERRORS
  "Set to ON if to pass -Werror compile parameter to the GCC compiler."
  OFF
)

option(
  ENABLE_COMPILER_CODE_ANALYZER
  "Set to ON if to pass -fanalyzer compile parameter to the GCC compiler. May dramatically increase the compile time."
  OFF
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/compile-options")

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  include(template-project-compile-options-MSVC)
else()
  include(template-project-compile-options-GCC)
endif()

set(
  EXTRA_COMPILE_OPTIONS 
  ${EXTRA_COMPILE_OPTIONS} 
  -DMAX_LOG_LEVEL=${MAX_LOG_LEVEL}
  -DDEFAULT_LOG_FILE_PATH="${DEFAULT_LOG_FILE_PATH}"
)

message(STATUS "COMPILER ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "COMPILE OPTIONS: ${EXTRA_COMPILE_OPTIONS}")
