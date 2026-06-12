cmake_minimum_required(VERSION 3.13)

# disabling the asserts for the tests
add_compile_options(-DNDEBUG=1)

add_subdirectory(SortHelper)
add_subdirectory(TypeHelper)
add_subdirectory(ImageRecordUrlAndPathHelper)
add_subdirectory(ImageLoader)
