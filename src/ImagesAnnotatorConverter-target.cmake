cmake_minimum_required(VERSION 3.13)

# ---------------------------------------------------------------------------
# The converter only binary: the annotations format conversion of the main
# application, with no graphical interface behind it.
#
# It is built out of the very same src/app component, the very same main.cpp
# and the very same object libraries of every other component - what it does
# not get is the app::Application class, which src/ImagesAnnotator-target.cmake
# hands to the main target alone. The IMAGES_ANNOTATOR_CONVERTER_ONLY
# definition below is what tells the sources both binaries share - the
# app::ApplicationFactory, which creates that class, and the help message
# printer, which names the running binary - that the class is not there.
#
# Nothing of the GUI toolkit reaches this target: the src/gui object libraries
# are linked into the main one alone, and no source of it names a toolkit
# header. The ITEST_ImagesAnnotatorConverter.no_gui_toolkit_linked integration
# test checks exactly that on the produced binary.
# ---------------------------------------------------------------------------
if(NOT ENABLE_IMAGES_ANNOTATOR_CONVERTER)
  message(STATUS
    "The ${PROJECT_CONVERTER_BINARY_NAME} target is excluded "
    "(ENABLE_IMAGES_ANNOTATOR_CONVERTER=OFF)")
  return()
endif()

add_executable(${PROJECT_CONVERTER_BINARY_NAME})

annotator_configure_app_target(${PROJECT_CONVERTER_BINARY_NAME})

target_compile_definitions(
  ${PROJECT_CONVERTER_BINARY_NAME}
  PRIVATE IMAGES_ANNOTATOR_CONVERTER_ONLY
)

# The Qt6 enabler turns the meta object, the user interface and the resource
# compilers on at project scope, for the targets of the graphical interface.
# Not one source of this target carries a Q_OBJECT, a .ui or a .qrc - it names
# no toolkit at all - so it is left out of all three rather than running them
# over sources which have nothing for them to find.
set_target_properties(
  ${PROJECT_CONVERTER_BINARY_NAME}
  PROPERTIES AUTOMOC OFF AUTOUIC OFF AUTORCC OFF
)
