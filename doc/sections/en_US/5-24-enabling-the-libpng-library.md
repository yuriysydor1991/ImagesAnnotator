## Enabling the libpng library

In order to enable the [libpng](http://www.libpng.org/pub/png/libpng.html) library (the PNG image format) for the project by using the installed system one or by fetching it through the available connection set an `ON` value to the `ENABLE_LIBPNG` CMake variable.

```
# Inside the source root directory

cmake -S . -B build -DENABLE_LIBPNG=ON
```

libpng depends on zlib, so keep `ENABLE_ZLIB=ON` (or a system zlib available) so the dependency resolves. See the `cmake/enablers/images/template-project-libpng-enabler.cmake` module for the targets (`PNG::PNG` for a system install, `png_static`/`png` for the FetchContent build) to link to your targets of interest.
