cmake_minimum_required(VERSION 3.13)

# ---------------------------------------------------------------------------
# ImagesAnnotator specific: freedesktop desktop-environment integration.
#
# Installs a .desktop launcher and a themed application icon so the running
# window is matched to its launcher and shown with a distinctive icon on the
# taskbar / GNOME Shell dash / alt-tab switcher.
#
# This is the ONLY mechanism that works on Wayland (and on the GNOME Shell dash
# even under X11): the compositor identifies a window by its app_id and looks up
# the matching <app-id>.desktop to decide the icon - the icon embedded in the
# binary is not consulted there. Three names therefore have to agree, and all
# come from the single APPLICATION_ID declared in
# template-project-misc-variables-declare.cmake:
#   - the installed launcher basename  <APPLICATION_ID>.desktop
#   - its Icon= key / the installed icon file name  <APPLICATION_ID>.png
#   - the window app_id, pinned at runtime by the GtkmmIniter via
#     Glib::set_prgname(APPLICATION_ID)
#
# Because the files are installed under the standard share/ layout, both the
# DEB package (CPack stages whatever is installed) and the flatpak (which
# installs into /app and exports /app/share/applications + /app/share/icons)
# pick them up automatically - no packager specific wiring is needed.
# ---------------------------------------------------------------------------

include(GNUInstallDirs)

set(DESKTOP_ENTRY_SRC "${CMAKE_SOURCE_DIR}/misc/desktop/template.desktop.in")
set(DESKTOP_ENTRY_DST "${CMAKE_BINARY_DIR}/${APPLICATION_ID}.desktop")

# @ONLY: the source only uses @VAR@ placeholders, and this avoids touching any
# bare $ that a desktop entry might legitimately contain.
configure_file("${DESKTOP_ENTRY_SRC}" "${DESKTOP_ENTRY_DST}" @ONLY)

install(
  FILES "${DESKTOP_ENTRY_DST}"
  DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/applications"
)

# The themed icon. The single shared logo GResource asset is reused as the app
# icon, renamed to the icon-theme name (<APPLICATION_ID>.png) on install. It is
# placed in a standard hicolor size directory; the icon loader scales it to
# whatever size the panel requests. (The asset is currently portrait 410x500,
# so it renders proportionally rather than filling the square - swap in a square
# multi-size icon later for a more polished look.)
set(APP_ICON_SRC
  "${CMAKE_SOURCE_DIR}/src/gui/gtkmm-common/ui/resources/images/kytok.org.ua-logo.png")

install(
  FILES "${APP_ICON_SRC}"
  DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/256x256/apps"
  RENAME "${APPLICATION_ID}.png"
)

message(STATUS "Desktop entry: ${APPLICATION_ID}.desktop (icon ${APPLICATION_ID}.png)")
