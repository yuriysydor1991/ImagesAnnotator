#!/bin/bash -e
#
# Produces the FreeBSD pkg package: a Release configure with the ENABLE_FREEBSD_PKG option
# enabled, then the `package` target of that build.
#
# The `package` target runs the preinstall step, which builds the project, so
# no separate build step is performed.
#
# Every parameter of the release-configure.sh and the release-build.sh scripts
# is accepted and forwarded, so an own -D<variable>=<value> override wins over
# the ENABLE_FREEBSD_PKG one below.
#
# The --install flag installs the produced package with the `sudo pkg add`
# command.

PROJECT_ROOT=$(realpath "$(dirname "$0")/../..")
BUILD_SCRIPTS_ROOT=$(realpath "$(dirname "$0")")

. "${BUILD_SCRIPTS_ROOT}/common.sh"

${BUILD_SCRIPTS_ROOT}/release-configure.sh -DENABLE_FREEBSD_PKG=ON "$@"

${BUILD_SCRIPTS_ROOT}/release-build.sh --target package "$@"

log "Look for the FreeBSD pkg package inside the ${RELEASE_BUILD_DIR} directory"

if [[ $* =~ --install ]] ; then
    install_built_package "*.pkg" sudo pkg add
fi
