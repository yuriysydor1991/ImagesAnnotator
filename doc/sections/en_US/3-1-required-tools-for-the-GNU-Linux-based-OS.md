## Required tools for the GNU/Linux based OS

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

To be able to develop UI in the specialized [Glade](https://en.wikipedia.org/wiki/Glade_Interface_Designer) IDE install it with the command:

```
sudo apt install -y glade
```
