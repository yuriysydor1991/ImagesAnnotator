## Вмикання пакувальника snap

Для того щоб увімкнути для побудови ціль `snap`, що призначена для генерування [SNAP](https://snapcraft.io/docs/) бінарного дистрибуційного пакунку проекту, необхідно сконфігурувати CMake проект з увімкненою змінною `ENABLE_SNAP`:

```
cmake -S . -B build -DENABLE_SNAP=ON
```

Далі необхідно побудувати ціль `snap`, яка виконує команду `snapcraft pack` у директорії побудови:

```
cmake --build build --target snap
```

Скрипт [scripts/build/build-snap.sh](/scripts/build/build-snap.sh) виконує обидва кроки одразу (дивись [Швидкі скрипти побудови](/doc/sections/uk_UA/5-38-quick-build-scripts.md)). Програму `snapcraft` і менеджер контейнерів, у якому вона збирає пакунок, описано у секції [Необов'язкові пакети для snap пакувальника](/doc/sections/uk_UA/3-11-optional-for-snap-packager.md).

Програма snapcraft називає пакунок `<назва snap>_<версія>_<архітектура>.snap`, де архітектура - це назва Debian центрального процесора, для якого його зібрано (наприклад `amd64` чи `arm64`), а ціль додає до неї назву та версію цільової операційної системи так само, як вони слідують за архітектурою у назві кожного іншого пакунку проекту, - це кеш-змінна `TEMPLATE_PROJECT_PACKAGE_SYSTEM`, описана у секції [Вмикання підтримки генерування DEB-пакетів з cpack](/doc/sections/uk_UA/5-10-enabling-DEB-package-generation-with-cpack.md). Тож із версією проекту `0.13.0` на системі `amd64` з Ubuntu 24.04 пакунок називається `imagesannotator_0.13.0_amd64-ubuntu-24.04.snap`.

### Файл snapcraft.yaml для кожного графічного інструментарію

Як і flatpak, snap збирає проект з його джерельних кодів, тож кожен графічний інструментарій має власний файл `snapcraft.yaml`, який встановлює пакунки, з якими інструментарій збирається, і додає до snap ті, з якими він запускається, а конфігурування обирає файл того інструментарію, який воно збирає (дивись [Розширення](/doc/sections/uk_UA/4-5-extensions.md)):

| Графічний інструментарій | Параметр конфігурування | snapcraft.yaml |
| --- | --- | --- |
| GTKmm3 (типовий) | `-DUSE_GTKMM3=ON` | [snapcraft-gtkmm3.yaml.in](/misc/packagers/snapcraft/snapcraft-gtkmm3.yaml.in) |
| GTKmm4 | `-DUSE_GTKMM4=ON` | [snapcraft-gtkmm4.yaml.in](/misc/packagers/snapcraft/snapcraft-gtkmm4.yaml.in) |
| Qt6 | `-DUSE_QT6=ON` | [snapcraft-qt6.yaml.in](/misc/packagers/snapcraft/snapcraft-qt6.yaml.in) |
| wxWidgets | `-DUSE_WXWIDGETS=ON` | [snapcraft-wxwidgets.yaml.in](/misc/packagers/snapcraft/snapcraft-wxwidgets.yaml.in) |
| жодного, збірка лише конвертера | `-DBUILD_IMAGES_ANNOTATOR_CONVERTER_ONLY=ON` | [snapcraft-converter.yaml.in](/misc/packagers/snapcraft/snapcraft-converter.yaml.in) |

Усі вони збираються на базі `core24` (пакунки Ubuntu 24.04) і перед проектом збирають три бібліотеки ImagesAnnotator, бібліотеки експортерів та імпортерів - з їхнім OpenCV, що дає їм засоби вирізання зображень і визначення їх розміру, які бінарному файлу конвертера нема чим замінити, адже графічного інструментарію він не має (дивись [Виконуваний файл ImagesAnnotatorConverter](/doc/sections/uk_UA/5-40-the-ImagesAnnotatorConverter-binary.md)). Щоб запакувати проект власним файлом `snapcraft.yaml`, передай його кеш-змінною `SNAPCRAFT_CONF_SRC`; якщо вона порожня, як типово, файл слідує за обраним інструментарієм.

### Кожен бінарний файл пакунку snap

Пакунок snap містить застосунок для кожного бінарного файлу, який збирає конфігурування, і кожен з них запускається власною командою:

```
sudo snap install --dangerous imagesannotator_0.13.0_amd64-ubuntu-24.04.snap

# графічна програма
imagesannotator.ImagesAnnotator

# конвертер
imagesannotator.ImagesAnnotatorConverter \
  --convert-from yolo4 \
  --convert-src ~/the-existing-dataset \
  --convert-to coco \
  --convert-dst ~/the-wanted-dataset
```

Графічний застосунок отримує файл робочого столу програми та інтерфейси, потрібні вікну, конвертер - лише домашню директорію. Збірка лише конвертера пакує названий за конвертером snap `imagesannotatorconverter`, єдиним застосунком якого є `imagesannotatorconverter.ImagesAnnotatorConverter`. Команда `sudo snap alias imagesannotator.ImagesAnnotatorConverter ImagesAnnotatorConverter` дає застосунку просту назву його бінарного файлу.

Детальніше у файлі [cmake/enablers/packagers/template-project-snap-enabler.cmake](/cmake/enablers/packagers/template-project-snap-enabler.cmake) про ціль `snap`.
