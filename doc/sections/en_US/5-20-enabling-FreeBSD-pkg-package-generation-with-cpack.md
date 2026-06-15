## Enabling FreeBSD pkg package generation with cpack

In order to be able to generate the FreeBSD `.pkg` binary package file configure the project to support the `cpack` command feature with the `ENABLE_FREEBSD_PKG` variable enabled:

```
# inside the project root directory 

cmake -B build -S . -DENABLE_FREEBSD_PKG=ON
```

Next, build all available targets required for the FreeBSD `pkg` package:

```
# inside the project root directory

cmake --build build --target all
```

Finally, execute the `cpack` command inside the template project build directory:

```
# inside the project build directory

cpack
```

The package file should be generated inside the project build root directory. For example, if project name wasn't changed and it's version is `0.12.0` the package name may look like `ImagesAnnotator-0.12.0-amd64.pkg` (the actual architecture suffix depends on the build host).

In order to examine details of the FreeBSD `pkg` package configuration visit the [cmake/enablers/packagers/template-project-freebsd-pkg-enabler.cmake](/cmake/enablers/packagers/template-project-freebsd-pkg-enabler.cmake) file.

The integration depends solely on `cmake` and `cpack` - no third-party application is invoked by the project's CMake module. CPack's built-in `FREEBSD` generator handles the binary package assembly internally, the same way the DEB generator handles `.deb` files.

The following cache variables may optionally be overridden in order to tune the generated package metadata:

- `TEMPLATE_PROJECT_FREEBSD_PKG_LICENSE` - SPDX-style license tag (default: `BSD2CLAUSE`). Common values are `BSD2CLAUSE`, `BSD3CLAUSE`, `GPLv3`, `MIT`, `APACHE20`.
- `TEMPLATE_PROJECT_FREEBSD_PKG_ORIGIN` - FreeBSD ports origin in `<category>/<name>` form (default: `misc/<project-binary-name-lowercased>`).
- `TEMPLATE_PROJECT_FREEBSD_PKG_CATEGORIES` - Semicolon-separated list of `pkg` categories (default: `misc`).

In addition to those, the package `WWW` field is taken from the project `HOMEPAGE_URL` declared in the top-level `CMakeLists.txt`, the maintainer email from `PROJECT_MAINTAINER_EMAIL`, and the comment/description from the project `DESCRIPTION`. There is no need to declare anything new in the project's CMake files in order to start producing packages - just turn the option on.

> **Runtime note.** Like the `DEB` generator which delegates to `dpkg-deb`, CPack's `FREEBSD` generator relies on `libpkg` from FreeBSD's `pkg` system. It is therefore expected to be run on FreeBSD (or on a Linux host where a CMake build with `libpkg` support has been installed). The CMake module itself is platform-agnostic - it only configures CPack and does not invoke any external program directly.
