## Enabling the static code analyzer target with cppcheck

In order to make the cppcheck target available for the execution, configure project with enabled `ENABLE_CPPCHECK` variable:

```
# inside the project root directory 

cmake -B build -S . -DENABLE_CPPCHECK=ON
```

And to perform the actual static code analysis by itself run the `cppcheck` target for the build:

```
# inside the project root directory

cmake --build build --target cppcheck
```

The `cppcheck` target details may be examined and/or altered in the `cmake/enablers/template-project-cppcheck-target.cmake` CMake submodule file.
