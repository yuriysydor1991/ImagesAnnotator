## Вмикання інтеграції бібліотеки ImagesAnnotator-DataDrivers

Файли проекту анотацій - тобто JSON документи, що містять анотовані зображення, їх розміри та іменовані прямокутники, намальовані на них - читаються та записуються окремою бібліотекою [ImagesAnnotator-DataDrivers](https://github.com/yuriysydor1991/ImagesAnnotator-DataDrivers.git). Розбір, серіалізація, об'єкти-значення записів зображень та правила злиття з урахуванням дублікатів раніше знаходились у цьому репозиторії та були винесені до тієї бібліотеки, щоб застосунок та будь-який інший інструмент могли користуватись одним і тим самим форматом файлів проекту.

Через це бібліотека є **обов'язковою**: CMake змінна `ENABLE_IMAGES_ANNOTATOR_DATA_DRIVERS` за замовчуванням має значення `ON`, і проект не збирається, якщо її вимкнути.

```
# в середині кореневої директорії проекту

cmake -S . -B build -DENABLE_IMAGES_ANNOTATOR_DATA_DRIVERS=ON
```

Вмикач шукає бібліотеку за допомогою `find_package()`. Запасного варіанту із завантаженням через Інтернет немає: сусідня бібліотека [ImagesAnnotator-DataExporters](/doc/sections/uk_UA/5-37-enabling-the-ImagesAnnotator-DataExporters-library.md), проти якої цей проект теж зібрано, знаходить драйвери даних власним викликом `find_package()` та включає їхні заголовки через директорію з версією `<ImagesAnnotatorDataDrivers-0.11/...>` - таку форму створює лише встановлення, - тому копія, завантажена у цю збірку, не змогла б їй прислужитись. Обидві бібліотеки споживаються встановленими.

### Вказування шляху до встановленої копії

```
git clone https://github.com/yuriysydor1991/ImagesAnnotator-DataDrivers.git
cmake -S ImagesAnnotator-DataDrivers -B build-iadd -DCMAKE_INSTALL_PREFIX=/opt/images-annotator
cmake --build build-iadd -j$(nproc)
cmake --install build-iadd
```

Бібліотеку, встановлену поза типовими шляхами пошуку, можна знайти за допомогою звичайної CMake змінної:

```
cmake -S . -B build -DCMAKE_PREFIX_PATH=/opt/images-annotator
```

Сценарій [misc/ci/install-annotator-libraries.sh](/misc/ci/install-annotator-libraries.sh) виконує саме ці кроки одразу для обох бібліотек, і саме ним їх збирають три конвеєри CI та маніфести пакувальників.

### Заміна репозиторію та тегу вихідних кодів

Ніщо у цій збірці не завантажує бібліотеку, але маніфести пакувальників - flatpak, snap та образ docker - збирають її у власному пісочниці та беруть репозиторій із цих двох змінних кешу:

```
cmake -S . -B build \
  -DTEMPLATE_APP_IADD_GIT=https://github.com/yuriysydor1991/ImagesAnnotator-DataDrivers.git \
  -DTEMPLATE_APP_IADD_GIT_TAG=main
```

### Заміна назви пакунка

Бібліотека містить свою основну (major) та проміжну (minor) версії у назвах встановленого пакунка, цілі та двійкового файлу - `ImagesAnnotatorDataDrivers-0.11`, що відповідає її публічному простору імен `ImagesAnnotatorDataDrivers011`, - тому дві проміжні версії встановлюються повністю паралельно. Її власні опції `LIB_INCLUDE_MINOR_IN_NAME` (за замовчуванням `ON`), `LIB_INCLUDE_MICRO_IN_NAME` та `LIB_NAME_SUFFIX` можуть скоротити або розширити цю назву далі. Назва, яку цей проект передає до `find_package()`, є змінною кешу, тому інакше названа збірка тієї самої бібліотеки залишається однією опцією командного рядка:

```
# бібліотека, зібрана з -DLIB_INCLUDE_MINOR_IN_NAME=OFF, тобто лише з основною версією

cmake -S . -B build -DTEMPLATE_APP_IADD_PACKAGE=ImagesAnnotatorDataDrivers-0
```

### Що проект бере з бібліотеки

- `ImageRecord` та `ImageRecordRect` - об'єкти-значення записів, які передаються по всьому проекту, включно з графічним інтерфейсом. Заголовки з такими назвами у `src/annotator-events/events/` тепер є псевдонімами типів бібліотеки.
- `IAnnotationsDB` - база даних проекту анотацій, яку тримає `AnnotatorController` та створює через `LibraryFacade::create_annotations_db()`.
- `IImagesPathsDBProvider` - інтерфейс доступу до записів тільки для читання, який споживає графічний інтерфейс та реалізує `AnnotatorController`.
- `IRecordsFactory` - фабрика записів, до якої `EventsFactory` проекту делегує свої методи створення записів.

Єдине місце, що називає бібліотеку, - це `src/annotator-events/events/DataDriversAliases.h`, у якому оголошено псевдонім `iadd` для простору імен `ImagesAnnotatorDataDrivers011`, що містить номер версії. Її заголовки включаються через директорію з версією `<ImagesAnnotatorDataDrivers-0.11/...>`: бібліотека експортерів даних встановлює власний публічний `LibraryFacade.h`, тому проста форма назвала б той із них, який компілятор побачить першим.
