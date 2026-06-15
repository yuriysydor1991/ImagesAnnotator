### Disabling system GTest probe 

To disable the system available GTest framework assets usage set appropriate value to the `GTEST_TRY_SYSTEM_PROBE` CMake variable by executing command like (GNU/Linux based):

```
cmake -B build -S . -DENABLE_UNIT_TESTS=ON -DGTEST_TRY_SYSTEM_PROBE=OFF && cmake --build build
```

During command execution project build system will try to make GTest available through the Internet only for current project with specified version in the `cmake/enablers/template-project-GTest-enabler.cmake` file.
