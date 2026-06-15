## Вмикання інтеграції libavif (AVIF)

Для того щоб увімкнути бібліотеку [libavif](https://github.com/AOMediaCodec/libavif) (формат зображень AVIF / AV1) для проекту, використовуючи встановлену версію або через завантаження з мережі, необхідно встановити значення `ON` для CMake змінної `ENABLE_LIBAVIF`:

```
# в середині кореневої директорії проекту

cmake -S . -B build -DENABLE_LIBAVIF=ON
```

Для власне кодування/декодування AVIF потрібен кодек; для побудови через FetchContent увімкни один з них (наприклад `-DAVIF_CODEC_AOM=ON` або `-DAVIF_CODEC_DAV1D=ON`). Дивись модуль `cmake/enablers/images/template-project-libavif-enabler.cmake` щодо цілі (`avif`), яку слід прилінкувати до твоїх цільових об'єктів.
