## Enabling the snap packager

In order to enable the project's `snap` package target designed to generate the project's [SNAP](https://snapcraft.io/docs/) binary package distributive, configure project with enabled `ENABLE_SNAP` CMake variable:

```
cmake -S . -B build -DENABLE_SNAP=ON
```

Visit the [cmake/enablers/packagers/template-project-snap-enabler.cmake](/cmake/enablers/packagers/template-project-snap-enabler.cmake) file to examine the details.
