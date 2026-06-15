## Command line build

The project uses the CMake build system so a build command should be familiar to the most of the C++ developers around the world!

Of course, project needs to be `git clone`-ed and it's root directory must be opened in the terminal or in your IDE first. After that you may execute next commands:

```
# from the project root

cmake -B build -S . && cmake --build build
```

Which effectively will create a directory named `build` (it's already added to the `.gitignore` list), configure project using the CMake available in the system (see the [Requirements](/doc/sections/en_US/3-requirements.md) section of this `README.md` file) and finally builds all the targets available in the project.
