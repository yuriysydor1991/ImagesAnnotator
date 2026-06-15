cmake_minimum_required(VERSION 3.13)

option(
    ENABLE_LIBXML2
    "Enables the libxml2 library for the project usage through system installed one or FetchContent by internet"
    OFF
)

if (NOT ENABLE_LIBXML2)
    return()
endif()

# original repo @ https://gitlab.gnome.org/GNOME/libxml2.git
set(TEMPLATE_APP_LIBXML2_GIT "https://github.com/GNOME/libxml2.git" CACHE STRING "The LibXml2 library git source repository")
set(TEMPLATE_APP_LIBXML2_GIT_TAG "master" CACHE STRING "The LibXml2 project git repository tag of interest")

template_project_default_3rdparty_enabler(
  NAME LibXml2 
  GIT_REPOSITORY ${TEMPLATE_APP_LIBXML2_GIT}
  GIT_TAG ${TEMPLATE_APP_LIBXML2_GIT_TAG}
)

target_link_libraries(
  ${PROJECT_BINARY_NAME}
  LibXml2::LibXml2
)
