## Enabling the Docker container build and run

To be able to build and run the Docker container with current template project' target binary file enable it's CMake target with setting the `ENABLE_DOCKER` variable into value `ON` by using the next command:

```
# inside the project root directory

cmake -S . -B build -DENABLE_DOCKER=ON
```

After that there should be available the Docker build and run CMake target `docker-single-run` which may be triggered with the command:

```
# inside the project root directory

cmake --build build --target docker-single-run 
```

Examine the `cmake/enablers/dockerers/template-project-docker-single-runner-target.cmake` file for the `docker-single-run` target details.

### Starter scripts

The [scripts/docker](/scripts/docker) directory holds the shortcuts which perform both commands above, so the whole run takes a single command:

| Script | Starts the CMake target |
| --- | --- |
| [single-run.sh](/scripts/docker/single-run.sh) | `docker-single-run` |
| [single-build.sh](/scripts/docker/single-build.sh) | `docker-single-build` |

```
# inside the project root directory

scripts/docker/single-run.sh
```

Each of them configures the `build/release` directory with the `ENABLE_DOCKER` option set to `ON` and then builds the target of interest, so no `docker` command of its own is ever issued. The `docker-single-run` CMake target depends on the `docker-single-build` one, which means the [single-run.sh](/scripts/docker/single-run.sh) script builds the image as well and the [single-build.sh](/scripts/docker/single-build.sh) one is only needed to build the image without starting a container.

The scripts accept the very same parameters as the [Quick build scripts](/doc/sections/en_US/5-38-quick-build-scripts.md) ones, since they are their callers:

```
# inside the project root directory

# reuses the already configured build/release directory
scripts/docker/single-run.sh --no-reconfigure

# an additional CMake option for the configure step
scripts/docker/single-run.sh -DDOCKERFILE_SINGLE_RUN_BASE_IMAGE_VERSION=22.04
```

Keep in mind that the `ENABLE_DOCKER` option makes the configure step require the `docker` executable to be present in the system, so the scripts fail the configure step on a machine with no Docker installed (see the [Optional for the Docker container runs](/doc/sections/en_US/3-10-optional-for-docker-container-runs.md) requirements section).
