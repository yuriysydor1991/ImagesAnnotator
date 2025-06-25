cmake_minimum_required(VERSION 3.13)

if (NOT ENABLE_COMPONENT_TESTS)
  return()
endif()

# disabling the asserts for the tests
add_compile_options(-DNDEBUG=1)

set(
  ANNOTATOR_BUSINESS_CTESTS_INCLUDE_DIR
  ${CMAKE_CURRENT_BINARY_DIR}
)

configure_file(
  ctests_decls.h.cmake.in
  ${ANNOTATOR_BUSINESS_CTESTS_INCLUDE_DIR}/ctests_decls.h
)

add_subdirectory(AnnotatorController)
