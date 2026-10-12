#!/bin/bash
#
# The annotations project of the application converts into the imgajz1 project
# file and back: the file name extension of the path tells the format the native
# project is written and read in, and the imgajz1 file is the regular JSON
# project file gzip compressed, which the stock gzip tools unpack into the very
# text of the JSON project file.

. "$(dirname "$0")/common.sh"

itest_init "$@"

itest_given_native_project

COMPRESSED="${WORK_DIR}/compressed/project.imgajz1"

"${CONVERTER_BINARY}" \
  --convert-from imagesannotator \
  --convert-src "${WORK_DIR}/project.json" \
  --convert-to imagesannotator \
  --convert-dst "${COMPRESSED}" \
  || itest_fail "The conversion into the imgajz1 file returned a non zero status"

if ! gzip -t "${COMPRESSED}" ; then
    itest_fail "The ${COMPRESSED} file is no valid gzip file"
fi

"${CONVERTER_BINARY}" \
  --convert-from imagesannotator \
  --convert-src "${COMPRESSED}" \
  --convert-to imagesannotator \
  --convert-dst "${WORK_DIR}/recovered/project.json" \
  || itest_fail "The conversion of the imgajz1 file returned a non zero status"

RECOVERED="${WORK_DIR}/recovered/project.json"

itest_expect_file_contains "${RECOVERED}" '"name": "cat"'
itest_expect_file_contains "${RECOVERED}" '"x": 50'
itest_expect_file_contains "${RECOVERED}" '"rel_path": "a.png"'

if ! gzip -dc "${COMPRESSED}" | cmp -s - "${RECOVERED}" ; then
    itest_fail "The unpacked ${COMPRESSED} differs from the JSON project file ${RECOVERED}"
fi
