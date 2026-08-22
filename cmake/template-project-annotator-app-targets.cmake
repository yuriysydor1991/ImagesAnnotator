cmake_minimum_required(VERSION 3.13)

# ---------------------------------------------------------------------------
# ImagesAnnotator specific: the application targets of the project.
#
# The project builds the same code into more than one binary - the main
# application and the converter only one, see
# cmake/template-project-misc-variables-declare.cmake - so a component of the
# src subtree hands its sources and its object library to every application
# target being built instead of naming one of them. The three functions below
# are that "every one of them", so a component never has to know which binaries
# this configure produces, and adding a third one would need no change under
# src at all.
#
# ANNOTATOR_APP_TARGETS is resolved before add_subdirectory(src), so it is
# already filled in wherever these are called from.
# ---------------------------------------------------------------------------

# @brief Adds the given sources to every application target being built.
function(annotator_app_targets_sources)
  foreach(ANNOTATOR_APP_TARGET IN LISTS ANNOTATOR_APP_TARGETS)
    target_sources(${ANNOTATOR_APP_TARGET} PRIVATE ${ARGV})
  endforeach()
endfunction()

# @brief Links the given libraries into every application target being built.
function(annotator_app_targets_link_libraries)
  foreach(ANNOTATOR_APP_TARGET IN LISTS ANNOTATOR_APP_TARGETS)
    target_link_libraries(${ANNOTATOR_APP_TARGET} ${ARGV})
  endforeach()
endfunction()

# @brief Applies to one application target everything both of them share: the
# main routine they are entered through, the include roots of the project and
# the install rule. What differs between them - the sources of the graphical
# interface and the IMAGES_ANNOTATOR_CONVERTER_ONLY definition - stays in
# src/ImagesAnnotator-target.cmake and src/ImagesAnnotatorConverter-target.cmake.
#
# The warning set and the -DMAX_LOG_LEVEL of this build are deliberately not
# repeated here: src/CMakeLists.txt applies them once at its directory scope,
# where the object libraries of every component and every test target inherit
# them as well, and both application targets are declared in that very
# directory. Adding them per target would compile the same sources with the same
# flags twice over.
function(annotator_configure_app_target ANNOTATOR_APP_TARGET)
  target_sources(
    ${ANNOTATOR_APP_TARGET}
    PRIVATE ${ANNOTATOR_SRC_ROOT}/main.cpp
  )

  target_include_directories(
    ${ANNOTATOR_APP_TARGET}
    PRIVATE ${CMAKE_SOURCE_DIR}
    PRIVATE ${ANNOTATOR_SRC_ROOT}
    PRIVATE ${CMAKE_BINARY_DIR}
  )

  install(
    TARGETS ${ANNOTATOR_APP_TARGET}
    RUNTIME DESTINATION ${PROJECT_BINARY_INSTALLATION_DIR}
  )
endfunction()
