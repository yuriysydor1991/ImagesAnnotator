cmake_minimum_required(VERSION 3.13)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(
  PROJECT_BINARY_NAME ${CMAKE_PROJECT_NAME}
  CACHE STRING "Project main binary name and target"
)

string(TOLOWER ${PROJECT_BINARY_NAME} PROJECT_BINARY_NAME_lower)

# ImagesAnnotator specific: the GTKmm3 based object library target name shared
# between the gtkmm3 sources and their component/unit tests.
set(
  GTKMM_OBJECT_NAME "${PROJECT_NAME}GTKmmObj"
  CACHE STRING "Project GTKmm based object library name and target"
)

# ---------------------------------------------------------------------------
# ImagesAnnotator specific: GUI toolkit selection.
#
# The project ships two interchangeable GTKmm GUI implementations under src/gui
# (gtkmm3 and gtkmm4). Exactly one is compiled into the single application
# binary. USE_GTKMM4 is the default. The two options are mutually exclusive:
# enabling one means disabling the other.
# ---------------------------------------------------------------------------
option(USE_GTKMM4 "Build the GTKmm4 (GTK 4) based GUI. Mutually exclusive with USE_GTKMM3." ON)
option(USE_GTKMM3 "Build the GTKmm3 (GTK 3) based GUI. Mutually exclusive with USE_GTKMM4." OFF)

if(USE_GTKMM3 AND USE_GTKMM4)
  message(FATAL_ERROR
    "USE_GTKMM3 and USE_GTKMM4 are mutually exclusive - enable exactly one "
    "(USE_GTKMM4 is the default). Re-run cmake with, e.g., "
    "-DUSE_GTKMM3=ON -DUSE_GTKMM4=OFF.")
endif()

if(NOT USE_GTKMM3 AND NOT USE_GTKMM4)
  message(FATAL_ERROR
    "Neither USE_GTKMM3 nor USE_GTKMM4 is enabled - enable exactly one GUI "
    "toolkit (USE_GTKMM4 is the default).")
endif()

if(USE_GTKMM4)
  set(GTKMM_PC "gtkmm-4.0")
  set(GTKMM_GUI_SUBDIR "gtkmm4")
  set(GTKMM_ACTIVE_DEFINE "USE_GTKMM4")
else()
  set(GTKMM_PC "gtkmm-3.0")
  set(GTKMM_GUI_SUBDIR "gtkmm3")
  set(GTKMM_ACTIVE_DEFINE "USE_GTKMM3")
endif()

# Propagate the active toolkit as a preprocessor define so the toolkit-neutral
# facade src/gui/GuiIniter.h (and the app component that consumes it) select the
# matching GtkmmIniter implementation. add_compile_definitions() here runs at
# project scope, so every target defined afterwards inherits the macro.
add_compile_definitions(${GTKMM_ACTIVE_DEFINE})

set(
  PROJECT_MAINTAINER "Yuriy Sydor"
  CACHE STRING "Project maintainer name (used for the DEB package)"
)

set(
  PROJECT_MAINTAINER_EMAIL "yuriysydor1991@gmail.com"
  CACHE STRING "Project maintainer email (used for the DEB package)"
)

set(
  MAX_LOG_LEVEL "3"
  CACHE STRING 
  "Sets the maximum severity of the compiled log messages. Error=0. Trace=5. By default Info=3"
)

set(
  PROJECT_BINARY_INSTALLATION_DIR "bin"
  CACHE STRING 
  "Sets the project main binary installation directory"
)

set(
  DEFAULT_LOG_FILE_PATH ""
  CACHE STRING 
  "Sets the default log file path"
)

option(
  ENABLE_LOGS_MICROSECONDS_TIME
  "Set to OFF to disable the microseconds fields included into the log messages"
  ON
)

string(TIMESTAMP PROJECT_CONFIGURE_DATE "%Y-%m-%d %H:%M:%S")

message(STATUS "CMAKE_PROJECT_NAME: ${CMAKE_PROJECT_NAME}")
message(STATUS "PROJECT_BINARY_NAME: ${PROJECT_BINARY_NAME}")
message(STATUS "PROJECT_CONFIGURE_DATE: ${PROJECT_CONFIGURE_DATE}")
