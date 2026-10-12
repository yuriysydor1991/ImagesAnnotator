#!/bin/bash -e
#
# Produces the DEB package which carries the three ImagesAnnotator libraries
# along with the project binaries, so it installs and runs on a system none of
# them has ever been installed on.
#
# The ImagesAnnotator-DataDrivers, the ImagesAnnotator-DataExporters and the
# ImagesAnnotator-DataImporters libraries are no distribution packages: the
# configure of the build-deb.sh script finds them only where they are installed
# already, and its package runs only where they are installed already. This
# script builds them first, with the misc/ci/install-annotator-libraries.sh
# script, into the build/annotator-libraries directory instead of the system,
# and then performs the build-deb.sh against them with the
# ENABLE_DEB_IMAGES_ANNOTATOR_LIBRARIES option enabled, which packs them into
# the package.
#
# The libraries are cloned from the main branches of their repositories, which
# the IADD_GIT, IADD_GIT_TAG, IADE_GIT, IADE_GIT_TAG, IADI_GIT and IADI_GIT_TAG
# environment variables of that script override.
#
# Every parameter of the build-deb.sh script is accepted and forwarded, the
# --install flag included, so an own -D<variable>=<value> override wins over the
# two below. The --no-reconfigure flag reuses the libraries the previous run
# built along with the configured build directory.

PROJECT_ROOT=$(realpath "$(dirname "$0")/../..")
BUILD_SCRIPTS_ROOT=$(realpath "$(dirname "$0")")

. "${BUILD_SCRIPTS_ROOT}/common.sh"

LIBRARIES_PREFIX="${PROJECT_ROOT}/build/annotator-libraries"

if [[ $* =~ --no-reconfigure ]] ; then
    log "User issued --no-reconfigure, reusing the libraries at ${LIBRARIES_PREFIX}"
else
    delete_previous_build "${LIBRARIES_PREFIX}"

    # the install script resorts to the sudo unless the parent directory of the
    # prefix is writable, so that directory has to exist before it is started
    mkdir -p "${LIBRARIES_PREFIX}"

    "${PROJECT_ROOT}/misc/ci/install-annotator-libraries.sh" "${LIBRARIES_PREFIX}"
fi

log "Packing the libraries of ${LIBRARIES_PREFIX} into the DEB package"

${BUILD_SCRIPTS_ROOT}/build-deb.sh \
  -DCMAKE_PREFIX_PATH="${LIBRARIES_PREFIX}" \
  -DENABLE_DEB_IMAGES_ANNOTATOR_LIBRARIES=ON \
  "$@"
