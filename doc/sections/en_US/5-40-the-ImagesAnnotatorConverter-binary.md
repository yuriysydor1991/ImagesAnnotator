## The ImagesAnnotatorConverter binary

Beside the `ImagesAnnotator` application the project builds a second binary,
`ImagesAnnotatorConverter`, which performs the annotations format conversion
and nothing else. It is the very same conversion the main binary offers through
its own [command line](/doc/sections/en_US/6-2-3-converting-the-annotations-formats.md)
- the flags, the formats and the behaviour are one and the same, because the
code behind them is one and the same - and what the second binary adds is a
deployment option rather than a feature: **no GUI toolkit is compiled into it,
and a converter only build needs none installed at all**.

That is what it exists for. A build server, a container image or a data
preparation pipeline which only has to turn one dataset layout into another one
gains no GTKmm, Qt6 or wxWidgets dependency to satisfy, and the produced binary
is roughly a third smaller than the graphical one.

### Usage

The binary accepts the very same parameters the main one does, so the
[Converting the annotations formats](/doc/sections/en_US/6-2-3-converting-the-annotations-formats.md)
section describes it in full. Everything it can do is a conversion:

```
# from the build dir

./src/ImagesAnnotatorConverter \
  --convert-from yolo4 \
  --convert-src ./the-existing-dataset \
  --convert-to coco \
  --convert-dst ./the-wanted-dataset
```

`--help` and `--version` work as they do for the main binary and name this one.
A command line which asks for no conversion has nothing to start - there is no
graphical interface behind this binary - so it is answered with the help
message and a non zero exit status.

### Building it

Both binaries are built by default, so a plain build of this project produces
`ImagesAnnotator` and `ImagesAnnotatorConverter` side by side, and both are
installed by the install step.

| CMake option | Default | Effect |
| --- | --- | --- |
| `ENABLE_IMAGES_ANNOTATOR_CONVERTER` | `ON` | Builds the `ImagesAnnotatorConverter` binary. Set it to `OFF` to build the main application alone. |
| `BUILD_IMAGES_ANNOTATOR_CONVERTER_ONLY` | `OFF` | Builds the `ImagesAnnotatorConverter` binary **alone**: the `ImagesAnnotator` target, the whole `src/gui` subtree and every GUI toolkit probe are excluded from the configure. Implies `ENABLE_IMAGES_ANNOTATOR_CONVERTER`. |
| `PROJECT_CONVERTER_BINARY_NAME` | `<project binary name>Converter` | The name of the second binary and of its CMake target. |

Building and installing the converter alone - the case a headless machine wants
- takes the second option. Neither GTKmm, nor Qt6, nor wxWidgets has to be
present for this to configure:

```
# inside the project root directory

cmake -B build/converter -S . -DBUILD_IMAGES_ANNOTATOR_CONVERTER_ONLY=ON
cmake --build build/converter -j$(nproc)
sudo cmake --install build/converter
```

The three ImagesAnnotator libraries are still required, since the conversion is
performed by them; see [Enabling the ImagesAnnotator-DataDrivers
library](/doc/sections/en_US/5-36-enabling-the-ImagesAnnotator-DataDrivers-library.md)
and the two sections after it.

**Two of them need their OpenCV for this binary.** The exporters library asks
its consumer to cut the annotated rectangles out of the pictures through
`IImageCropperFacility`, and the importers one asks it to measure them through
`IImageSizeFacility` - and it is the GUI toolkit of the main application which
implements the pair. This binary starts no toolkit at all, so the OpenCV cropper
and the OpenCV size reader those libraries ship themselves are the only
implementations it has. Their `ENABLE_OPENCV` option is `ON` by default, so a
stock installation of both needs nothing done about it; an installation built
with `-DENABLE_OPENCV=OFF`, or made on a machine carrying no OpenCV development
packages, leaves this binary unable to write the PyTorch Vision layout and
unable to read the YOLO v4, the three Ultralytics YOLO and the PyTorch Vision
ones, whose annotations are stored relative to a size it can then not measure.
OpenCV is therefore optional for the application and required here.

The `ENABLE_IMAGES_ANNOTATOR_CONVERTER=OFF` case is the mirror of that one: the
main application binary alone, exactly as the project was built before the
second target existed.

### How the two binaries share their code

The second binary is not a copy of anything. Every component of `src` - the
annotator business controller, the events, the loggers, the helpers and the
libcurl controller - is an object library which is linked into **every**
application target being built, and the `src/app` component hands its sources
to all of them the very same way, through the
`annotator_app_targets_sources()` and `annotator_app_targets_link_libraries()`
functions of
[cmake/template-project-annotator-app-targets.cmake](/cmake/template-project-annotator-app-targets.cmake).
Both binaries are entered through one and the same `src/main.cpp`.

Exactly one translation unit differs:
`src/app/applications/Application.cpp`, the class which starts the compiled in
`gui::GUIIniter`. It is handed to the main target alone
by [src/ImagesAnnotator-target.cmake](/src/ImagesAnnotator-target.cmake), while
[src/ImagesAnnotatorConverter-target.cmake](/src/ImagesAnnotatorConverter-target.cmake)
compiles its target with the `IMAGES_ANNOTATOR_CONVERTER_ONLY` preprocessor
definition instead. That definition is what tells the two sources both binaries
share about the missing class: `ApplicationFactory` neither includes nor creates
it, and the help and version messages name the binary being run. Nothing else
in the project branches on it.

Since the `app::Application` class is the only thing which names
`src/gui/GUIIniter.h`, leaving that one file out is what leaves the entire GUI
out - and the
`ITEST_ImagesAnnotatorConverter.no_gui_toolkit_linked` integration test proves
it on the produced file rather than on the build files, by refusing any toolkit
library among the ones the binary loads and any toolkit symbol among the ones it
carries. See [Project tests](/doc/sections/en_US/4-4-project-tests.md).
