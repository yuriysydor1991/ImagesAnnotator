cmake_minimum_required(VERSION 3.13)

option(
  ENABLE_IMAGES_ANNOTATOR_DATA_EXPORTERS
  "Set to ON to enable the ImagesAnnotator-DataExporters annotations dataset exporters library (system wide installed)"
  ON
)

# The option is honoured the same way every other third party enabler of the
# project honours its own, but unlike the optional ones this library is what
# writes the annotations out as a training dataset, so switching it off leaves
# the sources without the export context and without the three exporters the
# AnnotatorController runs. Say so here instead of letting the configure
# succeed and the compilation fail on a missing <IADataExportersFacade.h>.
if (NOT ENABLE_IMAGES_ANNOTATOR_DATA_EXPORTERS)
  message(WARNING
    "ENABLE_IMAGES_ANNOTATOR_DATA_EXPORTERS is OFF - the project will not "
    "compile: the export context and the dataset exporters the annotator "
    "controller runs come from that library.")
  return()
endif()

# The library carries its major and minor version inside the installed package,
# target and binary names, matching its version carrying public namespace, so
# that two minor versions install completely side by side. The default below is
# what the library produces out of the box; its own LIB_INCLUDE_MINOR_IN_NAME
# (ON by default), LIB_INCLUDE_MICRO_IN_NAME and LIB_NAME_SUFFIX options may
# shorten or extend that name. The name the find_package() probe asks for is
# therefore a cache variable rather than a literal.
set(TEMPLATE_APP_IADE_PACKAGE "ImagesAnnotatorDataExporters-0.12" CACHE STRING "The ImagesAnnotator-DataExporters CMake package (and target) name to look for")

# Where the library comes from. Nothing in this build fetches it - the two
# variables are what the packager manifests, which have to build it inside
# their own sandbox, are configured with (misc/packagers/flatpak.conf.json.in,
# misc/packagers/snapcraft.yaml.in and misc/Dockerfiles/Dockerfile.in).
set(TEMPLATE_APP_IADE_GIT "https://github.com/yuriysydor1991/ImagesAnnotator-DataExporters.git" CACHE STRING "The ImagesAnnotator-DataExporters library git source repository")
set(TEMPLATE_APP_IADE_GIT_TAG "main" CACHE STRING "The ImagesAnnotator-DataExporters project git repository tag of interest")

# No Internet fallback here, unlike most of the third party enablers of the
# project: this library is not consumable as a sub-project. Its own configure
# resolves the data drivers library with find_package(), and its public headers
# include the versioned <ImagesAnnotatorDataDrivers-0.12/...> directory, which
# only an installation provides. It is consumed installed, exactly the way its
# own "Using the library in your project" documentation section describes.
#
# No version is requested on purpose: the package name already carries the major
# and the minor version, which is what keeps incompatible releases apart.
find_package(${TEMPLATE_APP_IADE_PACKAGE} QUIET CONFIG)

if (NOT ${TEMPLATE_APP_IADE_PACKAGE}_FOUND)
  message(FATAL_ERROR
    "The ${TEMPLATE_APP_IADE_PACKAGE} library was not found in the system. "
    "Build and install it from "
    "https://github.com/yuriysydor1991/ImagesAnnotator-DataExporters.git "
    "(it needs the ImagesAnnotator-DataDrivers library installed first), point "
    "the project at it with -DCMAKE_PREFIX_PATH=<prefix>, or correct the "
    "package name with -DTEMPLATE_APP_IADE_PACKAGE=<name>. The project does "
    "not build without it: the annotations are written out as a training "
    "dataset exclusively by this library.")
endif()

set(IADE_LIBRARIES ${TEMPLATE_APP_IADE_PACKAGE}::${TEMPLATE_APP_IADE_PACKAGE})

message(STATUS "ImagesAnnotator-DataExporters target: ${IADE_LIBRARIES}")

# The library supplies the export context, the image cropper interface the GUI
# toolkits implement and the three exporters, which the object libraries of the
# src subtree (the events, the annotator controller and every GUI toolkit
# implementation) compile against. Propagate its usage requirements through
# link_libraries() to every target built below instead of only to the final
# executable, exactly as the data drivers enabler above it does, so the object
# libraries and the test executables compile and link. This is why the enabler
# is included before add_subdirectory(src) (see the pre feature-enabler).
link_libraries(${IADE_LIBRARIES})
