cmake_minimum_required(VERSION 3.13)

# Picks the AppImage packager implementation the CMake at hand is capable of:
# the CPack AppImage generator arrived with the version below, so an older
# CMake keeps the custom target enabler, which drives the appimagetool on its
# own, and never parses the CPack enabler file. The ENABLE_APPIMAGE_CPACK of a
# user demands the CPack one regardless of the ENABLE_APPIMAGE.
#
# ImagesAnnotator specific: both options are declared here rather than inside
# the implementation each of them selects, and so is everything the two share -
# the packaged binary, the destination name and the desktop entry / icon pair.
# Upstream each implementation repeats that block, which this project would
# otherwise have to adapt twice over.

set(APPIMAGE_CPACK_CMAKE_VERSION 4.2)

option(
  ENABLE_APPIMAGE
  "Set to ON to enable the AppImage package creation from the project build artifacts"
  OFF
)

option(
  ENABLE_APPIMAGE_CPACK
  "Set to ON to enable the AppImage package creation with the CPack generator (demands the CMake ${APPIMAGE_CPACK_CMAKE_VERSION})"
  OFF
)

if(ENABLE_APPIMAGE AND CMAKE_VERSION VERSION_GREATER_EQUAL ${APPIMAGE_CPACK_CMAKE_VERSION})
  set(ENABLE_APPIMAGE_CPACK ON)
endif()

if(NOT ENABLE_APPIMAGE AND NOT ENABLE_APPIMAGE_CPACK)
  return()
endif()

include(GNUInstallDirs)

set(
  TEMPLATE_PROJECT_APPIMAGE_ARCHITECTURE
  ${CMAKE_SYSTEM_PROCESSOR}
  CACHE STRING
  "The destination CPU architecture of the AppImage package (e.g. x86_64, aarch64, armhf, i686)"
)

# The binary the package starts: the graphical one whenever it is built, and the
# converter when it is the only one there.
set(APPIMAGE_BINARY_NAME ${ANNOTATOR_PRIMARY_APP_TARGET})

set(
  APPIMAGE_DST_NAME
  ${APPIMAGE_BINARY_NAME}-${CMAKE_PROJECT_VERSION}-${TEMPLATE_PROJECT_APPIMAGE_ARCHITECTURE}.AppImage
)

# ImagesAnnotator specific: an AppImage is built around a desktop entry naming
# an icon, and this project already installs exactly such a pair - the
# <APPLICATION_ID>.desktop launcher and the <APPLICATION_ID>.png themed icon of
# template-project-desktop-entry-enabler.cmake, which the feature enabler
# includes ahead of the packagers. Their names already agree the way an AppImage
# demands (the Icon= key names the icon file without its extension), so they are
# reused instead of the placeholder pair the upstream template generates for a
# project which carries no desktop integration of its own.
#
# A converter only build skips that enabler along with the graphical binary, so
# it is the one case where the placeholders of misc/packagers/appimage are
# configured - which is also what gives the command line converter an AppImage
# of its own.
if(DEFINED DESKTOP_ENTRY_DST)
  set(APPIMAGE_DESKTOP_NAME ${APPLICATION_ID}.desktop)
  set(APPIMAGE_ICON_NAME ${APPLICATION_ID}.png)
  set(APPIMAGE_DESKTOP_DST ${DESKTOP_ENTRY_DST})
  set(APPIMAGE_ICON_DST ${APP_ICON_SRC})
else()
  set(
    TEMPLATE_PROJECT_APPIMAGE_CATEGORIES
    "Graphics;"
    CACHE STRING
    "Semicolon terminated list of the desktop entry categories of the AppImage"
  )

  set(
    TEMPLATE_PROJECT_APPIMAGE_TERMINAL
    "true"
    CACHE STRING
    "Set to false when the packaged binary is a windowed application which needs no terminal"
  )

  set(
    APPIMAGE_ICON_SRC
    ${CMAKE_SOURCE_DIR}/misc/packagers/appimage/appimage.icon.svg.in
    CACHE STRING "The AppImage SVG icon source file cmake configured"
  )

  set(APPIMAGE_DESKTOP_NAME ${APPIMAGE_BINARY_NAME}.desktop)
  set(APPIMAGE_ICON_NAME ${APPIMAGE_BINARY_NAME}.svg)
  set(APPIMAGE_DESKTOP_DST ${CMAKE_BINARY_DIR}/${APPIMAGE_DESKTOP_NAME})
  set(APPIMAGE_ICON_DST ${CMAKE_BINARY_DIR}/${APPIMAGE_ICON_NAME})

  string(SUBSTRING ${CMAKE_PROJECT_NAME} 0 1 APPIMAGE_ICON_LETTER)

  configure_file(
    ${CMAKE_SOURCE_DIR}/misc/packagers/appimage/appimage.desktop.in
    ${APPIMAGE_DESKTOP_DST}
  )

  configure_file(${APPIMAGE_ICON_SRC} ${APPIMAGE_ICON_DST})

  # the launcher enabler installs the graphical pair at these very locations of
  # the freedesktop.org specification, which is where both implementations below
  # (and, for the CPack one, the generator itself) look the icon up
  install(
    FILES ${APPIMAGE_DESKTOP_DST}
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/applications
  )

  install(
    FILES ${APPIMAGE_ICON_DST}
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/scalable/apps
  )
endif()

message(STATUS "AppImage: ${APPIMAGE_DST_NAME}")
message(STATUS "AppImage desktop entry: ${APPIMAGE_DESKTOP_NAME} (icon ${APPIMAGE_ICON_NAME})")

if(ENABLE_APPIMAGE_CPACK)
  message(STATUS "AppImage packager: the CPack generator")
  include(template-project-appimage-cpack-enabler)
else()
  message(STATUS "AppImage packager: the appimagetool custom target")
  include(template-project-appimage-enabler)
endif()
