# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [v0.11.0] - 2025-07-08

- 07db55a Repairing binary name in the README.md header
- 6629ffc Refining UTEST_Application with for a new code introduced
- 88deedd Replacing the possible data lose notice with alpha stage notice
- 2c0d786 Implementing user asking for introduced data changes
- 2404322 Implementing the project changes indication
- 7568180 Introducing last saved facility into the current project AnnotationsDirDB class
- 4d76df1 Introducing ImageRecordsSet comparisement methods into appropriate classes
- 23cfeb3 Introducing possible data lose warning in the READMEs
- 977aa50 Fixing annotation edit value on deleted current
- 5634401 Introducing the Usage section in the readmes with window general overview image and description
- 8574561 Incrementing version to 0.11.0
- 1a48462 Renaming include guards which were not a part of the templated project
- b64defd Inserting the license notice into most of the *.h or *.cpp files
- d4d2a36 Inserting plain exporter to the CTs
- 94143c7 Editing READMEs
- c6c5517 Decomposing the plain exporter a bit
- be99b95 Implementing the plain exporter
- 477911c Implementing the event bridge 4 export
- 708c1e2 Introducing the annotations export submenu with first plain txt export option
- ebcb387 Extracting ImageRecordJSON2RecordConverter class from the main JSON importer
- 0a03197 Extracting methods from the JSON to records converter
- 6ad2d33 Extracting the ImageRecrodRect JSON converter into image rect record
- 2669c17 Refining the CTs
- af14b61 Introducing the sorter helper for images load and append
- 8f33576 Moving UI adjustment code into the wloader appropriate methods
- 973d107 Implementing a simple error dialog during image load failure
- e5e9b73 Adding the git commit into the about window
- 29d4786 Refining the window title with current json db file opened
- a335984 Adding CMake project description to the about window
- b636063 Introducing the FreeBSD simplified LICENSE file
- 23a06d0 Avoiding annotations rectangle flip
- c7ac753 Repair warnings and introducing new TypeHelper class with appropriate component
- cd273df Hiding resizers rects if mouse in not near by any
- 5a7ec9c Implementing the annotations rectangles resize facility
- d992083 Implementing the resize rects draw on the annotation rect with cursor shape change
- cdab0e6 Introducing the resize rectangles
- d20ccdb Making the non current rectangles smaller
- 0cd4f2a Avoiding seg fault on the images navigation buttons
- 0a3a8f6 Implementing a simple about dialog with appropriate content
- 8f82e02 Erasing odd looking label that shifts images button down
- ac76ce9 Replacing the static_casts to declared initial functions
- ca5b066 Repaired outline annotations avoid computations
- ce19754 introducing the label for the imported filepaths list
- adc705d Disallowing making annotations rectangles out of the image borders
- 77eb98a Enhancing initial scaling factor helper
- 228d81d Centering the draw area
- 8a21003 Introducing initial image scale factor computing to parent widget height
- 5d26e77 Spinners introduce try on the db or folder opening
- 2487495 Implementing all annotations filter out through the search field
- b05adbb Implementing the current annotation copy facility
- 2760052 Implementing statusbar current file display
- 972bf29 Introducing the status bar into the UI
- 8a9fdb3 Implementing the next and prev buttons functionality
- a99a7a2 Introducing the next and previous images button
- 489e444 Refining annotations label to be more specific
- b2c3219 Introducing the copy current annotation button to the UI
- e5d8661 Implementing the current rect reselection
- 46d79f4 Implementing the annotation name copy
- 1525599 Refining the annotations UI
- 1d7228c Implementing background reset on the image path label with all deleted annotations
- 81271bf Transferring the current rect edit into rect edit entry instead of search entry
- b1ce6db Introducing new name copy button and rect name edit entry
- 24929c3 Implementing the name annotation copying from all annotations list
- 4c60158 Refining the project close with annotations list
- 7360185 Storing currently saved as project as a current file
- 6f8bd18 Marking the selected rectangle with a red color and others blue
- f3a1c88 Implementing the rectangle area edit through the search field
- fbfd216 Deleting redundant delete button for the annotations
- 7820c9f Implementing the current rect erase facility
- 7124521 Implemented current image rects display and selecting
- e2f6853 Default naming the newly created rectangles
- 2a520b4 Implementing the project close
- d16e359 Changing the unordered_set container for the images files list to preserve the order
- af3cd5b cppcheck warnings repair
- a1f7266 Fixing paths and it's display
- afa4c82 Marking image label background during load, db abs path check
- 3177d70 Fixing the abs and rel path split for ImageRecord
- 4347eaf Implemented save as functionality
- 5042643 Introducing new annotations sub menu items for save and save as
- 934233a Introducing the ImageRecords2JSONConverter StoreRequest StoreRequestHandler for storing project into the json
- 229f50c Introducing the scaling factor to the json
- 64648f6 Implementing the annotations open menu subitem
- 25c6db4 Applying the code formatting
- 924d4d8 Introducing and implementing the images open directory menu item
- 03388cd Implementing a new images directory insert into current project
- 62e3805 Refining directory loading with images list widgets display
- cbf7e43 Introducing the JSON DB annotations file dialog opening on the appropriate button click
- 891a874 Renaming ImagesDBs into ImagesLoaders dir refined ImagesDirLoader usage accordingly
- 816f3dc Implementing basic json to image records db converter
- 76f4a2d Introducing structure to the JSON2ImageRecordsConverter
- 299d276 Introducing the basic annotations load into the json with CTs and json plan
- efba75a Removing the test images dir in the ApplicationContext
- 13bc7c6 Wrapping the images db with additional respective namespace
- e69929c Wrapping the annotation db class into the additional annotation namespace
- bc107a0 Moving the AnnotationsDirDB into the newly introduced AnnotationsDBs directory
- 37d00c9 Moving the ImagesDirDB into newly introduced ImagesDBs directory
- e92e331 introducing annotator-business CTs base class CTEST_AnnotatorController
- adede29 Introducing a basic CTEST_AnnotationsDirDB CT infrastructure files
- a6f014f Refining the AnnotatorController CT and naming it as the CTEST_ImagesDirDB for the ImagesDirDB related tests
- edca86d Incapsulating the image zoom in/out routines into the ImageRecord class
- c4d0326 Implementing marking the annotated images with a green color
- a6472f4 Implemented rectangle draws on the current image
- a735493 Memorizing every image zoom in the ImageRecord instance
- aaff98e Replacing the centraw image with the DrawingArea
- 80af15f Refining code for the cppcheck warnings
- efee8cb Introducing the image loading with current changes event, image scaling basics, refined UI, WindowEventsHandler
- 6038846 Incapsulating all widgets into the pane widget and making filepaths labels truncated
- 3c28540 Packing central widget and the images list into the Paned widget
- 31baaed Wrapping the rest of the listboxes into the scroll window
- 5b59914 Wrapping the Images DB list widget with a scroll window
- 79146b2 The root README.md small refinements
- 5bb78cc Implementing initial opened directory images paths load into UI widgets
- 2110bb9 Implementing images db provider object changed event for GtkmmIniter
- fe352b7 New ImagesDirProviderChanged event refined CTs
- 11c6950 Implementing a basic ImagesDirDB instance with basic business controller CTs
- 93ffda8 Implementing basic eventer, business controller, window loaders infrastructure
- 91cd15c Decomposing all the window pointers and ids into the separate class with loader
- aac0a86 Enhancing the window
- ad08485 Replacing the default application name with the new ImagesAnnotator name
- b1f9366 Implementing a basic UI structure of the annotation app
- d69a99b Avoiding temporary files
- 1a252b5 Fixing the sanitizers cmake file in the READMEs
- 8f746fa Avoiding unnecessary badly portable localtime_r usage
- 86656dc Introducing the thread id into simple logger
- a82ce51 Avoiding backward time in the simple logs
- ae231d3 Avoiding final log multiple str fetches
- 85e47f3 Refining the log current timestamp method, increasing thread safety
- bdd3ca3 Introducing the CMake configurable log severity messages compilation switch
- 3a3cb58 Introducing the appBoostLog branch info into the READMEs
- 99f5a83 Hiding the compile options cmake files under separate directory
- 0f3c4d5 Enhancing sanitizers combinations and new cmake dir for ones
- 116102c Introducing the sanitizers enabling info into the READMEs
- 7c7308a Removing redundant cmake traces messages
- a1a7b21 Introducing the GCC compiler sanitizers
- a16c76d Incrementing version to 0.10.0

