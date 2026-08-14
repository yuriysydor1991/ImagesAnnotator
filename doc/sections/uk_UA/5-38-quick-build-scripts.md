## Швидкі скрипти побудови

Директорія [scripts/build](/scripts/build) містить скрипти оболонки, які виконують кроки конфігурації, побудови і встановлення проекту системою CMake, отож цілий цикл `Release` або `Debug` виконується однією командою замість окремих викликів CMake, описаних у секції [Побудова проекту через командний рядок](/doc/sections/uk_UA/5-2-command-line-build.md).

Кожен скрипт тримає своє дерево побудови всередині директорії `build` проекту: `Release` у `build/release`, а `Debug` у `build/debug`.

Проект не будується без двох окремих бібліотек, з якими він збирається, і жодна з них не звантажується кроком конфігурації (див. секції [Увімкнення бібліотеки ImagesAnnotator-DataDrivers](/doc/sections/uk_UA/5-36-enabling-the-ImagesAnnotator-DataDrivers-library.md) та [Увімкнення бібліотеки ImagesAnnotator-DataExporters](/doc/sections/uk_UA/5-37-enabling-the-ImagesAnnotator-DataExporters-library.md)), отож їх слід один раз встановити перед запуском будь-якого зі скриптів нижче:

```
# в середині кореневої директорії проекту

misc/ci/install-annotator-libraries.sh
```

### Скрипти повного циклу

Кожен зі скриптів нижче виконує кроки конфігурації, побудови і встановлення один за одним:

| Скрипт | Виконує |
| --- | --- |
| [release.sh](/scripts/build/release.sh) | побудову `Release` |
| [debug.sh](/scripts/build/debug.sh) | побудову `Debug` з увімкненими тестами, документацією, `cppcheck`, `valgrind` і `callgrind` |
| [debug-cppcheck.sh](/scripts/build/debug-cppcheck.sh) | побудову `Debug` цілі `cppcheck` |
| [debug-valgrind.sh](/scripts/build/debug-valgrind.sh) | побудову `Debug` цілі `valgrind` |
| [debug-callgrind.sh](/scripts/build/debug-callgrind.sh) | побудову `Debug` цілі `callgrind` |
| [debug-clang-tidy.sh](/scripts/build/debug-clang-tidy.sh) | побудову `Debug` з параметром `ENABLE_CLANG_TIDY` |
| [debug-sanitizers.sh](/scripts/build/debug-sanitizers.sh) | побудову `Debug` з параметром `ENABLE_SANITIZERS` |
| [debug-sanitizers-threads.sh](/scripts/build/debug-sanitizers-threads.sh) | побудову `Debug` з параметром `ENABLE_SANITIZERS_THREADS` |
| [debug-compiler-analyzer.sh](/scripts/build/debug-compiler-analyzer.sh) | побудову `Debug` з параметром `ENABLE_COMPILER_CODE_ANALYZER` |

Повна побудова `Release` виконується однією командою:

```
# в середині кореневої директорії проекту

scripts/build/release.sh
```

### Скрипти окремих кроків

Кожен скрипт повного циклу вище є ланцюжком з трьох скриптів окремих кроків, які можна запускати самостійно, коли потрібен лише один крок:

| Скрипт | Виконує |
| --- | --- |
| [release-configure.sh](/scripts/build/release-configure.sh), [debug-configure.sh](/scripts/build/debug-configure.sh) | крок конфігурації `cmake -B <директорія побудови> -S .` |
| [release-build.sh](/scripts/build/release-build.sh), [debug-build.sh](/scripts/build/debug-build.sh) | крок побудови `cmake --build <директорія побудови>` |
| [release-install.sh](/scripts/build/release-install.sh), [debug-install.sh](/scripts/build/debug-install.sh) | крок встановлення `sudo cmake --install <директорія побудови>` |

### Параметри, які приймають скрипти

Скрипти приймають наступні власні параметри, які ніколи не передаються до CMake:

| Параметр | Дія |
| --- | --- |
| `--no-reconfigure` | Зберігає наявну директорію побудови і повністю пропускає крок конфігурації. Без цього параметра крок конфігурації **стирає** директорію побудови перед новою конфігурацією проекту. |
| `--install` | Виконує крок встановлення. Без цього параметра крок встановлення пропускається, отож для звичайної побудови пароль `sudo` ніколи не запитується. |

Будь-який аргумент виду `-D<змінна>=<значення>` передається до кроку конфігурації, що робить доступним для скриптів кожен параметр CMake з цієї документації:

```
# в середині кореневої директорії проекту

# побудова Release з увімкненою генерацією DEB-пакунка
scripts/build/release.sh -DENABLE_DEB=ON

# та ж побудова з повторним використанням вже сконфігурованої
# директорії побудови і наступним встановленням у префікс /usr
scripts/build/release.sh --no-reconfigure --install
```

Інструментарій графічного інтерфейсу обирається так само, оскільки це теж параметр CMake (див. секцію [Розширення](/doc/sections/uk_UA/4-5-extensions.md)). Коли не задано жодного з них, скрипти будують типовий графічний інтерфейс GTKmm3:

```
# в середині кореневої директорії проекту

scripts/build/release.sh -DUSE_GTKMM4=ON
scripts/build/release.sh -DUSE_WXWIDGETS=ON
scripts/build/release.sh -DUSE_QT6=ON
```

Решта аргументів передається до кроків побудови і встановлення, отож параметр CMake, який належить лише одному кроку, слід передавати відповідному скрипту окремого кроку, а не скрипту повного циклу:

```
# в середині кореневої директорії проекту

# побудова окремої цілі всередині вже сконфігурованої директорії побудови Debug
scripts/build/debug-build.sh --target cppcheck
```

Виклик конфігурації `cmake -B` не приймає ані параметра `--target`, ані `--verbose`, отож передача такого аргументу скрипту повного циклу завершить крок конфігурації помилкою.
