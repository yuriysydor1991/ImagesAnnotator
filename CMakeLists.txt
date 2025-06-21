cmake_minimum_required(VERSION 3.13)

# Change the PROJECT_NAME var value to change executable name
set(
  PROJECT_NAME "ImagesAnnotator"
  CACHE STRING "Project general name with an executable name"
)

project(
  ${PROJECT_NAME}
  VERSION 0.10.0
  DESCRIPTION "The data images annotation project with GTKmm3 and Glade usage"
  HOMEPAGE_URL "http://www.kytok.org.ua/"
  LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Git REQUIRED)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(template-project-misc-variables-declare)
include(template-project-feature-enabler)

add_subdirectory(src)
add_subdirectory(doc)

include(template-project-feature-enabler-post)
