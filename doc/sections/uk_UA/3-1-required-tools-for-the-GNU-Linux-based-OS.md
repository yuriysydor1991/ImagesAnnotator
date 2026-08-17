## Обов'язкові інструменти для ОС на базі GNU/Лінукс

Проект збирається проти трьох власних окремих бібліотек, і жодна з них не є пакунком дистрибутиву: [ImagesAnnotator-DataDrivers](https://github.com/yuriysydor1991/ImagesAnnotator-DataDrivers.git) - бази даних проекту анотацій, [ImagesAnnotator-DataExporters](https://github.com/yuriysydor1991/ImagesAnnotator-DataExporters.git) - експортерів набору даних - та [ImagesAnnotator-DataImporters](https://github.com/yuriysydor1991/ImagesAnnotator-DataImporters.git) - імпортерів набору даних. Усі три активатори лише шукають їх викликом `find_package()`, тому всі три потрібно встановити перед конфігурацією проекту. Долучений сценарій збирає і встановлює їх у порядку, потрібному залежності від драйверів даних двох інших:

```
misc/ci/install-annotator-libraries.sh
```

Він встановлює у `/usr/local`; передай інший префікс єдиним аргументом і вкажи його конфігурації через `-DCMAKE_PREFIX_PATH=<префікс>`. Самому сценарію потрібні компілятор, CMake і Git, описані нижче. Дивись [Вмикання інтеграції бібліотеки ImagesAnnotator-DataDrivers](/doc/sections/uk_UA/5-36-enabling-the-ImagesAnnotator-DataDrivers-library.md) [Вмикання інтеграції бібліотеки ImagesAnnotator-DataExporters](/doc/sections/uk_UA/5-37-enabling-the-ImagesAnnotator-DataExporters-library.md) та [Вмикання інтеграції бібліотеки ImagesAnnotator-DataImporters](/doc/sections/uk_UA/5-39-enabling-the-ImagesAnnotator-DataImporters-library.md).

Для того щоб виконати побудову проекту необхідно встановити компілятор GCC C++ разом з системою побудови проекту CMake і системою версіювання Git:

```
sudo apt install -y git g++ cmake
```

Проект містить дві взаємозамінні реалізації графічного інтерфейсу, що обираються під час конфігурації взаємовиключними опціями `USE_GTKMM3` / `USE_GTKMM4` (дивись розділ розширень). За умовчанням увімкнено `USE_GTKMM4`, тому встановіть пакунки розробника GTKmm-4.0:

```
sudo apt install -y pkg-config libgtkmm-4.0-dev
```

Щоб зібрати альтернативний інтерфейс GTKmm-3.0 (`-DUSE_GTKMM3=ON`, що автоматично вимикає `USE_GTKMM4`) встановіть пакунки розробника GTKmm-3.0 (замість або додатково):

```
sudo apt install -y pkg-config libgtkmm-3.0-dev
```

Щоб натомість зібрати необов'язковий інтерфейс wxWidgets (`-DUSE_WXWIDGETS=ON`, що перекриває пару GTKmm) встановіть пакунки розробника wxWidgets, які надають `wx-config` / модуль CMake `find_package(wxWidgets)`, котрий перевіряє активатор:

```
sudo apt install -y libwxgtk3.2-dev
```

Щоб натомість зібрати необов'язковий інтерфейс Qt6 (`-DUSE_QT6=ON`, що так само перекриває пару GTKmm) встановіть базові пакунки розробника Qt6, які надають конфігурацію пакунка CMake `find_package(Qt6 COMPONENTS Core Gui Widgets)`, котру перевіряє активатор, разом з інструментами `moc` та `rcc`, які він запускає:

```
sudo apt install -y qt6-base-dev
```

Для того щоб мати змогу змінювати візуальний інтерфейс необіхдно встановити спеціалізований інструмент розробки [Glade](https://en.wikipedia.org/wiki/Glade_Interface_Designer) за допомогою команди:

```
sudo apt install -y glade
```
