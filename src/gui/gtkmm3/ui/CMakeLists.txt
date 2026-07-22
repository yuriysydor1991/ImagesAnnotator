cmake_minimum_required(VERSION 3.13)

find_program(GRSC_EXEC glib-compile-resources)

set(RESOURCE_XML "${CMAKE_CURRENT_SOURCE_DIR}/template.gresource.xml")
set(RESOURCE_C "${CMAKE_BINARY_DIR}/template-resources.c")

# Track every file embedded by the gresource (GtkmmWindow.ui, CSS, images) so
# that editing any of them regenerates the compiled resource. Without this the
# custom command only depends on the .gresource.xml and stale UI gets baked into
# the binary.
execute_process(
  COMMAND ${GRSC_EXEC} --generate-dependencies ${RESOURCE_XML}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  OUTPUT_VARIABLE RESOURCE_DEPS
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
string(REPLACE "\n" ";" RESOURCE_DEPS "${RESOURCE_DEPS}")
list(TRANSFORM RESOURCE_DEPS PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")

add_custom_command(
  OUTPUT ${RESOURCE_C}
  COMMAND ${GRSC_EXEC}
          --target=${RESOURCE_C}
          --generate-source
          ${RESOURCE_XML}
  DEPENDS ${RESOURCE_XML} ${RESOURCE_DEPS}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "Generating GResource source for ${PROJECT_NAME} (cleaned compile options WITH NO -Wall -Wextra)"
)

add_library(
  gresourcelib OBJECT
  ${RESOURCE_C}
)

set_target_properties(gresourcelib PROPERTIES COMPILE_OPTIONS "")

target_include_directories(
  gresourcelib
  PRIVATE 
    ${GTKMM_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
)

target_link_libraries(
  ${PROJECT_BINARY_NAME}
  gresourcelib
)
