cmake_minimum_required(VERSION 3.13)

option(
    ENABLE_LIBTIFF
    "Enables the libtiff (TIFF image format) library for the project usage through system installed one or FetchContent by internet"
    OFF
)

if (NOT ENABLE_LIBTIFF)
    return()
endif()

set(TEMPLATE_APP_LIBTIFF_GIT "https://gitlab.com/libtiff/libtiff.git" CACHE STRING "The libtiff library git source repository")
set(TEMPLATE_APP_LIBTIFF_GIT_TAG "master" CACHE STRING "The libtiff project git repository tag/branch of interest")

template_project_default_3rdparty_enabler(
  NAME TIFF
  GIT_REPOSITORY ${TEMPLATE_APP_LIBTIFF_GIT}
  GIT_TAG ${TEMPLATE_APP_LIBTIFF_GIT_TAG}
)

# Link the following libtiff target to your target(s) of interest (e.g. the
# ${PROJECT_BINARY_NAME} executable or any of your own libraries):
#   - system install, via find_package(TIFF): TIFF::TIFF
#   - FetchContent source build:              tiff (also exported as TIFF::tiff)
# For example:
#   target_link_libraries(${PROJECT_BINARY_NAME} TIFF::TIFF)
