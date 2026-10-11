## Вмикання інтеграції бібліотеки ImagesAnnotator-DataImporters

Набір даних для навчання, який уже лежить на диску, читається назад до проекту анотацій - звичайні текстові файли, директорія навчання YOLO v4 (darknet), три розкладки Ultralytics YOLO, дескриптори JSON COCO та Create ML, XML Pascal VOC, розкладка PyTorch Vision `ImageFolder` із вирізаними прямокутниками і чотири розкладки [darknetxx](https://github.com/yuriysydor1991/darknetxx), його тренувальні проекти dxxwjz2p та dxxwjz3p і тренувальні директорії мереж dxxwjz2 та dxxwjz3, - окремою бібліотекою [ImagesAnnotator-DataImporters](https://github.com/yuriysydor1991/ImagesAnnotator-DataImporters.git). Вона є оберненням бібліотеки [ImagesAnnotator-DataExporters](/doc/sections/uk_UA/5-37-enabling-the-ImagesAnnotator-DataExporters-library.md): що та записує, ця читає, тож проект, експортований у будь-яку з тих тринадцяти розкладок, повертається з тими анотаціями, з якими починав, а набір даних, створений якимось іншим інструментом, уперше потрапляє до проекту.

Через це бібліотека є **обов'язковою**: CMake змінна `ENABLE_IMAGES_ANNOTATOR_DATA_IMPORTERS` за замовчуванням має значення `ON`, і проект не збирається, якщо її вимкнути.

```
# в середині кореневої директорії проекту

cmake -S . -B build -DENABLE_IMAGES_ANNOTATOR_DATA_IMPORTERS=ON
```

### Спочатку її треба встановити

На відміну від більшості вмикачів сторонніх бібліотек проекту, цей не має запасного варіанту із завантаженням через Інтернет - із тієї самої причини, з якої його не має вмикач експортерів: бібліотека споживається встановленою, так само як і бібліотека [ImagesAnnotator-DataDrivers](/doc/sections/uk_UA/5-36-enabling-the-ImagesAnnotator-DataDrivers-library.md), проти якої вона зібрана. Бібліотека імпортерів знаходить драйвери даних власним викликом `find_package()` та включає їхні заголовки через директорію з версією `<ImagesAnnotatorDataDrivers-0.12/...>`, яку створює лише встановлення.

Зберіть та встановіть обидві, спочатку драйвери даних:

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

Бібліотека лінкує також [zlib](https://zlib.net/), якою її розкладки darknetxx читають стиснуті gzip файли dxxwjz: встанови її пакунок розробки (`zlib1g-dev` у системах на базі Debian) перед її конфігуруванням або дозволь її вмикачу завантажити джерельні коди.

Спільний скрипт [misc/ci/install-annotator-libraries.sh](/misc/ci/install-annotator-libraries.sh) робить рівно це для всіх трьох бібліотек одразу, і це найшвидший спосіб підготувати машину для розробки.

### Як вказати проекту на встановлені копії

Та сама CMake змінна, через яку знаходяться дві споріднені бібліотеки, і вона приймає кожен префікс, коли вони різняться:

```
cmake -S . -B build -DCMAKE_PREFIX_PATH=/opt/images-annotator
```

Усі три спільні об'єкти мають бути досяжними для динамічного лінкера під час запуску застосунку. Або зареєструйте префікс (наприклад, додавши його до `/etc/ld.so.conf.d/` та виконавши `ldconfig`), або вкажіть його у середовищі:

```
LD_LIBRARY_PATH=/opt/images-annotator/lib ./ImagesAnnotator
```

### Що з цим роблять пакувальники

Бібліотека імпортерів так само не є пакунком дистрибутиву, тож пакувальники поводяться з нею точно так, як із двома іншими:

- пакунок [flatpak](/doc/sections/uk_UA/5-11-enabling-the-flatpak-package-generation-support.md), [snap](/doc/sections/uk_UA/5-19-enabling-the-snap-package-manager.md) та образ [docker](/doc/sections/uk_UA/5-12-enabling-the-docker-container-build-and-run.md) збирають усі три бібліотеки з їхніх сирців у створюваний артефакт, тож ці три лишаються самодостатніми. Репозиторій та ревізія походять із кеш-змінних `TEMPLATE_APP_IADI_GIT` / `TEMPLATE_APP_IADI_GIT_TAG` вмикача;
- пакунки [deb](/doc/sections/uk_UA/5-10-enabling-DEB-package-generation-with-cpack.md), [rpm](/doc/sections/uk_UA/5-22-enabling-RPM-package-generation-with-cpack.md), [FreeBSD pkg](/doc/sections/uk_UA/5-20-enabling-FreeBSD-pkg-package-generation-with-cpack.md) та [WiX MSI](/doc/sections/uk_UA/5-21-enabling-WIX-MSI-package-generation-with-cpack.md) несуть лише сам застосунок, тож цільова система потребує встановленого спільного об'єкта у спосіб, описаний у цій секції.

Три конвеєри CI встановлюють її тим самим скриптом [misc/ci/install-annotator-libraries.sh](/misc/ci/install-annotator-libraries.sh), змінну середовища `IADI_GIT_TAG` якого перевизначає конвеєр, що перевіряє невипущену ревізію бібліотеки.

### Перевизначення імені пакунка

Бібліотека несе мажорну та мінорну версії всередині імен встановлюваного пакунка, цілі та бінарника - `ImagesAnnotatorDataImporters-0.14`, що відповідає її публічному простору імен `ImagesAnnotatorDataImporters014` із версією, - тож дві мінорні версії встановлюються цілком паралельно. Ім'я, яке цей проект просить у `find_package()`, є кеш-змінною:

```
# бібліотека, зібрана з -DLIB_INCLUDE_MINOR_IN_NAME=OFF, тобто з іменем лише з мажорною версією

cmake -S . -B build -DTEMPLATE_APP_IADI_PACKAGE=ImagesAnnotatorDataImporters-0
```

### Що проект бере з бібліотеки

- `IImporter` та тринадцять нащадків `IADataImportersContext` - тринадцять імпортерів, які запускає `AnnotatorController`, коли GUI створює запит імпорту. Він будує їх через `IADataImportersFacade::create_importer()`, передаючи контекст потрібної розкладки.
- `IADataImportersContext` - конфігурація імпорту (директорія-джерело, база даних анотацій, до якої зливаються відновлені записи, та засіб вимірювання зображень), яку контролер заповнює через сеттери контексту. База даних, яку він передає, - це та сама, через яку редагується проект, тож імпорт доповнює відкритий проект, а не заміщує його, а злиття бібліотеки зберігає зображення, яке проект уже містить, - саме це й робить імпорт безпечним для повторення.
- `IImageSizeFacility` - засіб вимірювання зображень, потрібний сімом із тринадцяти імпортів. Бібліотека не декодує жодного формату зображень сама, а анотація формату проекту - це прямокутник у пікселях свого зображення, тож розкладка YOLO v4, дві тренувальні директорії darknetxx, які її розширюють, та три розкладки Ultralytics YOLO (чиї рамки зберігаються поділеними на той розмір) і PyTorch Vision (чия анотація і є обрізаним зображенням) просять застосунок виміряти картинки. Дескриптори COCO та Pascal VOC і два тренувальні проекти darknetxx несуть розміри зображень самі, а розкладки простого тексту та Create ML тримають свої прямокутники у власних пікселях зображення, тож ці шість читаються хоч би що надав застосунок - і кожен запит імпорту несе читача однаково, бо саме він заповнює розміри зображення відновленого запису. Кожен набір інструментів GUI цього проекту реалізує той інтерфейс над графічним стеком, який він уже лінкує: `GtkmmImageSizeProvider`, `QtImageSizeProvider` та `WxImageSizeProvider` - відповідники трьох обрізачів зображень з боку експорту. Заголовок проекту `src/annotator-events/events/IImageSizeFacilityProvider.h` є псевдонімом на нього.

Єдине місце, яке називає бібліотеку, - це `src/annotator-events/events/DataImportersAliases.h`, який оголошує псевдонім `iadi` простору імен з версією `ImagesAnnotatorDataImporters014`. Її заголовки включаються через директорію з версією `<ImagesAnnotatorDataImporters-0.14/...>`: префікс закріплює версію бібліотеки, з якою розмовляє кожна одиниця трансляції, а `IImporter.h` поруч із ним достатньо загальний, щоб зіткнутися у насиченому шляху підключення.
