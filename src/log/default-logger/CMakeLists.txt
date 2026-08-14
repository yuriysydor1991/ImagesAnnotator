cmake_minimum_required(VERSION 3.13)

add_library(
  TemplateProjectDefaultLoggerObj OBJECT
  DefaultLogger.cpp
)

target_include_directories(
  TemplateProjectDefaultLoggerObj 
  PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
)

target_link_libraries(
  ${PROJECT_BINARY_NAME}
  TemplateProjectDefaultLoggerObj
)

add_subdirectory(real-default-logger)
add_subdirectory(tests)

