## Enabling the libavif library (AVIF)

In order to enable the [libavif](https://github.com/AOMediaCodec/libavif) library (the AVIF / AV1 image format) for the project by using the installed system one or by fetching it through the available connection set an `ON` value to the `ENABLE_LIBAVIF` CMake variable.

```
# Inside the source root directory

cmake -S . -B build -DENABLE_LIBAVIF=ON
```

A codec is required to actually encode/decode AVIF; for the FetchContent build enable one (e.g. `-DAVIF_CODEC_AOM=ON` or `-DAVIF_CODEC_DAV1D=ON`). See the `cmake/enablers/images/template-project-libavif-enabler.cmake` module for the target (`avif`) to link to your targets of interest.
