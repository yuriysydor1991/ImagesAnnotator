cmake_minimum_required(VERSION 3.13)

# The conversion itself, which is what the converter only binary is built for
# and what the main binary offers through its own command line as well, so both
# application targets get these sources.
annotator_app_targets_sources(
  ${CMAKE_CURRENT_SOURCE_DIR}/ApplicationConverterComponent.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ApplicationFormatsConverter.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ConversionFormats.cpp
)