## [appGtkmm3Glade0.9.0] - 2025-06-06

- b6195f0 Release appGtkmm3Glade0.9.0 2025-06-06
- 66631ea Introducing misc/.cppcheck-suppress file to list cppcheck suppressions of the false positives
- c95cf9e Refined issues in the Docker READMEs doc
- be42016 Refining the Docker container target
- c60c433 Separating the Docker single container build and run targets
- 790813d Introducing the Docker single run container target.
- 4c55828 Adding project neatness to the READMEs
- bd3e867 Introducing the appLog4Cpp5 branch info into the READMEs
- 86e590b Implementing the logging macros and a simple logger to perform log messaging
- f3b05f1 Introducing the appMongoDBCpp4 branch info into the READMEs
- 6a44469 applying code formatting
- 7d6fa94 Making the flatpak source json conf file configurable during cmake conf stage
- d7b0f5a specifying the pkg-config tool as requirement for dev into hte READMEs
- 93735db Inserting the libgtkmm-3.0-dev into deb dependencies
- 7e0bfaf Refining the deb cmake info for cpack
- 6c6e88a Specifying org.gnome platfrom and sdk in the READMEs
- 510efad Replacing org.freedesktop platform and sdk to org.gnome in the flatpak json conf
- e649585 Renaming the UI.glade xml file to GtkmmWindow.ui
- e83d879 Fixing flatpak dbus connection issues
- ae6db12 Refining the flatpak target with json
- 87f4adb Introducing the appSQLiteCpp3 branch info into the READMEs
- d366f44 Refining the ApplicationContext mock
- 4afb98e Introducing the gitlab mirror into the READMEs
- 4f71ce3 Introducing the appMySQLCppConn branch info into the READMEs
- 0ae68ff Introducing the appBoostBeast branch info into the READMEs
- 397f089 Explicitly specifying the combining of the template branches
- 42e2ce1 introducing the appPgSQLxx branch into the READMEs
- de3a378 Introducing a new appWt4 branch info into the READMEs
- e4f5450 Introducing the error exit code for a cppcheck target
- 483221d Refinining the cppcheck target
- 8b31d9a Introducing the donation notice into the READMEs
- 8209f3b Introducing the appMeson branch info into the READMEs
- afab2bb Refining the branch info
- 8378895 Introducing the flatpak sections into the READMEs
- bf3b112 Introducing the flatpak package creation from the template source.
- d4ce2f0 Erasing the /analyze from the default MSVC compile options
- f751d51 Introducing the DEB package gen info in the READMEs
- e80b7fd Introducing the cpack configuration for the deb package generation
- d55cbc9 Introducing the LICENSE file with free of use license and notice
- 66a9e11 Introducing the appSDL2 branch into the READMEs
- 54639a2 Introducing clang-tidy
- f350d0a Introducing the FreeGlut/OpenGL appFreeGlut branch info into READMEs
- b64ce0f Introducing MSVC extra compile options for warnings
- e01f888 Adding MSVC settings file to the .gitignore
- f4500bd Avoiding the compilation warnings at the UTEST_CommandLineParser
- 81c6867 Introducing MS Windows tools install description into READMEs
- e541642 Adding the out directory for MSVC into .gitignore
- 0d20c0d Incrementing version to 0.9.0

