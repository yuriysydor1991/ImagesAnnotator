cmake_minimum_required(VERSION 3.13)

find_program(GRSC_EXEC glib-compile-resources)

set(RESOURCE_XML "${CMAKE_CURRENT_SOURCE_DIR}/template.gresource.xml")
set(RESOURCE_C "${CMAKE_BINARY_DIR}/template-resources.c")

# Search path used to resolve the <file> entries of template.gresource.xml:
# this toolkit's own ui directory first (GtkmmWindow.ui is GTK version
# specific), then the shared src/gui/gtkmm-common/ui tree that holds the single
# copy of the toolkit-agnostic CSS and images. Spelling the directories out here
# is what lets both toolkits keep identical relative <file> paths - and thus
# identical runtime resource paths - while the payload exists only once.
set(RESOURCE_SOURCEDIRS
  --sourcedir=${CMAKE_CURRENT_SOURCE_DIR}
  --sourcedir=${GTKMM_COMMON_UI_DIR}
)

# Track every file embedded by the gresource (GtkmmWindow.ui, CSS, images) so
# that editing any of them regenerates the compiled resource. Without this the
# custom command only depends on the .gresource.xml and stale UI gets baked into
# the binary.
execute_process(
  COMMAND ${GRSC_EXEC} ${RESOURCE_SOURCEDIRS} --generate-dependencies ${RESOURCE_XML}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  OUTPUT_VARIABLE RESOURCE_DEPS
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
string(REPLACE "\n" ";" RESOURCE_DEPS "${RESOURCE_DEPS}")

# glib-compile-resources reports a dependency it located in one of the
# --sourcedir entries as an absolute path, and echoes back the bare relative
# name from the XML for anything it could not resolve. Normalise the latter
# against this directory so add_custom_command() never gets a path that would be
# interpreted relative to the build tree.
set(RESOURCE_DEPS_RESOLVED "")
foreach(RESOURCE_DEP IN LISTS RESOURCE_DEPS)
  if(IS_ABSOLUTE "${RESOURCE_DEP}")
    list(APPEND RESOURCE_DEPS_RESOLVED "${RESOURCE_DEP}")
  else()
    list(APPEND RESOURCE_DEPS_RESOLVED "${CMAKE_CURRENT_SOURCE_DIR}/${RESOURCE_DEP}")
  endif()
endforeach()

add_custom_command(
  OUTPUT ${RESOURCE_C}
  COMMAND ${GRSC_EXEC}
          ${RESOURCE_SOURCEDIRS}
          --target=${RESOURCE_C}
          --generate-source
          ${RESOURCE_XML}
  DEPENDS ${RESOURCE_XML} ${RESOURCE_DEPS_RESOLVED}
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
