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

In case of the success target build, there will be created the flatpak file under the root build directory named, for example, the `ImagesAnnotator-0.9.0.flatpak`. In order to examine and/or change the flatpak generation target parameters visit the `cmake/enablers/packagers/template-project-flatpak-target.cmake` or `misc/packagers/flatpak.conf.json.in` files.

Note that the flatpak manifest builds GTKmm 3.24.8 inside the sandbox, so the bundled application is configured with `-DUSE_GTKMM3=ON` and ships the GTKmm3 GUI regardless of the GTKmm4 default used for ordinary builds. That is also why the application id carries the `gtkmm3` part below. To ship the GTKmm4 GUI instead, replace the `gtkmm` module of `misc/packagers/flatpak.conf.json.in` with a GTKmm 4.x one, drop the `-DUSE_GTKMM3=ON` config option and adjust `FLATPAK_PROJECT_URL` in `cmake/enablers/packagers/template-project-flatpak-target.cmake`.

Refer to the flatpak documentation on how to install and run applications distributed from a flatpaks files. For the current project defaults the install and run command may look like this:

```
# to install (version may change)
# inside the project build directory
flatpak install --user ImagesAnnotator-0.9.0.flatpak

# to run the application
flatpak run ua.org.kytok.template.gtkmm3.ImagesAnnotator
```
