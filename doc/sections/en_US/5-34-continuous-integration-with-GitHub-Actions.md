## Continuous integration with GitHub Actions

The project ships a ready to use [GitHub Actions](https://docs.github.com/en/actions) workflow at [.github/workflows/ci.yml](/.github/workflows/ci.yml). GitHub picks it up automatically, so a fork of the repository is checked by it without any additional configuration.

The workflow is the GitHub counterpart of the [misc/Jenkinsfile](/misc/Jenkinsfile) pipeline and performs the same checks:

- `Code format check (clang-format)` - configures with `-DENABLE_CLANGFORMAT=ON`, builds the `clang-format` target and fails when it modified any tracked file;
- `Repository hygiene` - fails when a tracked file contains git conflict markers;
- `Debug build and tests` - configures with `-DCMAKE_BUILD_TYPE=Debug -DENABLE_UNIT_TESTS=ON -DENABLE_COMPONENT_TESTS=ON -DENABLE_SANITIZERS=ON -DMAX_LOG_LEVEL=5`, builds and runs the unit and the component test suites;
- `Every declared test is executed by this pipeline` - the analogue of the last Jenkins stage;
- `Release build and install` - a `Release` build followed by a trial `cmake --install`;
- `cppcheck`, `clang-tidy`, `deb package` and `flatpak package` - the four opt-in jobs.

The `Debug build and tests` job is a matrix over the three selectable GUI toolkits - `gtkmm3` (the default), `gtkmm4` and `wxwidgets` - because each of them compiles its own `src/gui` subtree with its own test targets, and only the three of them together configure every test the project declares. The component tests are executed under an `xvfb-run` X server: they build real toolkit windows and self-skip when no display is available, so without it they would silently stay green instead of being run.

Each toolkit job uploads its `ctest` JUnit reports as a workflow artifact, and the packaging jobs upload the produced `*.deb` / `*.flatpak` files.

The four opt-in jobs correspond to the `RUN_CPPCHECK`, `RUN_CLANG_TIDY`, `RUN_DEB_PACKAGER` and `RUN_FLATPAK_PACKAGER` Jenkins parameters. They are skipped in the ordinary push and pull request runs and can be started

- from the `Actions` tab of the repository, by choosing the `CI` workflow, pressing `Run workflow` and ticking the wanted checkboxes,
- or automatically by the weekly scheduled run, which enables all of them.

The dependencies are installed by the jobs themselves with `apt-get`, so the system copies of GoogleTest, nlohmann/json and libcurl are used instead of the Internet fallback of the project enablers (see the requirements sections of this documentation). To speed the repeated runs up, the compilations are cached with [ccache](https://ccache.dev/) through the `CMAKE_C_COMPILER_LAUNCHER` / `CMAKE_CXX_COMPILER_LAUNCHER` CMake variables and the `actions/cache` action.
