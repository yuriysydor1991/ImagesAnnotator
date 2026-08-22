#!/bin/bash
#
# A dataset on the disk converts into the annotations project of the
# application, which is the way a project is recovered out of a training
# directory without opening the graphical interface.

. "$(dirname "$0")/common.sh"

itest_init "$@"

itest_given_plain_txt_dataset

"${CONVERTER_BINARY}" \
  --convert-from plain-txt \
  --convert-src "${WORK_DIR}/plain" \
  --convert-to imagesannotator \
  --convert-dst "${WORK_DIR}/recovered/project.json" \
  || itest_fail "The conversion returned a non zero status"

RECOVERED="${WORK_DIR}/recovered/project.json"

itest_expect_file_contains "${RECOVERED}" '"name": "cat"'
itest_expect_file_contains "${RECOVERED}" '"x": 50'
itest_expect_file_contains "${RECOVERED}" '"rel_path": "a.png"'
