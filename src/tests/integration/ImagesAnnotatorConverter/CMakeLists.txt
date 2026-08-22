cmake_minimum_required(VERSION 3.13)

# The integration tests of the converter only binary. Nothing is compiled here:
# every case is a shell script driving the produced binary the way a user does,
# so what a case asserts on is the file that would be shipped.
if(NOT TARGET ${PROJECT_CONVERTER_BINARY_NAME})
  message(STATUS
    "The ImagesAnnotatorConverter integration tests are skipped, the binary "
    "they drive is not being built")
  return()
endif()

set(ITESTS_WORK_DIR "${CMAKE_CURRENT_BINARY_DIR}/itests-data")

set(
  ITEST_CASES
  converts-a-native-project-into-a-coco-dataset
  converts-a-plain-txt-dataset-into-a-native-project
  reports-a-failing-conversion
  prints-the-conversion-help
  no-gui-toolkit-linked
)

set(ITEST_NAMES "")

foreach(ITEST_CASE IN LISTS ITEST_CASES)
  string(REPLACE "-" "_" ITEST_CASE_NAME "${ITEST_CASE}")

  set(ITEST_NAME "ITEST_${PROJECT_CONVERTER_BINARY_NAME}.${ITEST_CASE_NAME}")

  add_test(
    NAME ${ITEST_NAME}
    COMMAND
      ${CMAKE_COMMAND} -E env bash
      "${CMAKE_CURRENT_SOURCE_DIR}/${ITEST_CASE}.sh"
      "$<TARGET_FILE:${PROJECT_CONVERTER_BINARY_NAME}>"
      "${ITESTS_WORK_DIR}/${ITEST_CASE}"
  )

  list(APPEND ITEST_NAMES ${ITEST_NAME})
endforeach()

set_tests_properties(${ITEST_NAMES} PROPERTIES TIMEOUT 600)
