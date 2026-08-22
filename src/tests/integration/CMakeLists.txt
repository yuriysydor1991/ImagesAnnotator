cmake_minimum_required(VERSION 3.13)

# The integration tests drive the produced binaries the way a user does, so
# they are shell scripts rather than GoogleTest executables and they are gated
# on an option of their own (see
# cmake/enablers/template-project-GTest-enabler.cmake).
if(NOT ENABLE_INTEGRATION_TESTS)
  return()
endif()

add_subdirectory(ImagesAnnotatorConverter)
