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
