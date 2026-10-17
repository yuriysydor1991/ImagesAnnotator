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
