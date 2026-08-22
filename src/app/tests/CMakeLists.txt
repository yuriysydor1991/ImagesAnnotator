cmake_minimum_required(VERSION 3.13)

set(APP_MOCKS_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/mocks")

add_subdirectory(unit)
add_subdirectory(component)
