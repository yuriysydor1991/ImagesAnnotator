cmake_minimum_required(VERSION 3.13)

# ---------------------------------------------------------------------------
# The main application binary: the graphical interface over the annotations
# pipeline, plus the very same command line conversion the converter only
# binary beside it carries.
#
# It is excluded from the build by BUILD_IMAGES_ANNOTATOR_CONVERTER_ONLY, which
# is what makes a converter only build need no GUI toolkit at all - the four
# toolkit enablers and the src/gui subdirectory are skipped along with this
# target. See cmake/template-project-misc-variables-declare.cmake.
# ---------------------------------------------------------------------------
if(BUILD_IMAGES_ANNOTATOR_CONVERTER_ONLY)
  message(STATUS
    "The ${PROJECT_BINARY_NAME} target is excluded "
    "(BUILD_IMAGES_ANNOTATOR_CONVERTER_ONLY=ON)")
  return()
endif()

add_executable(${PROJECT_BINARY_NAME})

annotator_configure_app_target(${PROJECT_BINARY_NAME})

# The app::Application class is what this binary has and the converter only one
# has not: it starts the compiled in gui::GUIIniter, so the whole src/gui
# subtree follows the single translation unit added here. The sources both
# binaries share are handed over by src/app/CMakeLists.txt instead.
target_sources(
  ${PROJECT_BINARY_NAME}
  PRIVATE ${ANNOTATOR_SRC_ROOT}/app/Application.cpp
)
