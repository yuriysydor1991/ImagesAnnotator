cmake_minimum_required(VERSION 3.13)

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0 REQUIRED)

add_subdirectory(unit)
