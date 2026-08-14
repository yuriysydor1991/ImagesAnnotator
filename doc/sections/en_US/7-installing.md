
# Installing

Execute available install commands from the project's build directory.

## Default installation

In order to install generated executable (as shown previous) file into your's system binary default folder execute next command in the command line (GNU/Linux based):

```
# installs generated binary under the /usr/local/bin/ for example
# run from the project's build directory

sudo cmake --install .
```

Usually it's the `/usr/local/bin/` directory (on the Unix-like OS) which may be inaccessible from the `PATH` environment variable (e.g. can not be started as a regular command).

## Installation by the quick build scripts

The scripts of the [Quick build scripts](/doc/sections/en_US/5-38-quick-build-scripts.md) section perform the install step themselves when the `--install` parameter is given, so the whole configure, build and install cycle takes the single command:

```
# inside the project root directory

scripts/build/release.sh --install
```

The install step alone, against the already built `build/release` directory, is performed by its stage script:

```
# inside the project root directory

scripts/build/release-install.sh --install
```

Both of them install into the `/usr` prefix by the `sudo cmake --install` call, so the `sudo` password is asked for. Without the `--install` parameter the install step is skipped entirely and no password is ever asked.

## Custom installation path

To install binary into the system globally available directory add the `--prefix` parameter to the command above as next:

```
# replace the /usr path with our own if needed
# run from the project's build directory

sudo cmake --install . --prefix "/usr"
```

Examine the `PATH` environment variable to chose directory best suited for your current OS (execute `echo $PATH` in the terminal). Alternatively, any path may be specified.

## Documentation install

If project was configured to support the documentation install by the command which looks like next:

```
# inside the project build directory

cmake ../ -DENABLE_DOC_DOXYGEN=ON -DDOXYGEN_DO_INSTALL=ON
```

The installation command (for example, described in the [Default installation](#default-installation) section) will install the generated HTML documentation files into appropriate directories.
