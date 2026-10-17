## Вмикання підтримки генерації flatpak пакету

Для того щоб увімкнути підтримку генерації пакету постачання flatpak на даному проекті необхідно сконфігурувати систему побудови з увімкненим параметром `ENABLE_FLATPAK` (для GNU/Linux подібних ОС):

```
# всередині кореневої директорії проекту 

cmake -B build -S . -DENABLE_FLATPAK=ON
```

Успішно виконана команда у свою чергу увімкне ціль для побудови під назвою `flatpak`.

На кінець, щоб згенерувати пакет розповсюдження flatpak необхідно побудувати ціль `flatpak`:

```
# всередині кореневої директорії проекту

cmake --build build --target flatpak
```

У разі успішної побудови цілі у кореневій директорії побудови проекту буде створено файл, названий за бінарним файлом, який запускає flatpak, версією проекту, архітектурою центрального процесора, назвою та версією цільової операційної системи і SDK, яким зібрано пакет, наприклад `ImagesAnnotator-0.13.0-x86_64-ubuntu-24.04-org.gnome.Sdk-48.flatpak`. Архітектура - це та, яку виводить команда `flatpak --default-arch` (наприклад `x86_64` чи `aarch64`). Назва та версія цільової системи слідують за нею так само, як за архітектурою у назві кожного іншого пакету проекту, - це кеш-змінна `TEMPLATE_PROJECT_PACKAGE_SYSTEM`, описана у секції [Вмикання підтримки генерування DEB-пакетів з cpack](/doc/sections/uk_UA/5-10-enabling-DEB-package-generation-with-cpack.md), - а завершує назву SDK, як його називають члени `sdk` та `runtime-version` маніфесту flatpak, адже одну й ту саму версію проекту на одній і тій самій системі можна запакувати у flatpak для кожного графічного інструментарію (див. нижче). Для того щоб преглянути або/і змінити ціль побудови пакету flatpak необхідно відвідати файл [cmake/enablers/packagers/template-project-flatpak-target.cmake](/cmake/enablers/packagers/template-project-flatpak-target.cmake) або маніфести директорії [misc/packagers/flatpak](/misc/packagers/flatpak).

### Маніфест для кожного графічного інструментарію

Кожен графічний інструментарій збирається на власному середовищі виконання з власними модулями, тож кожен має власний маніфест flatpak, і конфігурування обирає маніфест того інструментарію, який воно збирає (див. [Розширення](/doc/sections/uk_UA/4-5-extensions.md)):

| Графічний інструментарій | Параметр конфігурування | Маніфест | Середовище виконання та SDK | Збирається у пісочниці |
| --- | --- | --- | --- | --- |
| GTKmm3 (типовий) | `-DUSE_GTKMM3=ON` | [flatpak-gtkmm3.conf.json.in](/misc/packagers/flatpak/flatpak-gtkmm3.conf.json.in) | `org.gnome.Platform` та `org.gnome.Sdk` 48 | graphviz, mm-common, GTKmm 3.24.8 |
| GTKmm4 | `-DUSE_GTKMM4=ON` | [flatpak-gtkmm4.conf.json.in](/misc/packagers/flatpak/flatpak-gtkmm4.conf.json.in) | `org.gnome.Platform` та `org.gnome.Sdk` 49 | libsigc++ 3.6, glibmm 2.86, cairomm 1.18, pangomm 2.56, GTKmm 4.20 |
| Qt6 | `-DUSE_QT6=ON` | [flatpak-qt6.conf.json.in](/misc/packagers/flatpak/flatpak-qt6.conf.json.in) | `org.kde.Platform` та `org.kde.Sdk` 6.10 | нічого, Qt6 входить до середовища виконання |
| wxWidgets | `-DUSE_WXWIDGETS=ON` | [flatpak-wxwidgets.conf.json.in](/misc/packagers/flatpak/flatpak-wxwidgets.conf.json.in) | `org.gnome.Platform` та `org.gnome.Sdk` 49 | wxWidgets 3.2.9 |
| жодного, збірка лише конвертера | `-DBUILD_IMAGES_ANNOTATOR_CONVERTER_ONLY=ON` | [flatpak-converter.conf.json.in](/misc/packagers/flatpak/flatpak-converter.conf.json.in) | `org.freedesktop.Platform` та `org.freedesktop.Sdk` 24.08 | нічого |

Кожен маніфест додатково збирає бібліотеку nlohmann json, модулі core, imgproc та imgcodecs бібліотеки OpenCV і три бібліотеки ImagesAnnotator та конфігурує проект з параметром свого рядка. OpenCV дає бібліотекам експортерів та імпортерів власні засоби вирізання зображень і визначення їх розміру, які бінарному файлу конвертера нема чим замінити, адже графічного інструментарію він не має (див. [Виконуваний файл ImagesAnnotatorConverter](/doc/sections/uk_UA/5-40-the-ImagesAnnotatorConverter-binary.md)).

