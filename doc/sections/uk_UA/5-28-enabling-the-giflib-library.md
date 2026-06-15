## Вмикання інтеграції giflib (GIF)

Для того щоб увімкнути бібліотеку [giflib](https://giflib.sourceforge.net/) (формат зображень GIF) для проекту, необхідно встановити значення `ON` для CMake змінної `ENABLE_GIFLIB`:

```
# в середині кореневої директорії проекту

cmake -S . -B build -DENABLE_GIFLIB=ON
```

giflib постачається лише зі збіркою на autotools, отож вона шукається виключно в системі (встанови `libgif-dev` на Debian/Ubuntu, `giflib-devel` на Fedora/openSUSE або `giflib` на Arch) - резервного механізму FetchContent немає. Дивись модуль `cmake/enablers/images/template-project-giflib-enabler.cmake` щодо цілі (`GIF::GIF`), яку слід прилінкувати до твоїх цільових об'єктів.
