## Enabling the libcurl

In order to enable [libcurl](https://uk.wikipedia.org/wiki/CURL) for the project by using the installed system one or by fetching it by the available connection set an `ON` value to the `ENABLE_LIBCURL` CMake variable.

```
# Inside the source root directory

cmake -S . -B build -DENABLE_LIBCURL=ON
```
