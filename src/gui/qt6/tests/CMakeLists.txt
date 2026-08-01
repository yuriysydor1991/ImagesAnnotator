cmake_minimum_required(VERSION 3.13)

# The window event handler / widget tests sit on top of the whole window
# subsystem, so they link against the project object libraries instead of
# re-listing that closure of source files. Shared by the unit and component
# tests. Mirrors the gtkmm3 EVENT_HANDLERS_OBJ_LIBS, minus the GTKmm-only
# gtkmm-common object (the Qt build ships its own QImage cropper instead).
set(EVENT_HANDLERS_OBJ_LIBS
  ${GTKMM_OBJECT_NAME}
  ImageAnnotatorQt6WindowObj
  ImageAnnotatorQt6WindowEventHandlersObj
  ImageAnnotatorQt6WindowCustomWidgetsObj
  ImagesAnnotatorControllerObj
  ImagesAnnotatorControllerDBsImagesDBObj
  ImagesAnnotatorControllerExportersObj
  ImagesAnnotatorEventControllerObj
  ImagesAnnotatorEventsObj
  ImagesAnnotatorHelpersObj
  TemplateProjectCURLControllerObj
  TemplateProjectSimpleLoggerObj
)

add_subdirectory(unit)
add_subdirectory(component)
