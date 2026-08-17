## Required tools for the GNU/Linux based OS

The project is built against three standalone libraries of its own, and none is
a distribution package: the
[ImagesAnnotator-DataDrivers](https://github.com/yuriysydor1991/ImagesAnnotator-DataDrivers.git)
annotations project database, the
[ImagesAnnotator-DataExporters](https://github.com/yuriysydor1991/ImagesAnnotator-DataExporters.git)
dataset exporters and the
[ImagesAnnotator-DataImporters](https://github.com/yuriysydor1991/ImagesAnnotator-DataImporters.git)
dataset importers. All three enablers only look them up with `find_package()`, so
all three have to be installed before the project is configured. The shipped
script builds and installs them in the order the data drivers dependency of the
other two needs:

```
misc/ci/install-annotator-libraries.sh
```

It installs into `/usr/local`; pass another prefix as its only argument and point
the configure at it with `-DCMAKE_PREFIX_PATH=<prefix>`. The script itself needs
the compiler, CMake and Git below. See [Enabling the ImagesAnnotator-DataDrivers
library](/doc/sections/en_US/5-36-enabling-the-ImagesAnnotator-DataDrivers-library.md)
[Enabling the ImagesAnnotator-DataExporters
library](/doc/sections/en_US/5-37-enabling-the-ImagesAnnotator-DataExporters-library.md)
and [Enabling the ImagesAnnotator-DataImporters
library](/doc/sections/en_US/5-39-enabling-the-ImagesAnnotator-DataImporters-library.md).

In order to build minimum project install the GCC C++ compiler with CMake and Git.

```
sudo apt install -y git g++ cmake
```

The project ships two interchangeable GUI implementations selected at configure
time by the mutually exclusive `USE_GTKMM3` / `USE_GTKMM4` options (see the
extensions section). `USE_GTKMM3` is the default, so install the GTKmm-3.0
development packages:

```
sudo apt install -y pkg-config libgtkmm-3.0-dev
```

To build the alternative GTKmm-4.0 GUI (`-DUSE_GTKMM4=ON`, which switches
`USE_GTKMM3` off automatically) install the GTKmm-4.0 development packages
instead (or in addition):

```
sudo apt install -y pkg-config libgtkmm-4.0-dev
```

To build the optional wxWidgets GUI instead (`-DUSE_WXWIDGETS=ON`, which overrides
the GTKmm pair) install the wxWidgets development packages, which provide the
`wx-config` / CMake `find_package(wxWidgets)` the enabler probes:

```
sudo apt install -y libwxgtk3.2-dev
```

To build the optional Qt6 GUI instead (`-DUSE_QT6=ON`, which likewise overrides
the GTKmm pair) install the Qt6 base development packages, which provide the
CMake package config `find_package(Qt6 COMPONENTS Core Gui Widgets)` the enabler
probes together with the `moc` and `rcc` tools it runs:

```
sudo apt install -y qt6-base-dev
```

To be able to develop UI in the specialized [Glade](https://en.wikipedia.org/wiki/Glade_Interface_Designer) IDE install it with the command:

```
sudo apt install -y glade
```
