cmake_minimum_required(VERSION 3.13)

# The GTKMM_* variables come from cmake/enablers/template-project-Gtkmm4-enabler.cmake,
# which probed pkg-config once at project scope before add_subdirectory(src).

# The window event handler tests sit on top of the whole window subsystem, so
# they link against the project object libraries instead of re-listing that
# closure of source files. Shared by the unit and component event handler tests.
set(EVENT_HANDLERS_OBJ_LIBS
  ${GTKMM_OBJECT_NAME}
  ImageAnnotatorGtkmmWindowObj
  ImageAnnotatorGtkmmWindowEventHandlersObj
  ImageAnnotatorGtkmmWindowCustomWidgetsObj
  ImagesAnnotatorGtkmmCommonObj
  ImagesAnnotatorControllerObj
  ImagesAnnotatorControllerDBsImagesDBObj
  ImagesAnnotatorEventControllerObj
  ImagesAnnotatorEventsObj
  ImagesAnnotatorHelpersObj
  TemplateProjectCURLControllerObj
  TemplateProjectSimpleLoggerObj
)

add_subdirectory(unit)
add_subdirectory(component)