## [appGtkmm3Glade0.8.0] - 2025-03-17

- 2518b7c Release appGtkmm3Glade0.8.0 2025-03-17
- 44abe1d Refining READMEs to introduce Glade UI explanations
- adb3bf5 Changing the glade file to UI.glade so it's representing ALL the GUI
- 0ee80c9 appending the Glade suffix to resources paths
- ab19d8c Applying clang-format
- 40c81c7 Erasing CMake resource compilation in favor glib-compile-resources compilation
- 093021b introducing the Glade app into the READMEs.
- df21556 Specifying the appGtkmm3Glade branch in the READMEs
- 7d1a162 Moving a src/gtkmm to the src/gtkmm3 directory with contents
- 752eb7b Erasing const reference for the get_glade_xml_data method
- d20dabf Applying clang-format
- 78f39c1 Changing the project descriptionin the root CMakeLists.txt.
- ef58e59 Incrementing version to 0.8.0
- bad8a5d Erasing GtkmmWindow class to prefer one from the .glade file
- 6fd97a3 Wrapping main window box with an actual Window widget for glade file
- 2b493f8 Introducing Glade-files load infrastructure to build UI with XML.
- eb7c98a Erasing annoying traces for the GTKmm CMake vars
- 5d5d338 Introducing the load_file_data procedure to C++ header file data transfers
- 12ebc3b Introducing MainWindow glade file for UI.

## [appGtkmm30.7.0] - 2025-03-16

