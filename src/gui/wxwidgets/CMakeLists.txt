cmake_minimum_required(VERSION 3.13)

# The GTKMM_* variables come from
# cmake/enablers/template-project-wxWidgets-enabler.cmake, which probed wxWidgets
# with find_package() once at project scope before add_subdirectory(src). They
# carry the wxWidgets include dirs / libraries here despite the historical name,
# so this file mirrors the gtkmm3 / gtkmm4 CMakeLists shape.

# Embed the shared logo PNG as a C byte array so the About dialog and the window
# icon have it without any installed resource file - the wx analogue of the GTKmm
# GResource-embedded logo. Generated at configure time; needs no external tool
# (glib-compile-resources has no wxWidgets counterpart).
set(WX_LOGO_SRC
  "${CMAKE_SOURCE_DIR}/src/gui/gtkmm-common/ui/resources/images/kytok.org.ua-logo.png")
set(WX_LOGO_HDR "${CMAKE_BINARY_DIR}/wx_embedded_logo.h")
file(READ "${WX_LOGO_SRC}" WX_LOGO_HEX HEX)
string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1," WX_LOGO_BYTES "${WX_LOGO_HEX}")
file(WRITE "${WX_LOGO_HDR}"
  "/* Auto-generated from ${WX_LOGO_SRC}; do not edit. */\n"
  "static const unsigned char wx_embedded_logo_png[] = { ${WX_LOGO_BYTES} };\n"
  "static const unsigned int wx_embedded_logo_png_len = sizeof(wx_embedded_logo_png);\n")

add_library(
  ${GTKMM_OBJECT_NAME} OBJECT
  WxWidgetsIniter.cpp
  MainWindowContext.cpp
)

target_include_directories(
  ${GTKMM_OBJECT_NAME}
  PRIVATE
    ${GTKMM_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
)

target_link_directories(
  ${GTKMM_OBJECT_NAME}
  PRIVATE ${GTKMM_LIBRARY_DIRS}
)

target_link_libraries(
  ${GTKMM_OBJECT_NAME}
  PRIVATE ${GTKMM_LIBRARIES}
)

target_include_directories(
  ${PROJECT_BINARY_NAME}
  PRIVATE ${GTKMM_INCLUDE_DIRS}
)

target_link_libraries(
  ${PROJECT_BINARY_NAME}
  ${GTKMM_OBJECT_NAME}
  ${GTKMM_LIBRARIES}
)

add_subdirectory(main-window)
add_subdirectory(tests)
