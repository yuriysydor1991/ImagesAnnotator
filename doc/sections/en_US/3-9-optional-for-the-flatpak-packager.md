## Optional for the flatpak packager

In order to generate the project flatpak package install the `flatpak-builder` command:

```
sudo apt install -y flatpak flatpak-builder
```

You'll also need the runtime and the SDK the flatpak manifest of the selected GUI toolkit names. The `flatpak` target installs them into the user installation from the flathub by itself, but they may be installed beforehand as well, for the default GTKmm3 manifest by a command that may look like this:

```
flatpak remote-add --user --if-not-exists flathub https://dl.flathub.org/repo/flathub.flatpakrepo
flatpak install --user flathub org.gnome.Platform//48 org.gnome.Sdk//48
```

The GTKmm4 and the wxWidgets manifests ask for the `org.gnome.Platform//49 org.gnome.Sdk//49` pair instead, the Qt6 one for the `org.kde.Platform//6.10 org.kde.Sdk//6.10` and the converter only one for the `org.freedesktop.Platform//24.08 org.freedesktop.Sdk//24.08`. Consult the flatpak documentation on how to list all available options.

Look for the details at the [Enabling the flatpak package generation support](/doc/sections/en_US/5-11-enabling-the-flatpak-package-generation-support.md).
