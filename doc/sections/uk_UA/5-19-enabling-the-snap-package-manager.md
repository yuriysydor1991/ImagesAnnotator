## Вмикання пакувальника snap

Для того щоб увімкнути для побудови ціль `snap`, що призначена для генерування [SNAP](https://snapcraft.io/docs/) бінарного дистрибуційного пакунку проекту, необхідно сконфігурувати CMake проект з увімкненою змінною `ENABLE_SNAP`:

```
cmake -S . -B build -DENABLE_SNAP=ON
```

Далі необхідно побудувати ціль `snap`, яка виконує команду `snapcraft pack` у директорії побудови:

```
cmake --build build --target snap
```

Скрипт [scripts/build/build-snap.sh](/scripts/build/build-snap.sh) виконує обидва кроки одразу (дивись [Швидкі скрипти побудови](/doc/sections/uk_UA/5-38-quick-build-scripts.md)). Програму `snapcraft` і менеджер контейнерів, у якому вона збирає пакунок, описано у секції [Необов'язкові пакети для snap пакувальника](/doc/sections/uk_UA/3-11-optional-for-snap-packager.md).

Програма snapcraft називає пакунок `<назва snap>_<версія>_<архітектура>.snap`, де архітектура - це назва Debian центрального процесора, для якого його зібрано (наприклад `amd64` чи `arm64`), а ціль додає до неї назву та версію цільової операційної системи так само, як вони слідують за архітектурою у назві кожного іншого пакунку проекту, - це кеш-змінна `TEMPLATE_PROJECT_PACKAGE_SYSTEM`, описана у секції [Вмикання підтримки генерування DEB-пакетів з cpack](/doc/sections/uk_UA/5-10-enabling-DEB-package-generation-with-cpack.md). Тож із версією проекту `0.13.0` на системі `amd64` з Ubuntu 24.04 пакунок називається `imagesannotator_0.13.0_amd64-ubuntu-24.04.snap`.

Детальніше у файлі [cmake/enablers/packagers/template-project-snap-enabler.cmake](/cmake/enablers/packagers/template-project-snap-enabler.cmake) про ціль `snap`.
