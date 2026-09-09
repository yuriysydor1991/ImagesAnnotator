## Optional for the AppImage packager

The `appimage` target invokes the `appimagetool` command, which the GNU/Linux distributions rarely package. Download it from the [appimagetool releases](https://github.com/AppImage/appimagetool/releases) and put it into a directory of the `PATH` under the `appimagetool` name:

```
# replace the x86_64 with the architecture of the host
curl -L -o appimagetool \
  https://github.com/AppImage/appimagetool/releases/download/continuous/appimagetool-x86_64.AppImage

chmod +x appimagetool

sudo mv appimagetool /usr/local/bin/
```

The `appimagetool` is an [AppImage](https://appimage.org/) itself, so the `libfuse` library is required to start it, the same way it is required to start the produced package:

```
sudo apt install -y libfuse2t64
```

Without the library both the tool and the produced package are still startable with the `--appimage-extract-and-run` option, or with the `APPIMAGE_EXTRACT_AND_RUN=1` environment variable set.

The CMake 4.2 and newer packs the AppImage with its own CPack generator rather than with the custom target of the older versions (see [Enabling the AppImage packager](/doc/sections/en_US/5-41-enabling-the-AppImage-packager.md)), and that generator rewrites the `RPATH` of the packaged binary with the `patchelf` command, which is a distribution package:

```
sudo apt install -y patchelf
```

Look for the details at the [Enabling the AppImage packager](/doc/sections/en_US/5-41-enabling-the-AppImage-packager.md).
