## Enabling RPM package generation with cpack

In order to be able to generate the `.rpm` package file configure the project to support the `cpack` command feature with the `ENABLE_RPM` variable enabled:

```
# inside the project root directory

cmake -B build -S . -DENABLE_RPM=ON
```

Next, build all available targets required for the `.rpm` package:

```
# inside the project root directory

cmake --build build --target all
```

Finally, execute the `cpack` command inside the template project build directory:

```
# inside the project build directory

cpack
```

The package file should be generated inside the project build root directory. For example, if the project name wasn't changed and its version is `0.12.0` the package name may look like `ImagesAnnotator-0.12.0-x86_64-fedora-42.rpm`.

Unless the project is cross compiled, the architecture is followed by the name and the version of the destination operating system, that is of the build host itself: the `ID` and the `VERSION_ID` fields of its `/etc/os-release` file, e.g. `fedora-42` or `rhel-9.6`. The `TEMPLATE_PROJECT_PACKAGE_SYSTEM` cache variable, shared by every packager of the project, holds it. A cross compiled build leaves it empty, so its package names the architecture alone; set it to name the destination system yourself (e.g. `-DTEMPLATE_PROJECT_PACKAGE_SYSTEM=rhel-9.6`), or to an empty string to name none.

In order to examine details of the `.rpm` package configuration visit the [cmake/enablers/packagers/template-project-rpm-enabler.cmake](/cmake/enablers/packagers/template-project-rpm-enabler.cmake) file.

The integration depends solely on `cmake` and `cpack` - no third-party application is invoked by the project's CMake module. CPack's built-in `RPM` generator handles the binary package assembly internally, the same way the `DEB` generator handles `.deb` files.

The following cache variables may optionally be overridden in order to tune the generated package metadata:

- `TEMPLATE_PROJECT_RPM_LICENSE` - SPDX-style license tag (default: `BSD-2-Clause`). Common values are `BSD-2-Clause`, `BSD-3-Clause`, `GPLv3`, `MIT`, `Apache-2.0`.
- `TEMPLATE_PROJECT_RPM_GROUP` - package group (default: `Applications/System`). Other typical values are `Development/Libraries`, `Development/Tools`, `Applications/Internet`.
- `TEMPLATE_PROJECT_RPM_RELEASE` - package release number (default: `1`). Bump it for rebuilds of the same upstream version.
- `TEMPLATE_PROJECT_RPM_PACKAGE_ARCHITECTURE` - architecture tag, which the package file name carries as well (default: the `CMAKE_SYSTEM_PROCESSOR` variable of the CMake). Other typical values are `aarch64`, `armv7hl`, `noarch`.

In addition to those, the package `URL` is taken from the project `HOMEPAGE_URL` declared in the top-level `CMakeLists.txt`, the maintainer contact from `PROJECT_MAINTAINER_EMAIL`, the vendor from `PROJECT_MAINTAINER`, and the summary/description from the project `DESCRIPTION`. There is no need to declare anything new in the project's CMake files in order to start producing packages - just turn the option on.

> **Runtime note.** Like the `DEB` generator which delegates to `dpkg-deb`, CPack's `RPM` generator relies on the `rpmbuild(8)` tool from the `rpm` system. It is therefore expected to be run on a RPM-based distribution (Fedora, RHEL, openSUSE, etc.) or on any host with the `rpm` package installed (on Debian/Ubuntu: `sudo apt install rpm`). The CMake module itself is platform-agnostic - it only configures CPack and does not invoke any external program directly.

### Packing the ImagesAnnotator libraries

The three ImagesAnnotator libraries the project is built against - the [ImagesAnnotator-DataDrivers](https://github.com/yuriysydor1991/ImagesAnnotator-DataDrivers.git), the [ImagesAnnotator-DataExporters](https://github.com/yuriysydor1991/ImagesAnnotator-DataExporters.git) and the [ImagesAnnotator-DataImporters](https://github.com/yuriysydor1991/ImagesAnnotator-DataImporters.git) - are no distribution packages, so the package above declares no dependency on them, and its binaries start only on a system which has them installed already. The `ENABLE_RPM_IMAGES_ANNOTATOR_LIBRARIES` option packs them into the package instead, the counterpart of the `ENABLE_DEB_IMAGES_ANNOTATOR_LIBRARIES` of [Enabling DEB package generation with cpack](/doc/sections/en_US/5-10-enabling-DEB-package-generation-with-cpack.md), the build and the `cpack` steps above staying the same:

```
# inside the project root directory

cmake -B build -S . -DCMAKE_INSTALL_PREFIX=/usr -DENABLE_RPM=ON -DENABLE_RPM_IMAGES_ANNOTATOR_LIBRARIES=ON
```

The package then carries the very shared library files the configure has found, each along with its SONAME link, in the library directory of the destination system - the `/usr/lib64` of a 64-bit Fedora, which its dynamic loader searches, rather than the `/usr/lib` their own install step uses. Nothing else has to be declared: the `rpmbuild` finds the dependencies of every binary of the package by itself - the libcurl, the zlib and the OpenCV the packed libraries are linked against and the GUI runtime of the graphical binary - and the packed libraries are provided by the package itself, so they add no dependency of their own. The `sudo dnf install ./<package>.rpm` installs the package along with all of them.
