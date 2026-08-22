## Quick build scripts

The [scripts/build](/scripts/build) directory holds the shell scripts which perform the CMake configure, build and install steps of the project, so a whole `Release` or `Debug` cycle takes a single command instead of the separate CMake calls described in the [Command line build](/doc/sections/en_US/5-2-command-line-build.md) section.

Every script keeps its build tree under the project `build` directory: the `Release` one at `build/release` and the `Debug` one at `build/debug`.

The project does not build without the three standalone libraries it is built against, and none of them is fetched by the configure step (see the [Enabling the ImagesAnnotator-DataDrivers library](/doc/sections/en_US/5-36-enabling-the-ImagesAnnotator-DataDrivers-library.md), the [Enabling the ImagesAnnotator-DataExporters library](/doc/sections/en_US/5-37-enabling-the-ImagesAnnotator-DataExporters-library.md) and the [Enabling the ImagesAnnotator-DataImporters library](/doc/sections/en_US/5-39-enabling-the-ImagesAnnotator-DataImporters-library.md) sections), so they are to be installed once before any of the scripts below is started:

```
# inside the project root directory

misc/ci/install-annotator-libraries.sh
```

### Entry point scripts

Each of the scripts below performs the configure, the build and the install steps in a row. The `Debug` ones which build the whole project perform the test step as well, in between of the build and the install ones:

| Script | Performs |
| --- | --- |
| [release.sh](/scripts/build/release.sh) | the `Release` build |
| [debug.sh](/scripts/build/debug.sh) | the `Debug` build with the tests, the documentation, the `cppcheck`, the `valgrind` and the `callgrind` enabled |
| [debug-cppcheck.sh](/scripts/build/debug-cppcheck.sh) | the `Debug` build of the `cppcheck` target |
| [debug-valgrind.sh](/scripts/build/debug-valgrind.sh) | the `Debug` build of the `valgrind` target |
| [debug-callgrind.sh](/scripts/build/debug-callgrind.sh) | the `Debug` build of the `callgrind` target |
| [debug-clang-tidy.sh](/scripts/build/debug-clang-tidy.sh) | the `Debug` build with the `ENABLE_CLANG_TIDY` option |
| [debug-sanitizers.sh](/scripts/build/debug-sanitizers.sh) | the `Debug` build with the `ENABLE_SANITIZERS` option |
| [debug-sanitizers-threads.sh](/scripts/build/debug-sanitizers-threads.sh) | the `Debug` build with the `ENABLE_SANITIZERS_THREADS` option |
| [debug-compiler-analyzer.sh](/scripts/build/debug-compiler-analyzer.sh) | the `Debug` build with the `ENABLE_COMPILER_CODE_ANALYZER` option |

The [debug-cppcheck.sh](/scripts/build/debug-cppcheck.sh), the [debug-valgrind.sh](/scripts/build/debug-valgrind.sh) and the [debug-callgrind.sh](/scripts/build/debug-callgrind.sh) scripts build a single CMake target which the test executables are not a part of, so those three perform no test step at all.

A complete `Release` build takes the single command:

```
# inside the project root directory

scripts/build/release.sh
```

### Stage scripts

Every entry point script above is a chain of the stage scripts below, which may be started on their own when a single step is needed:

| Script | Performs |
| --- | --- |
| [release-configure.sh](/scripts/build/release-configure.sh), [debug-configure.sh](/scripts/build/debug-configure.sh) | the `cmake -B <build dir> -S .` configure step |
| [release-build.sh](/scripts/build/release-build.sh), [debug-build.sh](/scripts/build/debug-build.sh) | the `cmake --build <build dir>` build step |
| [debug-test.sh](/scripts/build/debug-test.sh) | the `ctest` test step inside the `Debug` build directory |
| [release-install.sh](/scripts/build/release-install.sh), [debug-install.sh](/scripts/build/debug-install.sh) | the `sudo cmake --install <build dir>` install step |

### Accepted parameters

The scripts accept the next parameters of their own, which are never passed to the CMake:

| Parameter | Effect |
| --- | --- |
| `--no-reconfigure` | Keeps the existing build directory and skips the configure step entirely. Without the parameter the configure step **erases** the build directory before configuring the project anew. |
| `--test` | Performs the test step, which starts the `ctest` command inside the `Debug` build directory. Without the parameter the test step is skipped. The parameter belongs to the `Debug` scripts alone, as the tests are enabled by their configure step only. |
| `--install` | Performs the install step. Without the parameter the install step is skipped, so no `sudo` password is ever asked for a plain build. |

Any `-D<variable>=<value>` argument is passed to the configure step, which makes every CMake option of this documentation available to the scripts:

```
# inside the project root directory

# the Release build with the DEB package generation enabled
scripts/build/release.sh -DENABLE_DEB=ON

# the same build reusing the already configured build directory,
# followed by the installation into the /usr prefix
scripts/build/release.sh --no-reconfigure --install

# the Debug build with the whole test suite performed right after it
scripts/build/debug.sh --test
```

The three ImagesAnnotator libraries are found the very same way when they were installed outside of the CMake default search prefixes - which is what `misc/ci/install-annotator-libraries.sh <prefix>` does when it is given one - since `CMAKE_PREFIX_PATH` is a CMake variable as well:

```
# inside the project root directory

misc/ci/install-annotator-libraries.sh "$HOME/.local"

scripts/build/release.sh -DCMAKE_PREFIX_PATH="$HOME/.local"
```

The scripts install nothing of their own: they configure, build and (with `--install`) install **this** project only, so the three libraries stay the one step to run once beforehand.

The GUI toolkit is selected the very same way, since it is a CMake option too (see the [Extensions](/doc/sections/en_US/4-5-extensions.md) section). With none of them given the scripts build the default GTKmm3 GUI:

```
# inside the project root directory

scripts/build/release.sh -DUSE_GTKMM4=ON
scripts/build/release.sh -DUSE_WXWIDGETS=ON
scripts/build/release.sh -DUSE_QT6=ON
```

The rest of the arguments are passed to the build, the test and the install steps, so an option which belongs to a single step is to be given to that stage script rather than to an entry point script:

```
# inside the project root directory

# builds a single target inside the already configured Debug build directory
scripts/build/debug-build.sh --target cppcheck

# runs a part of the test suite inside the already built Debug build directory
scripts/build/debug-test.sh --test -R "CTEST_FormatsConverter\."
```

The `cmake -B` configure call accepts neither the `--target` nor the `--verbose` option, so passing such an argument to an entry point script fails the configure step.
