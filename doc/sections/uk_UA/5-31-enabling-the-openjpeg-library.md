## Вмикання інтеграції OpenJPEG (JPEG 2000)

Для того щоб увімкнути бібліотеку [OpenJPEG](https://www.openjpeg.org/) (формат зображень JPEG 2000) для проекту, використовуючи встановлену версію або через завантаження з мережі, необхідно встановити значення `ON` для CMake змінної `ENABLE_OPENJPEG`:

```
# в середині кореневої директорії проекту

cmake -S . -B build -DENABLE_OPENJPEG=ON
```

Дивись модуль `cmake/enablers/images/template-project-openjpeg-enabler.cmake` щодо цілі (`openjp2`), яку слід прилінкувати до твоїх цільових об'єктів.
