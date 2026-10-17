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

### A snapcraft.yaml per GUI toolkit

Like the flatpak, the snap builds the project from its sources, so each GUI toolkit has a `snapcraft.yaml` of its own, which installs the packages the toolkit is built against and stages the ones it runs with, and the configure picks the one of the toolkit it builds (see [Extensions](/doc/sections/en_US/4-5-extensions.md)):

| GUI toolkit | Configure option | snapcraft.yaml |
| --- | --- | --- |
| GTKmm3 (the default) | `-DUSE_GTKMM3=ON` | [snapcraft-gtkmm3.yaml.in](/misc/packagers/snapcraft/snapcraft-gtkmm3.yaml.in) |
| GTKmm4 | `-DUSE_GTKMM4=ON` | [snapcraft-gtkmm4.yaml.in](/misc/packagers/snapcraft/snapcraft-gtkmm4.yaml.in) |
| Qt6 | `-DUSE_QT6=ON` | [snapcraft-qt6.yaml.in](/misc/packagers/snapcraft/snapcraft-qt6.yaml.in) |
| wxWidgets | `-DUSE_WXWIDGETS=ON` | [snapcraft-wxwidgets.yaml.in](/misc/packagers/snapcraft/snapcraft-wxwidgets.yaml.in) |
| none, the converter only build | `-DBUILD_IMAGES_ANNOTATOR_CONVERTER_ONLY=ON` | [snapcraft-converter.yaml.in](/misc/packagers/snapcraft/snapcraft-converter.yaml.in) |

All of them build on the `core24` base (the Ubuntu 24.04 packages) and build the three ImagesAnnotator libraries ahead of the project, the exporters and the importers ones with their OpenCV, which gives them the image cropper and the size reader the converter binary has no GUI toolkit to stand in for (see [The ImagesAnnotatorConverter binary](/doc/sections/en_US/5-40-the-ImagesAnnotatorConverter-binary.md)). To pack the project with a `snapcraft.yaml` of your own, hand it with the `SNAPCRAFT_CONF_SRC` cache variable; left empty, the default, the file follows the selected toolkit.

Visit the [cmake/enablers/packagers/template-project-snap-enabler.cmake](/cmake/enablers/packagers/template-project-snap-enabler.cmake) file to examine the details.
