## Enabling the snap packager

In order to enable the project's `snap` package target designed to generate the project's [SNAP](https://snapcraft.io/docs/) binary package distributive, configure project with enabled `ENABLE_SNAP` CMake variable:

```
cmake -S . -B build -DENABLE_SNAP=ON
```

Next, build the `snap` target, which runs the `snapcraft pack` command inside the build directory:

```
cmake --build build --target snap
```

The [scripts/build/build-snap.sh](/scripts/build/build-snap.sh) script performs both steps at once (see [Quick build scripts](/doc/sections/en_US/5-38-quick-build-scripts.md)). The `snapcraft` and the container manager it builds in are described in [Optional for the snap packager](/doc/sections/en_US/3-11-optional-for-snap-packager.md).

The snapcraft names the package `<snap name>_<version>_<architecture>.snap`, the architecture being the Debian name of the CPU it is built for (e.g. `amd64` or `arm64`), and the target appends the name and the version of the destination operating system to it the way they follow the architecture in the name of every other package of the project - the `TEMPLATE_PROJECT_PACKAGE_SYSTEM` cache variable described in [Enabling DEB package generation with cpack](/doc/sections/en_US/5-10-enabling-DEB-package-generation-with-cpack.md). So with the project version `0.13.0` on an `amd64` Ubuntu 24.04 host the package is `imagesannotator_0.13.0_amd64-ubuntu-24.04.snap`.

Visit the [cmake/enablers/packagers/template-project-snap-enabler.cmake](/cmake/enablers/packagers/template-project-snap-enabler.cmake) file to examine the details.
