#!/bin/sh
#
# Builds and installs the three standalone libraries the project is built
# against: the ImagesAnnotator-DataDrivers annotations project database, the
# ImagesAnnotator-DataExporters dataset exporters and the
# ImagesAnnotator-DataImporters dataset importers. None is a distribution
# package and none is fetched by the project configure - the three enablers,
# cmake/enablers/template-project-ImagesAnnotator-Data{Drivers,Exporters,Importers}-enabler.cmake,
# only look them up with find_package() - so every environment that configures
# this project has to provide them first. That is what this script is for, and
# the three CI pipelines (misc/Jenkinsfile, .github/workflows/ci.yml and
# .gitlab/ci/) all call it instead of spelling the same steps out three times.
#
# Usage:
#
#   misc/ci/install-annotator-libraries.sh [install-prefix]
#
# The install prefix defaults to /usr/local. Point the project at a different
# one with -DCMAKE_PREFIX_PATH=<prefix> and, at the run time, with
# LD_LIBRARY_PATH=<prefix>/lib.
#
# The repositories and the revisions may be overridden through the environment,
# which is what a pipeline validating an unreleased library revision does:
#
#   IADD_GIT, IADD_GIT_TAG, IADE_GIT, IADE_GIT_TAG, IADI_GIT, IADI_GIT_TAG
#
set -eu

PREFIX="${1:-/usr/local}"

IADD_GIT="${IADD_GIT:-https://github.com/yuriysydor1991/ImagesAnnotator-DataDrivers.git}"
IADD_GIT_TAG="${IADD_GIT_TAG:-main}"
IADE_GIT="${IADE_GIT:-https://github.com/yuriysydor1991/ImagesAnnotator-DataExporters.git}"
IADE_GIT_TAG="${IADE_GIT_TAG:-main}"
IADI_GIT="${IADI_GIT:-https://github.com/yuriysydor1991/ImagesAnnotator-DataImporters.git}"
IADI_GIT_TAG="${IADI_GIT_TAG:-main}"

# The install step is the only one that may need the elevated rights, and only
# when the prefix is not writable by the current user.
SUDO=""
if [ ! -w "$(dirname "$PREFIX")" ] && [ "$(id -u)" -ne 0 ]; then
  SUDO="sudo"
fi

WORKDIR="$(mktemp -d)"
trap 'rm -rf "$WORKDIR"' EXIT

install_library()
{
  name="$1"
  url="$2"
  tag="$3"

  echo "=== $name ($url @ $tag) -> $PREFIX"

  git clone --depth 1 --branch "$tag" "$url" "$WORKDIR/$name"

  # CMAKE_PREFIX_PATH is what lets the exporters and the importers libraries
  # find the data drivers installed by the call before them.
  cmake -S "$WORKDIR/$name" -B "$WORKDIR/$name-build" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="$PREFIX" \
    -DCMAKE_PREFIX_PATH="$PREFIX"

  cmake --build "$WORKDIR/$name-build" -j"$(nproc)"

  $SUDO cmake --install "$WORKDIR/$name-build"
}

# The data drivers first: the other two are built against them.
install_library ImagesAnnotator-DataDrivers "$IADD_GIT" "$IADD_GIT_TAG"
install_library ImagesAnnotator-DataExporters "$IADE_GIT" "$IADE_GIT_TAG"
install_library ImagesAnnotator-DataImporters "$IADI_GIT" "$IADI_GIT_TAG"

# A prefix outside of the default linker search path is only complete once the
# loader knows about it.
if [ "$PREFIX" != "/usr" ] && [ "$PREFIX" != "/usr/local" ]; then
  echo "NOTE: run the project with LD_LIBRARY_PATH=$PREFIX/lib"
elif [ -x "$(command -v ldconfig)" ]; then
  $SUDO ldconfig
fi

echo "OK: all three libraries are installed into $PREFIX"
