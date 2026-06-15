## Вмикання інтеграції libtiff (TIFF)

Для того щоб увімкнути бібліотеку [libtiff](http://www.libtiff.org/) (формат зображень TIFF) для проекту, використовуючи встановлену версію або через завантаження з мережі, необхідно встановити значення `ON` для CMake змінної `ENABLE_LIBTIFF`:

```
# в середині кореневої директорії проекту

cmake -S . -B build -DENABLE_LIBTIFF=ON
```

Дивись модуль `cmake/enablers/images/template-project-libtiff-enabler.cmake` щодо цілей (`TIFF::TIFF` для системної версії, `tiff` для побудови через FetchContent), які слід прилінкувати до твоїх цільових об'єктів.
