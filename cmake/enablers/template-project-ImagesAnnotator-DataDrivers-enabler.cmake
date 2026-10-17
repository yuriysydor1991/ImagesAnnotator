cmake_minimum_required(VERSION 3.13)

option(
  ENABLE_IMAGES_ANNOTATOR_DATA_DRIVERS
  "Set to ON to enable the ImagesAnnotator-DataDrivers annotations database library (system wide installed)"
  ON
)

# The option is honoured the same way every other third party enabler of the
# project honours its own, but unlike the optional ones this library is what
# reads and writes the annotations project files, so switching it off leaves
# the sources without the record value objects and without the annotations
# database they are written against. Say so here instead of letting the
# configure succeed and the compilation fail on a missing <IADataDriversFacade.h>.
if (NOT ENABLE_IMAGES_ANNOTATOR_DATA_DRIVERS)
  message(WARNING
    "ENABLE_IMAGES_ANNOTATOR_DATA_DRIVERS is OFF - the project will not "
    "compile: the image record value objects and the annotations project "
    "database it is built against come from that library.")
  return()
endif()

# The library carries its major and minor version inside the installed package,
# target and binary names, matching its version carrying public namespace, so
# that two minor versions install completely side by side. The default below is
# what the library produces out of the box; its own LIB_INCLUDE_MINOR_IN_NAME
# (ON by default), LIB_INCLUDE_MICRO_IN_NAME and LIB_NAME_SUFFIX options may
# shorten or extend that name. The name the find_package() probe asks for is
# therefore a cache variable rather than a literal: pointing the project at a
# differently named build of the very same library - a major-only
# "ImagesAnnotatorDataDrivers-0" or a "ImagesAnnotatorDataDrivers-0.14-dev" one -
# stays a single -D on the command line.
set(TEMPLATE_APP_IADD_PACKAGE "ImagesAnnotatorDataDrivers-0.14" CACHE STRING "The ImagesAnnotator-DataDrivers CMake package (and target) name to look for")

# Where the library comes from. Nothing in this build fetches it - the two
# variables are what the packager manifests, which have to build it inside
# their own sandbox, are configured with (misc/packagers/flatpak.conf.json.in,
# misc/packagers/snapcraft.yaml.in and misc/Dockerfiles/Dockerfile.in).
set(TEMPLATE_APP_IADD_GIT "https://github.com/yuriysydor1991/ImagesAnnotator-DataDrivers.git" CACHE STRING "The ImagesAnnotator-DataDrivers library git source repository")
set(TEMPLATE_APP_IADD_GIT_TAG "main" CACHE STRING "The ImagesAnnotator-DataDrivers project git repository tag of interest")

# The library used to be pulled in through the Internet with FetchContent when
# the system carried no copy of it. That route is gone: the sibling
# ImagesAnnotator-DataExporters library, which the enabler next to this one
# brings in, resolves the data drivers with a find_package() of its own and
# includes their headers through the versioned
# <ImagesAnnotatorDataDrivers-0.14/...> directory - a form only an installation
# provides. Both libraries are therefore consumed installed, and the sources
# include their headers the prefixed way (see
# src/annotator-events/events/DataDriversAliases.h), which pins the library
# version every translation unit is talking to.
#
# No version is requested on purpose: the library installs its package Config
# file without the companion <package>ConfigVersion.cmake, so a versioned
# find_package() would never match it. The package name already carries the
# major and the minor version.
find_package(${TEMPLATE_APP_IADD_PACKAGE} QUIET CONFIG)

if (NOT ${TEMPLATE_APP_IADD_PACKAGE}_FOUND)
  message(FATAL_ERROR
    "The ${TEMPLATE_APP_IADD_PACKAGE} library was not found in the system. "
    "Build and install it from "
    "https://github.com/yuriysydor1991/ImagesAnnotator-DataDrivers.git, point "
    "the project at it with -DCMAKE_PREFIX_PATH=<prefix>, or correct the "
    "package name with -DTEMPLATE_APP_IADD_PACKAGE=<name>. The project does "
    "not build without it: the annotations project files are read and written "
    "exclusively by this library.")
endif()

set(IADD_LIBRARIES ${TEMPLATE_APP_IADD_PACKAGE}::${TEMPLATE_APP_IADD_PACKAGE})

message(STATUS "ImagesAnnotator-DataDrivers target: ${IADD_LIBRARIES}")

# The library supplies the image record value objects and the annotations
# database interface, which the object libraries all over the src subtree (the
# events, the annotator controller, the image loaders and every GUI toolkit
# implementation) compile against. Propagate its usage requirements through
# link_libraries() to every target built below instead of only to the final
# executable, so the object libraries and the test executables compile and
# link. This is why the enabler is included before add_subdirectory(src) (see
# the pre feature-enabler).
link_libraries(${IADD_LIBRARIES})
