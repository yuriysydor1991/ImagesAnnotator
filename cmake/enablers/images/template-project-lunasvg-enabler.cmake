cmake_minimum_required(VERSION 3.13)

option(
    ENABLE_LUNASVG
    "Enables the lunasvg (SVG vector image format) library for the project usage through system installed one or FetchContent by internet"
    OFF
)

if (NOT ENABLE_LUNASVG)
    return()
endif()

set(TEMPLATE_APP_LUNASVG_GIT "https://github.com/sammycage/lunasvg.git" CACHE STRING "The lunasvg library git source repository")
set(TEMPLATE_APP_LUNASVG_GIT_TAG "v2.4.1" CACHE STRING "The lunasvg project git repository tag of interest")

template_project_default_3rdparty_enabler(
  NAME lunasvg
  GIT_REPOSITORY ${TEMPLATE_APP_LUNASVG_GIT}
  GIT_TAG ${TEMPLATE_APP_LUNASVG_GIT_TAG}
)

# Link the following lunasvg target to your target(s) of interest (e.g. the
# ${PROJECT_BINARY_NAME} executable or any of your own libraries):
#   - system install, via find_package(lunasvg): lunasvg::lunasvg
#   - FetchContent source build:                 lunasvg
# (the v3.x line additionally pulls the plutovg rendering backend automatically)
# For example:
#   target_link_libraries(${PROJECT_BINARY_NAME} lunasvg::lunasvg)
