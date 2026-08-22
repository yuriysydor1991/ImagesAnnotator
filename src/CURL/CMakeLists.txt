cmake_minimum_required(VERSION 3.13)

message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")

add_library(
  TemplateProjectCURLControllerObj OBJECT
  CURLController.cpp
)

target_include_directories(
  TemplateProjectCURLControllerObj 
  PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${CURL_INCLUDE_DIRS}
)

annotator_app_targets_link_libraries(
  TemplateProjectCURLControllerObj
  ${CURL_LIBRARIES}
)

add_subdirectory(tests)
