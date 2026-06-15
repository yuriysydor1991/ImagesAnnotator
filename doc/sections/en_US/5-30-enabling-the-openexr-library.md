## Enabling the OpenEXR library (EXR / HDR)

In order to enable the [OpenEXR](https://openexr.com/) library (the EXR high dynamic range image format) for the project by using the installed system one or by fetching it through the available connection set an `ON` value to the `ENABLE_OPENEXR` CMake variable.

```
# Inside the source root directory

cmake -S . -B build -DENABLE_OPENEXR=ON
```

The FetchContent build additionally pulls the Imath dependency automatically. See the `cmake/enablers/images/template-project-openexr-enabler.cmake` module for the target (`OpenEXR::OpenEXR`) to link to your targets of interest.
