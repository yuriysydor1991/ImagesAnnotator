## Extensions

### Selecting the GUI toolkit (GTKmm3 / GTKmm4 / wxWidgets / Qt6)

The graphical user interface lives under `src/gui` and ships two interchangeable
GTKmm implementations, two optional non-GTKmm ones, plus the toolkit-agnostic
shared code:

- `src/gui/gtkmm3` — the default GTKmm3 (GTK 3) GUI;
- `src/gui/gtkmm4` — the GTKmm4 (GTK 4) GUI;
- `src/gui/wxwidgets` — an optional wxWidgets GUI, a feature-parity analogue of
  the GTKmm3 one built by hand-coding the window rather than loading a Glade
  file, and painting the canvas with `wxDC` instead of cairo (see below);
- `src/gui/qt6` — an optional Qt6 (Qt Widgets) GUI, likewise a feature-parity
  analogue of the GTKmm3 one: the window is built in code, the canvas is painted
  with `QPainter` and the resources are embedded through a Qt `.qrc` instead of a
  GResource (see below);
- `src/gui/gtkmm-common` — code that is source-compatible with both GTKmm
  toolkits (for example the `Gdk::Pixbuf` based image cropper) together with the
  single copy of the toolkit-agnostic GResource payload in
  `src/gui/gtkmm-common/ui/resources`.

Exactly one toolkit is compiled into the single application binary. It is chosen
at configure time by the mutually exclusive `USE_GTKMM3` / `USE_GTKMM4` CMake
options. Only the option you care about has to be given — whichever one is left
unspecified is derived from it, so the pair can never end up inconsistent:

