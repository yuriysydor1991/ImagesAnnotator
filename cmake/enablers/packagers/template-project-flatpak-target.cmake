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

# FLATPAK_PROJECT_URL (== APPLICATION_ID) is defined unconditionally by
# template-project-misc-variables-declare.cmake, which runs long before this
# enabler, so it is reused here as the flatpak application id instead of being
# re-hardcoded. The flatpak build forces -DUSE_GTKMM3=ON, so it resolves to the
# ua.org.kytok.template.gtkmm3.* name the manifest and bundle expect.
set(FLATPAK_REPO ${PROJECT_BINARY_NAME}-repo)
set(flatpakConfDst ${CMAKE_BINARY_DIR}/flatpak.conf.json)
set(
  FLATPAK_CONF_SRC 
  ${CMAKE_SOURCE_DIR}/misc/packagers/flatpak.conf.json.in 
  CACHE STRING "The flatpak json conf source file cmake configured"
)

configure_file(${FLATPAK_CONF_SRC} ${flatpakConfDst})

# ImagesAnnotator specific: the very same version of the project may be packed
# with another manifest (the FLATPAK_CONF_SRC above), and so with another SDK,
# on the very same system, so the bundle names the SDK it is built with after
# the CPU and the system: the "sdk" and the "runtime-version" members of the
# configured manifest (an sdk of the <name>/<arch>/<branch> form names its
# branch itself).
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
    "The flatpak manifest ${FLATPAK_CONF_SRC} names no \"sdk\" and "
    "\"runtime-version\" pair the flatpak bundle name could carry")
endif()

set(FLATPAK_SDK_NAME ${flatpakSdk}-${flatpakSdkVersion})

# the name and the version of the destination system follow the destination
# CPU, as in every other package name of the project, and the SDK follows them
include(template-project-package-system)

set(
  FLATPAK_DST_NAME
  ${PROJECT_BINARY_NAME}-${CMAKE_PROJECT_VERSION}-${FLATPAK_TARGET_ARCH}${TEMPLATE_PROJECT_PACKAGE_SYSTEM_SUFFIX}-${FLATPAK_SDK_NAME}.flatpak
)

message(STATUS "flatpak-builder: ${FLATPAKB_EXEC}")
message(STATUS "flatpak: ${FLATPAK_EXEC}")
message(STATUS "flatpak json conf src: ${FLATPAK_CONF_SRC}")
message(STATUS "flatpak bundle: ${FLATPAK_DST_NAME}")

add_custom_target(
  flatpak
  COMMAND 
    ${FLATPAKB_EXEC} --repo=${FLATPAK_REPO} "${CMAKE_BINARY_DIR}/flatpak-build" ${flatpakConfDst} &&
    ${FLATPAK_EXEC} build-bundle ${FLATPAK_REPO} ${FLATPAK_DST_NAME} ${FLATPAK_PROJECT_URL}
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  COMMENT "Executing the flatpak-builder command to generate the ${FLATPAK_DST_NAME} flatpak package."
)
