## Швидкі скрипти побудови

Директорія [scripts/build](/scripts/build) містить скрипти оболонки, які виконують кроки конфігурації, побудови і встановлення проекту системою CMake, отож цілий цикл `Release` або `Debug` виконується однією командою замість окремих викликів CMake, описаних у секції [Побудова проекту через командний рядок](/doc/sections/uk_UA/5-2-command-line-build.md).

Кожен скрипт тримає своє дерево побудови всередині директорії `build` проекту: `Release` у `build/release`, а `Debug` у `build/debug`.

Директорія [scripts/packages](/scripts/packages) містить те, що має бути встановлено перед запуском будь-якого з них: [install-debian.sh](/scripts/packages/install-debian.sh) і його псевдонім [install-ubuntu.sh](/scripts/packages/install-ubuntu.sh) встановлюють компілятор, CMake, GTest, дві бібліотеки, які типова конфігурація інакше звантажила б і зібрала з їхніх джерел, OpenCV, потрібний виконуваному файлу `ImagesAnnotatorConverter`, і пакунки розробника **одного** інструментарію графічного інтерфейсу - типового GTKmm3 або того, який названо єдиним аргументом:

```
# в середині кореневої директорії проекту

scripts/packages/install-debian.sh

# gtkmm3 (типовий), gtkmm4, wxwidgets, qt6 або all
scripts/packages/install-debian.sh gtkmm4
```

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

### Скрипти пакувальників

Кожен зі скриптів нижче виконує крок конфігурації `Release` з увімкненим одним параметром пакувальника, а потім будує ціль, яка створює пакунок. Окремого кроку побудови немає: кожна з тих цілей самостійно будує все, що їй потрібно, отож нічого зайвого до директорії `build/release` заздалегідь не збирається. Вони приймають параметри скриптів повного циклу вище, а створений пакунок з'являється саме там:

| Скрипт | Створює | З `--install` встановлює його через |
| --- | --- | --- |
| [build-deb.sh](/scripts/build/build-deb.sh) | пакунок DEB - параметр `ENABLE_DEB` і ціль `package` | `sudo apt-get install` |
| [build-flatpak.sh](/scripts/build/build-flatpak.sh) | зв'язку flatpak - параметр `ENABLE_FLATPAK` і ціль `flatpak` | `flatpak install --user` |
| [build-freebsd-pkg.sh](/scripts/build/build-freebsd-pkg.sh) | пакунок FreeBSD pkg - параметр `ENABLE_FREEBSD_PKG` і ціль `package` | `sudo pkg add` |
| [build-rpm.sh](/scripts/build/build-rpm.sh) | пакунок RPM - параметр `ENABLE_RPM` і ціль `package` | `sudo dnf install` |
| [build-snap.sh](/scripts/build/build-snap.sh) | пакунок snap - параметр `ENABLE_SNAP` і ціль `snap` | `sudo snap install --dangerous` |
| [build-wix.sh](/scripts/build/build-wix.sh) | інсталятор WIX MSI - параметр `ENABLE_WIX` і ціль `package` | `msiexec //i` |

Власний аргумент виду `-D<змінна>=<значення>` передається до кроку конфігурації після параметра пакувальника, отож він перекриває і цей параметр, і будь-яке інше налаштування проекту - зокрема інструментарій графічного інтерфейсу запакованого виконуваного файлу:

```
# в середині кореневої директорії проекту

scripts/build/build-deb.sh -DUSE_QT6=ON
```

Самі параметри пакувальників, потрібні їм інструменти та їхні власні налаштування описані у секціях [Вмикання підтримки генерування DEB-пакетів з cpack](/doc/sections/uk_UA/5-10-enabling-DEB-package-generation-with-cpack.md), [Вмикання підтримки генерації flatpak пакету](/doc/sections/uk_UA/5-11-enabling-the-flatpak-package-generation-support.md), [Вмикання підтримки генерування пакунків FreeBSD pkg з cpack](/doc/sections/uk_UA/5-20-enabling-FreeBSD-pkg-package-generation-with-cpack.md), [Вмикання підтримки генерування WIX MSI-пакетів з cpack](/doc/sections/uk_UA/5-21-enabling-WIX-MSI-package-generation-with-cpack.md), [Вмикання підтримки генерування RPM-пакунків з cpack](/doc/sections/uk_UA/5-22-enabling-RPM-package-generation-with-cpack.md) та [Вмикання пакувальника snap](/doc/sections/uk_UA/5-19-enabling-the-snap-package-manager.md).

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
| `--install` | Виконує крок встановлення. Без цього параметра крок встановлення пропускається, отож для звичайної побудови пароль `sudo` ніколи не запитується. Скрипти пакувальників натомість встановлюють щойно створений ними пакунок інструментом того пакувальника. |

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
