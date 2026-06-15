cmake_minimum_required(VERSION 3.13)

option(
  ENABLE_LIBCURL
  "Set to ON to enable the libcurl (by using system wide available or through the Internet)"
  ON
)

if (NOT ENABLE_LIBCURL)
  return()
endif()

set(TEMPLATE_APP_CURL_GIT "https://github.com/curl/curl.git" CACHE STRING "The CURL library git source repository")
set(TEMPLATE_APP_CURL_GIT_TAG "master" CACHE STRING "The CURL project git repository tag of interest")

# ImagesAnnotator specific: the executable and the tests consume CURL through
# the ${CURL_LIBRARIES} / ${CURL_INCLUDE_DIRS} variables (see src/CURL/CMakeLists.txt
# and the unit/component test CMakeLists). find_package() is therefore called
# directly at this (directory) scope so those result variables are visible to
# the whole src subtree. The shared template_project_default_3rdparty_enabler()
# helper performs the probe inside a function scope, where the FindCURL result
# variables would not propagate to the project targets (only the CURL::libcurl
# imported target would, which the upstream template links against instead). The
# enabler is included before add_subdirectory(src) by the pre feature-enabler so
# the variables are already defined while the src targets are configured.
find_package(CURL QUIET)

if (CURL_FOUND)
  message(STATUS "System already contains the CURL library")
  return()
endif()

message(STATUS "CURL was not found in the system (or probing is OFF)")
message(STATUS "Trying to make CURL library available through the Internet")
message(STATUS "CURL URL: ${TEMPLATE_APP_CURL_GIT}")
message(STATUS "CURL Tag: ${TEMPLATE_APP_CURL_GIT_TAG}")

include(FetchContent)

FetchContent_Declare(
  CURL
  GIT_REPOSITORY ${TEMPLATE_APP_CURL_GIT}
  GIT_TAG        ${TEMPLATE_APP_CURL_GIT_TAG}
)

FetchContent_MakeAvailable(CURL)
