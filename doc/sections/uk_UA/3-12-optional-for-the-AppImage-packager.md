## Необов'язкові програми для генерації пакунку AppImage

Ціль `appimage` викликає команду `appimagetool`, яку дистрибутиви GNU/Linux рідко пакують. Її необхідно звантажити зі сторінки [випусків appimagetool](https://github.com/AppImage/appimagetool/releases) і розмістити у одному з каталогів змінної `PATH` під назвою `appimagetool`:

```
# необхідно замінити x86_64 архітектурою цільової системи
curl -L -o appimagetool \
  https://github.com/AppImage/appimagetool/releases/download/continuous/appimagetool-x86_64.AppImage

chmod +x appimagetool

sudo mv appimagetool /usr/local/bin/
```

Сам `appimagetool` є пакунком [AppImage](https://appimage.org/), тож для його запуску необхідна бібліотека `libfuse`, так само як і для запуску згенерованого пакунку:

```
sudo apt install -y libfuse2t64
```

Без цієї бібліотеки і інструмент, і згенерований пакунок все ще можливо запустити з опцією `--appimage-extract-and-run` або зі встановленою змінною середовища `APPIMAGE_EXTRACT_AND_RUN=1`.

CMake версії 4.2 і новіший пакує AppImage власним генератором CPack, а не окремою ціллю старіших версій (дивись [Вмикання пакувальника AppImage](/doc/sections/uk_UA/5-41-enabling-the-AppImage-packager.md)), і той генератор переписує `RPATH` запакованого бінарного файлу командою `patchelf`, яка є пакунком дистрибутиву:

```
sudo apt install -y patchelf
```

Деталі у секції [Вмикання пакувальника AppImage](/doc/sections/uk_UA/5-41-enabling-the-AppImage-packager.md).
