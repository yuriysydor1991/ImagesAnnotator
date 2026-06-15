## Enabling vagrind's callgrind profiler analysis

In order to perform the application profiler analysis with help of the `valgrind` application enable it's support by setting the `ON` value for the `ENABLE_CALLGRIND` CMake variable:

```
# Inside the source root directory

cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DENABLE_CALLGRIND=ON
```

After configuring project with support for the `valgrind` profiling facility build the `callgrind` target:

```
# Inside the source root directory

cmake --build build --target callgrind
```

After the application run the Valgrind's `callgrind` module will generate the profiler analysis files called `callgrind.out.*` under the project's build directory. The `build/callgrind.out.*` files may be examined with help of the UI application called `kcachegrind`:

```
# Inside the source root directory

kcachegrind build/callgrind.out.*
```
