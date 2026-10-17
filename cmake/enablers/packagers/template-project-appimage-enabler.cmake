cmake_minimum_required(VERSION 3.13)

# The appimagetool driven implementation of the AppImage packager, selected by
# template-project-appimage-general-enabler.cmake for a CMake too old to carry
# the CPack AppImage generator. Every name it packs the AppDir around - the
# binary, the destination file, the desktop entry and the icon - is resolved by
# that file, which is the only one including this one.

find_program(APPIMAGETOOL_EXEC appimagetool REQUIRED)

set(APPIMAGE_APPDIR ${CMAKE_BINARY_DIR}/AppDir)

# the install step below places the project under its install prefix inside the
# AppDir, which is where the AppRun looks the binaries and the libraries up
set(APPIMAGE_APPDIR_PREFIX ${CMAKE_INSTALL_PREFIX})

configure_file(
  ${CMAKE_SOURCE_DIR}/misc/packagers/appimage/appimage.AppRun.in
  ${CMAKE_BINARY_DIR}/AppRun
)

# the AppImage runtime executes the AppRun, which the configure_file leaves
# without the execution permission
file(
  COPY ${CMAKE_BINARY_DIR}/AppRun
  DESTINATION ${APPIMAGE_APPDIR}
  FILE_PERMISSIONS
    OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE
)

message(STATUS "appimagetool: ${APPIMAGETOOL_EXEC}")

# The appimagetool reads the desktop entry from the AppDir root and resolves its
# Icon= key against that root as well, so the pair the install step places under
# share/ is repeated there. Both copies are made by the target rather than by
# this configure step, so that an edited launcher reaches the package without a
# reconfigure.
add_custom_target(
  appimage
  COMMAND ${CMAKE_COMMAND} -E make_directory ${APPIMAGE_APPDIR}
  COMMAND
    ${CMAKE_COMMAND} -E copy
      ${APPIMAGE_DESKTOP_DST} ${APPIMAGE_APPDIR}/${APPIMAGE_DESKTOP_NAME}
  COMMAND
    ${CMAKE_COMMAND} -E copy
      ${APPIMAGE_ICON_DST} ${APPIMAGE_APPDIR}/${APPIMAGE_ICON_NAME}
  COMMAND
    ${CMAKE_COMMAND} -E env DESTDIR=${APPIMAGE_APPDIR}
      ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target install
  COMMAND
    ${CMAKE_COMMAND} -E env ARCH=${TEMPLATE_PROJECT_APPIMAGE_ARCHITECTURE}
      ${APPIMAGETOOL_EXEC} ${APPIMAGE_APPDIR} ${APPIMAGE_DST_NAME}
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  COMMENT "Executing the appimagetool command to generate the ${APPIMAGE_DST_NAME} AppImage package."
)
