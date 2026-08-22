#!/bin/bash
#
# Shared helpers of the ImagesAnnotatorConverter integration tests.
#
# Every case is a script of its own, started by ctest as
#
#   bash <case>.sh <converter binary> <work directory>
#
# and reporting its verdict through the exit status, which is what ctest reads.
# What is driven here is the produced binary itself - no test framework, no
# mock and no library of this project is involved - so a case fails whenever
# the shipped converter would fail in the hands of a user.

set -euo pipefail

itest_fail()
{
    echo "FAIL: $*" >&2

    exit 1
}

# @brief Accepts the two arguments every case is started with and prepares an
# empty work directory of its own.
itest_init()
{
    CONVERTER_BINARY="${1:-}"
    WORK_DIR="${2:-}"

    if [[ -z "${CONVERTER_BINARY}" ]] || [[ -z "${WORK_DIR}" ]] ; then
        itest_fail "usage: $0 <converter binary> <work directory>"
    fi

    if [[ ! -x "${CONVERTER_BINARY}" ]] ; then
        itest_fail "No converter binary to run at ${CONVERTER_BINARY}"
    fi

    rm -rf "${WORK_DIR}"
    mkdir -p "${WORK_DIR}"
}

# @brief Fails unless the given file holds the given text.
itest_expect_file_contains()
{
    local fpath="$1"
    local wanted="$2"

    if [[ ! -f "${fpath}" ]] ; then
        itest_fail "No file was produced at ${fpath}"
    fi

    if ! grep -qF -- "${wanted}" "${fpath}" ; then
        itest_fail "The file ${fpath} carries no '${wanted}':
$(cat "${fpath}")"
    fi
}

# @brief Fails unless the given text is part of the given one.
itest_expect_output_contains()
{
    local produced="$1"
    local wanted="$2"

    if ! printf '%s' "${produced}" | grep -qF -- "${wanted}" ; then
        itest_fail "The output carries no '${wanted}':
${produced}"
    fi
}

# @brief Writes the annotations project of one 200x100 image carrying one
# 100x40 `cat` rectangle at 50,20. The project database is the one side of a
# conversion which names the image sizes itself, so no case here needs a real
# picture to be decoded.
itest_given_native_project()
{
    mkdir -p "${WORK_DIR}/imgs"

    echo "the-pixels-are-never-decoded-here" > "${WORK_DIR}/imgs/a.png"

    cat > "${WORK_DIR}/project.json" <<PROJECT_JSON
{
  "ImagesAnnotator": { "version": "0.12.0" },
  "annotations": [
    {
      "annotations": [
        {
          "annotations": [
            { "name": "cat", "rect": { "height": 40, "width": 100, "x": 50, "y": 20 } }
          ],
          "height": 100,
          "rel_path": "a.png",
          "scale_factor": 0.0,
          "width": 200
        }
      ],
      "db": { "path": "${WORK_DIR}/imgs" }
    }
  ]
}
PROJECT_JSON
}

# @brief Writes the plain text dataset of that very same rectangle.
itest_given_plain_txt_dataset()
{
    mkdir -p "${WORK_DIR}/imgs" "${WORK_DIR}/plain"

    echo "the-pixels-are-never-decoded-here" > "${WORK_DIR}/imgs/a.png"
    echo "${WORK_DIR}/imgs/a.png 1 50 20 100 40" > "${WORK_DIR}/plain/cat.txt"
}
