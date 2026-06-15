cmake_minimum_required(VERSION 3.13)

option(
  ENABLE_WIX
  "Set to ON to enable the WIX MSI package creation from the project build artifacts (MS Windows)"
  OFF
)

if(NOT ENABLE_WIX)
    return()
endif()

include(InstallRequiredSystemLibraries)

set(
  TEMPLATE_PROJECT_WIX_UPGRADE_GUID
  "12345678-1234-1234-1234-123456789012"
  CACHE STRING
  "Stable GUID identifying the product across upgrades - MUST be replaced with a project-unique value for a real release"
)

set(
  TEMPLATE_PROJECT_WIX_PROGRAM_MENU_FOLDER
  "${CMAKE_PROJECT_NAME}"
  CACHE STRING
  "Windows Start Menu folder name where the installer creates shortcuts"
)

set(
  TEMPLATE_PROJECT_WIX_CULTURES
  "en-US"
  CACHE STRING
  "Semicolon-separated list of UI cultures used by the WIX installer (e.g. en-US;uk-UA)"
)

set(
  TEMPLATE_PROJECT_WIX_LICENSE_RTF
  ""
  CACHE STRING
  "Optional absolute path to a RTF or TXT license file shown by the MSI installer"
)

set(
  TEMPLATE_PROJECT_WIX_PRODUCT_ICON
  ""
  CACHE STRING
  "Optional absolute path to an .ico file used as the MSI product icon"
)

set(CPACK_GENERATOR "WIX")
set(CPACK_PACKAGE_NAME ${CMAKE_PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_CONTACT ${PROJECT_MAINTAINER_EMAIL})
set(CPACK_PACKAGE_VENDOR "${PROJECT_MAINTAINER}")
set(CPACK_PACKAGE_DESCRIPTION ${CMAKE_PROJECT_DESCRIPTION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${CMAKE_PROJECT_DESCRIPTION})
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CMAKE_PROJECT_NAME})

set(CPACK_WIX_UPGRADE_GUID ${TEMPLATE_PROJECT_WIX_UPGRADE_GUID})
set(CPACK_WIX_PROGRAM_MENU_FOLDER ${TEMPLATE_PROJECT_WIX_PROGRAM_MENU_FOLDER})
set(CPACK_WIX_CULTURES ${TEMPLATE_PROJECT_WIX_CULTURES})
set(CPACK_WIX_ROOT_FOLDER_ID "ProgramFiles64Folder")

if(TEMPLATE_PROJECT_WIX_LICENSE_RTF)
  set(CPACK_WIX_LICENSE_RTF ${TEMPLATE_PROJECT_WIX_LICENSE_RTF})
endif()

if(TEMPLATE_PROJECT_WIX_PRODUCT_ICON)
  set(CPACK_WIX_PRODUCT_ICON ${TEMPLATE_PROJECT_WIX_PRODUCT_ICON})
endif()

include(CPack)
