#!/bin/bash -e

PROJECT_ROOT=$(realpath "$(dirname "$0")/../..")
BUILD_SCRIPTS_ROOT=$(realpath "$(dirname "$0")")

. "${BUILD_SCRIPTS_ROOT}/common.sh"

${BUILD_SCRIPTS_ROOT}/debug-configure.sh "$@"

${BUILD_SCRIPTS_ROOT}/debug-build.sh "$@"

${BUILD_SCRIPTS_ROOT}/debug-test.sh "$@"

${BUILD_SCRIPTS_ROOT}/debug-install.sh "$@"
