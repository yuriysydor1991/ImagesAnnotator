### Searching for the generated executable

To find executable in the project build directory execute the `find` command:

```
find . -name 'ImagesAnnotator'
```

The `ImagesAnnotator` is a default executable name. Replace it with your custom one if you change the name of the project in the root `CMakeLists.txt` file (the `CMAKE_PROJECT_NAME` and/or `PROJECT_BINARY_NAME` variable).
