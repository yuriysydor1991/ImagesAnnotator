cmake_minimum_required(VERSION 3.13)

option(
  ENABLE_DEB
  "Set to ON to enable the deb package creation from the project build artifacts"
  OFF
)

if(NOT ENABLE_DEB)
    return()
endif()

include(InstallRequiredSystemLibraries)

# the Debian name of the destination CPU, the one the CPack DEB generator asks
# the dpkg for when given none
execute_process(
  COMMAND dpkg --print-architecture
  OUTPUT_VARIABLE dpkgArchitecture
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

set(
  TEMPLATE_PROJECT_DEB_PACKAGE_ARCHITECTURE
  "${dpkgArchitecture}"
  CACHE STRING
  "The destination CPU architecture of the DEB package (e.g. amd64, arm64, armhf, i386)"
)

set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_NAME ${CMAKE_PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_CONTACT ${PROJECT_MAINTAINER_EMAIL})
set(CPACK_DEB_MAINTAINER "${PROJECT_MAINTAINER} ${PROJECT_MAINTAINER_EMAIL}")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${PROJECT_MAINTAINER} ${PROJECT_MAINTAINER_EMAIL}")
set(CPACK_DEB_COMPONENT_INSTALL ON)
set(CPACK_PACKAGE_DESCRIPTION ${CMAKE_PROJECT_DESCRIPTION})
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE ${TEMPLATE_PROJECT_DEB_PACKAGE_ARCHITECTURE})
# ImagesAnnotator specific: the GTKmm runtime is a hard dependency of the app.
# The concrete package name is declared by whichever GUI toolkit enabler ran
# (cmake/enablers/template-project-Gtkmm{3,4}-enabler.cmake), so no branch on
# USE_GTKMM3 / USE_GTKMM4 is needed here.
set(CPACK_DEBIAN_PACKAGE_DEPENDS "${GTKMM_DEB_RUNTIME_DEPENDS}")

# the CPACK_SYSTEM_NAME of the default file name names the system alone, so the
# destination CPU takes its place
set(
  CPACK_PACKAGE_FILE_NAME
  ${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${TEMPLATE_PROJECT_DEB_PACKAGE_ARCHITECTURE}
)

# ImagesAnnotator specific: the three ImagesAnnotator libraries the binaries are
# linked against are no distribution packages, so the package declares no
# dependency on them and its binaries start only on a system which has them
# installed already. The option below packs them into the package instead -
# the very shared objects the found CMake packages of the three library enablers
# point at, each with its SONAME link, into the lib directory their own install
# step uses - so the package is complete on its own. It is what
# scripts/build/build-deb-with-libraries.sh enables, once it has built the
# three libraries into the build directory.
option(
  ENABLE_DEB_IMAGES_ANNOTATOR_LIBRARIES
  "Set to ON to pack the three ImagesAnnotator libraries the binaries are linked against into the deb package"
  OFF
)

if(ENABLE_DEB_IMAGES_ANNOTATOR_LIBRARIES)
  foreach(IA_LIBRARY IN LISTS IADD_LIBRARIES IADE_LIBRARIES IADI_LIBRARIES)
    install(
      FILES $<TARGET_FILE:${IA_LIBRARY}> $<TARGET_SONAME_FILE:${IA_LIBRARY}>
      DESTINATION lib
    )

    message(STATUS "DEB package carries the ${IA_LIBRARY} library")
  endforeach()

  # The packed libraries load the libraries they are linked against in their
  # turn - the libcurl, the zlib and the OpenCV - which the system they were
  # built on has, but the one the package is installed on may lack. So
  # dpkg-shlibdeps names the distribution packages every binary of the package
  # needs, the three libraries included; it finds those three inside the
  # package itself, so they add no dependency of their own. The same scan makes
  # CPack add the ldconfig call to the postinst and the postrm scripts of the
  # package, which carries shared objects under /usr/lib now.
  set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
endif()

include(CPack)
