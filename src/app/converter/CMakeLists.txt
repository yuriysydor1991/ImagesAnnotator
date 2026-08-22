cmake_minimum_required(VERSION 3.13)

target_sources(
  ${PROJECT_BINARY_NAME}
  PRIVATE ApplicationFormatsConverter.cpp
  PRIVATE ConversionFormats.cpp
)
