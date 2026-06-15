cmake_minimum_required(VERSION 3.13)

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0 REQUIRED)

# The window event handler tests sit on top of the whole window subsystem, so
# they link against the project object libraries instead of re-listing that
# closure of source files. Shared by the unit and component event handler tests.
set(EVENT_HANDLERS_OBJ_LIBS
  ${GTKMM_OBJECT_NAME}
  ImageAnnotatorGtkmmWindowObj
  ImageAnnotatorGtkmmWindowEventHandlersObj
  ImageAnnotatorGtkmmWindowCustomWidgetsObj
  ImagesAnnotatorGtkmmHelperswObj
  ImagesAnnotatorControllerObj
  ImagesAnnotatorControllerDBsObj
  ImagesAnnotatorControllerDBsAnnotationsConvertersObj
  ImagesAnnotatorControllerDBsImagesDBObj
  ImagesAnnotatorControllerDBsMergerObj
  ImagesAnnotatorControllerExportersObj
  ImagesAnnotatorEventControllerObj
  ImagesAnnotatorEventsObj
  ImagesAnnotatorHelpersObj
  TemplateProjectCURLControllerObj
  TemplateProjectSimpleLoggerObj
)

add_subdirectory(unit)
add_subdirectory(component)
