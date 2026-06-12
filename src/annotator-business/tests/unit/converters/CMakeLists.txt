cmake_minimum_required(VERSION 3.13)

set(CONVERTERS_SRC
  ${CMAKE_SOURCE_DIR}/src/annotator-business/dbs/AnnotationsDBs/converters)

add_subdirectory(ImageRecords2JSONConverter)
add_subdirectory(ImageRecordRectJSON2RecordConverter)
add_subdirectory(ImageRecordJSON2RecordConverter)
add_subdirectory(JSON2ImageRecordsConverter)
