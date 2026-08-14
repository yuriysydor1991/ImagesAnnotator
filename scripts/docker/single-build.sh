#!/bin/bash -e

PROJECT_ROOT=$(realpath "$(dirname "$0")/../..")
BUILD_SCRIPTS_ROOT=$(realpath "${PROJECT_ROOT}/scripts/build")

${BUILD_SCRIPTS_ROOT}/release-configure.sh -DENABLE_DOCKER=ON "$@"

${BUILD_SCRIPTS_ROOT}/release-build.sh --target docker-single-build "$@"
