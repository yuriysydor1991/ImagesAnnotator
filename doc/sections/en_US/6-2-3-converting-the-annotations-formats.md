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

For the `imagesannotator` format the path is the project JSON file itself, and
for every other one it is the directory holding the dataset. A destination that
does not exist yet is created.

The `pytorch` one is a source only. Writing that layout cuts every annotated
rectangle out of the picture it was drawn over, and the cropping is done by the
imaging stack of the graphical toolkit the application was built against, which
a conversion never starts. Reading it back is unaffected and works as any other
source.

Both halves of a conversion are the very ones the menu entries drive: the
source is read into the annotations project the application edits, and that
project is then written out in the destination format. So a conversion goes
through the same readers and writers the graphical interface uses and behaves
exactly as the matching pair of menu entries would, and, as an import there,
nothing is ever written into the path that is read.

The exit status is zero when both halves succeeded, and non zero otherwise -
for an unknown format name, an unreadable source, a destination that can not be
created, or an empty result.
