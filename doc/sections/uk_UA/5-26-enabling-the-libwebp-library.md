## Вмикання інтеграції libwebp

Для того щоб увімкнути бібліотеку [libwebp](https://developers.google.com/speed/webp) (формат зображень WEBP) для проекту, використовуючи встановлену версію або через завантаження з мережі, необхідно встановити значення `ON` для CMake змінної `ENABLE_LIBWEBP`:

```
# в середині кореневої директорії проекту

cmake -S . -B build -DENABLE_LIBWEBP=ON
```

Дивись модуль `cmake/enablers/images/template-project-libwebp-enabler.cmake` щодо цілей (`WebP::webp` для системної версії, `webp`/`webpdecoder` для побудови через FetchContent), які слід прилінкувати до твоїх цільових об'єктів.