- 0b67e70 Release appGtkmm30.7.0 2025-03-16
- 512bbd0 Refining the READMEs for the GTKmm
- c3bb27c Margin the appGtkmm3 branch as current in the READMEs
- 79d22dc Introducing the appGtkmm3 branch info into the READMEs
- 045a1a4 Refining the Application UTs.
- d2eee0c Applying clang-format
- e19f268 Adding gtkmm-3.0 to required packages in READMEs
- eedd7ac Introducing the GTKmm-3.0 window infrastructure.
- 1da93db Replacing the /usr/bin prefix parameter with right /usr in installation section
- adc02d6 Introducing the lib branch info into the READMEs
- 6230596 Introducing the appQt6 branch in READMEs
- d044fe4 Incrementing version to 0.7.0

## [app0.6.0] - 2025-03-14

- 9bd89d8 Release app0.6.0 2025-03-14
- 1192ec5 Introducing branch info and README translations into uk_UA README
- 14da387 Introducing Doxygen doc with translated README as main page facility
- 074df94 Introducing COMPILE_WARNINGS_AS_ERRORS and ENABLE_COMPILER_CODE_ANALYZER CMake vars.
- 88156b6 Specifying translation documents for the root README under the doc
- 8b10176 Specifying the source file for the PROJECT_BINARY_NAME CMake variable
- 3920a3a Introducing multibranch info into the root README.md
- e19aef1 README.uk_UA.md links refinement
- ec31437 The root README.md file translation to uk_UA workflow final
- be2dd4d The root README.md file translation to uk_UA workflow (pre Installing executable)
- 421f046 The root README.md file translation to uk_UA workflow (pre Run section)
- 86ac202 The root README.md file translation to uk_UA workflow (pre clang-format)
- a443a83 The root README.md file translation to uk_UA workflow (pre enabling testing)
- 8da8536 The root README.md file translation to uk_UA workflow.
- cadecf3 Introducing translated root README in uk_UA.
- a5f282c Incrementing version to 0.6.0

## [app0.5.0] - 2025-03-12

- c60779c Release app0.5.0 2025-03-12
- edad5ce Specifying the cmake modules for the valgrind and cppcheck targets
- cc7c0e7 Specifying particular test run with ctest
- 2fe49df Introducing the Valgrind target doc for root README.md
- ee75ed3 Introducing the cppcheck doc into root README.md
- 2edc812 Fixing cppcheck target exec doc in the README.md
- 2058fe2 Introducing the Valgrind memory check target
- 37b2e48 Decomposing feature enabling if statements into separate file
- 5f96b82 Integrating the cppcheck code checking tool.
- c6aca03 Incrementing version to 0.5.0

## [app0.4.0] - 2025-03-10

- b15973a Release app0.4.0 2025-03-10
- 6329ab2 Sections reorder
- 40b7fdc Introducing clang-format doc into the root README.md
- 6dc997d Setting default OFF value for the clang-format target
- 4d9d227 Fixing absent cmake-modules search path from root
- 38f0c43 Decomposing root CMakeLists.txt var declares into separate module.
- ce9cd15 Adding the clang-format target
- c3aea63 Applied clang-format code formatter
- 6ec1125 Incrementing version to 0.4.0
- c3511ae Introducing Google-based .clang-format file.

## [app0.3.0] - 2025-03-08

- 7dacafc Release app0.3.0 2025-03-08
- e673166 Removing Doxygen mention from the extensions and it's refine.
- 21599fa Specifying optional packaged in the README.md file
- d301440 Introducing the Doxygen doc install feature with custom html name.
- a46f413 Adding README.md to the Doxygen documentation.
- 5de5b7a Adding description for the app namespace
- 1d60e08 Erasing %Z term for the PROJECT_CONFIGURE_DATE variable value format.
- b9b05ba CMake batch replace to a minimum version downgrade.
- 77d99d5 Making Doxygen output directory customizable
- ff46570 Refining README.md
- b5777ed Introducing the Doxygen subsection in the README.md
- 8665554 Refining some classes comments.
- f53a237 Refining Doxygen target
- 500ccf0 Introducing the Doxygen documentation creation target.
- 37ed418 Refining tests run section in the README.md
- bfd7e38 Fixing README.md type
- e0e7225 Fixing type in the CHANGELOG.md
- dfbf4ac Moving ENABLE_UNIT_TEST check to toplevel app UTs
- 21335bf Introducing a new PROJECT_BINARY_NAME CMake-var and CMake files little reorg
- d1f5ead Incrementing version to a 0.3.0

## [app0.2.0] - 2025-03-07

