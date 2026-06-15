## Enabling the OpenJPEG library (JPEG 2000)

In order to enable the [OpenJPEG](https://www.openjpeg.org/) library (the JPEG 2000 image format) for the project by using the installed system one or by fetching it through the available connection set an `ON` value to the `ENABLE_OPENJPEG` CMake variable.

```
# Inside the source root directory

cmake -S . -B build -DENABLE_OPENJPEG=ON
```

See the `cmake/enablers/images/template-project-openjpeg-enabler.cmake` module for the target (`openjp2`) to link to your targets of interest.
