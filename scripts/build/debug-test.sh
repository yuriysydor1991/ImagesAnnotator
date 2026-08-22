#!/bin/bash -e

PROJECT_ROOT=$(realpath "$(dirname "$0")/../..")
BUILD_SCRIPTS_ROOT=$(realpath "$(dirname "$0")")

. "${BUILD_SCRIPTS_ROOT}/common.sh"

if ! [[ $* =~ '--test' ]] ; then
    log "No test flag given, omiting"
    exit 0
fi

check_build_dir "${DEBUG_BUILD_DIR}"

log "Trying to test the ${DEBUG_BUILD_DIR}"

filter_script_args no "$@"

cd "${DEBUG_BUILD_DIR}"

ctest --output-on-failure "${CMAKE_ARGS[@]}"

log "Testing is done"
