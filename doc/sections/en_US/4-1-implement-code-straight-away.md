## Implement code straight away!

To proceed the application implementation right away look for the `GtkmmIniter.cpp` and/or `GtkmmIniter.h` files of the corresponding class which is designed to accept the initial code of the application. The GUI implementations live under `src/gui`: the default GTKmm4 one in `src/gui/gtkmm4`, the GTKmm3 one in `src/gui/gtkmm3` and the toolkit-agnostic shared code in `src/gui/gtkmm-common`. The GTKmm3 visual user interface (`src/gui/gtkmm3/ui/GtkmmWindow.ui`) may be developed in the [Glade](https://en.wikipedia.org/wiki/Glade_Interface_Designer) application; the GTKmm4 interface (`src/gui/gtkmm4/ui/GtkmmWindow.ui`) uses the GTK4 builder format together with a `Gio::Menu` menu model.

**But do not forget about the SOLID principles and code decomposing!**

It's preferable to create other directories which would contain implemented components of the application and include them into the `GtkmmIniter` class implementation, rather than put all the code inside the `GtkmmIniter` class itself (which may be justified only for a trivial project).

Additional code may be introduced into the `Application` class or by creating a descendant of the `IApplication` interface class in order to provide variability at the executable level.
