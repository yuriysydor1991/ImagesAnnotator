## Enabling the lunasvg library (SVG)

In order to enable the [lunasvg](https://github.com/sammycage/lunasvg) library (the SVG vector image format) for the project by using the installed system one or by fetching it through the available connection set an `ON` value to the `ENABLE_LUNASVG` CMake variable.

```
# Inside the source root directory

cmake -S . -B build -DENABLE_LUNASVG=ON
```

See the `cmake/enablers/images/template-project-lunasvg-enabler.cmake` module for the targets (`lunasvg::lunasvg` for a system install, `lunasvg` for the FetchContent build) to link to your targets of interest.
