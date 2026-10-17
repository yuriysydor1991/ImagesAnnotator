## Enabling the flatpak package generation support

In order to enable the flatpak generation of the redistribution package on the project configure it with enabled option `ENABLE_FLATPAK` (GNU/Linux and alike):

```
# inside the project root directory 

cmake -B build -S . -DENABLE_FLATPAK=ON
```

Which in order will enable the `flatpak` target.

And finally to generate the flatpak package run the `flatpak` target for the build:

```
# inside the project root directory 

cmake --build build --target flatpak
```

In case of the success target build, there will be created the flatpak file under the root build directory, named after the project binary, the project version, the destination CPU architecture, the destination operating system and the flatpak SDK the bundle is built with, for example the `ImagesAnnotator-0.13.0-x86_64-ubuntu-24.04-org.gnome.Sdk-48.flatpak`. The CPU architecture is the one the `flatpak --default-arch` command prints (e.g. `x86_64` or `aarch64`). The name and the version of the destination system follow it the way they follow the architecture in the name of every other package of the project - the `TEMPLATE_PROJECT_PACKAGE_SYSTEM` cache variable described in [Enabling DEB package generation with cpack](/doc/sections/en_US/5-10-enabling-DEB-package-generation-with-cpack.md) - and the SDK closes the name, as the `sdk` and the `runtime-version` members of the flatpak manifest name it, since the very same version of the project may be packed with another manifest (the `FLATPAK_CONF_SRC` cache variable), and so with another SDK, on the very same system. In order to examine and/or change the flatpak generation target parameters visit the `cmake/enablers/packagers/template-project-flatpak-target.cmake` or `misc/packagers/flatpak.conf.json.in` files.

Note that the flatpak manifest builds GTKmm 3.24.8 inside the sandbox, so the bundled application is configured with `-DUSE_GTKMM3=ON` (which also matches the default used for ordinary builds) and ships the GTKmm3 GUI. That is also why the application id carries the `gtkmm3` part below. To ship the GTKmm4 GUI instead, replace the `gtkmm` module of `misc/packagers/flatpak.conf.json.in` with a GTKmm 4.x one, drop the `-DUSE_GTKMM3=ON` config option and adjust `FLATPAK_PROJECT_URL` in `cmake/enablers/packagers/template-project-flatpak-target.cmake`.

Refer to the flatpak documentation on how to install and run applications distributed from a flatpaks files. For the current project defaults the install and run command may look like this:

```
# to install (version may change)
# inside the project build directory
flatpak install --user ImagesAnnotator-0.13.0-x86_64-ubuntu-24.04-org.gnome.Sdk-48.flatpak

# to run the application
flatpak run ua.org.kytok.template.gtkmm3.ImagesAnnotator
```
