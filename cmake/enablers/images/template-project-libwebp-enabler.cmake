cmake_minimum_required(VERSION 3.13)

option(
    ENABLE_LIBWEBP
    "Enables the libwebp (WEBP image format) library for the project usage through system installed one or FetchContent by internet"
    OFF
)

if (NOT ENABLE_LIBWEBP)
    return()
endif()

set(TEMPLATE_APP_LIBWEBP_GIT "https://github.com/webmproject/libwebp.git" CACHE STRING "The libwebp library git source repository")
set(TEMPLATE_APP_LIBWEBP_GIT_TAG "main" CACHE STRING "The libwebp project git repository tag/branch of interest")

template_project_default_3rdparty_enabler(
  NAME WebP
  GIT_REPOSITORY ${TEMPLATE_APP_LIBWEBP_GIT}
  GIT_TAG ${TEMPLATE_APP_LIBWEBP_GIT_TAG}
)

# Link one of the following WebP targets to your target(s) of interest (e.g. the
# ${PROJECT_BINARY_NAME} executable or any of your own libraries):
#   - system install, via find_package(WebP): WebP::webp (decode + encode),
#     WebP::webpdecoder (decode only), WebP::webpdemux, WebP::libwebpmux
#   - FetchContent source build: webp, webpdecoder, webpdemux, libwebpmux
#     (the WebP:: aliases are exported as well)
# For example:
#   target_link_libraries(${PROJECT_BINARY_NAME} WebP::webp)