- 74a55dc Release app0.2.0
- 150537c Fixing annoying -fanalyzer caused prolonged build
- 84d7c95 Fixing flags skipping in the CommandLineParser
- 57f0cef Causing to print help if missing command line parameter data.
- c7714dc A bit of a code formatting
- 72fed6d Newline in the project-global-decls.h.in
- dd6dccc Erasing the INVALID static field from App Factory and reusing one from the IApplication
- f2bd6f6 Introducing correct brief comment for the CommandLineParser class
- 905d423 Introducing return value explanation for an abstract run method
- 01e3f80 Fixing main.cpp comment mistake.
- 8f8bb4c Introducing comment to a IApplication INVALID static field
- 30aead4 Introducing comprehensive help message in the ApplicationHelpPrinter
- 19891ac Introducing sample ApplicationFactory UTs.
- 5401442 Introducing a sample CommandLineParser UTs.
- 5c77ee5 Introducing ApplicationVersionPrinter UTs.
- 848bea1 Adding ApplicationHelpPrinter sample UTs.
- eef8dd4 Adding ApplicationContext UTs.
- ff2adf4 Fixing absent includes for ApplicationContext mock
- 8096646 Fixing double Cpp in the binary name for README.md
- 7de3038 Introducing test run commands to the README.md
- e6a30c6 Enabling system GTest probe by the default
- 5967b0b Mentioning GTEST_TRY_SYSTEM_PROBE in the README.md
- 285b387 Adding GTest system probe code to CMakeLists.txt files.
- eebc595 Disabling the UTs by default.
- 458c11f Specifying command execution dir in README.md
- b749982 Refining README.md
- 487748a Integrating GTest and implementing a simple Application test.
- 1515ece Fixing gtest package requirement
- a46c1ba Fixed ApplicationFactory indentation issue.
- 833bc70 Specifying install commands directory in README.md
- e9b4d47 Adding installation section in the README.md
- a216d70 Adjusting execute_process command indentation.
- f4eb95e Adding binary install command.
- 6ead0f8 Refining set origin section in README.md
- 1cc9166 Introducing new info into the README.md
- a3ca641 Incrementing version to 0.2.0

## [v0.1.0] - 2025-03-04

- feded33 Refining CHANGELOG.md
- eaaee95 0.1.0 release
- 522b6b3 Adding changelog
- 5c75937 Refining the README.md file
- f8df49b Introducing additional info into the root README.md file
- a76890a Decomposing the compile options
- d5da609 Adding some comments.
- 28772c6 Introducing h-configure file for version printer
- b5fceb1 Introducing version printing application ApplicationVersionPrinter
- 6d4b9a0 Adding some comments to parser and applications
- ba2bf07 Introducing and integrating a CommandLineParser.
- 720500f Adding a basic help printer application class.
- 7429e21 Adding if-guards for created app instances.
- e2be348 Adding main application function explanatory comment.
- 8b6f9ef Adding ApplicationFactory class and integrating into main.cpp
- 6bbff32 Adding the default application's class Application
- c0debf2 Adding ApplicationContext and IApplication.
- 5a2fb71 Adding basic CMakeLists.txt files structure.
- f770df2 Adding a basic main.cpp
- 5876319 Adding the initial README.md
- 3f53c17 Adding .gitignore

# Releases

[unreleased](https://github.com/yuriysydor1991/ImagesAnnotator)

[v0.11.0](https://github.com/yuriysydor1991/ImagesAnnotator/releases/tag/0.11.0)
[appGtkmm3Glade0.9.0](https://github.com/yuriysydor1991/cpp-app-template/releases/tag/appGtkmm3Glade0.9.0)
[appGtkmm3Glade0.8.0](https://github.com/yuriysydor1991/cpp-app-template/releases/tag/appGtkmm3Glade0.8.0)
[appGtkmm30.7.0](https://github.com/yuriysydor1991/cpp-app-template/releases/tag/appGtkmm30.7.0)
[app0.6.0](https://github.com/yuriysydor1991/cpp-app-template/releases/tag/app0.6.0)
[app0.5.0](https://github.com/yuriysydor1991/cpp-app-template/releases/tag/app0.5.0)
[app0.4.0](https://github.com/yuriysydor1991/cpp-app-template/releases/tag/app0.4.0)
[app0.3.0](https://github.com/yuriysydor1991/cpp-app-template/releases/tag/app0.3.0)
[app0.2.0](https://github.com/yuriysydor1991/cpp-app-template/releases/tag/app0.2.0)
[v0.1.0](https://github.com/yuriysydor1991/cpp-app-template/releases/tag/v0.1.0)
