cmake_minimum_required(VERSION 3.13)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/enablers")

if(ENABLE_UNIT_TESTS OR ENABLE_COMPONENT_TESTS)
  enable_testing()
  include(template-project-make-GTest-available)
endif()

if(ENABLE_CLANGFORMAT)
  include(template-project-clang-format-target)
endif()

if(ENABLE_CPPCHECK)
  include(template-project-cppcheck-target)
endif()

if(ENABLE_VALGRIND)
  include(template-project-valgrind-target)
endif()

if(ENABLE_CLANG_TIDY)
  include(template-project-clang-tidy-target)
endif()

if(ENABLE_DEB)
  include(template-project-deb-enabler)
endif()

if(ENABLE_FLATPAK)
  include(template-project-flatpak-target)
endif()

if (ENABLE_SANITIZERS OR ENABLE_SANITIZERS_THREADS)
  include(template-project-sanitizers)
endif()

if (ENABLE_LIBCURL)
  include(template-project-libcurl-enabler)
endif()
