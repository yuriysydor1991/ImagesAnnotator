## Continuous integration with GitLab CI/CD

The project ships a ready to use [GitLab CI/CD](https://docs.gitlab.com/ee/ci/) pipeline declaration at [.gitlab-ci.yml](/.gitlab-ci.yml). GitLab picks it up automatically, so a mirror of the repository is checked by it as soon as a runner is available for the project, without anything having to be configured in the project settings.

That root file holds only the declarations the whole pipeline shares - the stage list, the `workflow` rules that keep a branch push from producing a second pipeline next to the merge request one, the common `variables` and the `default` image and `before_script` - and pulls the jobs themselves in with `include: local:`, one file per stage:

- [.gitlab/ci/check.yml](/.gitlab/ci/check.yml) - `clang-format` and `repository-hygiene`;
- [.gitlab/ci/test.yml](/.gitlab/ci/test.yml) - `debug-build-and-test` and `release`;
- [.gitlab/ci/verify.yml](/.gitlab/ci/verify.yml) - `tests-coverage`, `cppcheck` and `clang-tidy`;
- [.gitlab/ci/package.yml](/.gitlab/ci/package.yml) - `deb-package` and `flatpak-package`.

The included paths are resolved from the repository root rather than from the directory of the including file, and GitLab merges the files into a single configuration before it validates the pipeline. The entry point itself has to stay at the repository root: GitLab looks for `.gitlab-ci.yml` there and nowhere else, and pointing it at another path is a per project setting (`Settings` -> `CI/CD` -> `General pipelines` -> `CI/CD configuration file`) that is stored in GitLab rather than in the repository - so a fork, a mirror or an import of the repository would run no pipeline at all until somebody set it there again.

The pipeline is the GitLab counterpart of the [misc/Jenkinsfile](/misc/Jenkinsfile) one and performs the same checks, spread over the `check`, `test`, `verify` and `package` stages:

- `clang-format` - configures with `-DENABLE_CLANGFORMAT=ON`, builds the `clang-format` target and fails when it modified any tracked file;
- `repository-hygiene` - fails when a tracked file contains git conflict markers;
- `debug-build-and-test` - configures with `-DCMAKE_BUILD_TYPE=Debug -DENABLE_UNIT_TESTS=ON -DENABLE_COMPONENT_TESTS=ON -DENABLE_SANITIZERS=ON -DMAX_LOG_LEVEL=5`, builds and runs the unit and the component test suites;
- `tests-coverage` - the analogue of the last Jenkins stage;
- `release` - a `Release` build followed by a trial `cmake --install`;
- `cppcheck`, `clang-tidy`, `deb-package` and `flatpak-package` - the four opt-in jobs.

The `debug-build-and-test` job is a `parallel: matrix` over the three selectable GUI toolkits - `gtkmm3` (the default), `gtkmm4` and `wxwidgets` - because each of them compiles its own `src/gui` subtree with its own test targets, and only the three of them together configure every test the project declares. The component tests are executed under an `xvfb-run` X server: they build real toolkit windows and self-skip when no display is available, so without it they would silently stay green instead of being run.

The `ctest` JUnit reports are handed to GitLab as `artifacts:reports:junit`, so the test results are shown per test in the pipeline and merge request views. The packaging jobs keep the produced `*.deb` / `*.flatpak` files as job artifacts.

The four opt-in jobs correspond to the `RUN_CPPCHECK`, `RUN_CLANG_TIDY`, `RUN_DEB_PACKAGER` and `RUN_FLATPAK_PACKAGER` Jenkins parameters. They are created as manual jobs in the ordinary runs and can be started

- by pressing their play button in the pipeline view,
- by running the pipeline with the matching variable set to `true` (`Build` -> `Pipelines` -> `Run pipeline`, then adding for example the `RUN_CPPCHECK` variable with the `true` value), which is the equivalent of the Jenkins parameters form,
- or automatically by a scheduled pipeline (`Build` -> `Pipeline schedules`), which enables all of them.

Every job runs in the `ubuntu:24.04` container image and installs its dependencies with `apt-get`, so the system copies of GoogleTest, nlohmann/json and libcurl are used instead of the Internet fallback of the project enablers (see the requirements sections of this documentation). To speed the repeated runs up, the compilations are cached with [ccache](https://ccache.dev/) through the `CMAKE_C_COMPILER_LAUNCHER` / `CMAKE_CXX_COMPILER_LAUNCHER` CMake variables and the GitLab `cache` keyword.

Note that the `flatpak-package` job needs a runner whose container is allowed to create user namespaces, which the `bubblewrap` sandbox of `flatpak-builder` is built on. It can therefore fail on a locked down shared runner.
