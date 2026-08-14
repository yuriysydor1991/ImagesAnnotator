#!/bin/bash -e

PROJECT_ROOT=$(realpath "$(dirname "$0")/../..")
BUILD_SCRIPTS_ROOT=$(realpath "$(dirname "$0")")

. "${BUILD_SCRIPTS_ROOT}/common.sh"

check_build_dir "${DEBUG_BUILD_DIR}"

log "Trying to build the project ${DEBUG_BUILD_DIR}"

filter_script_args no "$@"

cmake --build "${DEBUG_BUILD_DIR}" -j$(nproc) "${CMAKE_ARGS[@]}"

log "Build is done"
