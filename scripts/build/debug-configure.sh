#!/bin/bash -e

PROJECT_ROOT=$(realpath "$(dirname "$0")/../..")
BUILD_SCRIPTS_ROOT=$(realpath "$(dirname "$0")")

. "${BUILD_SCRIPTS_ROOT}/common.sh"

if [[ $* =~ '--no-reconfigure' ]] ; then
    log "User issued --no-reconfigure"
    exit 0
fi

delete_previous_build "${DEBUG_BUILD_DIR}"

log "Trying to Debug-configure project ${PROJECT_ROOT} inside ${DEBUG_BUILD_DIR}"

filter_script_args yes "$@"

cmake -B "${DEBUG_BUILD_DIR}" -S "${PROJECT_ROOT}" \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_INSTALL_PREFIX="${INSTALL_PREFIX}" \
  -DENABLE_UNIT_TESTS=ON \
  -DENABLE_COMPONENT_TESTS=ON \
  -DENABLE_DOC_DOXYGEN=ON \
  -DMAX_LOG_LEVEL=5 \
  -DENABLE_CPPCHECK=ON \
  -DENABLE_CALLGRIND=ON \
  -DENABLE_VALGRIND=ON \
  "${CMAKE_ARGS[@]}"

log "Configuring done"
