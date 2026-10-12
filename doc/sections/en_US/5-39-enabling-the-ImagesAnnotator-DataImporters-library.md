## Enabling the ImagesAnnotator-DataImporters library

A training dataset that is already on the disk is read back into an annotations project - the plain text files, the YOLO v4 (darknet) training directory, the three Ultralytics YOLO layouts, the COCO and the Create ML JSON descriptors, the Pascal VOC XML one, the PyTorch Vision `ImageFolder` layout of cropped out rectangles and the four [darknetxx](https://github.com/yuriysydor1991/darknetxx) ones, its dxxwjz2p and dxxwjz3p training projects and its training directories of the dxxwjz2 and the dxxwjz3 networks - by the standalone [ImagesAnnotator-DataImporters](https://github.com/yuriysydor1991/ImagesAnnotator-DataImporters.git) library. It is the mirror image of the [ImagesAnnotator-DataExporters](/doc/sections/en_US/5-37-enabling-the-ImagesAnnotator-DataExporters-library.md) library: what that one writes, this one reads, so a project exported into any of those thirteen layouts comes back with the annotations it started with, and a dataset produced by some other tool enters a project for the first time.

The library is therefore **mandatory**: the `ENABLE_IMAGES_ANNOTATOR_DATA_IMPORTERS` CMake variable defaults to `ON` and the project does not build with it switched off.

```
# Inside the source root directory

cmake -S . -B build -DENABLE_IMAGES_ANNOTATOR_DATA_IMPORTERS=ON
```

### It has to be installed first

Unlike most of the third party enablers of the project, this one has no Internet fallback, for the very reason the exporters one has none: the library is consumed installed, and so is the [ImagesAnnotator-DataDrivers](/doc/sections/en_US/5-36-enabling-the-ImagesAnnotator-DataDrivers-library.md) library it is built against. The importers library resolves the data drivers with a `find_package()` of its own and includes their headers through the versioned `<ImagesAnnotatorDataDrivers-0.14/...>` directory, which only an installation provides.

Build and install both, the data drivers first:

```
git clone https://github.com/yuriysydor1991/ImagesAnnotator-DataDrivers.git
cmake -S ImagesAnnotator-DataDrivers -B build-iadd -DCMAKE_INSTALL_PREFIX=/opt/images-annotator
cmake --build build-iadd -j$(nproc)
cmake --install build-iadd

git clone https://github.com/yuriysydor1991/ImagesAnnotator-DataImporters.git
cmake -S ImagesAnnotator-DataImporters -B build-iadi \
  -DCMAKE_INSTALL_PREFIX=/opt/images-annotator \
  -DCMAKE_PREFIX_PATH=/opt/images-annotator
cmake --build build-iadi -j$(nproc)
cmake --install build-iadi
```

The library links the [zlib](https://zlib.net/) as well, which its darknetxx layouts read the gzip compressed dxxwjz files with: install its development package (`zlib1g-dev` on Debian based systems) before configuring it, or let its enabler fetch its sources.

The shared [misc/ci/install-annotator-libraries.sh](/misc/ci/install-annotator-libraries.sh) script does exactly this for all three libraries at once, which is the quickest way to provision a development machine.

### Pointing the project at the installed copies

The same CMake variable the two sibling libraries are found through, which takes every prefix when they differ:

```
cmake -S . -B build -DCMAKE_PREFIX_PATH=/opt/images-annotator
```

All three shared objects have to be reachable by the dynamic linker when the application is started. Either register the prefix (for example by adding it to `/etc/ld.so.conf.d/` and running `ldconfig`) or name it in the environment:

```
LD_LIBRARY_PATH=/opt/images-annotator/lib ./ImagesAnnotator
```

### What the packagers do about it

The importers library is no distribution package either, so the packagers treat it exactly as they treat the other two:

- the [flatpak](/doc/sections/en_US/5-11-enabling-the-flatpak-package-generation-support.md) bundle, the [snap](/doc/sections/en_US/5-19-enabling-the-snap-package-manager.md) and the [docker](/doc/sections/en_US/5-12-enabling-the-docker-container-build-and-run.md) image build all three libraries from their sources into the produced artifact, so those three stay self contained. The repository and the revision come from the `TEMPLATE_APP_IADI_GIT` / `TEMPLATE_APP_IADI_GIT_TAG` cache variables of the enabler;
- the [deb](/doc/sections/en_US/5-10-enabling-DEB-package-generation-with-cpack.md), the [rpm](/doc/sections/en_US/5-22-enabling-RPM-package-generation-with-cpack.md), the [FreeBSD pkg](/doc/sections/en_US/5-20-enabling-FreeBSD-pkg-package-generation-with-cpack.md) and the [WiX MSI](/doc/sections/en_US/5-21-enabling-WIX-MSI-package-generation-with-cpack.md) packages carry the application alone, so the target system needs the shared object installed the way this section describes.

The three CI pipelines install it through the same [misc/ci/install-annotator-libraries.sh](/misc/ci/install-annotator-libraries.sh) script, whose `IADI_GIT_TAG` environment variable is what a pipeline validating an unreleased library revision overrides.

### Overriding the package name

The library carries its major and minor version inside the installed package, target and binary names - `ImagesAnnotatorDataImporters-0.16`, matching its version carrying `ImagesAnnotatorDataImporters016` public namespace - so two minor versions install completely side by side. The name this project asks `find_package()` for is a cache variable:

```
# a library built with -DLIB_INCLUDE_MINOR_IN_NAME=OFF, that is a major-only name

cmake -S . -B build -DTEMPLATE_APP_IADI_PACKAGE=ImagesAnnotatorDataImporters-0
```

### What the project takes from the library

- `IImporter` and the thirteen `IADataImportersContext` layout descendants - the thirteen importers the `AnnotatorController` runs when the GUI raises an import request. It builds them through `IADataImportersFacade::create_importer()`, handing it the context of the wanted layout.
- `IADataImportersContext` - the import configuration (the source directory, the annotations database the recovered records are merged into and the image measuring service) the controller fills in through the setters of the context. The database it hands over is the very one the project is being edited through, so an import appends to the open project instead of replacing it, and the library merge keeps an image the project already holds - which is what makes an import safe to repeat.
- `IImageSizeFacility` - the image measuring service seven of the thirteen imports need. The library decodes no image format itself, and an annotation of the project format is a rectangle in the pixels of its image, so the YOLO v4 layout, the two darknetxx training directories which extend it and the three Ultralytics YOLO ones (whose boxes are stored divided by that size) and the PyTorch Vision one (whose annotation is the cropped image itself) ask the application to measure the pictures. The COCO and the Pascal VOC descriptors and the two darknetxx training projects carry the image dimensions themselves, and the plain text and the Create ML layouts hold their rectangles in the image own pixels, so those six are read whatever the application supplies - and every import request carries the reader all the same, since it is what fills the image dimensions of a recovered record in. Every GUI toolkit of this project implements that interface over the imaging stack it already links: `GtkmmImageSizeProvider`, `QtImageSizeProvider` and `WxImageSizeProvider`, the counterparts of the three image croppers of the export side. The `src/annotator-events/events/IImageSizeFacilityProvider.h` header of the project is an alias onto it.

The single place naming the library is `src/annotator-events/events/DataImportersAliases.h`, which declares the `iadi` alias of the versioned `ImagesAnnotatorDataImporters016` namespace. Its headers are included through the versioned `<ImagesAnnotatorDataImporters-0.16/...>` directory: the prefix pins the library version every translation unit is talking to, and `IImporter.h` beside it is generic enough to collide on a busy include path.
