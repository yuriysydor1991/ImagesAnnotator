## Project tests

Project already contains some simple test samples. Based on them you may implement full test of the introduced code. 

See the [Project build](/doc/sections/en_US/5-project-build.md) subsection to enable unit testing with CMake.

The project carries three kinds of tests, each behind an option of its own and all of them registered with `ctest`:

| Option | Tests |
| --- | --- |
| `ENABLE_UNIT_TESTS` | the `UTEST_*` GoogleTest executables, one class under test each, everything around it mocked |
| `ENABLE_COMPONENT_TESTS` | the `CTEST_*` GoogleTest executables, driving a whole component over the real collaborators it is built out of |
| `ENABLE_INTEGRATION_TESTS` | the `ITEST_*` shell scripts of [src/tests/integration](/src/tests/integration), driving a produced binary the way a user does |

The integration tests compile nothing: every case is a script started by `ctest` with the path of the binary it drives, so what it asserts on is the file that would be shipped. The [ImagesAnnotatorConverter](/src/tests/integration/ImagesAnnotatorConverter) ones convert real datasets with the converter binary and check that no GUI toolkit reached it - see [The ImagesAnnotatorConverter binary](/doc/sections/en_US/5-40-the-ImagesAnnotatorConverter-binary.md).
