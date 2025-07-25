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

target_link_libraries(
  ${PROJECT_BINARY_NAME}
  TemplateProjectCURLControllerObj
  ${CURL_LIBRARIES}
)
