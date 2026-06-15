cmake_minimum_required(VERSION 3.13)

option(
    ENABLE_LIBPNG
    "Enables the libpng (PNG image format) library for the project usage through system installed one or FetchContent by internet"
    OFF
)

if (NOT ENABLE_LIBPNG)
    return()
endif()

set(TEMPLATE_APP_LIBPNG_GIT "https://github.com/pnggroup/libpng.git" CACHE STRING "The libpng library git source repository")
set(TEMPLATE_APP_LIBPNG_GIT_TAG "libpng16" CACHE STRING "The libpng project git repository tag/branch of interest")

template_project_default_3rdparty_enabler(
  NAME PNG
  GIT_REPOSITORY ${TEMPLATE_APP_LIBPNG_GIT}
  GIT_TAG ${TEMPLATE_APP_LIBPNG_GIT_TAG}
)

# libpng depends on zlib: keep ENABLE_ZLIB on (or a system zlib available) so the
# dependency resolves for both the system and the FetchContent builds.
#
# Link one of the following libpng targets to your target(s) of interest (e.g. the
# ${PROJECT_BINARY_NAME} executable or any of your own libraries):
#   - system install, via find_package(PNG): PNG::PNG
#   - FetchContent source build:             png_static (static) or png (shared)
# For example:
#   target_link_libraries(${PROJECT_BINARY_NAME} PNG::PNG)