```
# nothing specified - GTKmm3, the default
cmake -S . -B build

# GTKmm4 instead: USE_GTKMM3 is switched off automatically
cmake -S . -B build -DUSE_GTKMM4=ON

# equivalent, and equally valid
cmake -S . -B build -DUSE_GTKMM3=OFF
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
define picked up by the toolkit-neutral `src/gui/GUIIniter.h` facade and the
GTKmm runtime package name used by the DEB packager. The option pair itself is
resolved once in `cmake/template-project-misc-variables-declare.cmake`, which is
the only place in the project that branches on it.

The optional wxWidgets GUI is selected by a separate `USE_WXWIDGETS` option
(default `OFF`). When it is `ON` it overrides the GTKmm pair — it forces both
`USE_GTKMM3` and `USE_GTKMM4` off (leaving their cache entries untouched, so
dropping `-DUSE_WXWIDGETS` restores the previous GTKmm choice) and builds the
`src/gui/wxwidgets` component instead:

```
# the wxWidgets GUI instead of GTKmm
cmake -S . -B build -DUSE_WXWIDGETS=ON
```

Its enabler, `cmake/enablers/template-project-wxWidgets-enabler.cmake`, probes
wxWidgets with `find_package(wxWidgets)` (there is no pkg-config `.pc` file for
it) and republishes the very same neutral `GTKMM_*` include / library / DEB
dependency variables and `GTKMM_GUI_SUBDIR` the GTKmm enablers do, so nothing
downstream branches on the toolkit beyond the single `add_subdirectory` switch in
`src/gui/CMakeLists.txt`. The wxWidgets component mirrors the `gtkmm3`
decomposition one-to-one in the `imagesAnnotatorWx` namespace and reuses the
toolkit-agnostic business, event and helper layers unchanged; it maps the GTKmm
calls to wx idioms (a hand-coded `wxFrame` with typed getters in place of the
Glade `.ui` + `Gtk::Builder`; a `wxScrolledWindow` painted with `wxDC` in place
of the cairo `Gtk::DrawingArea`; `wxImage`/`wxBitmap` for `Gdk::Pixbuf`; a
`wxListCtrl`/`wxListBox` rebuilt from the search-filtered subset for the
`Gtk::ListBox`es; `Bind()` for the GTK signals; `wxFileDialog`/`wxMessageDialog`/
`wxAboutBox` for the dialogs; a busy cursor for the spinner; and a `wxImage`
based `WxImageCropperProvider` for the PyTorch-Vision export cropper). It embeds
the shared logo as a generated C byte array rather than through a GResource, and
its `APPLICATION_ID` is `ua.org.kytok.template.wxwidgets.<binary>`. Note that the
wx build loads the image formats `wxImage` supports (PNG / JPEG / BMP / GIF /
TIFF / …); formats only gdk-pixbuf decodes are unavailable under it.

The optional Qt6 GUI is selected the same way, by its own `USE_QT6` option
(default `OFF`), which overrides the GTKmm pair exactly as `USE_WXWIDGETS` does
— and is mutually exclusive with it, so enabling both at once is a fatal error:

```
# the Qt6 GUI instead of GTKmm
cmake -S . -B build -DUSE_QT6=ON
```

Its enabler, `cmake/enablers/template-project-Qt6-enabler.cmake`, probes Qt with
`find_package(Qt6 COMPONENTS Core Gui Widgets)` (Qt ships no pkg-config `.pc`
files either) and republishes the very same neutral `GTKMM_*` include / library /
DEB dependency variables and `GTKMM_GUI_SUBDIR` the other enablers do. It
additionally switches on `CMAKE_AUTOMOC` and `CMAKE_AUTORCC` at project scope,
so Qt's meta-object compiler runs over the `Q_OBJECT` classes and `rcc` compiles
the resource file. `CMAKE_AUTOUIC` is deliberately left off: the Qt window is
built in code, and the only `*.ui` files in the repository are the GTK builder
XML of the GTKmm components, which `uic` must never touch.

The Qt6 component mirrors the `gtkmm3` decomposition one-to-one in the
`imagesAnnotatorQt6` namespace and reuses the toolkit-agnostic business, event
and helper layers unchanged. It maps the GTKmm calls to Qt idioms: a hand-coded
`QMainWindow` in place of the Glade `.ui` + `Gtk::Builder` — but the widgets keep
the GTKmm builder ids as their `QObject::objectName()`s, so
`WindowLoader::all_widget_are_valid()` still resolves every id of
`WindowDataContext`, through `QObject::findChild()` instead of through the
builder; a `QWidget` painted with `QPainter` inside a `QScrollArea` in place of
the cairo `Gtk::DrawingArea`; `QImage`/`QPixmap` for `Gdk::Pixbuf`; `QListWidget`s
rebuilt from the search-filtered subset for the `Gtk::ListBox`es;
`QObject::connect()` for the GTK signals, with the canvas re-emitting its mouse
and wheel events as Qt signals; `QKeySequence` accelerators on the `QAction`s for
the `Gtk::AccelGroup`; `QFileDialog`/`QMessageBox`/`QInputDialog` for the dialogs;
a `Qt::WaitCursor` override for the spinner; a `QShortcut` for the Up/Down image
navigation keys; a `QCloseEvent` filter for the window-close veto; and a `QImage`
based `QtImageCropperProvider` for the PyTorch-Vision export cropper. The
resources are embedded through `src/gui/qt6/ui/template.qrc` — the Qt analogue of
the GResource, aliasing the very same shared logo file the GTKmm builds embed —
and the GTKmm CSS becomes the Qt Style Sheet
`src/gui/qt6/ui/resources/css/main-window.qss`. Its `APPLICATION_ID` is
`ua.org.kytok.template.qt6.<binary>`, pinned at runtime with
`QGuiApplication::setDesktopFileName()` (the Qt counterpart of
`Glib::set_prgname()`). Note that the Qt build loads the image formats Qt's image
plugins support (PNG / JPEG / BMP / GIF / TIFF / WEBP / …); formats only
gdk-pixbuf decodes are unavailable under it.

### UI implementation

The `src/gui/gtkmm3/ui/GtkmmWindow.ui` file contains the default (GTKmm3)
application's UI implementation and may be edited with the
[Glade](https://en.wikipedia.org/wiki/Glade_Interface_Designer) application; the
GTKmm4 counterpart is `src/gui/gtkmm4/ui/GtkmmWindow.ui`. The GTKmm4 interface
uses the GTK4 builder format together with a `Gio::Menu` menu model rendered by a
`Gtk::PopoverMenuBar`, and its menu entries are `Gio::SimpleAction`s rather than
menu-item widgets.

### Resource embedding

To avoid any install and run dependencies the `GtkmmWindow.ui` UI file and any
other registered resource are embedded into the binary. Resource embedding is
performed through the active toolkit's `ui/template.gresource.xml` GResource file
(for example `src/gui/gtkmm4/ui/template.gresource.xml`) resources of which are
compiled into the C-resource by a `glib-compile-resources` command. Register new
resources in that file in order to make them available in the binary executable.
The examples of the resource obtaining and usage in the C++ code side may be
examined in the default implementation of the `gtkmm3` component.

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
