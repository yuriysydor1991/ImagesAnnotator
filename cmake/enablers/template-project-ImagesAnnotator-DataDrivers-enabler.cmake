cmake_minimum_required(VERSION 3.13)

option(
  ENABLE_IMAGES_ANNOTATOR_DATA_DRIVERS
  "Set to ON to enable the ImagesAnnotator-DataDrivers annotations database library (by using system wide available or through the Internet)"
  ON
)

# The option is honoured the same way every other third party enabler of the
# project honours its own, but unlike the optional ones this library is what
# reads and writes the annotations project files, so switching it off leaves
# the sources without the record value objects and without the annotations
# database they are written against. Say so here instead of letting the
# configure succeed and the compilation fail on a missing <LibraryFacade.h>.
if (NOT ENABLE_IMAGES_ANNOTATOR_DATA_DRIVERS)
  message(WARNING
    "ENABLE_IMAGES_ANNOTATOR_DATA_DRIVERS is OFF - the project will not "
    "compile: the image record value objects and the annotations project "
    "database it is built against come from that library.")
  return()
endif()

set(TEMPLATE_APP_IADD_GIT "https://github.com/yuriysydor1991/ImagesAnnotator-DataDrivers.git" CACHE STRING "The ImagesAnnotator-DataDrivers library git source repository")
set(TEMPLATE_APP_IADD_GIT_TAG "main" CACHE STRING "The ImagesAnnotator-DataDrivers project git repository tag of interest")

# The library carries its major version inside the installed package, target and
# binary names (see the LIB_INCLUDE_MINOR_IN_NAME / LIB_INCLUDE_MICRO_IN_NAME /
# LIB_NAME_SUFFIX options on the library side), so the name the find_package()
# probe below asks for is a cache variable rather than a literal: pointing the
# project at a differently named build of the very same library - a
# "ImagesAnnotatorDataDrivers-0.11" or a "ImagesAnnotatorDataDrivers-0-dev" one -
# stays a single -D on the command line.
#
# Only the package name is versioned. Neither the C++ include directives nor the
# namespace alias the sources use spell the version out: the headers are pulled
# in unprefixed (see src/annotator-events/events/DataDriversAliases.h), which is
# also the only form the library exports while it is built as a sub-project.
set(TEMPLATE_APP_IADD_PACKAGE "ImagesAnnotatorDataDrivers-0" CACHE STRING "The ImagesAnnotator-DataDrivers CMake package (and target) name to look for")

template_project_default_3rdparty_enabler(
  NAME ${TEMPLATE_APP_IADD_PACKAGE}
  GIT_REPOSITORY ${TEMPLATE_APP_IADD_GIT}
  GIT_TAG ${TEMPLATE_APP_IADD_GIT_TAG}
)

# A system wide installation is consumed through the namespaced imported target
# the library exports, while a copy pulled in by FetchContent is added to this
# very build, where the plain target name is the one that exists. Resolve
# whichever of the two is available into a single variable so nothing below has
# to know which of the two routes the library arrived through.
if (TARGET ${TEMPLATE_APP_IADD_PACKAGE}::${TEMPLATE_APP_IADD_PACKAGE})
  set(IADD_LIBRARIES ${TEMPLATE_APP_IADD_PACKAGE}::${TEMPLATE_APP_IADD_PACKAGE})
elseif (TARGET ${TEMPLATE_APP_IADD_PACKAGE})
  set(IADD_LIBRARIES ${TEMPLATE_APP_IADD_PACKAGE})
else()
  message(FATAL_ERROR
    "The ${TEMPLATE_APP_IADD_PACKAGE} library was neither found in the system "
    "nor made available through the Internet. Install it (or point the project "
    "at it with -DCMAKE_PREFIX_PATH=<prefix>), correct the package name with "
    "-DTEMPLATE_APP_IADD_PACKAGE=<name>, or switch the project to a source "
    "tree of your own with -DTEMPLATE_APP_IADD_GIT=<url>. The project does not "
    "build without it: the annotations project files are read and written "
    "exclusively by this library.")
endif()

message(STATUS "ImagesAnnotator-DataDrivers target: ${IADD_LIBRARIES}")

# The library supplies the image record value objects and the annotations
# database interface, which the object libraries all over the src subtree (the
# events, the annotator controller, the exporters, the image loaders and every
# GUI toolkit implementation) compile against. Propagate its usage requirements
# through link_libraries() to every target built below instead of only to the
# final executable, exactly as the nlohmann json enabler does, so the object
# libraries and the test executables compile and link no matter whether the
# library came from the system or was fetched through the Internet. This is why
# the enabler is included before add_subdirectory(src) (see the pre
# feature-enabler).
link_libraries(${IADD_LIBRARIES})
