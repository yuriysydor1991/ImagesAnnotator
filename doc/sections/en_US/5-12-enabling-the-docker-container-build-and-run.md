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
