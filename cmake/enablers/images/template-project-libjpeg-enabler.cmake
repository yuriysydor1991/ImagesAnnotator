cmake_minimum_required(VERSION 3.13)

option(
    ENABLE_LIBJPEG
    "Enables the libjpeg-turbo (JPG/JPEG image format) library for the project usage through system installed one or FetchContent by internet"
    OFF
)

if (NOT ENABLE_LIBJPEG)
    return()
endif()

set(TEMPLATE_APP_LIBJPEG_GIT "https://github.com/libjpeg-turbo/libjpeg-turbo.git" CACHE STRING "The libjpeg-turbo library git source repository")
set(TEMPLATE_APP_LIBJPEG_GIT_TAG "main" CACHE STRING "The libjpeg-turbo project git repository tag/branch of interest")

template_project_default_3rdparty_enabler(
  NAME JPEG
  GIT_REPOSITORY ${TEMPLATE_APP_LIBJPEG_GIT}
  GIT_TAG ${TEMPLATE_APP_LIBJPEG_GIT_TAG}
)

# Link one of the following JPEG targets to your target(s) of interest (e.g. the
# ${PROJECT_BINARY_NAME} executable or any of your own libraries):
#   - system install, via find_package(JPEG): JPEG::JPEG
#   - FetchContent source build (libjpeg-turbo): jpeg-static (static) or jpeg
#     (shared) for the libjpeg API, turbojpeg / turbojpeg-static for the
#     TurboJPEG API
# For example:
#   target_link_libraries(${PROJECT_BINARY_NAME} JPEG::JPEG)
