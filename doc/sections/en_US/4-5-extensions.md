## Extensions

### Selecting the GUI toolkit (GTKmm3 / GTKmm4)

The graphical user interface lives under `src/gui` and ships two interchangeable
implementations plus the toolkit-agnostic shared code:

- `src/gui/gtkmm4` — the default GTKmm4 (GTK 4) GUI;
- `src/gui/gtkmm3` — the GTKmm3 (GTK 3) GUI;
- `src/gui/gtkmm-common` — code that is source-compatible with both toolkits
  (for example the `Gdk::Pixbuf` based image cropper) together with the single
  copy of the toolkit-agnostic GResource payload in
  `src/gui/gtkmm-common/ui/resources`.

Exactly one toolkit is compiled into the single application binary. It is chosen
at configure time by the mutually exclusive `USE_GTKMM3` / `USE_GTKMM4` CMake
options. Only the option you care about has to be given — whichever one is left
unspecified is derived from it, so the pair can never end up inconsistent:

```
# nothing specified - GTKmm4, the default
cmake -S . -B build

# GTKmm3 instead: USE_GTKMM4 is switched off automatically
cmake -S . -B build -DUSE_GTKMM3=ON

# equivalent, and equally valid
cmake -S . -B build -DUSE_GTKMM4=OFF
```

Passing both options is still accepted as long as exactly one of them is `ON`;
CMake reports a fatal error when both are `ON` or both are `OFF`. Re-running
`cmake` in an existing build tree keeps the toolkit that tree was configured
with, and passing a single option again switches it — for example
`cmake -S . -B build -DUSE_GTKMM4=ON` moves a GTKmm3 tree over to GTKmm4.

Everything that follows from the choice is declared by the matching enabler,
`cmake/enablers/template-project-Gtkmm3-enabler.cmake` or
`cmake/enablers/template-project-Gtkmm4-enabler.cmake`: the `pkg-config` probe
that publishes the `GTKMM_*` variables for the whole `src` subtree, the
`src/gui` subdirectory to compile, the `USE_GTKMM3` / `USE_GTKMM4` preprocessor
define picked up by the toolkit-neutral `src/gui/GuiIniter.h` facade and the
GTKmm runtime package name used by the DEB packager. The option pair itself is
resolved once in `cmake/template-project-misc-variables-declare.cmake`, which is
the only place in the project that branches on it.

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

The `<file>` entries of both `template.gresource.xml` files are resolved against
two directories that the toolkit's `ui/CMakeLists.txt` passes to
`glib-compile-resources` as `--sourcedir`: the toolkit's own `ui` directory
first, then the shared `src/gui/gtkmm-common/ui` one. Toolkit-specific resources
such as `GtkmmWindow.ui` therefore stay next to their toolkit, while resources
that are byte-identical for GTKmm3 and GTKmm4 — the CSS and the images — are
stored only once under `src/gui/gtkmm-common/ui/resources` and embedded by both.
Add a new shared resource to that directory and register it in both
`template.gresource.xml` files under the very same relative path; add a
toolkit-specific one to `src/gui/gtkmm3/ui` or `src/gui/gtkmm4/ui` instead. The
resource prefix stays per toolkit (`.../gtkmm3/...` versus `.../gtkmm4/...`), so
the runtime paths used by `WindowDataContext` are unaffected by the sharing.

### Introducing custom command line parameters

In order to introduce some additional command line parameters for the binary look for the `CommandLineParser` class implementation. It contains command line parsing routines that are passed by `ApplicationFactory` class after the `main` function was called.

Add some additional custom fields into the `ApplicationContext` class in order to pass some custom command line flags and/or data to the `IApplication` interface abstract class descendants that will be created by the `ApplicationFactory` during command line arguments parse.

The GTKmm library also supports some of the command line parameters which are passed to the GTKmm application by default.
