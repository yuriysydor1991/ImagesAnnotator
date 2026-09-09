#!/bin/bash -e
#
# Produces the DEB package: a Release configure with the ENABLE_DEB option
# enabled, then the `package` target of that build.
#
# The `package` target runs the preinstall step, which builds the project, so
# no separate build step is performed.
#
# Every parameter of the release-configure.sh and the release-build.sh scripts
# is accepted and forwarded, so an own -D<variable>=<value> override wins over
# the ENABLE_DEB one below.
#
# The --install flag installs the produced package with the `sudo apt-get
# install` command.

PROJECT_ROOT=$(realpath "$(dirname "$0")/../..")
BUILD_SCRIPTS_ROOT=$(realpath "$(dirname "$0")")

. "${BUILD_SCRIPTS_ROOT}/common.sh"

${BUILD_SCRIPTS_ROOT}/release-configure.sh -DENABLE_DEB=ON "$@"

${BUILD_SCRIPTS_ROOT}/release-build.sh --target package "$@"

log "Look for the DEB package inside the ${RELEASE_BUILD_DIR} directory"

if [[ $* =~ --install ]] ; then
    install_built_package "*.deb" sudo apt-get install -y
fi
