## Enabling and performing code formatting target

In order to make `clang-format` target available set the `ENABLE_CLANGFORMAT` variable value to `ON` during the project configuration stage (GNU/Linux based):

```
# inside the project root directory 

cmake -B build -S . -DENABLE_CLANGFORMAT=ON
```

To perform the whole project code format in accordance with available `misc/.clang-format` code formatter configuration execute next building command:

```
# inside the project root directory

cmake --build build --target clang-format
```

The `clang-format` target details may be examined and/or altered in the `cmake/enablers/template-project-clang-format-target.cmake` CMake submodule file.
