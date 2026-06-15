**The images annotator application**

# Goal of the ImagesAnnotator

The goal of the image annotator is to provide information about the provided images' located objects in order to supply annotated data to the learning networks with next load into libraries like [OpenCV](https://github.com/opencv/opencv).

Current project also serves research purposes. So may look a bit over engineered.

Fast started with help of the C++ templated project located at [https://github.com/yuriysydor1991/cpp-app-template](https://github.com/yuriysydor1991/cpp-app-template)

Translations of the current README.md may be found in the `doc` subdirectory:
- [doc/README.uk_UA.md](doc/README.uk_UA.md)

**WARNING! Project is in the alpha stage! Save and backup your data often!**

See more at the [kytok.org.ua](http://www.kytok.org.ua/)

💵 Donate at [http://kytok.org.ua/page/pozertvy](http://kytok.org.ua/page/pozertvy)

# Usage

The main application window looks like next.

![ImagesAnnotator window view example](doc/images/ImagesAnnotator-0.11.0.png)

On the top located regular menu:

- `Images` to import images directories,
- `Annotations` to manage current project,
- `Help` for help options.

Next in line located buttons to open and append new image containing directory (duplicated functionality from `Images` menu), increase and decrease zoom on the current opened image, the next and previous image if any from the opened images in the queue (left list panel), and finally duplicated button to open already existing annotations DB (currently only internal JSON format is supported).

The images that are listed on the green background on the left panel indicating having at least one annotation.

On the right panel there are list all the annotations in the project list box and the currently opened image annotations only list box. The `Copy annotation name` button designed to copy text from a top list box selected annotation to the current image selected annotation. The `copy annotation` duplicates current selected image annotation. The delete button in the bottom of the current image annotations list designed to delete current image selected annotation respectively.

On the bottom of the window there is a status panel that currently indicates only opened JSON db file.

After annotating the images of interest, use the export submenu of the `Annotations` menu to export data to widely used annotation formats.

# Implementing custom exporters

Current ImagesAnnotator application stores the annotations database in the `JSON` file format. It makes it easy to implement a custom export script into required format to train neural network with the help of `Python` or `Bash`.

# Documentation contents

**Document is under the refinement**

1. [Cloning the ImagesAnnotator project](/doc/sections/en_US/1-cloning-the-cxx-template-project.md)
1. [Forking and replacing the origin](/doc/sections/en_US/2-forking-and-replacing-the-origin.md)
1. [Requirements](/doc/sections/en_US/3-requirements.md)
    1. [Required tools for the GNU/Linux based OS](/doc/sections/en_US/3-1-required-tools-for-the-GNU-Linux-based-OS.md)
    1. [Required tools for the MS Windows based OS](/doc/sections/en_US/3-2-required-tools-for-the-MS-Windows-based-OS.md)
    1. [Optional for the tests](/doc/sections/en_US/3-3-optional-for-the-tests.md)
    1. [Optional for the documentation](/doc/sections/en_US/3-4-optional-for-the-documentation.md)
    1. [Optional for the code formatting](/doc/sections/en_US/3-5-optional-for-the-code-formatting.md)
    1. [Optional for the code analyzer (cppcheck)](/doc/sections/en_US/3-6-optional-for-the-code-analyzer-cppcheck.md)
    1. [Optional for the code analyzer with clang-tidy](/doc/sections/en_US/3-7-optional-for-the-code-analyzer-with-clang-tidy.md)
    1. [Optional for the memory check with Valgrind](/doc/sections/en_US/3-8-optional-for-the-memory-checkwith-Valgrind.md)
    1. [Optional for the flatpak packager](/doc/sections/en_US/3-9-optional-for-the-flatpak-packager.md)
    1. [Optional for the Docker container runs](/doc/sections/en_US/3-10-optional-for-docker-container-runs.md)
    1. [Optional for the snap packager](/doc/sections/en_US/3-11-optional-for-snap-packager.md)
1. [Project structure](/doc/sections/en_US/4-project-structure.md)
    1. [Implement code straight away!](/doc/sections/en_US/4-1-implement-code-straight-away.md)
    1. [Changing the project and executable name](/doc/sections/en_US/4-2-changing-the-project-and-executable-name.md)
    1. [Version tracking and other project parameters](/doc/sections/en_US/4-3-version-tracking-and-other-project-parameters.md)
    1. [Minimal possible versions](/doc/sections/en_US/4-6-minimal-possible-versions.md)
    1. [Project tests](/doc/sections/en_US/4-4-project-tests.md)
        1. [Google Test](/doc/sections/en_US/4-4-1-google-test.md)
    1. [Extensions](/doc/sections/en_US/4-5-extensions.md)
1. [Project build](/doc/sections/en_US/5-project-build.md)
    1. [IDE build](/doc/sections/en_US/5-1-IDE-build.md)
    1. [Command line build](/doc/sections/en_US/5-2-command-line-build.md)
    1. Enabling testing
        1. [Enabling unit and component testing](/doc/sections/en_US/5-3-1-enabling-unit-testing.md)
        1. [Disabling system GTest probe](/doc/sections/en_US/5-3-2-disabling-system-GTest-probe.md)
    1. [Documentation build](/doc/sections/en_US/5-4-documentation-build.md)
    1. [Configuring the documentation install support](/doc/sections/en_US/5-5-configuring-the-documentation-install-support.md)
    1. [Enabling and performing code formatting target](/doc/sections/en_US/5-6-enabling-and-performing-code-formatting-target.md)
    1. [Enabling the static code analyzer target with cppcheck](/doc/sections/en_US/5-7-enabling-the-static-code-analyzer-target-with-cppcheck.md)
    1. [Enabling the static code analyzer with clang-tidy](/doc/sections/en_US/5-8-enabling-static-code-analyzer-with-clang-tidy.md)
    1. [Enabling the dynamic memory check target with valgrind](/doc/sections/en_US/5-9-enabling-the-dynamic-memory-check-target-with-valgrind.md)
    1. [Enabling DEB package generation with cpack](/doc/sections/en_US/5-10-enabling-DEB-package-generation-with-cpack.md)
    1. [Enabling the flatpak package generation support](/doc/sections/en_US/5-11-enabling-the-flatpak-package-generation-support.md)
    1. [Enabling the Docker container build and run](/doc/sections/en_US/5-12-enabling-the-docker-container-build-and-run.md)
    1. [Enabling sanitizers](/doc/sections/en_US/5-13-enabling-sanitizers.md)
    1. [Enabling the libcurl](/doc/sections/en_US/5-14-enabling-libcurl.md)
    1. [Enabling gprof profiler analysis](/doc/sections/en_US/5-15-enabling-gprof-profiler-analysis.md)
    1. [Enabling vagrind's callgrind profiler analysis](/doc/sections/en_US/5-16-enabling-valgrinds-callgrind-profiler-analysis.md)
    1. [Enabling Jenkins pipeline inside Docker container](/doc/sections/en_US/5-17-enabling-Jenkins-pipeline-inside-Docker-container.md)
    1. [Enabling the nlohmann json library](/doc/sections/en_US/5-18-enabling-the-nlohmann-json-library.md)
    1. [Enabling the snap packager](/doc/sections/en_US/5-19-enabling-the-snap-package-manager.md)
    1. [Enabling FreeBSD pkg package generation with cpack](/doc/sections/en_US/5-20-enabling-FreeBSD-pkg-package-generation-with-cpack.md)
    1. [Enabling WIX MSI package generation with cpack](/doc/sections/en_US/5-21-enabling-WIX-MSI-package-generation-with-cpack.md)
    1. [Enabling RPM package generation with cpack](/doc/sections/en_US/5-22-enabling-RPM-package-generation-with-cpack.md)
    1. [Enabling the zlib library](/doc/sections/en_US/5-23-enabling-the-zlib-library.md)
    1. [Enabling the libpng library](/doc/sections/en_US/5-24-enabling-the-libpng-library.md)
    1. [Enabling the libjpeg library](/doc/sections/en_US/5-25-enabling-the-libjpeg-library.md)
    1. [Enabling the libwebp library](/doc/sections/en_US/5-26-enabling-the-libwebp-library.md)
    1. [Enabling the lunasvg library (SVG)](/doc/sections/en_US/5-27-enabling-the-lunasvg-library.md)
    1. [Enabling the giflib library (GIF)](/doc/sections/en_US/5-28-enabling-the-giflib-library.md)
    1. [Enabling the libtiff library (TIFF)](/doc/sections/en_US/5-29-enabling-the-libtiff-library.md)
    1. [Enabling the OpenEXR library (EXR / HDR)](/doc/sections/en_US/5-30-enabling-the-openexr-library.md)
    1. [Enabling the OpenJPEG library (JPEG 2000)](/doc/sections/en_US/5-31-enabling-the-openjpeg-library.md)
    1. [Enabling the libavif library (AVIF)](/doc/sections/en_US/5-32-enabling-the-libavif-library.md)
    1. [Enabling the libheif library (HEIF/HEIC)](/doc/sections/en_US/5-33-enabling-the-libheif-library.md)
1. [Run the executable](/doc/sections/en_US/6-run-the-executable.md)
    1. [IDE run](/doc/sections/en_US/6-1-IDE-run.md)
    1. Command line run
        1. [Searching for the generated executable](/doc/sections/en_US/6-2-1-searching-for-the-generated-executable.md)
        1. [Starting the generated executable](/doc/sections/en_US/6-2-2-starting-the-generated-executable.md)
    1. Tests run
        1. [Run tests by the ctest](/doc/sections/en_US/6-3-1-run-tests-by-the-ctest.md)
1. [Installing](/doc/sections/en_US/7-installing.md)
