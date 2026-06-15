## Changing the project and executable name

Change the project name by editing the `project()` call (which sets `CMAKE_PROJECT_NAME`) in the project's root `CMakeLists.txt` file, and/or change the executable / target name through the `PROJECT_BINARY_NAME` variable located at `cmake/template-project-misc-variables-declare.cmake`. It is recommended to do so the executable will represent your new application name instead of templated default one - the `ImagesAnnotator`.
