## Extensions

### Selecting the GUI toolkit (GTKmm3 / GTKmm4)

The graphical user interface lives under `src/gui` and ships two interchangeable
implementations plus the toolkit-agnostic shared code:

- `src/gui/gtkmm4` — the default GTKmm4 (GTK 4) GUI;
- `src/gui/gtkmm3` — the GTKmm3 (GTK 3) GUI;
- `src/gui/gtkmm-common` — code that is source-compatible with both toolkits
  (for example the `Gdk::Pixbuf` based image cropper).

Exactly one toolkit is compiled into the single application binary. It is chosen
at configure time by the mutually exclusive `USE_GTKMM3` / `USE_GTKMM4` CMake
options. `USE_GTKMM4` is enabled by default; enabling one option requires
disabling the other (CMake reports a fatal error if both or neither are set):

```
# default - GTKmm4
cmake -S . -B build

# GTKmm3 instead
cmake -S . -B build -DUSE_GTKMM3=ON -DUSE_GTKMM4=OFF
```

### UI implementation

The `src/gui/gtkmm4/ui/GtkmmWindow.ui` file contains the default (GTKmm4)
application's UI implementation; the GTKmm3 counterpart is
`src/gui/gtkmm3/ui/GtkmmWindow.ui` and may be edited with the
[Glade](https://en.wikipedia.org/wiki/Glade_Interface_Designer) application. The
GTKmm4 interface uses the GTK4 builder format together with a `Gio::Menu` menu
model rendered by a `Gtk::PopoverMenuBar`, and its menu entries are
`Gio::SimpleAction`s rather than menu-item widgets.

### Resource embedding

To avoid any install and run dependencies the `GtkmmWindow.ui` UI file and any
other registered resource are embedded into the binary. Resource embedding is
performed through the active toolkit's `ui/template.gresource.xml` GResource file
(for example `src/gui/gtkmm4/ui/template.gresource.xml`) resources of which are
compiled into the C-resource by a `glib-compile-resources` command. Register new
resources in that file in order to make them available in the binary executable.
The examples of the resource obtaining and usage in the C++ code side may be
examined in the default implementation of the `gtkmm4` component.

### Introducing custom command line parameters

In order to introduce some additional command line parameters for the binary look for the `CommandLineParser` class implementation. It contains command line parsing routines that are passed by `ApplicationFactory` class after the `main` function was called.

Add some additional custom fields into the `ApplicationContext` class in order to pass some custom command line flags and/or data to the `IApplication` interface abstract class descendants that will be created by the `ApplicationFactory` during command line arguments parse.

The GTKmm library also supports some of the command line parameters which are passed to the GTKmm application by default.
