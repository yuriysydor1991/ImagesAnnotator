## Вмикання інтеграції libjpeg

Для того щоб увімкнути бібліотеку [libjpeg-turbo](https://libjpeg-turbo.org/) (формат зображень JPG/JPEG) для проекту, використовуючи встановлену версію або через завантаження з мережі, необхідно встановити значення `ON` для CMake змінної `ENABLE_LIBJPEG`:

```
# в середині кореневої директорії проекту

cmake -S . -B build -DENABLE_LIBJPEG=ON
```

Дивись модуль `cmake/enablers/images/template-project-libjpeg-enabler.cmake` щодо цілей (`JPEG::JPEG` для системної версії, `jpeg-static`/`jpeg` або `turbojpeg` для побудови через FetchContent), які слід прилінкувати до твоїх цільових об'єктів.
