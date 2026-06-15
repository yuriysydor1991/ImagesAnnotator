## Enabling WIX MSI package generation with cpack

In order to be able to generate the Microsoft Windows `.msi` installer file configure the project to support the `cpack` command feature with the `ENABLE_WIX` variable enabled:

```
# inside the project root directory

cmake -B build -S . -DCMAKE_BUILD_TYPE=Release -DENABLE_WIX=ON
```

Next, build all available targets required for the `.msi` package:

```
# inside the project root directory

cmake --build build
```

Finally, execute the `cpack` command inside the template project build directory:

```
# inside the project build directory

cpack -C Release
```

The package file should be generated inside the project build root directory. For example, if the project name wasn't changed and its version is `0.12.0` the package name may look like `ImagesAnnotator-0.12.0-win64.msi` (the exact architecture suffix depends on the build host).

In order to examine details of the WIX MSI package configuration visit the [cmake/enablers/packagers/template-project-wix-enabler.cmake](/cmake/enablers/packagers/template-project-wix-enabler.cmake) file.

The integration depends solely on `cmake` and `cpack` - no third-party application is invoked by the project's CMake module. CPack's built-in `WIX` generator handles the `.msi` package assembly internally, the same way the DEB generator handles `.deb` files.

The following cache variables may optionally be overridden in order to tune the generated installer:

- `TEMPLATE_PROJECT_WIX_UPGRADE_GUID` - stable GUID that uniquely identifies the product across versions (default: a placeholder `12345678-1234-1234-1234-123456789012`). **For a real release you must replace it with a project-unique GUID**, otherwise upgrade detection will collide with any other project still using the placeholder. A new GUID can be produced with PowerShell `[guid]::NewGuid()` or any GUID generator.
- `TEMPLATE_PROJECT_WIX_PROGRAM_MENU_FOLDER` - Windows Start Menu folder name where the installer creates shortcuts (default: project name).
- `TEMPLATE_PROJECT_WIX_CULTURES` - Semicolon-separated list of UI cultures used by the installer (default: `en-US`, e.g. `en-US;uk-UA`).
- `TEMPLATE_PROJECT_WIX_LICENSE_RTF` - Optional absolute path to a RTF or TXT license file shown by the installer.
- `TEMPLATE_PROJECT_WIX_PRODUCT_ICON` - Optional absolute path to a `.ico` file used as the MSI product icon.

In addition to those, the package vendor is taken from `PROJECT_MAINTAINER`, the maintainer contact from `PROJECT_MAINTAINER_EMAIL`, and the description from the project `DESCRIPTION`. The install root under `Program Files` is the `CMAKE_PROJECT_NAME`. There is no need to declare anything new in the project's CMake files in order to start producing installers - just turn the option on.

> **Runtime note.** Like the `DEB` generator which delegates to `dpkg-deb`, CPack's `WIX` generator delegates to the [WiX Toolset](https://wixtoolset.org/) (the `candle.exe` / `light.exe` tools). It is therefore expected to be run on Microsoft Windows with the WiX Toolset installed and available on the `PATH`. The CMake module itself is platform-agnostic - it only configures CPack and does not invoke any external program directly.
