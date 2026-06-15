## Enabling sanitizers

In order to enable the sanitizers to check application at the runtime, configure project with the command:

```
cmake -S . -B build -DENABLE_SANITIZERS=ON
```

For a general purpose sanitizers. See the `cmake/compile-options/sanitizers/template-project-sanitizers.cmake` to examine available list of the sanitizers.

Alternatively, enable the threads sanitizers with the command:


```
cmake -S . -B build -DENABLE_SANITIZERS_THREADS=ON
```

to check the possible data rises of the newly introduced multithreaded application code.

Do not combine the `ENABLE_SANITIZERS` and `ENABLE_SANITIZERS_THREADS`.

Any findings will be printed into the application' stdout/stderr after the build and run.
