#!/bin/bash -e
#
# Produces the WIX MSI installer: a Release configure with the ENABLE_WIX option
# enabled, then the `package` target of that build.
#
# The `package` target runs the preinstall step, which builds the project, so
# no separate build step is performed.
#
# Every parameter of the release-configure.sh and the release-build.sh scripts
# is accepted and forwarded, so an own -D<variable>=<value> override wins over
# the ENABLE_WIX one below.
#
# The --install flag installs the produced package with the `msiexec //i`
# command.

PROJECT_ROOT=$(realpath "$(dirname "$0")/../..")
BUILD_SCRIPTS_ROOT=$(realpath "$(dirname "$0")")

. "${BUILD_SCRIPTS_ROOT}/common.sh"

${BUILD_SCRIPTS_ROOT}/release-configure.sh -DENABLE_WIX=ON "$@"

${BUILD_SCRIPTS_ROOT}/release-build.sh --target package "$@"

log "Look for the WIX MSI installer inside the ${RELEASE_BUILD_DIR} directory"

if [[ $* =~ --install ]] ; then
    install_built_package "*.msi" msiexec //i
fi
