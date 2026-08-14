#!/bin/bash -e

PROJECT_ROOT=$(realpath "$(dirname "$0")/../..")
BUILD_SCRIPTS_ROOT=$(realpath "$(dirname "$0")")

. "${BUILD_SCRIPTS_ROOT}/common.sh"

if ! [[ $* =~ '--install' ]] ; then
    log "No install flag given, omiting"
    exit 0
fi

check_build_dir "${RELEASE_BUILD_DIR}"

log "Trying to install ${RELEASE_BUILD_DIR}"

filter_script_args no "$@"

sudo cmake --install "${RELEASE_BUILD_DIR}" --prefix ${INSTALL_PREFIX} "${CMAKE_ARGS[@]}"
