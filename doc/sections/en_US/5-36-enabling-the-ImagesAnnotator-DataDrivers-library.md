## Enabling the ImagesAnnotator-DataDrivers library

The annotations project files - the JSON documents holding the annotated images, their sizes and the named rectangles drawn over them - are read and written by the standalone [ImagesAnnotator-DataDrivers](https://github.com/yuriysydor1991/ImagesAnnotator-DataDrivers.git) library. The parser, the serializer, the image record value objects and the duplicate aware merging rules used to live inside this repository and were extracted into that library so the application and any other tool may share one and the same project file format.

The library is therefore **mandatory**: the `ENABLE_IMAGES_ANNOTATOR_DATA_DRIVERS` CMake variable defaults to `ON` and the project does not build with it switched off.

```
# Inside the source root directory

cmake -S . -B build -DENABLE_IMAGES_ANNOTATOR_DATA_DRIVERS=ON
```

The enabler looks the library up with `find_package()`. There is no Internet fallback: the sibling [ImagesAnnotator-DataExporters](/doc/sections/en_US/5-37-enabling-the-ImagesAnnotator-DataExporters-library.md) library, which this project is built against as well, resolves the data drivers with a `find_package()` of its own and includes their headers through the versioned `<ImagesAnnotatorDataDrivers-0.11/...>` directory - a form only an installation provides - so a copy fetched into this build could not serve it. Both libraries are consumed installed.

### Pointing the project at an installed copy

```
git clone https://github.com/yuriysydor1991/ImagesAnnotator-DataDrivers.git
cmake -S ImagesAnnotator-DataDrivers -B build-iadd -DCMAKE_INSTALL_PREFIX=/opt/images-annotator
cmake --build build-iadd -j$(nproc)
cmake --install build-iadd
```

A library installed outside of the default search prefixes is found through the usual CMake variable:

```
cmake -S . -B build -DCMAKE_PREFIX_PATH=/opt/images-annotator
```

The [misc/ci/install-annotator-libraries.sh](/misc/ci/install-annotator-libraries.sh) script performs exactly the steps above for both libraries at once, and is what the three CI pipelines and the packager manifests build them with.

### Overriding the source repository and the tag

Nothing in this build fetches the library, but the packager manifests - the flatpak, the snap and the docker image - build it inside their own sandbox and take the repository from these two cache variables:

```
cmake -S . -B build \
  -DTEMPLATE_APP_IADD_GIT=https://github.com/yuriysydor1991/ImagesAnnotator-DataDrivers.git \
  -DTEMPLATE_APP_IADD_GIT_TAG=main
```

### Overriding the package name

The library carries its major and minor version inside the installed package, target and binary names - `ImagesAnnotatorDataDrivers-0.11`, matching its version carrying `ImagesAnnotatorDataDrivers011` public namespace - so two minor versions install completely side by side. Its own `LIB_INCLUDE_MINOR_IN_NAME` (`ON` by default), `LIB_INCLUDE_MICRO_IN_NAME` and `LIB_NAME_SUFFIX` options may shorten or extend that name further. The name this project asks `find_package()` for is a cache variable, so a differently named build of the very same library stays a single command line option:

```
# a library built with -DLIB_INCLUDE_MINOR_IN_NAME=OFF, that is a major-only name

cmake -S . -B build -DTEMPLATE_APP_IADD_PACKAGE=ImagesAnnotatorDataDrivers-0
```

### What the project takes from the library

- `ImageRecord` and `ImageRecordRect` - the record value objects the whole project, the GUI included, passes around. The `src/annotator-events/events/` headers of those names are aliases onto the library types now.
- `IAnnotationsDB` - the annotations project database the `AnnotatorController` holds, created through `LibraryFacade::create_annotations_db()`.
- `IImagesPathsDBProvider` - the read only records access interface the GUI consumes, which the `AnnotatorController` implements.
- `IRecordsFactory` - the records factory the project `EventsFactory` delegates its record creating methods to.

The single place naming the library is `src/annotator-events/events/DataDriversAliases.h`, which declares the `iadd` alias of the versioned `ImagesAnnotatorDataDrivers011` namespace. Its headers are included through the versioned `<ImagesAnnotatorDataDrivers-0.11/...>` directory: the data exporters library installs a public `LibraryFacade.h` of its own, so the plain spelling would name whichever of the two the compiler sees first.
