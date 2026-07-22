cmake_minimum_required(VERSION 3.13)

# The graphical user interface subcomponent. The concrete GTKmm implementation
# (gtkmm3 or gtkmm4) is selected at configure time through the mutually
# exclusive USE_GTKMM3 / USE_GTKMM4 options (see
# cmake/template-project-misc-variables-declare.cmake). Only one toolkit is
# ever compiled into the single application binary, so the toolkit-agnostic
# gtkmm-common sources are built once against whichever GTKmm is active.

find_package(PkgConfig)

# GTKMM_PC is derived from the USE_GTKMM3 / USE_GTKMM4 switch and resolves to
# either "gtkmm-3.0" or "gtkmm-4.0". The resulting GTKMM_* variables are
# inherited by the gtkmm-common and toolkit-specific subdirectories.
pkg_check_modules(GTKMM ${GTKMM_PC} REQUIRED)

add_subdirectory(gtkmm-common)
add_subdirectory(${GTKMM_GUI_SUBDIR})
