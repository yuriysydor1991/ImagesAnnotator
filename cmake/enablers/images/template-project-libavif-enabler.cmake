cmake_minimum_required(VERSION 3.13)

option(
    ENABLE_LIBAVIF
    "Enables the libavif (AVIF / AV1 image format) library for the project usage through system installed one or FetchContent by internet"
    OFF
)

if (NOT ENABLE_LIBAVIF)
    return()
endif()

set(TEMPLATE_APP_LIBAVIF_GIT "https://github.com/AOMediaCodec/libavif.git" CACHE STRING "The libavif library git source repository")
set(TEMPLATE_APP_LIBAVIF_GIT_TAG "main" CACHE STRING "The libavif project git repository tag/branch of interest")

template_project_default_3rdparty_enabler(
  NAME libavif
  GIT_REPOSITORY ${TEMPLATE_APP_LIBAVIF_GIT}
  GIT_TAG ${TEMPLATE_APP_LIBAVIF_GIT_TAG}
)

# Link the following libavif target to your target(s) of interest (e.g. the
# ${PROJECT_BINARY_NAME} executable or any of your own libraries):
#   - system install, via find_package(libavif): avif
#   - FetchContent source build:                 avif
#     (enable a codec for the FetchContent build to encode/decode, e.g.
#      -DAVIF_CODEC_AOM=ON or -DAVIF_CODEC_DAV1D=ON)
# For example:
#   target_link_libraries(${PROJECT_BINARY_NAME} avif)
