cmake_minimum_required(VERSION 3.13)

# disabling the asserts for the tests
add_compile_options(-DNDEBUG=1)

add_subdirectory(Application)
add_subdirectory(ApplicationContext)
add_subdirectory(ApplicationHelpPrinter)
add_subdirectory(ApplicationVersionPrinter)
add_subdirectory(CommandLineParser)
add_subdirectory(ApplicationFactory)
add_subdirectory(ConversionFormats)
add_subdirectory(ApplicationFormatsConverter)
add_subdirectory(ApplicationConverterComponent)
