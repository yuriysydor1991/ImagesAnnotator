cmake_minimum_required(VERSION 3.13)

option(
  ENABLE_SNAP
  "Set to ON to enable the snap package creation from the project build artifacts"
  OFF
)

if(NOT ENABLE_SNAP)
    return()
endif()

include(GNUInstallDirs)

set(TEMPLATE_PROJECT_SNAP_GRADE "devel" CACHE STRING "The snap package grade")
set(TEMPLATE_PROJECT_SNAP_CONFINEMENT "strict" CACHE STRING "The snap package confinement")

# ImagesAnnotator specific: each GUI toolkit brings the packages it is built
# against and the packages it runs with of its own, so each has a snapcraft.yaml
# of its own under misc/packagers/snapcraft, named after the
# ANNOTATOR_PACKAGE_FLAVOR (the converter only build has one as well, which
# carries no toolkit at all). The cache variable stays empty unless the user
# hands a file of their own, so a reconfigure switching the toolkit switches the
# file along with it.
set(
  SNAPCRAFT_CONF_SRC
  ""
  CACHE STRING
  "The snapcraft.yaml source file cmake configured (empty: the one of the selected GUI toolkit under misc/packagers/snapcraft)"
)

if(SNAPCRAFT_CONF_SRC)
  set(snapcraftConfSrc ${SNAPCRAFT_CONF_SRC})
else()
  set(
    snapcraftConfSrc
    ${CMAKE_SOURCE_DIR}/misc/packagers/snapcraft/snapcraft-${ANNOTATOR_PACKAGE_FLAVOR}.yaml.in
  )
endif()

# The snap is named after the binary it starts - the graphical one, or the
# converter of a converter only build - as the snap names allow no capitals.
string(TOLOWER ${ANNOTATOR_PRIMARY_APP_TARGET} SNAP_NAME)

# snapcraft installs the part with the DESTDIR set to the part install root, so
# the binaries land under the install prefix of the snap and the command paths
# are relative to the snap root
if(IS_ABSOLUTE "${PROJECT_BINARY_INSTALLATION_DIR}")
  set(snapBinaryDir "${PROJECT_BINARY_INSTALLATION_DIR}")
else()
  set(snapBinaryDir "${CMAKE_INSTALL_PREFIX}/${PROJECT_BINARY_INSTALLATION_DIR}")
endif()

# ImagesAnnotator specific: an app of the snap per binary built, each started by
# the `<snap name>.<binary>` command of its own - e.g. the
# imagesannotator.ImagesAnnotator and the
# imagesannotator.ImagesAnnotatorConverter. The graphical binary gets its desktop
# entry and the interfaces of a window, the command line converter the home
# directory alone.
set(SNAP_APPS "")

foreach(appTarget IN LISTS ANNOTATOR_APP_TARGETS)
  file(RELATIVE_PATH snapAppCommand / "${snapBinaryDir}/${appTarget}")

  string(APPEND SNAP_APPS "\n  ${appTarget}:\n    command: ${snapAppCommand}")

  if(appTarget STREQUAL PROJECT_BINARY_NAME)
    file(
      RELATIVE_PATH snapDesktopFile /
      ${CMAKE_INSTALL_FULL_DATAROOTDIR}/applications/${APPLICATION_ID}.desktop
    )

    string(
      APPEND SNAP_APPS
      "\n    desktop: ${snapDesktopFile}"
      "\n    plugs:\n      - home\n      - desktop\n      - desktop-legacy\n      - wayland\n      - x11\n      - opengl"
    )
  else()
    string(APPEND SNAP_APPS "\n    plugs:\n      - home")
  endif()
endforeach()

configure_file(
  ${snapcraftConfSrc}
  ${CMAKE_CURRENT_BINARY_DIR}/snap/snapcraft.yaml
)

# The snapcraft names the package <name>_<version>_<arch>.snap itself, the arch
# being the Debian name of the CPU it is built for - the one of the build host,
# as the snapcraft.yaml declares no other platform. The name and the version of
# the destination system follow the CPU, as in every other package name of the
# project, so the target renames the package it packs.
execute_process(
  COMMAND dpkg --print-architecture
  OUTPUT_VARIABLE snapArchitecture
  OUTPUT_STRIP_TRAILING_WHITESPACE
  ERROR_QUIET
)

if(NOT snapArchitecture)
  set(snapArchitecture ${CMAKE_SYSTEM_PROCESSOR})
endif()

include(template-project-package-system)

set(snapcraftDstName ${SNAP_NAME}_${CMAKE_PROJECT_VERSION}_${snapArchitecture}.snap)
set(
  SNAP_DST_NAME
  ${SNAP_NAME}_${CMAKE_PROJECT_VERSION}_${snapArchitecture}${TEMPLATE_PROJECT_PACKAGE_SYSTEM_SUFFIX}.snap
)

find_program(SNAPCRAFT_EXEC snapcraft)

message(STATUS "snapcraft.yaml src: ${snapcraftConfSrc}")
message(STATUS "snap package: ${SNAP_DST_NAME}")

add_custom_target(
  snap
  COMMAND ${SNAPCRAFT_EXEC} pack
  COMMAND ${CMAKE_COMMAND} -E rename ${snapcraftDstName} ${SNAP_DST_NAME}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "Packing project with help of the snapcraft into the ${SNAP_DST_NAME} snap package"
)
