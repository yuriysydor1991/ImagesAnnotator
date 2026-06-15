cmake_minimum_required(VERSION 3.13)

option(
  ENABLE_DOCKER
  "Set to ON to enable the dockers run"
  OFF
)

if (NOT ENABLE_DOCKER)
    return()
endif()

message(STATUS "Docker runner target enabled")

find_program(DOCKER_EXEC docker REQUIRED)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/dockerers")

include(template-project-docker-variables-declare)

configure_file(${DOCKERFILE_SRC} ${DOCKERFILE_DST})

include(template-project-docker-single-runner-target)
include(template-project-docker-Jenkins-pipeline-runner-target)
