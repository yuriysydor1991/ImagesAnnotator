cmake_minimum_required(VERSION 3.13)

option(
    ENABLE_GIFLIB
    "Enables the giflib (GIF image format) library for the project (system installation only)"
    OFF
)

if (NOT ENABLE_GIFLIB)
    return()
endif()

# giflib ships an autotools build only (no upstream CMake / FetchContent target),
# so it is probed from the system exclusively - install libgif-dev (Debian/Ubuntu),
# giflib-devel (Fedora/openSUSE) or giflib (Arch) on the host.
find_package(GIF REQUIRED)

message(STATUS "giflib ${GIF_VERSION} found: ${GIF_INCLUDE_DIRS}")

# Link the following GIF target to your target(s) of interest (e.g. the
# ${PROJECT_BINARY_NAME} executable or any of your own libraries):
#   - system install, via find_package(GIF): GIF::GIF
# For example:
#   target_link_libraries(${PROJECT_BINARY_NAME} GIF::GIF)
