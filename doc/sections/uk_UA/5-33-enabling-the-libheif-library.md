## Вмикання інтеграції libheif (HEIF/HEIC)

Для того щоб увімкнути бібліотеку [libheif](https://github.com/strukturag/libheif) (формат зображень HEIF/HEIC) для проекту, використовуючи встановлену версію або через завантаження з мережі, необхідно встановити значення `ON` для CMake змінної `ENABLE_LIBHEIF`:

```
# в середині кореневої директорії проекту

cmake -S . -B build -DENABLE_LIBHEIF=ON
```

Для власне декодування HEIF/HEIC потрібен кодек (наприклад libde265). Дивись модуль `cmake/enablers/images/template-project-libheif-enabler.cmake` щодо цілі (`heif`), яку слід прилінкувати до твоїх цільових об'єктів.
