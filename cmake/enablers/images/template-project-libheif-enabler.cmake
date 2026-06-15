cmake_minimum_required(VERSION 3.13)

option(
    ENABLE_LIBHEIF
    "Enables the libheif (HEIF/HEIC image format) library for the project usage through system installed one or FetchContent by internet"
    OFF
)

if (NOT ENABLE_LIBHEIF)
    return()
endif()

set(TEMPLATE_APP_LIBHEIF_GIT "https://github.com/strukturag/libheif.git" CACHE STRING "The libheif library git source repository")
set(TEMPLATE_APP_LIBHEIF_GIT_TAG "master" CACHE STRING "The libheif project git repository tag/branch of interest")

template_project_default_3rdparty_enabler(
  NAME libheif
  GIT_REPOSITORY ${TEMPLATE_APP_LIBHEIF_GIT}
  GIT_TAG ${TEMPLATE_APP_LIBHEIF_GIT_TAG}
)

# Link the following libheif target to your target(s) of interest (e.g. the
# ${PROJECT_BINARY_NAME} executable or any of your own libraries):
#   - system install, via find_package(libheif): heif (also exported as libheif::heif)
#   - FetchContent source build:                 heif
#     (enable a codec for the FetchContent build to decode, e.g. libde265)
# For example:
#   target_link_libraries(${PROJECT_BINARY_NAME} heif)
