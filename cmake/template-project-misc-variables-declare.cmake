cmake_minimum_required(VERSION 3.13)

set(
  PROJECT_BINARY_NAME ${PROJECT_NAME}
  CACHE STRING "Project main binary name and target"
)

set(
  GTKMM_OBJECT_NAME "${PROJECT_NAME}GTKmmObj"
  CACHE STRING "Project main binary name and target"
)

set(
  PROJECT_MAINTAINER "Yuriy Sydor"
  CACHE STRING "Project maintainer name (used for the DEB package)"
)

set(
  PROJECT_MAINTAINER_EMAIL "yuriysydor1991@gmail.com"
  CACHE STRING "Project maintainer email (used for the DEB package)"
)

option(
  ENABLE_UNIT_TESTS 
  "Set to ON value if unit tests build and run should be available"
  OFF
)

option(
  GTEST_TRY_SYSTEM_PROBE 
  "Set to ON value if current project CMake files should probe the system GTest"
  ON
)

option(
  ENABLE_DOC_DOXYGEN 
  "Set to ON value if Doxygen documentation generation should be available"
  OFF
)

set(
  DOXYGEN_OUTPUT_LANGUAGE "English"
  CACHE STRING "Sets the Doxygen output language for the generated documentation (see doc/Doxyfile.in)"
)

set(
  DOXYGEN_MAIN_PAGE_MD "README.md"
  CACHE STRING "Sets the Doxygen main README file to place into documentation the home page."
)

set(
  DOXYGEN_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/doc
  CACHE STRING "Sets the doxygen documentation output directory."
)

option(
  DOXYGEN_DO_INSTALL
  "Set to on if CMake should install the generated documentation during install command executed."
  OFF 
)

set(
  DOXYGEN_OUT_HTML_NAME ${PROJECT_BINARY_NAME}-html
  CACHE STRING "The Doxygen output HTML directory name (for the Doxyfile)"
)

set(
  DOXYGEN_OUT_HTML ${DOXYGEN_OUTPUT_DIR}/${DOXYGEN_OUT_HTML_NAME}
  CACHE STRING "Sets the doxygen documentation output full directory path."
)

option(
  ENABLE_CLANGFORMAT
  "Set to ON if to enable the clang-format command execution target."
  OFF
)

option(
  ENABLE_CPPCHECK
  "Set to ON if to enable the cppcheck execution target."
  OFF
)

option(
  ENABLE_VALGRIND
  "Set to ON if to enable the valgrind memory check execution target."
  OFF
)

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

option(
  ENABLE_CLANG_TIDY
  "Set to ON if enable the clang-tidy code check target."
  OFF
)

option(
  ENABLE_DEB
  "Set to ON to enable the deb package creation from the project build artifacts"
  OFF
)

option(
  ENABLE_FLATPAK
  "Set to ON to enable the flatpak package creation from the project build artifacts"
  OFF
)

option(
  ENABLE_DOCKER
  "Set to ON to enable the dockers run"
  OFF
)

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

set(
  MAX_LOG_LEVEL "3"
  CACHE STRING 
  "Sets the maximum severity of the compiled log messages. Error=0. Trace=5. By default Info=3"
)

option(
  ENABLE_COMPONENT_TESTS 
  "Set to ON value if components tests build and run should be available"
  OFF
)

option(
  ENABLE_LIBCURL
  "Set to ON to enable the libcurl (by using system wide available or through the Internet)"
  ON
)

option(
  ENABLE_GPROF
  "Set to ON to enable the gprof application profiler analysis"
  OFF
)

string(TIMESTAMP PROJECT_CONFIGURE_DATE "%Y-%m-%d %H:%M:%S")

execute_process(
  COMMAND "${GIT_EXECUTABLE}" rev-parse HEAD
  OUTPUT_VARIABLE PROJECT_GIT_COMMIT
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
  COMMAND "${GIT_EXECUTABLE}" branch --show-current
  OUTPUT_VARIABLE PROJECT_CURRENT_BRANCH_NAME
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

file(
  GLOB_RECURSE 
  ALLSOURCES 
  LIST_DIRECTORIES false 
  CONFIGURE_DEPENDS
  "src/*.h" "src/*.cpp"
)

message(STATUS "PROJECT_NAME: ${PROJECT_NAME}")
message(STATUS "PROJECT_BINARY_NAME: ${PROJECT_BINARY_NAME}")
message(STATUS "PROJECT_CONFIGURE_DATE: ${PROJECT_CONFIGURE_DATE}")
