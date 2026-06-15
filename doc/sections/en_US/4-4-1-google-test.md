### Google Test

Currently test samples are based on the GTest framework. GTest framework by itself becomes available by the `FetchContent_Declare`/`FetchContent_MakeAvailable` CMake-commands if no system GTest is available. 

Of course, the project's CMakeLists.txt files are probing the GTest only if tests are enabled by `ENABLE_UNIT_TESTS` CMake variable. System GTest probe may be turned OFF by setting appropriate value to the `GTEST_TRY_SYSTEM_PROBE` CMake variable.

Look for a `cmake/enablers/template-project-GTest-enabler.cmake` to see details or change GTest version etc..
