## Enabling gprof profiler analysis

In order to perform the application profiler analysis with help of the `gprof` application enable it's support by setting the `ON` value for the `ENABLE_GPROF` CMake variable:

```
# Inside the source root directory

cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DENABLE_GPROF=ON
```

After configuring project with support for the `gprof` profiling facility build the `gprof-analyse` target:

```
# Inside the source root directory

cmake --build build --target gprof-analyze
```

Which will execute application and starts the `gprof` profiling analyze tool for `gmon.out` generated file under the project's build directory (for example `build/gmon.out`). The resulting analysis may be examined by `gprof-analyze.txt` under the project's build directory.

Profiling with the `gprof` may be enabled only with `Debug` build mode.
