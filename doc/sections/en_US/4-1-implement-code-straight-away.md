## Implement code straight away!

To proceed the application implementation right away look for the `GtkmmIniter.cpp` and/or `GtkmmIniter.h` files of the corresponding class which is designed to accept the initial code of the application. The visual user interface may be developed in the [Glade](https://en.wikipedia.org/wiki/Glade_Interface_Designer) application by opening the `src/gtkmm3/ui/GtkmmWindow.ui` file.

**But do not forget about the SOLID principles and code decomposing!**

It's preferable to create other directories which would contain implemented components of the application and include them into the `GtkmmIniter` class implementation, rather than put all the code inside the `GtkmmIniter` class itself (which may be justified only for a trivial project).

Additional code may be introduced into the `Application` class or by creating a descendant of the `IApplication` interface class in order to provide variability at the executable level.
