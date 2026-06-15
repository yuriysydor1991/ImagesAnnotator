cmake_minimum_required(VERSION 3.13)

set(
  DOCKER_SINGLE_BUILD_CMD
    DOCKER_HOST=${DOCKER_HOST_STR} DOCKER_BUILDKIT=1 ${DOCKER_EXEC} build 
      --build-context project=${CMAKE_SOURCE_DIR} 
      --build-arg CACHEBUST="${PROJECT_CONFIGURE_DATE}" 
      -t ${DOCKER_SINGLE_RUN_NAME} .
)

# ImagesAnnotator specific: the app is a GTKmm3 GUI, so the container run
# forwards the host X11 display, the D-Bus session and the at-spi bus.
set(
  DOCKER_SINGLE_RUN_CMD
    xhost +local:docker &&
    DOCKER_HOST=${DOCKER_HOST_STR}
    ${DOCKER_EXEC} run --rm -t
    --security-opt apparmor=unconfined
    --ipc=host
    -e DISPLAY=$$DISPLAY
    -v /tmp/.X11-unix:/tmp/.X11-unix:ro
    -e DBUS_SESSION_BUS_ADDRESS=$DBUS_SESSION_BUS_ADDRESS
    -v $(XDG_RUNTIME_DIR)/bus:$(XDG_RUNTIME_DIR)/bus
    -v $(XDG_RUNTIME_DIR)/at-spi/bus_0:$(XDG_RUNTIME_DIR)/at-spi/bus_0
    ${DOCKER_SINGLE_RUN_NAME} &&
    xhost -local:docker
)

message(STATUS "docker single build command: ${DOCKER_SINGLE_BUILD_CMD}")
message(STATUS "docker single run command: ${DOCKER_SINGLE_RUN_CMD}")

add_custom_target(
  docker-single-build
  COMMAND ${DOCKER_SINGLE_BUILD_CMD}
  COMMENT "Building the docker single run instance"
)

add_custom_target(
  docker-single-run
  COMMAND ${DOCKER_SINGLE_RUN_CMD}
  DEPENDS docker-single-build
  COMMENT "Running the docker single run instance"
)
