## Enabling DEB package generation with cpack

In order to be able to generate the `deb` package file configure project to support the `cpack` command feature with a `ENABLE_DEB` variable enabled:

```
# inside the project root directory 

cmake -B build -S . -DENABLE_DEB=ON
```

Next, build all available targets required for the `deb` package

```
# inside the project root directory

cmake --build build --target all
```

Finally, execute the `cpack` command inside the template project build directory:

```
# inside the project build directory

cpack
```

The package file should be generated inside the project build root directory. For example, if project name was't change and it's version is 0.8.0 so the package name may look like `ImagesAnnotator-0.8.0-Linux.deb`.

In order to examine details of the `deb` package configuration visit the `cmake/enablers/packagers/template-project-deb-enabler.cmake` file.

The generated package declares the GUI runtime it was built against as a dependency (`libgtkmm-3.0-1v5`, `libgtkmm-4.0-0`, `libwxgtk3.2-1t64` or `libqt6widgets6`). That name is not decided by the DEB enabler itself: it is published as `GTKMM_DEB_RUNTIME_DEPENDS` by whichever GUI toolkit enabler ran, so the packaged dependency automatically follows the `USE_GTKMM3` / `USE_GTKMM4` / `USE_WXWIDGETS` / `USE_QT6` selection described in the extensions section.
