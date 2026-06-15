cmake_minimum_required(VERSION 3.13)

option(
  ENABLE_OPENCV
  "Set to ON to enable the OpenCV integration (probe the system installation)"
  OFF
)

if (NOT ENABLE_OPENCV)
  return()
endif()

set(
  TEMPLATE_APP_OPENCV_COMPONENTS
  "core;objdetect;imgcodecs;imgproc"
  CACHE STRING
  "Semicolon-separated list of OpenCV components required by the project"
)

find_package(OpenCV REQUIRED COMPONENTS ${TEMPLATE_APP_OPENCV_COMPONENTS})

message(STATUS "OpenCV ${OpenCV_VERSION} found at ${OpenCV_INCLUDE_DIRS}")

target_link_libraries(
  ${PROJECT_BINARY_NAME}
  PUBLIC ${OpenCV_LIBS}
)
