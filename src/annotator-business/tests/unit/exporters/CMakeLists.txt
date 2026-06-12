cmake_minimum_required(VERSION 3.13)

set(EXPORTERS_SRC ${CMAKE_SOURCE_DIR}/src/annotator-business/exporters)
set(HELPERS_SRC ${CMAKE_SOURCE_DIR}/src/helpers)
set(EVENTS_SRC ${CMAKE_SOURCE_DIR}/src/annotator-events/events)

add_subdirectory(PlainTxt2FolderExporter)
add_subdirectory(Yolo42FolderExporter)
