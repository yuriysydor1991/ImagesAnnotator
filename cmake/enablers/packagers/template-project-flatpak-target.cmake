cmake_minimum_required(VERSION 3.13)

option(
  ENABLE_FLATPAK
  "Set to ON to enable the flatpak package creation from the project build artifacts"
  OFF
)

if(NOT ENABLE_FLATPAK)
  return()
endif()

find_program(FLATPAKB_EXEC flatpak-builder REQUIRED)
find_program(FLATPAK_EXEC flatpak REQUIRED)

execute_process(
  COMMAND ${FLATPAK_EXEC} --default-arch
  OUTPUT_VARIABLE FLATPAK_TARGET_ARCH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

# ImagesAnnotator specific: the flatpak is the application of the binary a
# package starts - the graphical one, or the converter of a converter only
# build - so its id and its command are the ANNOTATOR_PRIMARY_APP_ID and the
# ANNOTATOR_PRIMARY_APP_TARGET of template-project-misc-variables-declare.cmake
# rather than a hardcoded pair. Every other binary built beside it lands into
# the very same flatpak, where the `flatpak run --command=<binary> <id>` starts
# it, and where the launcher of template-project-desktop-entry-enabler.cmake
# registers it under an id of its own.
set(FLATPAK_APP_ID ${ANNOTATOR_PRIMARY_APP_ID})
set(FLATPAK_COMMAND ${ANNOTATOR_PRIMARY_APP_TARGET})

# ImagesAnnotator specific: each GUI toolkit brings a runtime, an SDK and the
# modules built on top of them of its own, so each has a manifest of its own
# under misc/packagers/flatpak, named after the ANNOTATOR_PACKAGE_FLAVOR (the
# converter only build has one as well, which carries no toolkit at all). The
# cache variable stays empty unless the user hands a manifest of their own, so a
# reconfigure switching the toolkit switches the manifest along with it.
set(
  FLATPAK_CONF_SRC
  ""
  CACHE STRING
  "The flatpak json conf source file cmake configured (empty: the one of the selected GUI toolkit under misc/packagers/flatpak)"
)

if(FLATPAK_CONF_SRC)
  # a build directory configured before the manifests were split per toolkit
  # holds the path of the single manifest they replaced
  if(NOT EXISTS ${FLATPAK_CONF_SRC})
    message(FATAL_ERROR
      "The FLATPAK_CONF_SRC manifest ${FLATPAK_CONF_SRC} does not exist; "
      "reconfigure with -DFLATPAK_CONF_SRC= to pack with the manifest of the "
      "selected GUI toolkit")
  endif()

  set(flatpakConfSrc ${FLATPAK_CONF_SRC})
else()
  set(
    flatpakConfSrc
    ${CMAKE_SOURCE_DIR}/misc/packagers/flatpak/flatpak-${ANNOTATOR_PACKAGE_FLAVOR}.conf.json.in
  )
endif()

set(FLATPAK_REPO ${PROJECT_BINARY_NAME}-repo)
set(flatpakConfDst ${CMAKE_BINARY_DIR}/flatpak.conf.json)
set(FLATPAK_DEPS_REMOTE flathub)
set(FLATPAK_DEPS_REMOTE_URL https://dl.flathub.org/repo/flathub.flatpakrepo)

configure_file(${flatpakConfSrc} ${flatpakConfDst})

# ImagesAnnotator specific: the very same version of the project may be packed
# into a flatpak per GUI toolkit on the very same system, so the bundle names
# the SDK it is built with after the CPU and the system: the "sdk" and the
# "runtime-version" members of the configured manifest (an sdk of the
# <name>/<arch>/<branch> form names its branch itself).
file(READ ${flatpakConfDst} flatpakConf)

string(REGEX MATCH "\"sdk\"[ \t\r\n]*:[ \t\r\n]*\"([^\"]*)\"" flatpakSdkMember "${flatpakConf}")
set(flatpakSdk "${CMAKE_MATCH_1}")

string(REGEX MATCH "\"runtime-version\"[ \t\r\n]*:[ \t\r\n]*\"([^\"]*)\"" flatpakRuntimeVersionMember "${flatpakConf}")
set(flatpakSdkVersion "${CMAKE_MATCH_1}")

if(flatpakSdk MATCHES "^([^/]+)/[^/]*/([^/]+)$")
  set(flatpakSdk "${CMAKE_MATCH_1}")
  set(flatpakSdkVersion "${CMAKE_MATCH_2}")
endif()

if(NOT flatpakSdk OR NOT flatpakSdkVersion)
  message(FATAL_ERROR
    "The flatpak manifest ${flatpakConfSrc} names no \"sdk\" and "
    "\"runtime-version\" pair the flatpak bundle name could carry")
endif()

set(FLATPAK_SDK_NAME ${flatpakSdk}-${flatpakSdkVersion})

# the name and the version of the destination system follow the destination
# CPU, as in every other package name of the project, and the SDK follows them
include(template-project-package-system)

set(
  FLATPAK_DST_NAME
  ${FLATPAK_COMMAND}-${CMAKE_PROJECT_VERSION}-${FLATPAK_TARGET_ARCH}${TEMPLATE_PROJECT_PACKAGE_SYSTEM_SUFFIX}-${FLATPAK_SDK_NAME}.flatpak
)

message(STATUS "flatpak-builder: ${FLATPAKB_EXEC}")
message(STATUS "flatpak: ${FLATPAK_EXEC}")
message(STATUS "flatpak json conf src: ${flatpakConfSrc}")
message(STATUS "flatpak application id: ${FLATPAK_APP_ID} (command ${FLATPAK_COMMAND})")
message(STATUS "flatpak bundle: ${FLATPAK_DST_NAME}")

# The runtime and the SDK the manifest names are installed from the flathub on
# demand, as every toolkit asks for a pair of its own, and the --force-clean
# lets a repeated build reuse the flatpak-build directory of the previous one.
add_custom_target(
  flatpak
  COMMAND
    ${FLATPAK_EXEC} remote-add --user --if-not-exists ${FLATPAK_DEPS_REMOTE} ${FLATPAK_DEPS_REMOTE_URL} &&
    ${FLATPAKB_EXEC} --user --install-deps-from=${FLATPAK_DEPS_REMOTE} --assumeyes --force-clean
      --repo=${FLATPAK_REPO} "${CMAKE_BINARY_DIR}/flatpak-build" ${flatpakConfDst} &&
    ${FLATPAK_EXEC} build-bundle ${FLATPAK_REPO} ${FLATPAK_DST_NAME} ${FLATPAK_APP_ID}
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  COMMENT "Executing the flatpak-builder command to generate the ${FLATPAK_DST_NAME} flatpak package."
)
