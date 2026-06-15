## Enabling the dynamic memory check target with valgrind

In order to make the `valgrind` target available for the execution, configure project with the enabled `ENABLE_VALGRIND` variable like:

```
# inside the project root directory 

cmake -B build -S . -DENABLE_VALGRIND=ON
```

To perform the dynamic memory check on the application build run next command:

```
# inside the project root directory

cmake --build build --target valgrind
```

The `valgrind` target details may be examined and/or altered in the `cmake/enablers/template-project-valgrind-target.cmake` CMake submodule file.

**Warning! It will start application.** The dynamic memory check requires application to be started and go through the full cycle. Ensure that application execution will have a finite time.
