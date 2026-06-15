## Minimal possible versions

The project intentionally declares the lowest possible versions of the build tools and the language standard it relies on - for example the `cmake_minimum_required(VERSION 3.13)` requirement and the C++ `17` standard (`CMAKE_CXX_STANDARD`) set in the root `CMakeLists.txt`. Keeping these requirements as low as practical lets the template be configured, built and installed across the widest possible range of environments, including older distributions and toolchains.

This is a deliberate default and not a hard limit. You may, of course, raise any of the used versions whenever your target environment or your application needs it - bump the `cmake_minimum_required(...)` value, set a newer `CMAKE_CXX_STANDARD` (for example `20` or `23`), or require newer versions of any third-party dependency. Increasing a version only narrows the set of systems that are able to build the project, it does not change the template structure itself.
