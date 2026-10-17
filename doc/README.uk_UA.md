**Програма для розмітки і коментування зображень ImagesAnnotator**

# Ціль проекту коментатора зображень

Ціль проекту коментатора зображень являється розмітка і надання інформації про зміст обраних зображень для її постачання для тренування нейромереж або ШІ з наступним завантаженням натренованої мережі у бібліотеки комп'ютерного бачення на подобі [OpenCV](https://github.com/opencv/opencv).

Окрім своєї основної цілі проект виконує також дослідницькі задачі. Отож може виглядати більш ускладненим ніж зазвичай.

Реалізація проекту значно пришвидшена завдяки C++ проекту-шаблону розміщеного за адресою [https://github.com/yuriysydor1991/cpp-app-template](https://github.com/yuriysydor1991/cpp-app-template)

**УВАГА! Поточний проект анотатора зображень знаходиться у альфа стані! Тому необхідно часто зберігати дані і робити резервні копії даних!**

Більше за посиланням [kytok.org.ua](http://www.kytok.org.ua/)

💵 Підтримай проект за посиланням [http://kytok.org.ua/page/pozertvy](http://kytok.org.ua/page/pozertvy)

# Використання

## Головне вікно

Основне вікно програми після її запуску може виглядати наступним чином.

![ImagesAnnotator window view example](images/ImagesAnnotator-0.11.0.png)

Зверху вікна розміщене меню:

- `Images` для імпортування і додавання директорії з зображеннями до поточного проекту,

- `Annotations` для управління поточним проектом,

- `Help` для допомоги.

Наступними у рядок розміщені кнопки для відкривання і додавання нової директорії з зображеннями до поточного проекту (дубльований з меню `Images`), кнопки для збільшення або зменшення поточного обраного зображення на центральній робочій області, далі розміщені кнопки наступного чи попереднього зображення з списку відкрити зображень на лівій панелі списку, і на кінець продубльована кнопка для відкриття існуючого проекту бази анотацій - файлу проекту анотацій застосунку, або звичайного JSON-файлу, або стисненого gzip файлу imgajz1 (дивіться [Файл проекту анотацій](#файл-проекту-анотацій) нижче).

Зображення що позначені зеленим кольором на лівій панелі вказують що у кожному з них присутня хоча-б одна анотація.

На правій панелі присутній список усіх анотацій чи розмітки зображень у поточному проекті. Під ним розміщений список анотацій для поточного обраного зображення з центральної робочої області. Кнопка `Copy annotation name` призначена для копіювання тексту з обраної анотації зі списку усіх анотацій у обрану анотацію поточного зображення. Кнопка `copy annotation` дублює обрану анотацію поточного зображення. Кнопка видалення знизу списку анотацій поточного зображення видаляє обрану анотацію поточного обраного зображення відповідно.

Внизу вікна розміщена панель статусу котра на даний момент висвітлює тільки поточний шлях до відкритого файлу проекту.

## Експорт наборів даних

Після внесення анотацій до обраних зображень можна скористатись субменю експорту у меню `Annotations` що дозволяє експортувати зображення у широко відомі формати анотування. Записуються тринадцять розкладок наборів даних, і субменю імпорту поруч читає назад кожну з них:

- **Звичайний текст** - по одному файлу `<імʼя-анотації>.txt` на кожне імʼя анотації, де кожен рядок називає зображення і його прямокутники;

- **YOLO v4 (darknet)** - ціла тренувальна директорія darknet для детектора: `data/obj.names`, `data/obj.data`, опис мережі `cfg/yolov4-obj.cfg`, записаний під класи проекту, скопійовані зображення з їхніми нормалізованими файлами міток `.txt`, переліки `train.txt` і `val.txt` та порожня `backup/`;

- **Ultralytics YOLO - детекція** - розкладка, яку читає кожен випуск Ultralytics: єдиний дескриптор `data.yaml` над директоріями `images/train` та `labels/train`, один рядок `клас центр-x центр-y ширина висота` на прямокутник;

- **Ultralytics YOLO - орієнтовані обмежувальні рамки** - та сама директорія і той самий дескриптор, один рядок із чотирьох кутів рамки на прямокутник;

- **Ultralytics YOLO - сегментація** - та сама директорія і той самий дескриптор, один рядок-багатокутник на прямокутник;

- **COCO JSON** - скопійовані зображення і єдиний дескриптор `annotations/instances_default.json` над ними, той, який читають Detectron2, MMDetection, torchvision, CVAT, FiftyOne, Label Studio і Roboflow;

- **Pascal VOC XML** - скопійовані зображення під `JPEGImages/`, по одному дескриптору `Annotations/<основа>.xml` на зображення над ними і переліки `ImageSets/Main`, що їх називають, - ті самі файли, які записує і читає [LabelImg](https://github.com/HumanSignal/labelImg);

- **Create ML JSON** - скопійовані зображення і єдиний дескриптор `annotations.json` поруч із ними в одній пласкій директорії, тека, яку бере Create ML від Apple, щоб натренувати детектор Core ML;

- **DarknetXX** - субменю одразу над пунктом YOLO v4 із чотирма розкладками, які [darknetxx](https://github.com/yuriysydor1991/darknetxx) тренує з нуля, кожна - стиснутий gzip JSON-документ darknetxx, що несе мережу YOLO v4 проекту та імена її класів, без ваг (файл dxxwjz1 з darknetxx містить самі лише ваги, тож пункту для нього немає):
  - **dxxwjz3p project** - файл проекту `yolov4-obj.dxxwjz3p` поруч із директорією `images/` скопійованих зображень та порожньою `backup/`: цілий тренувальний проект лише з JSON-даних - опцій cfg мережі, імен класів та анотацій зображень, - який тренують `darknetxx --train-cfg yolov4-obj.dxxwjz3p` та `darknetxx-train-gui`, оновлюючи проект вагами;
  - **dxxwjz2p project** - той самий проект у файлі `yolov4-obj.dxxwjz2p`, який містить cfg мережі як текст;
  - **dxxwjz3 with YOLO4 folder** - директорія YOLO v4 вище з мережею для тренування `cfg/yolov4-obj.dxxwjz3` поруч із її cfg, яку `darknetxx --train-cfg cfg/yolov4-obj.dxxwjz3` тренує без жодного іншого вказаного файлу;
  - **dxxwjz2 with YOLO4 folder** - те саме з мережею для тренування `cfg/yolov4-obj.dxxwjz2`, з текстом cfg;

- **PyTorch Vision `ImageFolder`** - одна директорія на імʼя анотації, що містить зображення, обрізані до прямокутників із тим іменем.

## Імпорт наборів даних

Набір даних, який уже лежить на диску, рухається у зворотному напрямку: субменю імпорту поруч із експортом читає ті розкладки назад до проекту, який редагується. Його субменю `DarknetXX`, розташоване одразу над пунктом YOLO v4 так само, як і субменю експорту, читає чотири розкладки darknetxx: пункти `dxxwjz3p project` та `dxxwjz2p project` відкривають файл проекту - той, що його записує експорт, або той, який тренування darknetxx оновило своїми вагами, - і читають анотації його зображень, а пункти `dxxwjz3 with YOLO4 folder` та `dxxwjz2 with YOLO4 folder` читають директорію YOLO v4 з іменами класів її мережі, коли в ній немає файлу `data/obj.names`. Проект натренованої мережі несе сотні мегабайтів ваг, які імпорт переступає частина за частиною замість того, щоб завантажувати їх. Відновлені зображення додаються до вже відкритих - зображення, яке проект уже містить, лишається як було, - тож імпорт можна повторювати, і він нічого не записує до директорії, яку читає.

## Конвертація наборів даних без вікна

Ні експорт, ні імпорт не потребують вікна. За наявності флагів `--convert-from`, `--convert-src`, `--convert-to` та `--convert-dst` виконуваний файл читає набір даних одного формату та записує його назад у іншому форматі без запуску графічного інтерфейсу, повідомляючи про підготовку та про конвертацію лише повідомленнями журналу, - тому конвертація набору даних придатна для скрипта та для конвеєра побудови. Проект анотацій самого застосунку є одним з форматів, поряд з тринадцятьма форматами наборів даних із субменю імпорту, - файл проекту imgajz1 для шляху з розширенням імені файлу `.imgajz1`, JSON-файл для будь-якого іншого шляху. Дивіться [Конвертація форматів анотацій](/doc/sections/uk_UA/6-2-3-converting-the-annotations-formats.md).

## Файл проекту анотацій

Меню `Annotations` зберігає проект в один із двох файлів бібліотеки [ImagesAnnotator-DataDrivers](https://github.com/yuriysydor1991/ImagesAnnotator-DataDrivers.git), а котрий із них - визначає розширення імені файлу у шляху, і для відкриття, і для збереження:

- **`*.imgajz1`** - файл imgajz1 (стиснений (zipped) JSON-файл images annotator першої версії), рідний формат файлу проекту застосунку: звичайний JSON-файл проекту, стиснений gzip за допомогою `ZlibController` бібліотеки, - той самий текст файлу JSON, без жодних доповнень і вилучень. Проект займає у ньому приблизно в 16 разів менше місця на диску: проект з 20000 зображень займає 0,99 МБ замість 16,3 МБ свого JSON-файлу, а зберігається і відкривається приблизно за той самий час;

- **будь-який інший шлях**, насамперед `*.json`, - звичайний JSON-текст проекту, файл, який писала кожна попередня версія застосунку.

Тож введіть у діалозі `Save as` ім'я, що закінчується на `.imgajz1`, щоб зберегти проект стисненим, і ім'я, що закінчується на `.json`, щоб зберегти його JSON-текстом: проект, відкритий з одного з них, цим самим збереженням перетворюється на інший, а звичайне `Save` продовжує з файлом проекту. Діалоги файлу проекту показують обидва види файлів. Стандартні інструменти gzip теж перетворюють ці два файли: `gzip -dc project.imgajz1 > project.json` і `gzip -c project.json > project.imgajz1`. Подробиці формату дивіться у секції [Стиснений формат проекту imgajz1](https://github.com/yuriysydor1991/ImagesAnnotator-DataDrivers/blob/main/doc/sections/uk_UA/4-project-structure/4-11-the-imgajz1-compressed-project-format.md) бібліотеки.

# Залежності

Застосунок збирається проти трьох власних окремих бібліотек -
[ImagesAnnotator-DataDrivers](https://github.com/yuriysydor1991/ImagesAnnotator-DataDrivers.git)
(база даних проекту анотацій),
[ImagesAnnotator-DataExporters](https://github.com/yuriysydor1991/ImagesAnnotator-DataExporters.git)
(запис наборів даних) та
[ImagesAnnotator-DataImporters](https://github.com/yuriysydor1991/ImagesAnnotator-DataImporters.git)
(читання наборів даних), - а також проти одного з чотирьох взаємозамінних
наборів інструментів GUI і невеликого набору сторонніх бібліотек:

![Залежності ImagesAnnotator](diagrams/images/dependencies.svg)

Жодна з трьох власних бібліотек не є пакунком дистрибутиву і жодна не
звантажується кроком конфігурації цього проекту, тож усі три встановлюються
заздалегідь - долучений скрипт
[misc/ci/install-annotator-libraries.sh](/misc/ci/install-annotator-libraries.sh)
збирає та встановлює їх однією командою. Див. [Вмикання інтеграції бібліотеки
ImagesAnnotator-DataDrivers](/doc/sections/uk_UA/5-36-enabling-the-ImagesAnnotator-DataDrivers-library.md),
[Вмикання інтеграції бібліотеки
ImagesAnnotator-DataExporters](/doc/sections/uk_UA/5-37-enabling-the-ImagesAnnotator-DataExporters-library.md)
та [Вмикання інтеграції бібліотеки
ImagesAnnotator-DataImporters](/doc/sections/uk_UA/5-39-enabling-the-ImagesAnnotator-DataImporters-library.md).

Діаграму намальовано вручну за джерелом PlantUML за адресою
[doc/diagrams/plantuml/dependencies.puml](diagrams/plantuml/dependencies.puml).
Редагуй той файл щоразу, коли змінюється залежність, і змінюй малюнок разом із
ним. Генерування джерела в окрему директорію показує, з чим порівнювати
малюнок, не перезаписуючи його:

```
# з кореневої директорії проекту

plantuml -tsvg -o "$(pwd)/build/diagrams" doc/diagrams/plantuml/dependencies.puml
```

# Реалізування користувацьких експортерів/перетворювачів

Поточна програма для анотацій зображень зберігає свою базу даних анотацій у вигляді текстового файлу формату `JSON` - звичайним JSON-текстом або тим самим текстом, стисненим gzip у файлі проекту imgajz1 (`json.load(gzip.open("project.imgajz1"))` у `Python`, `gzip -dc project.imgajz1 | jq .` у `Bash`). Це набагато полегшує створення користувацьких скриптів експорту у потрібний формат для тренування нейро мереж за допомогою таких мов як `Python` чи `Bash`.

Експортери, які постачає сам застосунок - по одному на кожну з тринадцяти розкладок, перелічених у розділі [Використання](#використання), - знаходяться в окремій бібліотеці [ImagesAnnotator-DataExporters](https://github.com/yuriysydor1991/ImagesAnnotator-DataExporters.git), тому новий формат можна додати і туди, і тоді його отримає кожен інструмент, побудований на цій бібліотеці. Див. [Вмикання інтеграції бібліотеки ImagesAnnotator-DataExporters](/doc/sections/uk_UA/5-37-enabling-the-ImagesAnnotator-DataExporters-library.md).

Читання набору даних назад є оберненням цього: тринадцять імпортерів, які постачає застосунок - по одному на кожну розкладку, яку він експортує, - знаходяться в окремій бібліотеці [ImagesAnnotator-DataImporters](https://github.com/yuriysydor1991/ImagesAnnotator-DataImporters.git). Див. [Вмикання інтеграції бібліотеки ImagesAnnotator-DataImporters](/doc/sections/uk_UA/5-39-enabling-the-ImagesAnnotator-DataImporters-library.md).

Переглянь директорію `doc` щодо можливих перекладів поточного md-документа:
- `en_US` за відносною адресою [README.md](/README.md)

# Зміст документації

**Даний документ у процесі покращення**

1. [Клонування проекту ImagesAnnotator](/doc/sections/uk_UA/1-cloning-the-cxx-template-project.md)
1. [Вимоги](/doc/sections/uk_UA/3-requirements.md)
    1. [Обов'язкові інструменти для ОС на базі GNU/Лінукс](/doc/sections/uk_UA/3-1-required-tools-for-the-GNU-Linux-based-OS.md)
    1. [Обов'язкові інструменти для ОС на базі MS Windows](/doc/sections/uk_UA/3-2-required-tools-for-the-MS-Windows-based-OS.md)
    1. [Необов'язкові пакети для тестів](/doc/sections/uk_UA/3-3-optional-for-the-tests.md)
    1. [Необов'язкові пакети для створення документації](/doc/sections/uk_UA/3-4-optional-for-the-documentation.md)
    1. [Необов'язкові пакети для форматування коду](/doc/sections/uk_UA/3-5-optional-for-the-code-formatting.md)
    1. [Необов'язкові пакети для статичного аналізатора коду cppcheck](/doc/sections/uk_UA/3-6-optional-for-the-code-analyzer-cppcheck.md)
    1. [Необов'язкові пакети для статичного аналізатора коду clang-tidy](/doc/sections/uk_UA/3-7-optional-for-the-code-analyzer-with-clang-tidy.md)
    1. [Необов'язкові пакети для перевірки використання пам'яті за допомогою Valgrind](/doc/sections/uk_UA/3-8-optional-for-the-memory-checkwith-Valgrind.md)
    1. [Необов'язкові програми для генерації пакету flatpak](/doc/sections/uk_UA/3-9-optional-for-the-flatpak-packager.md)
    1. [Необов'язкові пакети для запуску контейнера Docker](/doc/sections/uk_UA/3-10-optional-for-docker-container-runs.md)
    1. [Необов'язкові пакети для snap пакувальника](/doc/sections/uk_UA/3-11-optional-for-snap-packager.md)
    1. [Необов'язкові програми для генерації пакунку AppImage](/doc/sections/uk_UA/3-12-optional-for-the-AppImage-packager.md)
1. [Структура проекту](/doc/sections/uk_UA/4-project-structure.md)
    1. [Реалізуй код одразу!](/doc/sections/uk_UA/4-1-implement-code-straight-away.md)
    1. [Зміна назви проекту і головного виконуваного файлу](/doc/sections/uk_UA/4-2-changing-the-project-and-executable-name.md)
    1. [Реалізація власних нащадків IApplication](/doc/sections/uk_UA/4-7-implementing-your-onw-IApplication-descendants.md)
    1. [Версіювання і інші параметри проекту](/doc/sections/uk_UA/4-3-version-tracking-and-other-project-parameters.md)
    1. [Мінімально можливі версії](/doc/sections/uk_UA/4-6-minimal-possible-versions.md)
    1. [Тести проекту](/doc/sections/uk_UA/4-4-project-tests.md)
        1. [Фреймворк тестів Google Test](/doc/sections/uk_UA/4-4-1-google-test.md)
    1. [Розширення](/doc/sections/uk_UA/4-5-extensions.md)
1. [Побудова проекту](/doc/sections/uk_UA/5-project-build.md)
    1. [Побудова за допомогою IDE](/doc/sections/uk_UA/5-1-IDE-build.md)
    1. [Побудова проекту через командний рядок](/doc/sections/uk_UA/5-2-command-line-build.md)
    1. [Швидкі скрипти побудови](/doc/sections/uk_UA/5-38-quick-build-scripts.md)
    1. [Виконуваний файл ImagesAnnotatorConverter](/doc/sections/uk_UA/5-40-the-ImagesAnnotatorConverter-binary.md)
    1. Вмикання тестів
        1. [Вмикання юніт- і компонент-тестів](/doc/sections/uk_UA/5-3-1-enabling-unit-testing.md)
        1. [Запобігання використання GTest з ОС](/doc/sections/uk_UA/5-3-2-disabling-system-GTest-probe.md)
    1. [Побудова документації](/doc/sections/uk_UA/5-4-documentation-build.md)
    1. [Вмикання підтримки встановлення документації](/doc/sections/uk_UA/5-5-configuring-the-documentation-install-support.md)
    1. Якість коду та санітайзери
        1. [Вмикання підтримки форматування коду](/doc/sections/uk_UA/5-6-enabling-and-performing-code-formatting-target.md)
        1. [Вмикання підтримки цілі статичного аналізатора коду cppcheck](/doc/sections/uk_UA/5-7-enabling-the-static-code-analyzer-target-with-cppcheck.md)
        1. [Вмикання підтримки статичного аналізатора коду clang-tidy](/doc/sections/uk_UA/5-8-enabling-static-code-analyzer-with-clang-tidy.md)
        1. [Вмикання цілі динамічної перевірки пам'яті Valgrind](/doc/sections/uk_UA/5-9-enabling-the-dynamic-memory-check-target-with-valgrind.md)
        1. [Вмикання перевірок під час виконання програми (sanitizers)](/doc/sections/uk_UA/5-13-enabling-sanitizers.md)
    1. Профайлери
        1. [Вмикання підтримки профілювання з gprof](/doc/sections/uk_UA/5-15-enabling-gprof-profiler-analysis.md)
        1. [Вмикання підтримки профілювання за допомогою valgrind/callgrind](/doc/sections/uk_UA/5-16-enabling-valgrinds-callgrind-profiler-analysis.md)
    1. Контейнери та CI
        1. [Вмикання підтримки запуску Docker-контейнера](/doc/sections/uk_UA/5-12-enabling-the-docker-container-build-and-run.md)
        1. [Вмикання конвеєра Jenkins всередині Docker контейнера](/doc/sections/uk_UA/5-17-enabling-Jenkins-pipeline-inside-Docker-container.md)
        1. [Безперервна інтеграція за допомогою GitHub Actions](/doc/sections/uk_UA/5-34-continuous-integration-with-GitHub-Actions.md)
        1. [Безперервна інтеграція за допомогою GitLab CI/CD](/doc/sections/uk_UA/5-35-continuous-integration-with-GitLab-CI.md)
    1. Пакувальники
        1. [Вмикання підтримки генерування DEB-пакетів з cpack](/doc/sections/uk_UA/5-10-enabling-DEB-package-generation-with-cpack.md)
        1. [Вмикання підтримки генерації flatpak пакету](/doc/sections/uk_UA/5-11-enabling-the-flatpak-package-generation-support.md)
        1. [Вмикання пакувальника snap](/doc/sections/uk_UA/5-19-enabling-the-snap-package-manager.md)
        1. [Вмикання підтримки генерування пакунків FreeBSD pkg з cpack](/doc/sections/uk_UA/5-20-enabling-FreeBSD-pkg-package-generation-with-cpack.md)
        1. [Вмикання підтримки генерування WIX MSI-пакетів з cpack](/doc/sections/uk_UA/5-21-enabling-WIX-MSI-package-generation-with-cpack.md)
        1. [Вмикання підтримки генерування RPM-пакунків з cpack](/doc/sections/uk_UA/5-22-enabling-RPM-package-generation-with-cpack.md)
        1. [Вмикання пакувальника AppImage](/doc/sections/uk_UA/5-41-enabling-the-AppImage-packager.md)
    1. Бібліотеки ImagesAnnotator
        1. [Вмикання інтеграції бібліотеки ImagesAnnotator-DataDrivers](/doc/sections/uk_UA/5-36-enabling-the-ImagesAnnotator-DataDrivers-library.md)
        1. [Вмикання інтеграції бібліотеки ImagesAnnotator-DataExporters](/doc/sections/uk_UA/5-37-enabling-the-ImagesAnnotator-DataExporters-library.md)
        1. [Вмикання інтеграції бібліотеки ImagesAnnotator-DataImporters](/doc/sections/uk_UA/5-39-enabling-the-ImagesAnnotator-DataImporters-library.md)
    1. Бібліотеки
        1. [Вмикання інтеграції libcurl](/doc/sections/uk_UA/5-14-enabling-libcurl.md)
1. [Запуск головного виконуваного файлу](/doc/sections/uk_UA/6-run-the-executable.md)
    1. [Запуск через IDE](/doc/sections/uk_UA/6-1-IDE-run.md)
    1. Запуск з терміналу
        1. [Пошук згенерованого виконуваного бінарного файлу](/doc/sections/uk_UA/6-2-1-searching-for-the-generated-executable.md)
        1. [Запуск згенерованого виконуваного файлу](/doc/sections/uk_UA/6-2-2-starting-the-generated-executable.md)
        1. [Конвертація форматів анотацій](/doc/sections/uk_UA/6-2-3-converting-the-annotations-formats.md)
    1. Запуск тестів
        1. [Запуск тестів за допомогою ctest](/doc/sections/uk_UA/6-3-1-run-tests-by-the-ctest.md)
1. [Встановлення](/doc/sections/uk_UA/7-installing.md)
