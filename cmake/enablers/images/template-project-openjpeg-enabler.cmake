cmake_minimum_required(VERSION 3.13)

option(
    ENABLE_OPENJPEG
    "Enables the OpenJPEG (JPEG 2000 image format) library for the project usage through system installed one or FetchContent by internet"
    OFF
)

if (NOT ENABLE_OPENJPEG)
    return()
endif()

set(TEMPLATE_APP_OPENJPEG_GIT "https://github.com/uclouvain/openjpeg.git" CACHE STRING "The OpenJPEG library git source repository")
set(TEMPLATE_APP_OPENJPEG_GIT_TAG "master" CACHE STRING "The OpenJPEG project git repository tag/branch of interest")

template_project_default_3rdparty_enabler(
  NAME OpenJPEG
  GIT_REPOSITORY ${TEMPLATE_APP_OPENJPEG_GIT}
  GIT_TAG ${TEMPLATE_APP_OPENJPEG_GIT_TAG}
)

# Link the following OpenJPEG target to your target(s) of interest (e.g. the
# ${PROJECT_BINARY_NAME} executable or any of your own libraries):
#   - system install, via find_package(OpenJPEG): openjp2
#   - FetchContent source build:                  openjp2
# For example:
#   target_link_libraries(${PROJECT_BINARY_NAME} openjp2)
