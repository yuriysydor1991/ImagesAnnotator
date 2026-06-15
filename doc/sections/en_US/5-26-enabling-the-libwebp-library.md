## Enabling the libwebp library

In order to enable the [libwebp](https://developers.google.com/speed/webp) library (the WEBP image format) for the project by using the installed system one or by fetching it through the available connection set an `ON` value to the `ENABLE_LIBWEBP` CMake variable.

```
# Inside the source root directory

cmake -S . -B build -DENABLE_LIBWEBP=ON
```

See the `cmake/enablers/images/template-project-libwebp-enabler.cmake` module for the targets (`WebP::webp` for a system install, `webp`/`webpdecoder` for the FetchContent build) to link to your targets of interest.
