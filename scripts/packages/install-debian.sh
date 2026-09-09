#!/bin/bash -e
#
# Installs the distribution packages required to build and run the project
# locally.
#
# Exactly one GUI toolkit is compiled into the binary (see the extensions
# section), so exactly one toolkit development package set is installed. Name
# the toolkit as the only argument, or leave it out for the GTKmm3 default:
#
#   scripts/packages/install-debian.sh [gtkmm3|gtkmm4|wxwidgets|qt6|all]
#
# The three ImagesAnnotator libraries the project is built against are no
# distribution packages, so they are none of this script's business - install
# them with misc/ci/install-annotator-libraries.sh afterwards.

PROJECT_ROOT=$(realpath "$(dirname "$0")/../..")

. "${PROJECT_ROOT}/scripts/common.sh"

PACKAGES=(
    build-essential
    cmake
    git
    googletest
    libgmock-dev
    libgtest-dev
    pkg-config
    # ENABLE_LIBCURL and ENABLE_NLOHMANN_JSON are ON by default and their
    # enablers build the missing library from its sources, which every
    # configure would then pay for.
    libcurl4-openssl-dev
    nlohmann-json3-dev
    # The ENABLE_OPENCV of the two dataset libraries is ON by default and is
    # dropped silently instead, which leaves the ImagesAnnotatorConverter
    # binary unable to write the PyTorch Vision layout and to read the five
    # layouts stored relative to an image size.
    libopencv-dev
)

TOOLKIT=${1:-gtkmm3}

case "${TOOLKIT}" in
    gtkmm3)    PACKAGES+=(libgtkmm-3.0-dev) ;;
    gtkmm4)    PACKAGES+=(libgtkmm-4.0-dev) ;;
    wxwidgets) PACKAGES+=(libwxgtk3.2-dev) ;;
    qt6)       PACKAGES+=(qt6-base-dev) ;;
    all)       PACKAGES+=(libgtkmm-3.0-dev libgtkmm-4.0-dev libwxgtk3.2-dev qt6-base-dev) ;;
    *)         log_fatal "Unknown GUI toolkit ${TOOLKIT}: name gtkmm3, gtkmm4, wxwidgets, qt6 or all" ;;
esac

APT_SUDO=""

if [[ ${EUID} -ne 0 ]] ; then
    APT_SUDO="sudo"
fi

log "Installing ${#PACKAGES[@]} packages for the ${TOOLKIT} GUI toolkit"

${APT_SUDO} apt-get update

${APT_SUDO} apt-get install -y "${PACKAGES[@]}"

log "The required packages are installed"
log "Install the three ImagesAnnotator libraries next: misc/ci/install-annotator-libraries.sh"
