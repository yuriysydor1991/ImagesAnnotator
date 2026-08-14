#!/bin/bash -e

PROJECT_ROOT=$(realpath "$(dirname "$0")/../..")
BUILD_SCRIPTS_ROOT=$(realpath "$(dirname "$0")")

. "${BUILD_SCRIPTS_ROOT}/common.sh"

if [[ $* =~ --no-reconfigure ]] ; then
    log "User issued --no-reconfigure"
    exit 0
fi

delete_previous_build "${RELEASE_BUILD_DIR}"

log "Trying to Release-configure project ${PROJECT_ROOT} inside ${RELEASE_BUILD_DIR}"

filter_script_args yes "$@"

cmake -B "${RELEASE_BUILD_DIR}" -S "${PROJECT_ROOT}" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX="${INSTALL_PREFIX}" \
  "${CMAKE_ARGS[@]}"

log "Configuring done"
