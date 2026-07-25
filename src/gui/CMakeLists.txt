cmake_minimum_required(VERSION 3.13)

# The graphical user interface subcomponent. The concrete GTKmm implementation
# (gtkmm3 or gtkmm4) is selected at configure time through the mutually
# exclusive USE_GTKMM3 / USE_GTKMM4 options (see
# cmake/template-project-misc-variables-declare.cmake). Only one toolkit is
# ever compiled into the single application binary, so the toolkit-agnostic
# gtkmm-common sources are built once against whichever GTKmm is active.

# The GTKMM_* variables (and GTKMM_GUI_SUBDIR below) come from whichever GUI
# toolkit enabler ran - cmake/enablers/template-project-Gtkmm{3,4}-enabler.cmake
# - which probed pkg-config once at project scope before add_subdirectory(src).
# They are inherited here and by every subdirectory underneath.

# Single instance of the toolkit-agnostic GResource payload (CSS, images). Both
# gtkmm3 and gtkmm4 embed exactly the same files under their own resource
# prefix, so the tree lives here once and each toolkit's ui/CMakeLists.txt hands
# this directory to glib-compile-resources as an extra --sourcedir. That keeps
# the <file> entries in every template.gresource.xml - and therefore the runtime
# resource paths - unchanged while removing the duplicated copies.
# The toolkit-agnostic gtkmm-common sources are GTKmm/gdk-pixbuf based, so they
# are built only for the two GTKmm toolkits. The wxWidgets implementation is
# self-contained under its own subdirectory (it ships a wxImage based cropper
# instead of the gdk-pixbuf one), so for it only the selected GUI subdirectory is
# added.
if(USE_WXWIDGETS)
  add_subdirectory(${GTKMM_GUI_SUBDIR})
else()
  set(GTKMM_COMMON_UI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/gtkmm-common/ui")

  add_subdirectory(gtkmm-common)
  add_subdirectory(${GTKMM_GUI_SUBDIR})
endif()
