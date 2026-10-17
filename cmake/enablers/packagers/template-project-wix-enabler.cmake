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

# the architecture the CPack WIX generator picks when given none
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(wixArchitecture x64)
else()
  set(wixArchitecture x86)
endif()

set(
  TEMPLATE_PROJECT_WIX_ARCHITECTURE
  ${wixArchitecture}
  CACHE STRING
  "The destination CPU architecture of the MSI installer (e.g. x64, arm64, x86)"
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
set(CPACK_WIX_ARCHITECTURE ${TEMPLATE_PROJECT_WIX_ARCHITECTURE})

if(TEMPLATE_PROJECT_WIX_LICENSE_RTF)
  set(CPACK_WIX_LICENSE_RTF ${TEMPLATE_PROJECT_WIX_LICENSE_RTF})
endif()

if(TEMPLATE_PROJECT_WIX_PRODUCT_ICON)
  set(CPACK_WIX_PRODUCT_ICON ${TEMPLATE_PROJECT_WIX_PRODUCT_ICON})
endif()

# the CPACK_SYSTEM_NAME of the default file name (win64 or win32) names no CPU,
# so the destination one takes its place, followed by the name and the version
# of the destination system
include(template-project-package-system)

set(
  CPACK_PACKAGE_FILE_NAME
  ${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${TEMPLATE_PROJECT_WIX_ARCHITECTURE}${TEMPLATE_PROJECT_PACKAGE_SYSTEM_SUFFIX}
)

include(CPack)
