cmake_minimum_required(VERSION 3.13)

option(
  ENABLE_JENKINS_DOCKER_PIPELINE
  "Set to ON to enable the Jenkins pipeline run inside the Docker container"
  OFF
)

option(
  JENKINS_PIPELINE_FORCE_REBUILD
  "Set to ON to force rebuild the default Jenkins pipeline image and container"
  OFF
)

set(
  JENKINS_PIPELINE_DOCKER_IMAGE_NAME
  "${PROJECT_BINARY_NAME_lower}-jenkins-pipeline-image"
  CACHE STRING 
  "The Jenkins pipeline Dockerfile image name"
)

set(
  JENKINS_PIPELINE_DOCKER_CONTAINER_NAME
  "${PROJECT_BINARY_NAME_lower}-jenkins-pipeline-run-container"
  CACHE STRING 
  "The Jenkins pipeline Dockerfile container name"
)

set(
  JENKINS_PIPELINES_PANEL_HTTP_PORT
  "8080"
  CACHE STRING
  "The Jenkins UI port on the docker container"
)

set(
  JENKINS_PIPELINE_DOCKERFILE_SRC 
  "${CMAKE_SOURCE_DIR}/misc/Dockerfiles/Dockerfile.Jenkins.pipeline.in" 
  CACHE STRING 
  "The Jenkins pipeline Dockerfile full path"
)

set(
  JENKINS_PIPELINE_DOCKERFILE_DST
  "Dockerfile.Jenkins.pipeline"
  CACHE STRING 
  "The Jenkins pipeline Dockerfile destination name"
)

if (NOT ENABLE_JENKINS_DOCKER_PIPELINE)
  return()
endif()

find_program(SCRIPT_EXEC script REQUIRED)

configure_file(${JENKINS_PIPELINE_DOCKERFILE_SRC} ${JENKINS_PIPELINE_DOCKERFILE_DST})

execute_process(
  COMMAND ${SCRIPT_EXEC} -q -c "DOCKER_HOST=${DOCKER_HOST_STR} ${DOCKER_EXEC} images -q ${JENKINS_PIPELINE_DOCKER_IMAGE_NAME}" /dev/null
  OUTPUT_VARIABLE JENKINS_PIPELINE_DOCKER_IMAGE_NAME_PRESENT
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
  COMMAND ${SCRIPT_EXEC} -q -c "DOCKER_HOST=${DOCKER_HOST_STR} ${DOCKER_EXEC} ps -aq -f name=${JENKINS_PIPELINE_DOCKER_CONTAINER_NAME}" /dev/null
  OUTPUT_VARIABLE JENKINS_PIPELINE_DOCKER_CONTAINER_PRESENT
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

if (JENKINS_PIPELINE_FORCE_REBUILD)  
  if (NOT JENKINS_PIPELINE_DOCKER_CONTAINER_PRESENT STREQUAL "")
    message(STATUS "Stopping and erasing the ${JENKINS_PIPELINE_DOCKER_CONTAINER_NAME} container")
    set(JENKINS_PIPELINE_DOCKER_CONTAINER_PRESENT "")
    execute_process(
      COMMAND DOCKER_HOST=${DOCKER_HOST_STR} ${DOCKER_EXEC} container stop ${JENKINS_PIPELINE_DOCKER_CONTAINER_NAME}
    )
    execute_process(
      COMMAND DOCKER_HOST=${DOCKER_HOST_STR} ${DOCKER_EXEC} container rm ${JENKINS_PIPELINE_DOCKER_CONTAINER_NAME}
    )
  endif()
  
  if (NOT JENKINS_PIPELINE_DOCKER_IMAGE_NAME_PRESENT STREQUAL "")
    message(STATUS "Erasing the ${JENKINS_PIPELINE_DOCKER_IMAGE_NAME} image")
    set(JENKINS_PIPELINE_DOCKER_IMAGE_NAME_PRESENT "")
    execute_process(
      COMMAND DOCKER_HOST=${DOCKER_HOST_STR} ${DOCKER_EXEC} rm ${JENKINS_PIPELINE_DOCKER_IMAGE_NAME}
    )
  endif()
endif()

if (JENKINS_PIPELINE_DOCKER_IMAGE_NAME_PRESENT STREQUAL "")
  set(
    JENKINS_PIPELINE_DOCKER_BUILD_CMD
      DOCKER_HOST=${DOCKER_HOST_STR} DOCKER_BUILDKIT=1 ${DOCKER_EXEC} build
        -f "${JENKINS_PIPELINE_DOCKERFILE_DST}"
        --build-context project=${CMAKE_SOURCE_DIR} 
        --build-arg CACHEBUST="${PROJECT_CONFIGURE_DATE}" 
        -t ${JENKINS_PIPELINE_DOCKER_IMAGE_NAME} .
  )
else()
  set(
    JENKINS_PIPELINE_DOCKER_BUILD_CMD
      echo "Image ${JENKINS_PIPELINE_DOCKER_IMAGE_NAME} already build"
  )
endif()

# -d --restart=on-failure
if (JENKINS_PIPELINE_DOCKER_CONTAINER_PRESENT STREQUAL "")
  set(
    JENKINS_PIPELINE_DOCKER_RUN_CMD
      DOCKER_HOST=${DOCKER_HOST_STR} ${DOCKER_EXEC} run -it
      -p ${JENKINS_PIPELINES_PANEL_HTTP_PORT}:8080
      -p 50000:50000
      --name ${JENKINS_PIPELINE_DOCKER_CONTAINER_NAME}
      ${JENKINS_PIPELINE_DOCKER_IMAGE_NAME}
  )
else()
  set(
    JENKINS_PIPELINE_DOCKER_RUN_CMD
      DOCKER_HOST=${DOCKER_HOST_STR} ${DOCKER_EXEC} container start 
        -ia ${JENKINS_PIPELINE_DOCKER_CONTAINER_NAME}
  )
endif()

message(STATUS "Jenkins pipeline docker build command: ${JENKINS_PIPELINE_DOCKER_BUILD_CMD}")
message(STATUS "Jenkins pipeline docker run command: ${JENKINS_PIPELINE_DOCKER_RUN_CMD}")

add_custom_target(
  jenkins-pipeline-docker-build
  COMMAND ${JENKINS_PIPELINE_DOCKER_BUILD_CMD}
  COMMENT "Building the Jenkins pipeline Docker run instance"
)

add_custom_target(
  jenkins-pipeline-docker-run
  COMMAND echo "Connect to the Jenkins GUI at started docker at http://127.0.0.1:${JENKINS_PIPELINES_PANEL_HTTP_PORT}"
  COMMAND ${JENKINS_PIPELINE_DOCKER_RUN_CMD}
  DEPENDS jenkins-pipeline-docker-build
  COMMENT "Running the Jenkins pipeline inside docker run instance"
)

