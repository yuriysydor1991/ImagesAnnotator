#!/bin/bash
#
# The help message of the converter names the binary being run and lists the
# four conversion flags together with every format they accept.

. "$(dirname "$0")/common.sh"

itest_init "$@"

PRINTED="$("${CONVERTER_BINARY}" --help)"

itest_expect_output_contains "${PRINTED}" "$(basename "${CONVERTER_BINARY}") [OPTIONS]"

for CONVERSION_FLAG in --convert-from --convert-src --convert-to --convert-dst ; do
    itest_expect_output_contains "${PRINTED}" "${CONVERSION_FLAG}"
done

for CONVERSION_FORMAT in imagesannotator plain-txt yolo4 ultralytics-detect \
                         ultralytics-obb ultralytics-segment coco pascal-voc \
                         createml pytorch ; do
    itest_expect_output_contains "${PRINTED}" "${CONVERSION_FORMAT}"
done

# the native project is told by the file name extension of its path
itest_expect_output_contains "${PRINTED}" ".imgajz1 file name extension"
