cmake_minimum_required(VERSION 3.13)

target_sources(
  ${PROJECT_BINARY_NAME}
  PRIVATE ApplicationContext.cpp
  PRIVATE Application.cpp
  PRIVATE ApplicationFactory.cpp
  PRIVATE ApplicationHelpPrinter.cpp
  PRIVATE CommandLineParser.cpp
  PRIVATE ApplicationVersionPrinter.cpp
)

add_subdirectory(converter)
add_subdirectory(tests)
