#!/bin/bash -e
#
# Installs the packages required to build and develop the project on Ubuntu.
#
# Ubuntu shares the package set with Debian.

exec "$(dirname "$0")/install-debian.sh" "$@"
