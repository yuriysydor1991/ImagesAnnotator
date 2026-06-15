## Вмикання інтеграції lunasvg (SVG)

Для того щоб увімкнути бібліотеку [lunasvg](https://github.com/sammycage/lunasvg) (векторний формат зображень SVG) для проекту, використовуючи встановлену версію або через завантаження з мережі, необхідно встановити значення `ON` для CMake змінної `ENABLE_LUNASVG`:

```
# в середині кореневої директорії проекту

cmake -S . -B build -DENABLE_LUNASVG=ON
```

Дивись модуль `cmake/enablers/images/template-project-lunasvg-enabler.cmake` щодо цілей (`lunasvg::lunasvg` для системної версії, `lunasvg` для побудови через FetchContent), які слід прилінкувати до твоїх цільових об'єктів.
