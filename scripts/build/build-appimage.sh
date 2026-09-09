#!/bin/bash -e
#
# Produces the AppImage package: a Release configure with the ENABLE_APPIMAGE
# option enabled, then the `appimage` target of that build.
#
# The `appimage` target installs the project into its AppDir, which builds it,
# so no separate build step is performed.
#
# Every parameter of the release-configure.sh and the release-build.sh scripts
# is accepted and forwarded, so an own -D<variable>=<value> override wins over
# the ENABLE_APPIMAGE one below.
#
# The --install flag installs the produced package with the `install` command
# into the ${HOME}/.local/bin directory.

PROJECT_ROOT=$(realpath "$(dirname "$0")/../..")
BUILD_SCRIPTS_ROOT=$(realpath "$(dirname "$0")")

. "${BUILD_SCRIPTS_ROOT}/common.sh"

${BUILD_SCRIPTS_ROOT}/release-configure.sh -DENABLE_APPIMAGE=ON "$@"

${BUILD_SCRIPTS_ROOT}/release-build.sh --target appimage "$@"

log "Look for the AppImage package inside the ${RELEASE_BUILD_DIR} directory"

if [[ $* =~ --install ]] ; then
    install_built_package "*.AppImage" install -D -m 755 -t "${HOME}/.local/bin"
fi
