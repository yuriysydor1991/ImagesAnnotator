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

The package file should be generated inside the project build root directory. For example, if the project name wasn't changed and its version is `0.12.0` the package name may look like `ImagesAnnotator-0.12.0-1.x86_64.rpm`.

In order to examine details of the `.rpm` package configuration visit the [cmake/enablers/packagers/template-project-rpm-enabler.cmake](/cmake/enablers/packagers/template-project-rpm-enabler.cmake) file.

The integration depends solely on `cmake` and `cpack` - no third-party application is invoked by the project's CMake module. CPack's built-in `RPM` generator handles the binary package assembly internally, the same way the `DEB` generator handles `.deb` files.

The following cache variables may optionally be overridden in order to tune the generated package metadata:

- `TEMPLATE_PROJECT_RPM_LICENSE` - SPDX-style license tag (default: `BSD-2-Clause`). Common values are `BSD-2-Clause`, `BSD-3-Clause`, `GPLv3`, `MIT`, `Apache-2.0`.
- `TEMPLATE_PROJECT_RPM_GROUP` - package group (default: `Applications/System`). Other typical values are `Development/Libraries`, `Development/Tools`, `Applications/Internet`.
- `TEMPLATE_PROJECT_RPM_RELEASE` - package release number (default: `1`). Bump it for rebuilds of the same upstream version.
- `TEMPLATE_PROJECT_RPM_PACKAGE_ARCHITECTURE` - architecture tag (default: `x86_64`). Other typical values are `aarch64`, `armv7hl`, `noarch`.

In addition to those, the package `URL` is taken from the project `HOMEPAGE_URL` declared in the top-level `CMakeLists.txt`, the maintainer contact from `PROJECT_MAINTAINER_EMAIL`, the vendor from `PROJECT_MAINTAINER`, and the summary/description from the project `DESCRIPTION`. There is no need to declare anything new in the project's CMake files in order to start producing packages - just turn the option on.

> **Runtime note.** Like the `DEB` generator which delegates to `dpkg-deb`, CPack's `RPM` generator relies on the `rpmbuild(8)` tool from the `rpm` system. It is therefore expected to be run on a RPM-based distribution (Fedora, RHEL, openSUSE, etc.) or on any host with the `rpm` package installed (on Debian/Ubuntu: `sudo apt install rpm`). The CMake module itself is platform-agnostic - it only configures CPack and does not invoke any external program directly.
