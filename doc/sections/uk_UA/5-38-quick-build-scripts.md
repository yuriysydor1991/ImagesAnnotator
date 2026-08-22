## Швидкі скрипти побудови

Директорія [scripts/build](/scripts/build) містить скрипти оболонки, які виконують кроки конфігурації, побудови і встановлення проекту системою CMake, отож цілий цикл `Release` або `Debug` виконується однією командою замість окремих викликів CMake, описаних у секції [Побудова проекту через командний рядок](/doc/sections/uk_UA/5-2-command-line-build.md).

Кожен скрипт тримає своє дерево побудови всередині директорії `build` проекту: `Release` у `build/release`, а `Debug` у `build/debug`.

Проект не будується без трьох окремих бібліотек, з якими він збирається, і жодна з них не звантажується кроком конфігурації (див. секції [Увімкнення бібліотеки ImagesAnnotator-DataDrivers](/doc/sections/uk_UA/5-36-enabling-the-ImagesAnnotator-DataDrivers-library.md), [Увімкнення бібліотеки ImagesAnnotator-DataExporters](/doc/sections/uk_UA/5-37-enabling-the-ImagesAnnotator-DataExporters-library.md) та [Вмикання інтеграції бібліотеки ImagesAnnotator-DataImporters](/doc/sections/uk_UA/5-39-enabling-the-ImagesAnnotator-DataImporters-library.md)), отож їх слід один раз встановити перед запуском будь-якого зі скриптів нижче:

```
# в середині кореневої директорії проекту

misc/ci/install-annotator-libraries.sh
```

### Скрипти повного циклу

Кожен зі скриптів нижче виконує кроки конфігурації, побудови і встановлення один за одним. Ті зі скриптів `Debug`, які будують увесь проект, виконують ще й крок тестування, поміж кроками побудови і встановлення:

| Скрипт | Виконує |
| --- | --- |
| [release.sh](/scripts/build/release.sh) | побудову `Release` |
| [debug.sh](/scripts/build/debug.sh) | побудову `Debug` з увімкненими модульними, компонентними та інтеграційними тестами, документацією, `cppcheck`, `valgrind` і `callgrind` |
| [debug-cppcheck.sh](/scripts/build/debug-cppcheck.sh) | побудову `Debug` цілі `cppcheck` |
| [debug-valgrind.sh](/scripts/build/debug-valgrind.sh) | побудову `Debug` цілі `valgrind` |
| [debug-callgrind.sh](/scripts/build/debug-callgrind.sh) | побудову `Debug` цілі `callgrind` |
| [debug-clang-tidy.sh](/scripts/build/debug-clang-tidy.sh) | побудову `Debug` з параметром `ENABLE_CLANG_TIDY` |
| [debug-sanitizers.sh](/scripts/build/debug-sanitizers.sh) | побудову `Debug` з параметром `ENABLE_SANITIZERS` |
| [debug-sanitizers-threads.sh](/scripts/build/debug-sanitizers-threads.sh) | побудову `Debug` з параметром `ENABLE_SANITIZERS_THREADS` |
| [debug-compiler-analyzer.sh](/scripts/build/debug-compiler-analyzer.sh) | побудову `Debug` з параметром `ENABLE_COMPILER_CODE_ANALYZER` |

Скрипти [debug-cppcheck.sh](/scripts/build/debug-cppcheck.sh), [debug-valgrind.sh](/scripts/build/debug-valgrind.sh) і [debug-callgrind.sh](/scripts/build/debug-callgrind.sh) будують окрему ціль CMake, до складу якої виконувані файли тестів не входять, отож ці три кроку тестування не виконують зовсім.

Повна побудова `Release` виконується однією командою:

```
# в середині кореневої директорії проекту

scripts/build/release.sh
```

### Скрипти окремих кроків

Кожен скрипт повного циклу вище є ланцюжком зі скриптів окремих кроків нижче, які можна запускати самостійно, коли потрібен лише один крок:

