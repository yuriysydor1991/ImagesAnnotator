cmake_minimum_required(VERSION 3.13)

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

if(NOT ENABLE_DOC_DOXYGEN)
  return()
endif()

find_package(Doxygen REQUIRED)

set(DOXYGEN_IN Doxyfile.in)
set(DOXYGEN_OUT ${CMAKE_BINARY_DIR}/Doxyfile)

if (DOXYGEN_OUTPUT_LANGUAGE STREQUAL "Ukrainian")
  set(DOXYGEN_MAIN_PAGE_MD "doc/README.uk_UA.md")
endif()

configure_file(
  ${DOXYGEN_IN} 
  ${DOXYGEN_OUT}
)

add_custom_target(
  Doxygen-doc
  COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  COMMENT "Generating project documentation with Doxygen"
  VERBATIM
)

if(DOXYGEN_DO_INSTALL)
  message(STATUS "Doxygen html doc install enabled for the ${DOXYGEN_OUT_HTML} directory")

  install(
    DIRECTORY ${DOXYGEN_OUT_HTML}
    DESTINATION share/docs
    COMPONENT Documentation
  )
endif()
