## Enabling the ImagesAnnotator-DataExporters library

The annotated images are written out as a training dataset - the plain text files, the YOLO v4 (darknet) training directory and the PyTorch Vision `ImageFolder` layout of cropped out rectangles - by the standalone [ImagesAnnotator-DataExporters](https://github.com/yuriysydor1991/ImagesAnnotator-DataExporters.git) library. The three exporters used to live inside this repository and were extracted into that library so the application and any other tool may produce one and the same dataset out of one and the same annotations project file.

The library is therefore **mandatory**: the `ENABLE_IMAGES_ANNOTATOR_DATA_EXPORTERS` CMake variable defaults to `ON` and the project does not build with it switched off.

```
# Inside the source root directory

cmake -S . -B build -DENABLE_IMAGES_ANNOTATOR_DATA_EXPORTERS=ON
```

### It has to be installed first

Unlike most of the third party enablers of the project, this one has no Internet fallback: the library is consumed installed, and so is the [ImagesAnnotator-DataDrivers](/doc/sections/en_US/5-36-enabling-the-ImagesAnnotator-DataDrivers-library.md) library it is built against. The exporters library resolves the data drivers with a `find_package()` of its own and includes their headers through the versioned `<ImagesAnnotatorDataDrivers-0.11/...>` directory, which only an installation provides, so a copy pulled in as a sub-project could not be configured at all.

Build and install both, the data drivers first:

```
git clone https://github.com/yuriysydor1991/ImagesAnnotator-DataDrivers.git
cmake -S ImagesAnnotator-DataDrivers -B build-iadd -DCMAKE_INSTALL_PREFIX=/opt/images-annotator
cmake --build build-iadd -j$(nproc)
cmake --install build-iadd

git clone https://github.com/yuriysydor1991/ImagesAnnotator-DataExporters.git
cmake -S ImagesAnnotator-DataExporters -B build-iade \
  -DCMAKE_INSTALL_PREFIX=/opt/images-annotator \
  -DCMAKE_PREFIX_PATH=/opt/images-annotator
cmake --build build-iade -j$(nproc)
cmake --install build-iade
```

### Pointing the project at the installed copies

Libraries installed outside of the default search prefixes are found through the usual CMake variable, which takes both prefixes when they differ:

```
cmake -S . -B build -DCMAKE_PREFIX_PATH=/opt/images-annotator
```

Both shared objects have to be reachable by the dynamic linker when the application is started. Either register the prefix (for example by adding it to `/etc/ld.so.conf.d/` and running `ldconfig`) or name it in the environment:

```
LD_LIBRARY_PATH=/opt/images-annotator/lib ./ImagesAnnotator
```

### What the packagers do about it

Neither library is a distribution package, so the packagers of this project differ in what they can promise:

- the [flatpak](/doc/sections/en_US/5-11-enabling-the-flatpak-package-generation-support.md) bundle, the [snap](/doc/sections/en_US/5-19-enabling-the-snap-package-manager.md) and the [docker](/doc/sections/en_US/5-12-enabling-the-docker-container-build-and-run.md) image build both libraries from their sources into the produced artifact, so those three stay self contained. The repositories and the revisions they build come from the `TEMPLATE_APP_IADD_GIT` / `TEMPLATE_APP_IADD_GIT_TAG` and `TEMPLATE_APP_IADE_GIT` / `TEMPLATE_APP_IADE_GIT_TAG` cache variables of the two enablers;
- the [deb](/doc/sections/en_US/5-10-enabling-DEB-package-generation-with-cpack.md), the [rpm](/doc/sections/en_US/5-22-enabling-RPM-package-generation-with-cpack.md), the [FreeBSD pkg](/doc/sections/en_US/5-20-enabling-FreeBSD-pkg-package-generation-with-cpack.md) and the [WiX MSI](/doc/sections/en_US/5-21-enabling-WIX-MSI-package-generation-with-cpack.md) packages carry the application alone. Their dependency lists name distribution packages, and there is none to name for these two, so the target system needs both shared objects installed the way this section describes.

The three CI pipelines - [misc/Jenkinsfile](/misc/Jenkinsfile), [.github/workflows/ci.yml](/.github/workflows/ci.yml) and [.gitlab/ci](/.gitlab/ci) - install them through the shared [misc/ci/install-annotator-libraries.sh](/misc/ci/install-annotator-libraries.sh) script before they configure anything. The same script is the quickest way to provision a development machine, and its `IADD_GIT_TAG` / `IADE_GIT_TAG` environment variables are what a pipeline validating an unreleased library revision overrides.

### Overriding the package name

The library carries its major and minor version inside the installed package, target and binary names - `ImagesAnnotatorDataExporters-0.11`, matching its version carrying `ImagesAnnotatorDataExporters011` public namespace - so two minor versions install completely side by side. Its own `LIB_INCLUDE_MINOR_IN_NAME` (`ON` by default), `LIB_INCLUDE_MICRO_IN_NAME` and `LIB_NAME_SUFFIX` options may shorten or extend that name further. The name this project asks `find_package()` for is a cache variable, so a differently named build of the very same library stays a single command line option:

```
# a library built with -DLIB_INCLUDE_MINOR_IN_NAME=OFF, that is a major-only name

cmake -S . -B build -DTEMPLATE_APP_IADE_PACKAGE=ImagesAnnotatorDataExporters-0
```

### What the project takes from the library

- `IExporter` and the three `LibraryContext` layout descendants - the three exporters the `AnnotatorController` runs when the GUI raises an export request. It builds them through `LibraryFacade::create_exporter()`, handing it the context of the wanted layout, instead of instantiating exporter classes of its own.
- `LibraryContext` - the export configuration (the destination directory and the annotations database to read) the controller fills in through the setters of the context. The optional image cropper sits on `PyTorchExportLibraryContext`, the one layout which cuts rectangles out.
- `IImageCropperFacility` - the image cropping service the PyTorch Vision export needs. The library decodes no image format itself, so every GUI toolkit of this project implements that interface over the imaging stack it already links: `GtkmmImageCropperProvider`, `QtImageCropperProvider` and `WxImageCropperProvider`. The `src/annotator-events/events/IImageCropperFacilityProvider.h` header of the project is an alias onto it now.

The single place naming the library is `src/annotator-events/events/DataExportersAliases.h`, which declares the `iade` alias of the versioned `ImagesAnnotatorDataExporters011` namespace. Its headers are included through the versioned `<ImagesAnnotatorDataExporters-0.11/...>` directory: both libraries install a public `LibraryFacade.h`, so the plain spelling would name whichever of the two the compiler sees first.
