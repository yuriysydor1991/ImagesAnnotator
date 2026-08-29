## Реалізація власних нащадків IApplication

Абстрактний клас `IApplication` і усі його нащадки винесені у окрему директорію-підкомпонент `src/app/applications` компонента `src/app`:

```
src/app/applications
├── Application.cpp
├── Application.h
├── ApplicationHelpPrinter.cpp
├── ApplicationHelpPrinter.h
├── ApplicationVersionPrinter.cpp
├── ApplicationVersionPrinter.h
├── CMakeLists.txt
├── IApplication.h
├── converter
│   ├── ApplicationConverterComponent.cpp
│   ├── ApplicationConverterComponent.h
│   ├── ApplicationFormatsConverter.cpp
│   ├── ApplicationFormatsConverter.h
│   ├── CMakeLists.txt
│   ├── ConversionFormats.cpp
│   ├── ConversionFormats.h
│   └── IFormatsConverter.h
└── tests
    ├── CMakeLists.txt
    ├── mocks
    │   ├── Application
    │   ├── ApplicationConverterComponent
    │   ├── ApplicationFormatsConverter
    │   ├── ApplicationHelpPrinter
    │   └── ApplicationVersionPrinter
    └── unit
        ├── Application
        ├── ApplicationConverterComponent
        ├── ApplicationFormatsConverter
        ├── ApplicationHelpPrinter
        ├── ApplicationVersionPrinter
        ├── CMakeLists.txt
        └── ConversionFormats
```

У корені компонента `src/app` лишається те, що керує програмою, а не є нею: `ApplicationContext`, який передається у кожен виклик `run()`, `CommandLineParser`, який його заповнює, `ApplicationFactory`, який вирішує якого саме нащадка вимагає командний рядок, і `ApplicationBinaryName.h`, через який повідомлення називають запущений бінарний файл.

Звісно що можна реалізувати окремий новий потомок абстракного класу `IApplication` щоб реалізувати високорівненву варіативність поведінки виконуваного файлу без засмічування незв'язаними `if`-виразами і іншим кодом оригінального класу `Application` (у якому буде міститись реалізація за умовчанням) (пам'ятаймо про принцип Єдиної відповідальності від SOLID).

Створення потомка абстрактного класу `IApplication` може бути виконаним за допомогою прямого безпосереднього створення нового потомка-класу або перевизначенням наявних класів-потомків `IApplication` на подобі `Application`, `ApplicationHelpPrinter`, `ApplicationVersionPrinter` чи `app::converter::ApplicationConverterComponent` із субдиректорії `converter`.

Зареєструй новостворений клас-потомок абстрактного класу `IApplication` у методі `create_application` від `ApplicationFactory`, який відповідальний за створення необхідної релазації програми у відповідності до вводу користувача у вигляді параметрів командного рядка.

Це може бути виконаним реалізацією нового потомка класу `ApplicationFactory` і віртальним перевизначенням його створюючих методів на подобі `create_application` і/або інших (потрібно також викликати відповідний статичний метод у головній функції виконуваного бінарного файлу `main` що розміщена у файлі `main.cpp`).

### Де розміщувати файли нового нащадка

1. Файли заголовку і реалізації нового класу необхідно розмістити у директорії `src/app/applications` і додати новий файл `.cpp` до виклику `annotator_app_targets_sources` у файлі побудови `src/app/applications/CMakeLists.txt`. Даний виклик передає сирці кожній цілі-програмі, яку виробляє дана конфігурація - головному бінарному файлу `ImagesAnnotator` і виключно конвертувальному `ImagesAnnotatorConverter` - тож вказаний там нащадок потрапляє в обидва.
1. Нащадок, який може нести лише один із бінарних файлів, додається у власний файл цілі того бінарного файлу, як `src/ImagesAnnotator-target.cmake` додає `Application.cpp`: даний клас запускає вкомпільований `gui::GUIIniter` і тому тягне за собою усе піддерево `src/gui`, саме без якого і будується виключно конвертувальний бінарний файл.
1. Нащадок, який сам є цілим підкомпонентом, отримує власну директорію у `src/app/applications` із власним файлом `CMakeLists.txt`, доданим викликом `add_subdirectory` у `src/app/applications/CMakeLists.txt` - так, як це зроблено для директорії `converter`, яка містить `ApplicationConverterComponent` і перетворення, яке той запускає.
1. Модульний тест нового класу необхідно розмістити у директорії `src/app/applications/tests/unit/<ВашаПрограма>` і зареєструвати дану директорію за допомогою виклику `add_subdirectory` у файлі побудови `src/app/applications/tests/unit/CMakeLists.txt`. За приклад можна взяти сусідні модульні тести.
1. Якщо новий клас необхідно підміняти GMock-об'єктом у тестах іншого класу (як це зроблено для модульного тесту класу `ApplicationFactory`), то такий макет необхідно розмістити у файлі `src/app/applications/tests/mocks/<ВашаПрограма>/src/app/applications/<ВашаПрограма>.h`. Дана директорія макетів доступна усьому компоненту `src/app` через змінну CMake `${APPLICATIONS_MOCKS_ROOT}`, а власні макети компонента `src/app` — через змінну `${APP_MOCKS_ROOT}`. Обидві оголошені у `src/app/CMakeLists.txt`, тож набір тестів будь-якої з половин компонента дістається обох.
