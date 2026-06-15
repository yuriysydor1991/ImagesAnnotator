cmake_minimum_required(VERSION 3.13)

option(
  ENABLE_SANITIZERS
  "Set to ON to enable the sanitizers compile options"
  OFF
)

option(
  ENABLE_SANITIZERS_THREADS
  "Set to ON to enable the sanitizers for the threads compile options"
  OFF
)

if (NOT (ENABLE_SANITIZERS OR ENABLE_SANITIZERS_THREADS))
  return()
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/sanitizers")

if (ENABLE_SANITIZERS)
  include(template-project-sanitizers-leak-and-address)
endif()

if (ENABLE_SANITIZERS_THREADS)
  include(template-project-sanitizers-threads)
endif()
