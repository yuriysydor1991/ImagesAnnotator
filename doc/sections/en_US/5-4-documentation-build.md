## Documentation build

Currently it's possible to auto-generate the project documentation by the Doxygen tool from the available sources comments.

To enable Doxygen documentation CMake-target during the project configure process call a command that sets the `ENABLE_DOC_DOXYGEN` CMake variable to the `ON` value (GNU/Linux based):

```
# inside the project root directory 

cmake -B build -S . -DENABLE_DOC_DOXYGEN=ON
```

Which effectively will create a directory named `build` inside the project root directory, enters it by a `cd` command and configures project to enable Doxygen documentation build.

Finally build the documentation by executing the command:

```
# inside the project root directory

cmake --build build --target Doxygen-doc
```

Which in turn will generate the `doc/ImagesAnnotator-html` directory (already added to the `.gitignore` file) which will contain the HTML-type documentation. In order to open and examine generated documentation open the `doc/ImagesAnnotator-html/index.html` file. The `ImagesAnnotator-html` directory name will change if changed default executable name for the project by setting a new value for the `PROJECT_BINARY_NAME` variable in the root `CMakeLists.txt` or the `DOXYGEN_OUT_HTML_NAME` which in turn set the whole name for the directory.

The `doc/Doxyfile.in` file contains all available Doxygen configuration parameters which may be changed in order to change the documentation output.
