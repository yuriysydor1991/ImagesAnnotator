## Вмикання інтеграції libpng

Для того щоб увімкнути бібліотеку [libpng](http://www.libpng.org/pub/png/libpng.html) (формат зображень PNG) для проекту, використовуючи встановлену версію або через завантаження з мережі, необхідно встановити значення `ON` для CMake змінної `ENABLE_LIBPNG`:

```
# в середині кореневої директорії проекту

cmake -S . -B build -DENABLE_LIBPNG=ON
```

libpng залежить від zlib, отож залиш `ENABLE_ZLIB=ON` (або наявну системну zlib) щоб залежність вирішилась. Дивись модуль `cmake/enablers/images/template-project-libpng-enabler.cmake` щодо цілей (`PNG::PNG` для системної версії, `png_static`/`png` для побудови через FetchContent), які слід прилінкувати до твоїх цільових об'єктів.