Ціль `flatpak` сама встановлює з flathub середовище виконання та SDK, які називає обраний маніфест (параметр `--install-deps-from=flathub` команди `flatpak-builder`), і додає віддалене сховище flathub до встановлення користувача, якщо його там немає, тож зміна інструментарію не потребує жодної іншої підготовки. Див. [Необов'язковий програми для генерації пакету flatpak](/doc/sections/uk_UA/3-9-optional-for-the-flatpak-packager.md).

Щоб запакувати проект власним маніфестом, передай його кеш-змінною `FLATPAK_CONF_SRC` (наприклад `-DFLATPAK_CONF_SRC=/path/to/manifest.json.in`): він конфігурується так само, а його члени `sdk` та `runtime-version` називають пакет. Якщо змінна порожня, як типово, маніфест слідує за обраним інструментарієм, зокрема й під час переконфігурування, яке змінює інструментарій.

### Встановлення та запуск

Необхідно звернутися до документації програми flatpak для того щоб дізнитись як встановлювати або запускати програми котрі розповсюджуються у вигляді пакету flatpak. Для не змінених паарметрів поточного проекту вони можуть виглядати наступним чином:

```
# для встановлення (версія може змінитись)
# всередині директорії побудови проекту 
flatpak install --user ImagesAnnotator-0.13.0-x86_64-ubuntu-24.04-org.gnome.Sdk-48.flatpak

# для запуску встановленої програми
flatpak run ua.org.kytok.template.gtkmm3.ImagesAnnotator
```

Ідентифікатор програми містить графічний інструментарій (`gtkmm3`, `gtkmm4`, `qt6` чи `wxwidgets`), тож flatpak пакети різних інструментаріїв встановлюються поруч.

### Кожен бінарний файл пакету flatpak

Пакет flatpak запускає графічний бінарний файл, але містить кожен бінарний файл, який збирає конфігурування, - зокрема й `ImagesAnnotatorConverter`, якщо його не вимкнено параметром `-DENABLE_IMAGES_ANNOTATOR_CONVERTER=OFF`. Кожен бінарний файл має власний ідентифікатор програми:

| Бінарний файл | Ідентифікатор програми |
| --- | --- |
| `ImagesAnnotator` | `ua.org.kytok.template.<інструментарій>.ImagesAnnotator` |
| `ImagesAnnotatorConverter` | `ua.org.kytok.template.<інструментарій>.ImagesAnnotator.Converter` |

Перший з них - ідентифікатор самого пакету flatpak. Конвертер зареєстровано під другим власним ярликом запуску - файлом `<ідентифікатор>.desktop`, який проект встановлює поруч з ярликом програми, який запускає конвертер у терміналі і прихований з меню програм, - і flatpak експортує цей ярлик, спрямувавши його на конвертер, так само як пакет flatpak LibreOffice реєструє свій Writer під ідентифікатором `org.libreoffice.LibreOffice.writer`. Будь-який бінарний файл, окрім власної команди пакету, flatpak запускає параметром `--command` команди `flatpak run`:

```
flatpak run --command=ImagesAnnotatorConverter --filesystem=home \
  ua.org.kytok.template.gtkmm3.ImagesAnnotator \
  --convert-from yolo4 \
  --convert-src ~/the-existing-dataset \
  --convert-to coco \
  --convert-dst ~/the-wanted-dataset
```

Параметр `--filesystem=home` надає конвертеру домашню директорію на цей один запуск: графічна програма дістається файлів хоста через портал вибору файлів, який програмі командного рядка ні до чого, а пісочниця, яку оголошує маніфест, не відкриває їй жодної директорії хоста.

Збірка лише конвертера (`-DBUILD_IMAGES_ANNOTATOR_CONVERTER_ONLY=ON`) пакує сам конвертер останнім маніфестом таблиці вище у названий за ним файл `ImagesAnnotatorConverter-0.13.0-x86_64-ubuntu-24.04-org.freedesktop.Sdk-24.08.flatpak`. Ідентифікатор конвертера є ідентифікатором цього пакету flatpak, а його маніфест сам надає йому домашню директорію:

```
flatpak install --user ImagesAnnotatorConverter-0.13.0-x86_64-ubuntu-24.04-org.freedesktop.Sdk-24.08.flatpak

flatpak run ua.org.kytok.template.gtkmm3.ImagesAnnotator.Converter \
  --convert-from yolo4 \
  --convert-src ~/the-existing-dataset \
  --convert-to coco \
  --convert-dst ~/the-wanted-dataset
```

Частина інструментарію у цьому ідентифікаторі - та, яку визначає конфігурування, `gtkmm3`, якщо не запитано іншого інструментарію, хоча збірка лише конвертера жодного не збирає.
