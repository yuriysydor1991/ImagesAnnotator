## Enabling the flatpak package generation support

In order to enable the flatpak generation of the redistribution package on the project configure it with enabled option `ENABLE_FLATPAK` (GNU/Linux and alike):

```
# inside the project root directory 

cmake -B build -S . -DENABLE_FLATPAK=ON
```

Which in order will enable the `flatpak` target.

And finally to generate the flatpak package run the `flatpak` target for the build:

```
# inside the project root directory 

cmake --build build --target flatpak
```

In case of the success target build, there will be created the flatpak file under the root build directory, named after the binary the flatpak starts, the project version, the destination CPU architecture, the destination operating system and the flatpak SDK the bundle is built with, for example the `ImagesAnnotator-0.13.0-x86_64-ubuntu-24.04-org.gnome.Sdk-48.flatpak`. The CPU architecture is the one the `flatpak --default-arch` command prints (e.g. `x86_64` or `aarch64`). The name and the version of the destination system follow it the way they follow the architecture in the name of every other package of the project - the `TEMPLATE_PROJECT_PACKAGE_SYSTEM` cache variable described in [Enabling DEB package generation with cpack](/doc/sections/en_US/5-10-enabling-DEB-package-generation-with-cpack.md) - and the SDK closes the name, as the `sdk` and the `runtime-version` members of the flatpak manifest name it, since the very same version of the project may be packed into a flatpak per GUI toolkit (see below) on the very same system. In order to examine and/or change the flatpak generation target parameters visit the [cmake/enablers/packagers/template-project-flatpak-target.cmake](/cmake/enablers/packagers/template-project-flatpak-target.cmake) file or the manifests of the [misc/packagers/flatpak](/misc/packagers/flatpak) directory.

### A manifest per GUI toolkit

Each GUI toolkit is built against a runtime and modules of its own, so each has a flatpak manifest of its own, and the configure picks the one of the toolkit it builds (see [Extensions](/doc/sections/en_US/4-5-extensions.md)):

| GUI toolkit | Configure option | Manifest | Runtime and SDK | Built inside the sandbox |
| --- | --- | --- | --- | --- |
| GTKmm3 (the default) | `-DUSE_GTKMM3=ON` | [flatpak-gtkmm3.conf.json.in](/misc/packagers/flatpak/flatpak-gtkmm3.conf.json.in) | `org.gnome.Platform` and `org.gnome.Sdk` 48 | graphviz, mm-common, GTKmm 3.24.8 |
| GTKmm4 | `-DUSE_GTKMM4=ON` | [flatpak-gtkmm4.conf.json.in](/misc/packagers/flatpak/flatpak-gtkmm4.conf.json.in) | `org.gnome.Platform` and `org.gnome.Sdk` 49 | libsigc++ 3.6, glibmm 2.86, cairomm 1.18, pangomm 2.56, GTKmm 4.20 |
| Qt6 | `-DUSE_QT6=ON` | [flatpak-qt6.conf.json.in](/misc/packagers/flatpak/flatpak-qt6.conf.json.in) | `org.kde.Platform` and `org.kde.Sdk` 6.10 | nothing, the runtime carries the Qt6 |
| wxWidgets | `-DUSE_WXWIDGETS=ON` | [flatpak-wxwidgets.conf.json.in](/misc/packagers/flatpak/flatpak-wxwidgets.conf.json.in) | `org.gnome.Platform` and `org.gnome.Sdk` 49 | wxWidgets 3.2.9 |
| none, the converter only build | `-DBUILD_IMAGES_ANNOTATOR_CONVERTER_ONLY=ON` | [flatpak-converter.conf.json.in](/misc/packagers/flatpak/flatpak-converter.conf.json.in) | `org.freedesktop.Platform` and `org.freedesktop.Sdk` 24.08 | nothing |

Every manifest additionally builds the nlohmann json library, the core, the imgproc and the imgcodecs modules of the OpenCV and the three ImagesAnnotator libraries, and configures the project with the option of its row. The OpenCV gives the exporters and the importers libraries the image cropper and the size reader of their own, which the converter binary has no GUI toolkit to stand in for (see [The ImagesAnnotatorConverter binary](/doc/sections/en_US/5-40-the-ImagesAnnotatorConverter-binary.md)).

