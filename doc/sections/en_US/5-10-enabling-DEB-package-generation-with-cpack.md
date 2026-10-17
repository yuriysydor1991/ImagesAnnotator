## Enabling DEB package generation with cpack

In order to be able to generate the `deb` package file configure project to support the `cpack` command feature with a `ENABLE_DEB` variable enabled:

```
# inside the project root directory 

cmake -B build -S . -DENABLE_DEB=ON
```

Next, build all available targets required for the `deb` package

```
# inside the project root directory

cmake --build build --target all
```

Finally, execute the `cpack` command inside the template project build directory:

```
# inside the project build directory

cpack
```

The package file should be generated inside the project build root directory. For example, if project name was't change and it's version is 0.8.0 so the package name may look like `ImagesAnnotator-0.8.0-amd64-ubuntu-24.04.deb`.

After the version the package name carries the destination CPU architecture, the one of the package `Architecture` field as well. The `TEMPLATE_PROJECT_DEB_PACKAGE_ARCHITECTURE` cache variable holds it (default: the `dpkg --print-architecture` output, e.g. `amd64` or `arm64`), so override it when cross compiling the project or packaging it on a host without the `dpkg`.

Unless the project is cross compiled, the architecture is followed by the name and the version of the destination operating system, that is of the build host itself: the `ID` and the `VERSION_ID` fields of its `/etc/os-release` file, e.g. `ubuntu-24.04` or `debian-13` (a rolling release like the Debian sid names its `ID` alone). The `TEMPLATE_PROJECT_PACKAGE_SYSTEM` cache variable, shared by every packager of the project, holds it. A cross compiled build leaves it empty, so its package names the architecture alone; set it to name the destination system yourself (e.g. `-DTEMPLATE_PROJECT_PACKAGE_SYSTEM=debian-13`), or to an empty string to name none.

In order to examine details of the `deb` package configuration visit the `cmake/enablers/packagers/template-project-deb-enabler.cmake` file.

The generated package declares the GUI runtime it was built against as a dependency (`libgtkmm-3.0-1v5`, `libgtkmm-4.0-0`, `libwxgtk3.2-1t64` or `libqt6widgets6`). That name is not decided by the DEB enabler itself: it is published as `GTKMM_DEB_RUNTIME_DEPENDS` by whichever GUI toolkit enabler ran, so the packaged dependency automatically follows the `USE_GTKMM3` / `USE_GTKMM4` / `USE_WXWIDGETS` / `USE_QT6` selection described in the extensions section.

### Packing the ImagesAnnotator libraries

The three ImagesAnnotator libraries the project is built against - the [ImagesAnnotator-DataDrivers](https://github.com/yuriysydor1991/ImagesAnnotator-DataDrivers.git), the [ImagesAnnotator-DataExporters](https://github.com/yuriysydor1991/ImagesAnnotator-DataExporters.git) and the [ImagesAnnotator-DataImporters](https://github.com/yuriysydor1991/ImagesAnnotator-DataImporters.git) - are no distribution packages, so the package above declares no dependency on them: it installs anywhere, but its binaries start only on a system which has the three libraries installed already. The `ENABLE_DEB_IMAGES_ANNOTATOR_LIBRARIES` option packs them into the package instead, the build and the `cpack` steps above staying the same:

```
# inside the project root directory

cmake -B build -S . -DENABLE_DEB=ON -DENABLE_DEB_IMAGES_ANNOTATOR_LIBRARIES=ON
```

The package then carries the very shared library files the configure has found, each along with its SONAME link, under the `/usr/lib` directory their own install step uses. Those libraries are linked against the `libcurl`, the `zlib` and the `OpenCV` in their turn, which the system the package is installed on may lack, so with the option enabled the package declares the dependencies the `dpkg-shlibdeps` tool computes for every binary inside it as well, next to the GUI runtime one. The tool finds the three packed libraries inside the package itself, so they add no dependency of their own. It comes with the `dpkg-dev` package, which the `build-essential` one depends on, and CPack needs the `file` utility as well, to find the binaries to hand to the tool. The same scan adds the `ldconfig` call to the `postinst` and the `postrm` scripts of the package.

The [scripts/build/build-deb-with-libraries.sh](/scripts/build/build-deb-with-libraries.sh) script takes none of the libraries from the system: it builds the three of them into the `build/annotator-libraries` directory with the [misc/ci/install-annotator-libraries.sh](/misc/ci/install-annotator-libraries.sh) script first, and then produces the package against them with the option enabled (see [Quick build scripts](/doc/sections/en_US/5-38-quick-build-scripts.md)):

```
# inside the project root directory

scripts/build/build-deb-with-libraries.sh
```

The libraries are cloned from the `main` branches of their repositories; the `IADD_GIT`, `IADD_GIT_TAG`, `IADE_GIT`, `IADE_GIT_TAG`, `IADI_GIT` and `IADI_GIT_TAG` environment variables of that script override both the repositories and the branches - with the release tags of the libraries a release of the project is built against, for example.