| Скрипт | Виконує |
| --- | --- |
| [release-configure.sh](/scripts/build/release-configure.sh), [debug-configure.sh](/scripts/build/debug-configure.sh) | крок конфігурації `cmake -B <директорія побудови> -S .` |
| [release-build.sh](/scripts/build/release-build.sh), [debug-build.sh](/scripts/build/debug-build.sh) | крок побудови `cmake --build <директорія побудови>` |
| [debug-test.sh](/scripts/build/debug-test.sh) | крок тестування `ctest` всередині директорії побудови `Debug` |
| [release-install.sh](/scripts/build/release-install.sh), [debug-install.sh](/scripts/build/debug-install.sh) | крок встановлення `sudo cmake --install <директорія побудови>` |

### Параметри, які приймають скрипти

Скрипти приймають наступні власні параметри, які ніколи не передаються до CMake:

| Параметр | Дія |
| --- | --- |
| `--no-reconfigure` | Зберігає наявну директорію побудови і повністю пропускає крок конфігурації. Без цього параметра крок конфігурації **стирає** директорію побудови перед новою конфігурацією проекту. |
| `--test` | Виконує крок тестування, який запускає команду `ctest` всередині директорії побудови `Debug`. Без цього параметра крок тестування пропускається. Параметр належить лише скриптам `Debug`, оскільки тести вмикає тільки їхній крок конфігурації. |
| `--install` | Виконує крок встановлення. Без цього параметра крок встановлення пропускається, отож для звичайної побудови пароль `sudo` ніколи не запитується. |

Будь-який аргумент виду `-D<змінна>=<значення>` передається до кроку конфігурації, що робить доступним для скриптів кожен параметр CMake з цієї документації:

```
# в середині кореневої директорії проекту

# побудова Release з увімкненою генерацією DEB-пакунка
scripts/build/release.sh -DENABLE_DEB=ON

# та ж побудова з повторним використанням вже сконфігурованої
# директорії побудови і наступним встановленням у префікс /usr
scripts/build/release.sh --no-reconfigure --install

# побудова Debug з виконанням усього набору тестів одразу після неї
scripts/build/debug.sh --test
```

Три бібліотеки ImagesAnnotator знаходяться так само, коли їх встановлено поза типовими префіксами пошуку CMake - а саме це робить `misc/ci/install-annotator-libraries.sh <префікс>`, коли йому передано префікс, - бо `CMAKE_PREFIX_PATH` є такою ж змінною CMake:

```
# в середині кореневої директорії проекту

misc/ci/install-annotator-libraries.sh "$HOME/.local"

scripts/build/release.sh -DCMAKE_PREFIX_PATH="$HOME/.local"
```

Самі скрипти нічого не встановлюють: вони конфігурують, збирають і (з `--install`) встановлюють лише **цей** проект, тож три бібліотеки лишаються тим єдиним кроком, який слід виконати один раз заздалегідь.

Інструментарій графічного інтерфейсу обирається так само, оскільки це теж параметр CMake (див. секцію [Розширення](/doc/sections/uk_UA/4-5-extensions.md)). Коли не задано жодного з них, скрипти будують типовий графічний інтерфейс GTKmm3:

```
# в середині кореневої директорії проекту

scripts/build/release.sh -DUSE_GTKMM4=ON
scripts/build/release.sh -DUSE_WXWIDGETS=ON
scripts/build/release.sh -DUSE_QT6=ON
```

Решта аргументів передається до кроків побудови, тестування і встановлення, отож параметр, який належить лише одному кроку, слід передавати відповідному скрипту окремого кроку, а не скрипту повного циклу:

```
# в середині кореневої директорії проекту

# побудова окремої цілі всередині вже сконфігурованої директорії побудови Debug
scripts/build/debug-build.sh --target cppcheck

# запуск частини набору тестів всередині вже побудованої директорії побудови Debug
scripts/build/debug-test.sh --test -R "CTEST_FormatsConverter\."
```

Виклик конфігурації `cmake -B` не приймає ані параметра `--target`, ані `--verbose`, отож передача такого аргументу скрипту повного циклу завершить крок конфігурації помилкою.
