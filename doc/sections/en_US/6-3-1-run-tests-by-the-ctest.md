### Run tests by the ctest

If enabled by the developer through the `ENABLE_UNIT_TESTS` variable and successfully build, the one may run all available test by the `ctest` command from a project build directory (GNU/Linux based):

```
# run from the project build directory

ctest
```

Alternatively, run the `ctest` command from any location by specifying the test dir (GNU/Linux based):

```
ctest --tests-dir /path/to/the/project/build/directory
```

The [debug-test.sh](/scripts/build/debug-test.sh) script of the [Quick build scripts](/doc/sections/en_US/5-38-quick-build-scripts.md) section performs the very same `ctest` call inside the `build/debug` directory, either on its own or as a step of the `Debug` build scripts, whenever the `--test` parameter is given (GNU/Linux based):

```
# inside the project root directory

scripts/build/debug-test.sh --test
```
