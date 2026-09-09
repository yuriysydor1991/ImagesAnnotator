## Enabling the AppImage packager

In order to enable the project's `appimage` target designed to generate the project's [AppImage](https://appimage.org/) single file distributive, configure project with enabled `ENABLE_APPIMAGE` CMake variable:

```
# inside the project root directory

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr -DENABLE_APPIMAGE=ON
```

Next, build the `appimage` target:

```
# inside the project root directory

cmake --build build --target appimage
```

The [scripts/build/build-appimage.sh](/scripts/build/build-appimage.sh) script performs both steps at once, and installs the produced package into the `${HOME}/.local/bin` directory when it is given the `--install` flag (see [Quick build scripts](/doc/sections/en_US/5-38-quick-build-scripts.md)):

```
# inside the project root directory

scripts/build/build-appimage.sh
```

The package file is generated inside the project build root directory. Its name carries the packaged binary, the project version and the destination CPU architecture, so with the project version `0.12.0` on an `x86_64` host it is `ImagesAnnotator-0.12.0-x86_64.AppImage`.

The produced package is a single executable file which needs no installation at all:

```
# inside the project build root directory

chmod +x ImagesAnnotator-0.12.0-x86_64.AppImage

./ImagesAnnotator-0.12.0-x86_64.AppImage
```

### The two implementations

The AppImage is packed by the CPack `AppImage` generator, which arrived with the CMake 4.2, whenever the CMake performing the configure is that new. An older CMake keeps the `appimagetool` driven custom target instead, which does the same job on its own. The configure says which one it took:

```
-- AppImage packager: the CPack generator
-- AppImage packager: the appimagetool custom target
```

Both implementations produce the same package under the same name and are both built by the same `appimage` target, so nothing outside of them - neither the script above nor a pipeline - has to know which one is in use. `ENABLE_APPIMAGE_CPACK=ON` demands the CPack one regardless of the `ENABLE_APPIMAGE`, and fails the configure on a CMake older than 4.2.

Both need the `appimagetool` command, and the CPack one additionally needs the `patchelf` it rewrites the packaged binary `RPATH` with. See [Optional for the AppImage packager](/doc/sections/en_US/3-12-optional-for-the-AppImage-packager.md).

The `CMAKE_INSTALL_PREFIX` of the configure step above is the prefix the binary is placed under inside the package, not a directory of the host, as the whole packed directory becomes the file system root of the running package.

### The desktop entry and the icon

An AppImage is built around a desktop entry naming an icon, and this project already installs exactly such a pair - the `<application id>.desktop` launcher and the `<application id>.png` themed icon of [cmake/enablers/template-project-desktop-entry-enabler.cmake](/cmake/enablers/template-project-desktop-entry-enabler.cmake), whose names agree the way an AppImage demands. The packager reuses them, so the package carries the very launcher and the very icon an installed copy of the application does, and switching the GUI toolkit switches them along with the application id (see [Extensions](/doc/sections/en_US/4-5-extensions.md)).

The `ImagesAnnotatorConverter` only build (`BUILD_IMAGES_ANNOTATOR_CONVERTER_ONLY=ON`, see [The ImagesAnnotatorConverter binary](/doc/sections/en_US/5-40-the-ImagesAnnotatorConverter-binary.md)) carries no launcher, the desktop integration belonging to the graphical binary alone. It is the one case where the packager configures a desktop entry and an icon of its own, from the templates of the [misc/packagers/appimage](/misc/packagers/appimage) directory, which is what gives the command line converter an AppImage as well:

- [appimage.desktop.in](/misc/packagers/appimage/appimage.desktop.in) - the desktop entry the desktop environments integrate the package with;
- [appimage.icon.svg.in](/misc/packagers/appimage/appimage.icon.svg.in) - the icon the desktop entry names.

The `appimagetool` implementation additionally configures [appimage.AppRun.in](/misc/packagers/appimage/appimage.AppRun.in) into the `AppRun` startup script, which exports the `LD_LIBRARY_PATH` of the packaged libraries and starts the project binary. The CPack one generates its own `AppRun` out of the desktop entry instead.

### Cache variables

The following cache variables may optionally be overridden in order to tune the produced package:

- `TEMPLATE_PROJECT_APPIMAGE_ARCHITECTURE` - the destination CPU architecture (default: the `CMAKE_SYSTEM_PROCESSOR` variable of the CMake). Other typical values are `aarch64`, `armhf`, `i686`.
- `TEMPLATE_PROJECT_APPIMAGE_CATEGORIES` - the semicolon terminated desktop entry categories (default: `Graphics;`).
- `TEMPLATE_PROJECT_APPIMAGE_TERMINAL` - the `Terminal` key of the desktop entry (default: `true`).
- `APPIMAGE_ICON_SRC` - the SVG icon source file the CMake configures beside the package (default: the template one above).

The last three describe the desktop entry the packager generates itself, so they take effect for the converter only build alone. The graphical build takes both from the launcher enabler, where the `Categories`, the `Terminal` key and the icon of the installed application are declared once for the application, the DEB package, the flatpak and the AppImage together.

The destination CPU architecture ends up in the package file name and is given to the `appimagetool` as the `ARCH` environment variable it requires. Override it when cross compiling the project, so the package the CMake names is the package the `appimagetool` embeds.

In order to examine details of the AppImage package configuration visit the [cmake/enablers/packagers/template-project-appimage-general-enabler.cmake](/cmake/enablers/packagers/template-project-appimage-general-enabler.cmake) file, which resolves everything the two implementations share and includes the one it picks - [template-project-appimage-cpack-enabler.cmake](/cmake/enablers/packagers/template-project-appimage-cpack-enabler.cmake) or [template-project-appimage-enabler.cmake](/cmake/enablers/packagers/template-project-appimage-enabler.cmake).

> **Runtime note.** Unlike the flatpak and the snap packagers, which build the project inside their own sandboxes with the dependencies they install themselves, the `appimagetool` packs the installed project alone. The libraries of the host the project links against - the GUI toolkit and the three ImagesAnnotator libraries among them - are therefore expected on the machine the package runs, unless they are installed into the packed directory by the project itself. Build the package on the oldest distribution the package is meant to run on to keep the `glibc` requirement of the binary as low as possible.
