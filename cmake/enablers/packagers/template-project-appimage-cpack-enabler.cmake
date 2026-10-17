# The CPack AppImage generator is available since the CMake 4.2, so the
# template-project-appimage-general-enabler includes this file on the user
# demand alone. Every name it packages around is resolved by that file.
cmake_minimum_required(VERSION 4.2)

include(InstallRequiredSystemLibraries)

set(CPACK_GENERATOR "AppImage")
set(CPACK_PACKAGE_NAME ${CMAKE_PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_CONTACT ${PROJECT_MAINTAINER_EMAIL})
set(CPACK_PACKAGE_VENDOR "${PROJECT_MAINTAINER}")
set(CPACK_PACKAGE_DESCRIPTION ${CMAKE_PROJECT_DESCRIPTION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${CMAKE_PROJECT_DESCRIPTION})

# the generator demands the desktop entry among the installed files and the
# icon at its freedesktop.org location, named by the Icon= key of that entry
# without the file extension - which is what the launcher enabler installs and
# what the placeholder pair of the general enabler repeats
set(CPACK_PACKAGE_ICON ${APPIMAGE_ICON_NAME})
set(CPACK_APPIMAGE_DESKTOP_FILE ${APPIMAGE_DESKTOP_NAME})

# the CPACK_SYSTEM_NAME of the default file name names the system alone, so the
# destination CPU and the destination system land into the package name the
# custom target enabler gives
set(CPACK_PACKAGE_FILE_NAME ${APPIMAGE_PACKAGE_NAME})

# ImagesAnnotator specific: the AppRun the generator generates starts the binary
# the desktop entry names and nothing else, while the one of
# misc/packagers/appimage starts every binary the package carries under the
# name it is started with - so the package is given that one. The generator
# takes an AppRun it finds at the root of the AppDir instead of generating its
# own, and stages the project into that very root, which is why the AppRun is
# configured with no prefix of its own. It is copied there by a script CPack
# runs once the project is staged, rather than by an install() rule, so that
# no AppRun lands into the install prefix of a plain install step.
set(APPIMAGE_APPDIR_PREFIX "")

configure_file(
  ${CMAKE_SOURCE_DIR}/misc/packagers/appimage/appimage.AppRun.in
  ${CMAKE_BINARY_DIR}/AppRun
)

set(appimageAppRunScript ${CMAKE_BINARY_DIR}/appimage-cpack-apprun.cmake)

file(
  WRITE ${appimageAppRunScript}
  "file(\n"
  "  COPY \"${CMAKE_BINARY_DIR}/AppRun\"\n"
  "  DESTINATION \"\${CPACK_TEMPORARY_DIRECTORY}\"\n"
  "  FILE_PERMISSIONS\n"
  "    OWNER_READ OWNER_WRITE OWNER_EXECUTE\n"
  "    GROUP_READ GROUP_EXECUTE\n"
  "    WORLD_READ WORLD_EXECUTE\n"
  ")\n"
)

set(CPACK_PRE_BUILD_SCRIPTS ${appimageAppRunScript})

include(CPack)

# the appimage target of the custom target enabler, so both implementations
# produce their package the same way for the build scripts and the pipeline
add_custom_target(
  appimage
  COMMAND
    ${CMAKE_COMMAND} -E env ARCH=${TEMPLATE_PROJECT_APPIMAGE_ARCHITECTURE}
      ${CMAKE_CPACK_COMMAND}
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  COMMENT "Executing the cpack command to generate the AppImage package."
)
