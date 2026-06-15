### Enabling unit and component testing

To enable project unit test availability (for building and running) reconfigure it with enabled `ENABLE_UNIT_TESTS` variable as follows (GNU/Linux based):

```
# from the project root

cmake -B build -S . -DENABLE_UNIT_TESTS=ON && cmake --build build
```

The `ctest` may be used afterwards.

Component tests are enabled the same way through the `ENABLE_COMPONENT_TESTS` variable and may be combined with the unit tests:

```
# from the project root

cmake -B build -S . -DENABLE_UNIT_TESTS=ON -DENABLE_COMPONENT_TESTS=ON && cmake --build build
```

The component tests of the GTKmm widgets require an available display server to run; they are skipped automatically in a headless environment.
