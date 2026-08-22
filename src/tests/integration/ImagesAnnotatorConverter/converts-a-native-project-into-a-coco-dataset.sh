#!/bin/bash
#
# The annotations project of the application converts into a COCO dataset, and
# the rectangle it carried comes out of the produced descriptor unchanged.

. "$(dirname "$0")/common.sh"

itest_init "$@"

itest_given_native_project

"${CONVERTER_BINARY}" \
  --convert-from imagesannotator \
  --convert-src "${WORK_DIR}/project.json" \
  --convert-to coco \
  --convert-dst "${WORK_DIR}/coco" \
  || itest_fail "The conversion returned a non zero status"

COCO_DESCRIPTOR="${WORK_DIR}/coco/annotations/instances_default.json"

itest_expect_file_contains "${COCO_DESCRIPTOR}" '"bbox": [50, 20, 100, 40]'
itest_expect_file_contains "${COCO_DESCRIPTOR}" '"name": "cat"'
itest_expect_file_contains "${COCO_DESCRIPTOR}" '"width": 200'

if [[ ! -f "${WORK_DIR}/coco/images/a.png" ]] ; then
    itest_fail "The image the descriptor names was not copied over"
fi
