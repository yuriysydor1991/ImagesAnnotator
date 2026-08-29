cmake_minimum_required(VERSION 3.13)

# Every IApplication descendant of the project but the Application.cpp
# translation unit, which is the graphical interface entry point and is handed
# to the main application target alone by src/ImagesAnnotator-target.cmake. The
# converter only binary is the very same component compiled without it, so the
# two message printers below - and the conversion application of the converter
# subdirectory - are the code both binaries share rather than a copy each.
annotator_app_targets_sources(
  ${CMAKE_CURRENT_SOURCE_DIR}/ApplicationHelpPrinter.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ApplicationVersionPrinter.cpp
)

add_subdirectory(converter)
add_subdirectory(tests)
