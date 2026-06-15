cmake_minimum_required(VERSION 3.13)

option(
    ENABLE_OPENEXR
    "Enables the OpenEXR (EXR high dynamic range image format) library for the project usage through system installed one or FetchContent by internet"
    OFF
)

if (NOT ENABLE_OPENEXR)
    return()
endif()

set(TEMPLATE_APP_OPENEXR_GIT "https://github.com/AcademySoftwareFoundation/openexr.git" CACHE STRING "The OpenEXR library git source repository")
set(TEMPLATE_APP_OPENEXR_GIT_TAG "main" CACHE STRING "The OpenEXR project git repository tag/branch of interest")

template_project_default_3rdparty_enabler(
  NAME OpenEXR
  GIT_REPOSITORY ${TEMPLATE_APP_OPENEXR_GIT}
  GIT_TAG ${TEMPLATE_APP_OPENEXR_GIT_TAG}
)

# Link the following OpenEXR target to your target(s) of interest (e.g. the
# ${PROJECT_BINARY_NAME} executable or any of your own libraries):
#   - system install, via find_package(OpenEXR): OpenEXR::OpenEXR
#   - FetchContent source build:                 OpenEXR::OpenEXR
#     (the FetchContent build pulls its Imath dependency automatically)
# For example:
#   target_link_libraries(${PROJECT_BINARY_NAME} OpenEXR::OpenEXR)
