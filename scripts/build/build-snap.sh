#!/bin/bash -e
#
# Produces the snap package: a Release configure with the ENABLE_SNAP option
# enabled, then the `snap` target of that build.
#
# snapcraft builds the project from the sources inside its own environment, so
# no build step is performed and the build directory needs no artifacts.
#
# Every parameter of the release-configure.sh and the release-build.sh scripts
# is accepted and forwarded, so an own -D<variable>=<value> override wins over
# the ENABLE_SNAP one below.
#
# The --install flag installs the produced package with the `sudo snap install
# --dangerous` command.

PROJECT_ROOT=$(realpath "$(dirname "$0")/../..")
BUILD_SCRIPTS_ROOT=$(realpath "$(dirname "$0")")

. "${BUILD_SCRIPTS_ROOT}/common.sh"

${BUILD_SCRIPTS_ROOT}/release-configure.sh -DENABLE_SNAP=ON "$@"

${BUILD_SCRIPTS_ROOT}/release-build.sh --target snap "$@"

log "Look for the snap package inside the ${RELEASE_BUILD_DIR} directory"

if [[ $* =~ --install ]] ; then
    install_built_package "*.snap" sudo snap install --dangerous
fi
