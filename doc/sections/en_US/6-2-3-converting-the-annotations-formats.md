### Converting the annotations formats

Beside the graphical interface the executable carries a converter of its own,
which reads an annotations dataset of one format and writes it back in another
one. It is asked for by four command line flags, and it starts no window: the
application tells what it is preparing and converting through the log messages
alone, so it fits a script and a build pipeline.

```
# from the build dir

./src/ImagesAnnotator \
  --convert-from yolo4 \
  --convert-src ./the-existing-dataset \
  --convert-to coco \
  --convert-dst ./the-wanted-dataset
```

The project also builds a second binary, `ImagesAnnotatorConverter`, which
carries this very conversion and no graphical interface at all - the same flags,
the same formats, the same behaviour - and which may be built and installed on
its own, without any GUI toolkit present. See [The ImagesAnnotatorConverter
binary](/doc/sections/en_US/5-40-the-ImagesAnnotatorConverter-binary.md).

- `--convert-from FORMAT` - the annotations format to read,
- `--convert-src PATH` - the path to read the annotations from,
- `--convert-to FORMAT` - the annotations format to write,
- `--convert-dst PATH` - the path to write the annotations into.

The four go together. Naming some of them and not the others is an error, and
the help message is printed instead; naming none of them starts the graphical
interface as before.

A `FORMAT` is one of the names below. The first one is the annotations project
of the application itself - the JSON file the
[ImagesAnnotator-DataDrivers](https://github.com/yuriysydor1991/ImagesAnnotator-DataDrivers.git)
library reads and writes, the very file the `Annotations` menu opens and stores
- and the nine others are the dataset layouts of the
[ImagesAnnotator-DataImporters](https://github.com/yuriysydor1991/ImagesAnnotator-DataImporters.git)
and the
[ImagesAnnotator-DataExporters](https://github.com/yuriysydor1991/ImagesAnnotator-DataExporters.git)
libraries, the same ones the import and the export submenus offer:

| `FORMAT` | The annotations it names |
| --- | --- |
| `imagesannotator` | the annotations project JSON file of the application |
| `plain-txt` | the plain text files of one line per rectangle |
| `yolo4` | the darknet YOLO v4 training directory |
| `ultralytics-detect` | the Ultralytics YOLO detection dataset |
| `ultralytics-obb` | the Ultralytics YOLO oriented bounding box dataset |
| `ultralytics-segment` | the Ultralytics YOLO instance segmentation dataset |
| `coco` | the COCO object detection JSON descriptor |
| `pascal-voc` | the Pascal VOC XML devkit |
| `createml` | the Create ML object detection JSON descriptor |
| `pytorch` | the PyTorch Vision `ImageFolder` layout |
| `dxxwjz2` | the darknetxx training directory: the YOLO v4 one with the dxxwjz2 network to train |
| `dxxwjz2p` | the darknetxx dxxwjz2p training project |
| `dxxwjz3` | the darknetxx training directory: the YOLO v4 one with the dxxwjz3 network to train |
| `dxxwjz3p` | the darknetxx dxxwjz3p training project |

For the `imagesannotator` format the path is the project JSON file itself, and
for every other one it is the directory holding the dataset - the source path of
the `dxxwjz2p` and the `dxxwjz3p` projects may name the project file itself as
well. A destination that
does not exist yet is created.

Writing the `pytorch` one cuts every annotated rectangle out of the picture it
was drawn over, so that one layout of the nine reads the pixels of the images
instead of their annotations alone. The cutters the graphical toolkits of this
application implement are not there for a conversion, which starts no interface
at all, so the cutting is done by the OpenCV image cropper the
[ImagesAnnotator-DataExporters](/doc/sections/en_US/5-37-enabling-the-ImagesAnnotator-DataExporters-library.md)
library ships itself. That cropper is what its `ENABLE_OPENCV` option builds,
and that option is `ON` by default, so a stock installation of the library
converts into this layout as it does into every other one. An installation
built with `-DENABLE_OPENCV=OFF`, or on a machine with no OpenCV development
packages, carries no cropper - and then this one destination fails with the
library saying so, while reading the layout back is unaffected either way.

Both halves of a conversion are the very ones the menu entries drive: the
source is read into the annotations project the application edits, and that
project is then written out in the destination format. So a conversion goes
through the same readers and writers the graphical interface uses and behaves
exactly as the matching pair of menu entries would, and, as an import there,
nothing is ever written into the path that is read.

The exit status is zero when both halves succeeded, and non zero otherwise -
for an unknown format name, an unreadable source, a destination that can not be
created, or an empty result.
