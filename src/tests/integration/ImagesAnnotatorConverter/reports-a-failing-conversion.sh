#!/bin/bash
#
# A conversion which could not be performed ends with a non zero exit status,
# which is what makes the binary usable from a script.

. "$(dirname "$0")/common.sh"

itest_init "$@"

if "${CONVERTER_BINARY}" \
     --convert-from coco \
     --convert-src "${WORK_DIR}/nothing-here" \
     --convert-to yolo4 \
     --convert-dst "${WORK_DIR}/out" > /dev/null 2>&1 ; then
    itest_fail "A conversion of a source that does not exist reported success"
fi

if "${CONVERTER_BINARY}" \
     --convert-from imagesannotator \
     --convert-src "${WORK_DIR}/nothing-here.json" \
     --convert-to nosuch \
     --convert-dst "${WORK_DIR}/out" > /dev/null 2>&1 ; then
    itest_fail "A conversion into an unknown format reported success"
fi

if [[ -e "${WORK_DIR}/out" ]] ; then
    itest_fail "A failed conversion wrote into its destination"
fi