The `flatpak` target installs the runtime and the SDK the picked manifest names from the flathub by itself (the `--install-deps-from=flathub` of the `flatpak-builder`), adding the flathub remote to the user installation when it has none, so switching the toolkit needs no other preparation. See [Optional for the flatpak packager](/doc/sections/en_US/3-9-optional-for-the-flatpak-packager.md).

To pack the project with a manifest of your own, hand it with the `FLATPAK_CONF_SRC` cache variable (e.g. `-DFLATPAK_CONF_SRC=/path/to/manifest.json.in`): it is configured the same way, and its `sdk` and `runtime-version` members name the bundle. Left empty, the default, the manifest follows the selected toolkit, a reconfigure switching the toolkit included.

### Installing and running

Refer to the flatpak documentation on how to install and run applications distributed from a flatpaks files. For the current project defaults the install and run command may look like this:

```
# to install (version may change)
# inside the project build directory
flatpak install --user ImagesAnnotator-0.13.0-x86_64-ubuntu-24.04-org.gnome.Sdk-48.flatpak

# to run the application
flatpak run ua.org.kytok.template.gtkmm3.ImagesAnnotator
```

The application id carries the GUI toolkit (the `gtkmm3`, the `gtkmm4`, the `qt6` or the `wxwidgets`), so the flatpaks of different toolkits install side by side.

### Every binary of the flatpak

The flatpak starts the graphical binary, but it carries every binary the configure builds - the `ImagesAnnotatorConverter` as well, unless it is switched off with the `-DENABLE_IMAGES_ANNOTATOR_CONVERTER=OFF`. Each binary has an application id of its own:

| Binary | Application id |
| --- | --- |
| `ImagesAnnotator` | `ua.org.kytok.template.<toolkit>.ImagesAnnotator` |
| `ImagesAnnotatorConverter` | `ua.org.kytok.template.<toolkit>.ImagesAnnotator.Converter` |

The first one is the id of the flatpak itself. The converter is registered under the second one by a launcher of its own - the `<id>.desktop` file the project installs beside the one of the application, which runs it in a terminal and is hidden from the application menus - and the flatpak exports that launcher pointed at the converter, the way the LibreOffice flatpak registers its Writer under the `org.libreoffice.LibreOffice.writer` id. A flatpak starts any binary other than its own command with the `--command` option of the `flatpak run`:

```
flatpak run --command=ImagesAnnotatorConverter --filesystem=home \
  ua.org.kytok.template.gtkmm3.ImagesAnnotator \
  --convert-from yolo4 \
  --convert-src ~/the-existing-dataset \
  --convert-to coco \
  --convert-dst ~/the-wanted-dataset
```

The `--filesystem=home` grants the converter the home directory for this one run: the graphical application reaches the files of the host through the file chooser portal, which a command line program has no use of, while the sandbox the manifest declares opens no directory of the host to it.

A converter only build (`-DBUILD_IMAGES_ANNOTATOR_CONVERTER_ONLY=ON`) packs the converter alone, with the last manifest of the table above, into the `ImagesAnnotatorConverter-0.13.0-x86_64-ubuntu-24.04-org.freedesktop.Sdk-24.08.flatpak` named after it. The converter id is the id of that flatpak, and its manifest grants it the home directory itself:

```
flatpak install --user ImagesAnnotatorConverter-0.13.0-x86_64-ubuntu-24.04-org.freedesktop.Sdk-24.08.flatpak

flatpak run ua.org.kytok.template.gtkmm3.ImagesAnnotator.Converter \
  --convert-from yolo4 \
  --convert-src ~/the-existing-dataset \
  --convert-to coco \
  --convert-dst ~/the-wanted-dataset
```

The toolkit part of that id is the one the configure resolves, the `gtkmm3` unless another toolkit is asked for, although a converter only build builds none.
