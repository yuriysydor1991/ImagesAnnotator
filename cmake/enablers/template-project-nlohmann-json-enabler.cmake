cmake_minimum_required(VERSION 3.13)

option(
  ENABLE_NLOHMANN_JSON
  "Set to ON to enable the nlohmann json library (by using system wide available or through Internet)"
  ON
)

if (NOT ENABLE_NLOHMANN_JSON)
  return()
endif()

set(TEMPLATE_APP_NLOHMANN_GIT "https://github.com/nlohmann/json.git" CACHE STRING "The Nlohmann JSON library git source repository")
set(TEMPLATE_APP_NLOHMANN_GIT_TAG "v3.11.3" CACHE STRING "The Nlohmann JSON project git repository tag of interest")

template_project_default_3rdparty_enabler(
  NAME nlohmann_json
  GIT_REPOSITORY ${TEMPLATE_APP_NLOHMANN_GIT}
  GIT_TAG ${TEMPLATE_APP_NLOHMANN_GIT_TAG}
)

# The nlohmann json is a header only library that is included across several
# of the project object libraries (annotations DBs, converters, mergers and
# the annotator controller). Propagate its usage requirements (the include
# directories) to every target built below instead of only the final
# executable, so the object libraries compile no matter whether the library
# came from the system or was fetched through the Internet. This is why the
# enabler is included before add_subdirectory(src) (see the pre feature-enabler).
link_libraries(nlohmann_json::nlohmann_json)
