cmake_minimum_required(VERSION 3.13)

# The name and the version of the destination operating system, which the
# package file names of the CPack packagers carry after the destination CPU
# (e.g. the -amd64-ubuntu-24.04 of a DEB package). Every such packager includes
# this file and appends the TEMPLATE_PROJECT_PACKAGE_SYSTEM_SUFFIX it sets.

set(packageSystem "")

# a cross compiled package runs on a system other than the one it is built on,
# which the build knows nothing about, so it names no system of its own
if(NOT CMAKE_CROSSCOMPILING)
  # the ID and VERSION_ID fields of the os-release(5) file the Linux
  # distributions and the FreeBSD have (the DISTRIB_ID and DISTRIB_VERSION_ID
  # queries of the cmake_host_system_information read the same, but demand the
  # CMake 3.22)
  foreach(osReleaseFile /etc/os-release /usr/lib/os-release)
    if(EXISTS ${osReleaseFile})
      file(STRINGS ${osReleaseFile} osReleaseId LIMIT_COUNT 1 REGEX "^ID=")
      file(STRINGS ${osReleaseFile} osReleaseVersionId LIMIT_COUNT 1 REGEX "^VERSION_ID=")
      string(REGEX REPLACE "^ID=[\"']?([^\"']*)[\"']?$" "\\1" osReleaseId "${osReleaseId}")
      string(REGEX REPLACE "^VERSION_ID=[\"']?([^\"']*)[\"']?$" "\\1" osReleaseVersionId "${osReleaseVersionId}")
      break()
    endif()
  endforeach()

  if(osReleaseId AND osReleaseVersionId)
    set(packageSystem ${osReleaseId}-${osReleaseVersionId})
  elseif(osReleaseId)
    # a rolling release (the Arch Linux, the Debian sid) has no version
    set(packageSystem ${osReleaseId})
  elseif(NOT CMAKE_SYSTEM_NAME STREQUAL "Linux" AND CMAKE_SYSTEM_VERSION)
    # the Microsoft Windows has no os-release file, and its CMAKE_SYSTEM_VERSION
    # is the system one (e.g. 10.0.26100) - unlike the kernel one of a Linux
    string(TOLOWER ${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_VERSION} packageSystem)
  endif()
endif()

set(
  TEMPLATE_PROJECT_PACKAGE_SYSTEM
  "${packageSystem}"
  CACHE STRING
  "The destination operating system name and version the package file names end with (e.g. ubuntu-24.04, fedora-42, freebsd-14.3, windows-10.0.26100), none when empty"
)

if(TEMPLATE_PROJECT_PACKAGE_SYSTEM)
  set(TEMPLATE_PROJECT_PACKAGE_SYSTEM_SUFFIX -${TEMPLATE_PROJECT_PACKAGE_SYSTEM})
else()
  set(TEMPLATE_PROJECT_PACKAGE_SYSTEM_SUFFIX "")
endif()
