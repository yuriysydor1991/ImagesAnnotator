## Вмикання інтеграції бібліотеки ImagesAnnotator-DataExporters

Анотовані зображення записуються у вигляді набору даних для навчання - звичайні текстові файли, директорія навчання YOLO v4 (darknet) та розкладка PyTorch Vision `ImageFolder` із вирізаними прямокутниками - окремою бібліотекою [ImagesAnnotator-DataExporters](https://github.com/yuriysydor1991/ImagesAnnotator-DataExporters.git). Ці три експортери раніше знаходились у цьому репозиторії та були винесені до тієї бібліотеки, щоб застосунок та будь-який інший інструмент могли створювати один і той самий набір даних з одного й того самого файлу проекту анотацій.

Через це бібліотека є **обов'язковою**: CMake змінна `ENABLE_IMAGES_ANNOTATOR_DATA_EXPORTERS` за замовчуванням має значення `ON`, і проект не збирається, якщо її вимкнути.

```
# в середині кореневої директорії проекту

cmake -S . -B build -DENABLE_IMAGES_ANNOTATOR_DATA_EXPORTERS=ON
```

### Спочатку її треба встановити

На відміну від більшості вмикачів сторонніх бібліотек проекту, цей не має запасного варіанту із завантаженням через Інтернет: бібліотека споживається встановленою, так само як і бібліотека [ImagesAnnotator-DataDrivers](/doc/sections/uk_UA/5-36-enabling-the-ImagesAnnotator-DataDrivers-library.md), проти якої вона зібрана. Бібліотека експортерів знаходить драйвери даних власним викликом `find_package()` та включає їхні заголовки через директорію з версією `<ImagesAnnotatorDataDrivers-0.11/...>`, яку створює лише встановлення, тому копію, підключену як підпроект, взагалі не вдалось би сконфігурувати.

Зберіть та встановіть обидві, спочатку драйвери даних:

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

### Вказування шляху до встановлених копій

Бібліотеки, встановлені поза типовими шляхами пошуку, знаходяться за допомогою звичайної CMake змінної, яка приймає обидва префікси, якщо вони різні:

```
cmake -S . -B build -DCMAKE_PREFIX_PATH=/opt/images-annotator
```

Обидва спільні об'єкти мають бути доступними динамічному лінкеру під час запуску застосунку. Або зареєструйте префікс (наприклад, додавши його до `/etc/ld.so.conf.d/` та виконавши `ldconfig`), або вкажіть його у змінній середовища:

```
LD_LIBRARY_PATH=/opt/images-annotator/lib ./ImagesAnnotator
```

### Що з цим роблять пакувальники

Жодна з бібліотек не є пакунком дистрибутиву, тому пакувальники проекту дають різні гарантії:

- пакунок [flatpak](/doc/sections/uk_UA/5-11-enabling-the-flatpak-package-generation-support.md), [snap](/doc/sections/uk_UA/5-19-enabling-the-snap-package-manager.md) та образ [docker](/doc/sections/uk_UA/5-12-enabling-the-docker-container-build-and-run.md) збирають обидві бібліотеки з вихідних кодів усередину створеного артефакту, тому ці три залишаються самодостатніми. Репозиторії та ревізії, які вони збирають, беруться зі змінних кешу `TEMPLATE_APP_IADD_GIT` / `TEMPLATE_APP_IADD_GIT_TAG` та `TEMPLATE_APP_IADE_GIT` / `TEMPLATE_APP_IADE_GIT_TAG` двох вмикачів;
- пакунки [deb](/doc/sections/uk_UA/5-10-enabling-DEB-package-generation-with-cpack.md), [rpm](/doc/sections/uk_UA/5-22-enabling-RPM-package-generation-with-cpack.md), [FreeBSD pkg](/doc/sections/uk_UA/5-20-enabling-FreeBSD-pkg-package-generation-with-cpack.md) та [WiX MSI](/doc/sections/uk_UA/5-21-enabling-WIX-MSI-package-generation-with-cpack.md) містять лише сам застосунок. Їхні списки залежностей називають пакунки дистрибутиву, а для цих двох бібліотек такого пакунка немає, тому на цільовій системі обидва спільні об'єкти мають бути встановлені так, як описано у цьому розділі.

Три конвеєри CI - [misc/Jenkinsfile](/misc/Jenkinsfile), [.github/workflows/ci.yml](/.github/workflows/ci.yml) та [.gitlab/ci](/.gitlab/ci) - встановлюють їх спільним сценарієм [misc/ci/install-annotator-libraries.sh](/misc/ci/install-annotator-libraries.sh) перед будь-яким конфігуруванням. Цей самий сценарій - найшвидший спосіб підготувати машину для розробки, а його змінні середовища `IADD_GIT_TAG` / `IADE_GIT_TAG` є тим, що перевизначає конвеєр, який перевіряє ще не випущену ревізію бібліотеки.

### Заміна назви пакунка

Бібліотека містить свою основну (major) та проміжну (minor) версії у назвах встановленого пакунка, цілі та двійкового файлу - `ImagesAnnotatorDataExporters-0.11`, що відповідає її публічному простору імен `ImagesAnnotatorDataExporters011`, - тому дві проміжні версії встановлюються повністю паралельно. Її власні опції `LIB_INCLUDE_MINOR_IN_NAME` (за замовчуванням `ON`), `LIB_INCLUDE_MICRO_IN_NAME` та `LIB_NAME_SUFFIX` можуть скоротити або розширити цю назву далі. Назва, яку цей проект передає до `find_package()`, є змінною кешу, тому інакше названа збірка тієї самої бібліотеки залишається однією опцією командного рядка:

```
# бібліотека, зібрана з -DLIB_INCLUDE_MINOR_IN_NAME=OFF, тобто лише з основною версією

cmake -S . -B build -DTEMPLATE_APP_IADE_PACKAGE=ImagesAnnotatorDataExporters-0
```

### Що проект бере з бібліотеки

- `IExporter` та перелічення `ExportFormat` - три експортери, які запускає `AnnotatorController`, коли графічний інтерфейс надсилає запит на експорт. Він створює їх через `LibraryFacade::create_exporter()`, а не через власні класи експортерів.
- `ExportContext` - конфігурація експорту (директорія призначення, база даних анотацій для читання та необов'язковий обрізач зображень), яку заповнює контролер.
- `IImageCropperFacility` - служба обрізання зображень, потрібна експорту до PyTorch Vision. Бібліотека сама не декодує жодного формату зображень, тому кожен графічний набір інструментів цього проекту реалізує цей інтерфейс засобами тієї бібліотеки зображень, яку вже лінкує: `GtkmmImageCropperProvider`, `QtImageCropperProvider` та `WxImageCropperProvider`. Заголовок проекту `src/annotator-events/events/IImageCropperFacilityProvider.h` тепер є псевдонімом для нього.

Єдине місце, що називає бібліотеку, - це `src/annotator-events/events/DataExportersAliases.h`, у якому оголошено псевдонім `iade` для простору імен `ImagesAnnotatorDataExporters011`, що містить номер версії. Її заголовки включаються через директорію з версією `<ImagesAnnotatorDataExporters-0.11/...>`: обидві бібліотеки встановлюють публічний `LibraryFacade.h`, тому проста форма назвала б той із них, який компілятор побачить першим.
