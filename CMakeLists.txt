cmake_minimum_required(VERSION 3.13)

project(
  ImagesAnnotator
  VERSION 0.12.0
  DESCRIPTION "The data images annotation project with GTKmm3 and Glade usage"
  HOMEPAGE_URL "http://www.kytok.org.ua/"
  LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/template-project-bootstrap.cmake)

add_subdirectory(src)
add_subdirectory(doc)

include(template-project-feature-enabler-post)
