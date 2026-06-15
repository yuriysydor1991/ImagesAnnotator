## Extensions

### UI implementation

The project's `src/gtkmm3/ui/GtkmmWindow.ui` file contains the general application's UI default implementation. Open it with the [Glade](https://en.wikipedia.org/wiki/Glade_Interface_Designer) application in order to edit project's UI implementation.

### Resource embedding

To avoid any install and run dependencies the `src/gtkmm3/ui/GtkmmWindow.ui` UI file and any other registered resource are embedding into the binary. Resource embedding is performed through the `src/gtkmm3/ui/template.gresource.xml` GResource file resources of which are compiled into the C-resource by a `glib-compile-resources` command. Register new resources in the `src/gtkmm3/ui/template.gresource.xml` file in order to make them available in the binary executable. The examples of the resource obtaining and usage in the C++ code side may be examined in the default implementation of the `gtkmm3` component.

### Introducing custom command line parameters

In order to introduce some additional command line parameters for the binary look for the `CommandLineParser` class implementation. It contains command line parsing routines that are passed by `ApplicationFactory` class after the `main` function was called.

Add some additional custom fields into the `ApplicationContext` class in order to pass some custom command line flags and/or data to the `IApplication` interface abstract class descendants that will be created by the `ApplicationFactory` during command line arguments parse.

The GTKmm library also supports some of the command line parameters which are passed to the GTKmm application by default.
